#include "filesystem_op.h"
#include <algorithm>
#include <iostream>
#include <thread>
#include <fstream>



void DirectoryLogScan::scan_root_path()
{
    try
    {
        for (const auto & entry : std::filesystem::directory_iterator(_path))
        {
            if(entry.is_directory())
            {
                dirs_info_ptr.push_back(std::shared_ptr<DirInfo>(new DirInfo(entry.path())));
            }
            else if(entry.is_regular_file())
            {
                if(entry.path().string().ends_with(".log"))
                    path_files_info.push_back(FileInfo(entry.path()));
            }
        }
        std::vector<std::shared_ptr<std::thread>> threads;
        for(auto i : dirs_info_ptr)
        {
            threads.push_back(std::shared_ptr<std::thread>(new std::thread(&DirectoryLogScan::collect_files, this, i)));
        }
        for(auto i : threads)
        {
            i->join();
        }
    }
    catch(...)
    {
        throw;
    }
}

void DirectoryLogScan::collect_files(std::shared_ptr<DirInfo> dir_info_ptr)
{
    dir_info_ptr->scan_path();
    for(auto i : dir_info_ptr->path_files_info)
    {
        std::lock_guard<std::mutex> lock(_files_info_mutex);
        path_files_info.push_back(i);
    }
}

void DirectoryLogScan::analyze_log_files()
{
    std::vector<std::shared_ptr<std::thread>> threads;
    for(auto i : path_files_info)
    {
        threads.push_back(std::shared_ptr<std::thread>(new std::thread(&DirectoryLogScan::analyze_file, this, i)));
    }
    for(auto i : threads)
    {
        i->join();
    }
    for(auto i : this->log_statistics)
    {
        sort_log_statistics.push_back(i.second);
    }
    std::sort(sort_log_statistics.begin(), sort_log_statistics.end(), std::greater<LogStat>());
}

void DirectoryLogScan::line_parse(std::string line)
{
    int pos = line.find('[');
    pos = line.find('[', pos + 1);
    int endpos = line.find(']', pos + 1);
    std::string log_level = line.substr(pos + 1, endpos - pos - 1);
    pos = line.find('[', pos + 1);
    endpos = line.find(']', pos + 1);
    std::string process_name = line.substr(pos + 1, endpos - pos - 1);

    if(process_name == "" || process_name == "\0" || process_name == " " || process_name == "\n")
        return;

    std::lock_guard<std::mutex> lock(map_mutex);
    if(log_statistics.find(process_name) == log_statistics.end())
    {
        log_statistics.insert(std::pair<std::string, LogStat>(process_name, LogStat(process_name)));
    }

    if(log_level == "Trace")
    {
        log_statistics.at(process_name).trace++;
        return;
    }
    if(log_level == "Debug")
    {
        log_statistics.at(process_name).debug++;
        return;
    }
    if(log_level == "Info")
    {
        log_statistics.at(process_name).info++;
        return;
    }
    if(log_level == "Warn")
    {
        log_statistics.at(process_name).warn++;
        return;
    }
    if(log_level == "Error")
    {
        log_statistics.at(process_name).error++;
        return;
    }
}

void DirectoryLogScan::analyze_file(FileInfo file_info)
{
    std::string line;
 
    std::ifstream in(file_info.file_path);
    if (in.is_open())
    {
        while (getline(in, line))
        {
            this->line_parse(line);
        }
    }
    in.close();
}




void DirInfo::scan_path()
{
    for (const auto & entry : std::filesystem::directory_iterator(_path))
    {
        if(entry.is_regular_file())
        {
            if(entry.path().string().ends_with(".log"))
                path_files_info.push_back(FileInfo(entry.path()));
        }
    }
}





std::ostream& operator<<(std::ostream& ostream, DirectoryLogScan const& scan)
{
    ostream<<"Analyze log files in directory: "<<scan._path<<std::endl;
    ostream<<std::setw(PROCESS_PRINT_WIDTH)<<std::left<<"Process"<<"|"<<"  "<<
    std::setw(LOG_LEVEL_PRINT_WIDTH)<<"Trace"
    <<std::setw(LOG_LEVEL_PRINT_WIDTH)<<"Debug"
    <<std::setw(LOG_LEVEL_PRINT_WIDTH)<<"Info"
    <<std::setw(LOG_LEVEL_PRINT_WIDTH)<<"Warn"
    <<std::setw(LOG_LEVEL_PRINT_WIDTH)<<"Error"<<std::endl;

    for(auto i : scan.sort_log_statistics)
    {
        ostream<<std::setw(PROCESS_PRINT_WIDTH)<<std::left<<i.name<<"|"<<"  "<<
        std::setw(LOG_LEVEL_PRINT_WIDTH)<<i.trace<<
        std::setw(LOG_LEVEL_PRINT_WIDTH)<<i.debug<<
        std::setw(LOG_LEVEL_PRINT_WIDTH)<<i.info<<
        std::setw(LOG_LEVEL_PRINT_WIDTH)<<i.warn<<
        std::setw(LOG_LEVEL_PRINT_WIDTH)<<i.error<<std::endl;
    }
    return ostream;
}

std::ostream& operator<<(std::ostream& ostream, DirInfo const& dir_scan)
{
    ostream<<"path: "<<dir_scan._path<<std::endl<<"files:"<<std::endl;
    for(auto i : dir_scan.path_files_info)
    {
        ostream<<i;
    }
    return ostream;
}

std::ostream& operator<<(std::ostream& ostream, FileInfo const& file_info)
{
    ostream<<"path: "<<file_info.file_path<<std::endl;
    return ostream;
}



bool operator<(const LogStat& a, const LogStat& b)
{
    return (a.debug + a.error + a.info + a.trace + a.warn < b.debug + b.error + b.info + b.trace + b.warn);
}

bool operator>(const LogStat& a, const LogStat& b)
{
    return (a.debug + a.error + a.info + a.trace + a.warn > b.debug + b.error + b.info + b.trace + b.warn);
}

bool operator<=(const LogStat& a, const LogStat& b)
{
    return (a.debug + a.error + a.info + a.trace + a.warn <= b.debug + b.error + b.info + b.trace + b.warn);
}

bool operator>=(const LogStat& a, const LogStat& b)
{
    return (a.debug + a.error + a.info + a.trace + a.warn >= b.debug + b.error + b.info + b.trace + b.warn);
}