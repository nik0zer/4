#include <gtest/gtest.h>
#include "filesystem_op.h"
#include <fstream>
#include <iostream>


TEST(Task_4_test, Subtest_1) 
{
    std::string path = "../1";
    std::string test_log_path = "../log_files_for_test/1_test.log";
    std::string answer_path = "../log_files_for_test/1.log";
    DirectoryLogScan scan(path);
    scan.scan_root_path();
    scan.analyze_log_files();

    std::ofstream out(test_log_path);
    out<<scan;
    out.close();

    std::ifstream in(answer_path);
    std::ifstream test_in(test_log_path);

    ASSERT_FALSE(!in);
    ASSERT_FALSE(!test_in);

    std::string ans_line;
    std::string test_line;

    while(getline(in, ans_line))
    {
        getline(test_in, test_line);
        ASSERT_STREQ(ans_line.c_str(), test_line.c_str());
    }
    ASSERT_TRUE(1);
}


TEST(Task_4_test, Subtest_2) 
{
    std::string path = "../2";
    std::string test_log_path = "../log_files_for_test/2_test.log";
    std::string answer_path = "../log_files_for_test/2.log";
    DirectoryLogScan scan(path);
    scan.scan_root_path();
    scan.analyze_log_files();

    std::ofstream out(test_log_path);
    out<<scan;
    out.close();

    std::ifstream in(answer_path);
    std::ifstream test_in(test_log_path);

    ASSERT_FALSE(!in);
    ASSERT_FALSE(!test_in);

    std::string ans_line;
    std::string test_line;

    while(getline(in, ans_line))
    {
        getline(test_in, test_line);
        ASSERT_STREQ(ans_line.c_str(), test_line.c_str());
    }
    ASSERT_TRUE(1);
}

int main()
{
    ::testing::InitGoogleTest();
    return RUN_ALL_TESTS();
}