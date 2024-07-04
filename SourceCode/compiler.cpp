#include "compiler.h"
#include <QProcess>
#include <QTextStream>
#include <QDebug>
#include <QString>
#include <fstream>
#include <filesystem>
#include <string>
#include <QDir>
#include <QDebug>

namespace fs = std::filesystem;

Compiler::Compiler() {
    cmakeBin = QDir::currentPath() + "/CMake/bin/cmake.exe";
}

Compiler::~Compiler() {

}

// 删除文件夹的函数，在编译出现问题，或者重编译时调用
void Compiler::deleteFile() {
    // 删除目录
    std::error_code ec;
    if (fs::remove_all(projectDir.toStdString(), ec)) {
        qDebug() << "目录已删除：" << projectDir;
    } else {
        qWarning() << "目录删除失败： " << projectDir;
        qWarning() << "错误信息： " << QString::fromStdString(ec.message());
    }
}

// 重建文件路径的函数。如果之前存在就重建，如果不存在就新建。
bool recreatePath(const QString &path) {
    QFileInfo checkFile(path);
    QDir dir(path);

    // 检查路径是否存在
    if (checkFile.exists()) {
        // 如果路径存在，删除路径
        if (checkFile.isDir()) {
            if (!dir.removeRecursively()) {
                qWarning() << "删除路径失败:" << path;
                return false;
            }
        }
    }

    // 创建新的路径
    if (!dir.mkpath(path)) {
        qWarning() << "新建路径失败:" << path;
        return false;
    }

    qDebug() << "新建路径成功:" << path;
    return true;
}

// 生成CMake文件
void Compiler::createCmakeList() {

    QString MinGcc_str = basicDir + "/mingw64/bin/gcc.exe";
    QString MinGpp_str = basicDir + "/mingw64/bin/g++.exe";
    QString cmakeFilePath = sourceCodeDir + "/CMakeLists.txt";

    // 卫语句
    QFile file(cmakeFilePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qWarning() << "无法创建并打开CMakeLists.txt文件：" << cmakeFilePath;
        return;
    }

    QTextStream out(&file);
    out.setAutoDetectUnicode(true); // 自动检测编码

    out << "cmake_minimum_required(VERSION 3.10)\n";
    out << "project(" << exeName << ")\n";
    out << "set(CMAKE_CXX_STANDARD 17)\n";
    out << "include_directories(${PROJECT_SOURCE_DIR})\n";
    out << "file(GLOB SOURCES \"*.cpp\" \"*.h\")\n";

    // 添加对 UTF-8 编码的支持
    out << "if (CMAKE_CXX_COMPILER_ID STREQUAL \"GNU\")\n";
    out << "    set(CMAKE_CXX_FLAGS \"${CMAKE_CXX_FLAGS} -fexec-charset=UTF-8 -finput-charset=UTF-8\")\n";
    out << "elseif (CMAKE_CXX_COMPILER_ID STREQUAL \"MSVC\")\n";
    out << "    add_compile_options(/source-charset:utf-8 /execution-charset:utf-8)\n";
    out << "endif()\n";

    // 指定 GNU 编译器路径
    out << "set(CMAKE_C_COMPILER \"" << MinGcc_str << "\")\n";
    out << "set(CMAKE_CXX_COMPILER \"" << MinGpp_str << "\")\n";

    out << "add_executable(" << exeName << " ${SOURCES})\n";

    file.close();

    if (file.error()) {
        qWarning() << "写入CMakeLists.txt文件失败：" << cmakeFilePath;
    }
}

bool Compiler::compileProject() {

    // 创建构建目录，如果不存在的话
    if (!recreatePath(projectDir)) {
        return false;
    }

    // 在目录下生成CMake文件
    createCmakeList();

    // 卫语句：检查CMake二进制文件是否存在
    if (!fs::exists(cmakeBin.toStdString())) {
        qWarning() << "您是否缺失了CMake？";
        deleteFile(); // 如果失败就删除目录
        return false;
    }

    // 生成 CMake 配置命令，使用双引号包裹路径，以处理路径中的空格和特殊字符
    QString cmakeCommand = QString("\"%1\" -S \"%2\" -B \"%3\"").arg(cmakeBin, sourceCodeDir, projectDir);
    qDebug() << cmakeCommand; // 演示用

    // 生成 CMake 构建命令，使用双引号包裹路径，以处理路径中的空格和特殊字符
    QString buildCommand = QString("\"%1\" --build \"%2\"").arg(cmakeBin, projectDir);
    qDebug() << buildCommand; // 演示用

    // 执行 CMake 配置命令
    QProcess process;
    process.start(cmakeCommand);
    process.waitForFinished();
    if (process.exitCode() != 0) {
        qWarning() << "CMake配置失败！";
        qWarning() << "输出信息: " << process.readAllStandardOutput();
        qWarning() << "错误信息: " << process.readAllStandardError();
        deleteFile(); // 如果失败就删除目录
        return false;
    }

    // 执行 CMake 构建命令
    process.start(buildCommand);
    process.waitForFinished();
    if (process.exitCode() != 0) {
        qWarning() << "项目构建失败！";
        qWarning() << "输出信息: " << process.readAllStandardOutput();
        qWarning() << "错误信息: " << process.readAllStandardError();
        deleteFile(); // 如果失败就删除目录
        return false;
    }
    return true;
}

QString Compiler::compileInterface(const QString& targetDir, const QString& baseDir, const QString& name) {

    // 初始化一些值
    sourceCodeDir = targetDir;
    basicDir = baseDir;
    exeName = name;
    historyDir = basicDir + "/history";
    projectDir = QString("%1/%2").arg(historyDir, exeName);


    // 卫语句：调用编译函数，并且返回空字符串表示失败
    if (!compileProject()) {
        return "";
    }

    // 返回生成的可执行文件路径
    QString exePath = QString("%1/Debug/%2.exe").arg(projectDir, exeName);
    return exePath;
}
