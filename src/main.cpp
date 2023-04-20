#include "filesystem_op.h"
#include <iostream>


int main()
{
    std::string path;
    std::cin>>path;
    DirectoryLogScan scan(path);
    scan.scan_root_path();
    scan.analyze_log_files();
    std::cout<<scan;
}