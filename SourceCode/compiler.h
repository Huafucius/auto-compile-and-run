#ifndef COMPILER_H
#define COMPILER_H

#include<QDir>

class Compiler
{
public:
    Compiler();
    ~Compiler();

    // 对外开放的接口，进行编译并存储在固定目录
    QString compileInterface(const QString &targetDir, const QString &basicDir, const QString &exeName);

private:
    // 生成CMake文件
    void createCmakeList();

    // 实际进行编译
    bool compileProject();

    // 删除文件的函数
    void deleteFile();

    // 检查路径是否存在的函数
    bool checkPathExists(const QString &path);

    QString cmakeBin; // CMake的路径

    QString basicDir; // 根目录
    QString historyDir; // 历史目录，也就是总项目目录
    QString projectDir; // 单个项目目录
    QString sourceCodeDir; // 源码目录
    QString exeName;
};

#endif // COMPILER_H
