#include "compile.h"

#include <iostream>
#include <fstream>
#include <filesystem>
#include <cstdlib>
#include <string>


namespace fs = std::filesystem;

// 生成CMake文件
void Compile::create_cmake_list(const std::string& source_dir,
                                const std::string& exe_name) {
    std::ofstream cmakeFile(source_dir + "/CMakeLists.txt");
    cmakeFile << "cmake_minimum_required(VERSION 3.10)\n";
    cmakeFile << "project(" << exe_name << ")\n";
    cmakeFile << "set(CMAKE_CXX_STANDARD 17)\n";
    cmakeFile << "include_directories(${PROJECT_SOURCE_DIR})\n";

    cmakeFile << "file(GLOB SOURCES \"*.cpp\" \"*.h\")\n";
    cmakeFile << "add_executable(" << exe_name << " ${SOURCES})\n";
    cmakeFile.close();
}


/**
 * @brief 编译指定目录中的项目
 *
 * @param sourceDir 源代码目录
 * @param buildDir 构建目录
 * @return true 编译成功
 * @return false 编译失败
 */
bool Compile::compile_project(const std::string& source_dir,
                              const std::string& build_dir,
                              const std::string& name) {
    int ret = 0; // 命令行执行指示符

    // 创建构建目录，如果不存在的话
    fs::create_directory(build_dir);

    // 生成CMake文件
    create_cmake_list(source_dir, name);

    // 检查CMake二进制文件是否存在
    if (!fs::exists(cmake_bin)) {
        std::cerr << "CMake binaries are missing. Ensure cmake directory is properly extracted." << std::endl;
        return false;
    }

    // 生成 CMake 配置命令
    std::string cmakeCommand = "\"" + cmake_bin + "\"" + " -S " + source_dir + " -B " +  build_dir;
    std::cout << cmakeCommand << std::endl;

    // 生成 CMake 构建命令
    std::string buildCommand = "\"" + cmake_bin + "\"" + " --build " + build_dir;
    std::cout << buildCommand;

    // 执行 CMake 配置命令
    ret = std::system(cmakeCommand.c_str());
    if (ret != 0) {
        std::cerr << "CMake configuration failed!" << std::endl;
        return false;
    }
    // 执行 CMake 构建命令
    ret = std::system(buildCommand.c_str());
    if (ret != 0) {
        std::cerr << "Build failed!" << std::endl;
        return false;
    }

    return true;
}

/**
 * @brief 接口函数，用于接收源代码目录和构建目录，并返回编译结果
 *
 * @param source_dir 源代码目录
 * @param build_dir 构建目录
 * @return std::string 编译结果信息
 */
std::string Compile::compile_project_interface(const std::string& source_dir,
                                               const std::string& father_build_dir,
                                               const std::string& name) {

    // 生成项目目录
    const std::string son_build_dir = father_build_dir + "/" + name;

    // 调用编译项目函数
    if (compile_project(source_dir, son_build_dir, name)) {

        // 返回生成的可执行文件路径
        std::string exe_path = son_build_dir + "/Debug/" + name + ".exe";
        return exe_path;
    }

    else {
        // 返回空字符串表示失败
        return "";
    }
}
