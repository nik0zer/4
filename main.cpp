#include "filesystem_op.h"
#include <iostream>
#include <fstream>

#define OUT_TO_FILE 1
const std::string out_file_path("../1.log");

int main()
{
    std::string path;
    std::cin>>path;
    DirectoryLogScan scan(path);
    scan.scan_root_path();
    scan.analyze_log_files();
    if(OUT_TO_FILE)
    {
        std::ofstream out_file(out_file_path);
        if (!out_file)
        {
            std::cerr<<"Ошибка открытия файла.\n";
            std::cout<<scan;
            return 0;
        }
        out_file<<scan;
        out_file.close();
        return 0;
    }
    std::cout<<scan;
}