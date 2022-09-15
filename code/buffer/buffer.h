#ifndef BUFFER_H
#define BUFFER_H

#include <cstring>
#include <iostream>
#include <unistd.h>
#include <sys/uio.h>
#include <vector>
#include <atomic>
#include <assert.h>


/*
利用标准库容器封装char，实现自动增长的缓冲区；
利用正则与状态机解析HTTP请求报文，实现处理静态资源的请求；
*/


class Buffer{
public:
    Buffer(int initBuffSize=1024);
    ~Buffer()=default;
    //类的成员函数后面加 const，表明这个函数不会对这个类对象的数据成员（准确地说是非静态数据成员）作任何改变。
    size_t WritableBytes() const;  //可写的字节数
    size_t ReadableBytes() const;  //可读的字节数
    size_t PrependableBytes() const; //可以扩展的字节数

    const char* Peek() const;
    void EnsureWriteable(size_t len); //整理或申请足够的空间
    void HasWritten(size_t len);

    void Retrieve(size_t len);
    void RetrieveUntil(const char *end);

    void RetrieveAll();
    std::string RetrieveAllToStr();
    
    const char* BeginWriteConst() const;
    char* BeginWrite();

    void Append(const std::string& str);
    void Append(const char* str, size_t len);
    void Append(const void* data,size_t len);
    void Append(const Buffer& buff);

    //ssize_t是有符号整型 size_t 就是无符号型的ssize_t
    ssize_t ReadFd(int fd, int* Errno);
    ssize_t WriteFd(int fd, int* Errno);

private:
    //下划线表示私有变量
    char* BeginPtr_();
    const char* BeginPtr_() const;
    void MakeSpace_(size_t len); //创建新的空间

    std::vector<char> buffer_; //具体装数据的缓存
    //std::atomic对int, char, bool等数据结构进行原子性封装，在多线程环境中，对std::atomic对象的访问不会造成竞争-冒险。利用std::atomic可实现数据结构的无锁设计。
    std::atomic<std::size_t> readPos_; //读的位置
    std::atomic<std::size_t> writePos_; //写的位置


};

#endif //BUFFER_H