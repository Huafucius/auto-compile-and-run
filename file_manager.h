#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

#include <string>
#include <vector>

// 文件管理类，用于管理文件的版本历史记录
class FileManager {
public:
    // 构造函数，接受一个用于存储文件的目录路径
    FileManager(const std::string &directory);

    // 保存文件，将文件复制到存储目录并添加时间戳
    void saveFile(const std::string &filePath, const std::string &fileName);

    // 列出指定文件的所有历史版本
    std::vector<std::string> listFileHistory(const std::string &fileName);

    // 读取指定版本的文件内容
    std::string readFile(const std::string &fileName, const std::string &version);

private:
    // 获取当前时间的时间戳，用于版本管理
    std::string getTimestamp();

    std::string directory; // 存储文件的目录路径
};

#endif // FILE_MANAGER_H
