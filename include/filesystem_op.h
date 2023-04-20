#ifndef FILESYSTEM_OP
#define FILESYSTEM_OP
#include <string>
#include <vector>
#include <memory>
#include <filesystem>
#include <iostream>
#include <mutex>
#include <thread>
#include <fstream>

class DirectoryLogScan;

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
    void collect_files(std::shared_ptr<DirInfo>);
    void analyze_file(FileInfo file_info);

    public:
    std::vector<FileInfo> path_files_info;
    std::vector<std::shared_ptr<DirInfo>> dirs_info_ptr;
    void scan_root_path();
    void analyze_log_files();
    

    DirectoryLogScan(std::filesystem::path path) : _path(path) {}
    friend std::ostream& operator<<(std::ostream& ostream, DirectoryLogScan const& scan);
};




#endif