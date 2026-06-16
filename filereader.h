#ifndef FILEREADER_H
#define FILEREADER_H

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>


struct Block
{
    std::string data;
    size_t globalOffset;   // Offset from the beginning of the file
    int blockIndex;        // Block number (index in the general list)

    Block() : globalOffset(0), blockIndex(-1) {}
    Block(const char* ptr, size_t sz, size_t off, int idx = -1)
        : globalOffset(off), blockIndex(idx)
    {
        data.assign(ptr, ptr + sz);
    }
};


class FileReader
{
public:
    FileReader(const std::string& filename);
    ~FileReader();

    bool readNextBlock(std::vector<char>& buffer, size_t blockSize);

    size_t getCurrentPos() const;
    size_t getFileSize() const;
    bool isEof() const;

private:
    std::ifstream m_file;
    size_t m_fileSize;
    size_t m_currentPos;
    bool m_eof;
};

#endif // FILEREADER_H
