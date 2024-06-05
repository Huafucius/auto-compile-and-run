#include <iostream>
#include <filesystem>
#include <cstdlib>
#include <string>

namespace fs = std::filesystem;

/**
 * @brief 编译指定目录中的项目
 * 
 * @param sourceDir 源代码目录
 * @param buildDir 构建目录
 * @return true 编译成功
 * @return false 编译失败
 */
bool compileProject(const std::string& sourceDir, const std::string& buildDir) {
    // 创建构建目录，如果不存在的话
    fs::create_directory(buildDir);

    // 设置CMake的路径
    std::string cmakeDir = "cmake";
    std::string cmakeBin = cmakeDir + "/bin/cmake.exe";

    // 检查CMake二进制文件是否存在
    if (!fs::exists(cmakeBin)) {
        std::cerr << "CMake binaries are missing. Ensure cmake directory is properly extracted." << std::endl;
        return false;
    }

    // 生成 CMake 配置命令
    std::string cmakeCommand = cmakeBin + " -S " + sourceDir + " -B " + buildDir;
    // 生成 CMake 构建命令
    std::string buildCommand = cmakeBin + " --build " + buildDir;

    // 执行 CMake 配置命令
    int ret = std::system(cmakeCommand.c_str());
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
 * @param sourceDir 源代码目录
 * @param buildDir 构建目录
 * @return std::string 编译结果信息
 */
std::string compileProjectInterface(const std::string& sourceDir, const std::string& buildDir) {
    // 调用编译项目函数
    if (compileProject(sourceDir, buildDir)) {
        // 返回成功信息和生成的可执行文件路径
        return "Project compiled successfully!\nExecutable is located at: " + buildDir + "/Debug/MyExampleProject.exe";
    } else {
        // 返回失败信息
        return "Compilation failed!";
    }
}

int main() {
    // 示例源代码目录和构建目录，可以通过实际接口传递参数
    std::string sourceDir = "src"; // 示例源代码目录
    std::string buildDir = "build"; // 示例构建目录

    // 调用接口函数，获取编译结果
    std::string result = compileProjectInterface(sourceDir, buildDir);
    
    // 输出编译结果
    std::cout << result << std::endl;

    return 0;
}
