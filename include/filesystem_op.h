#ifndef FILESYSTEM_OP
#define FILESYSTEM_OP
#include <string>
#include <vector>
#include <memory>
#include <filesystem>
#include <mutex>
#include <map>

enum Constants
{
    PROCESS_PRINT_WIDTH = 40,
    LOG_LEVEL_PRINT_WIDTH = 20
};

class DirectoryLogScan;

class LogStat
{
    public:
    std::string name;
    int trace;
    int debug;
    int info;
    int warn;
    int error;

    LogStat(std::string name) : name(name) 
    {
        trace = 0;
        debug = 0;
        info = 0;
        warn = 0;
        error = 0;
    }
};

class FileInfo
{
    public:
    std::string file_path;
    FileInfo(std::string file_path) : file_path(file_path) {}
    FileInfo(const FileInfo& file_info) : file_path(file_info.file_path) {}
    FileInfo(FileInfo&& file_info) : file_path(file_info.file_path) {}

    friend std::ostream& operator<<(std::ostream& ostream, FileInfo const& file_info);
};

class DirInfo
{
    private:
    std::filesystem::path _path;
    std::vector<FileInfo> path_files_info;
    void scan_path();
    DirInfo(std::filesystem::path path) : _path(path) {}

    public:
    friend DirectoryLogScan;
    friend std::ostream& operator<<(std::ostream& ostream, DirInfo const& dir_scan);
};

class DirectoryLogScan
{
    private:
    std::filesystem::path _path;
    std::mutex _files_info_mutex;
    std::mutex map_mutex;
    void collect_files(std::shared_ptr<DirInfo>);
    void analyze_file(FileInfo file_info);
    void line_parse(std::string line);
    std::map<std::string, LogStat> log_statistics;

    public:
    std::vector<LogStat> sort_log_statistics;
    std::vector<FileInfo> path_files_info;
    std::vector<std::shared_ptr<DirInfo>> dirs_info_ptr;
    void scan_root_path();
    void analyze_log_files();
    

    DirectoryLogScan(std::filesystem::path path) : _path(path) {}
    friend std::ostream& operator<<(std::ostream& ostream, DirectoryLogScan const& scan);
};




#endif