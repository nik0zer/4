#include "filesystem_op.h"

void DirectoryLogScan::scan_root_path()
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
    for(auto i : path_files_info)
    {
        analyze_file(i);
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
            std::cout << line << std::endl;
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
    ostream<<"path: "<<scan._path<<std::endl<<"files:"<<std::endl;
    for(auto i : scan.path_files_info)
    {
        ostream<<i;
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
