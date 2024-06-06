#ifndef COMPILE_H
#define COMPILE_H

#include<QDir>
#include <string>

class Compile
{
public:
    Compile(){};
    std::string compile_project_interface(const std::string &source_dir, const std::string &build_dir, const std::string &name);

private:
    void create_cmake_list(const std::string &source_dir, const std::string &exe_name);
    bool compile_project(const std::string &source_dir, const std::string &build_dir, const std::string &name);

    // 设置CMake的路径
    std::string cmake_bin = QDir::currentPath().toStdString() + "/CMake/bin/cmake.exe";
};

#endif // COMPILE_H
