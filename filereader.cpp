#include "filereader.h"


FileReader::FileReader(const std::string& filename)
    : m_fileSize(0), m_currentPos(0), m_eof(false)
{
    m_file.open(filename, std::ios::binary);
    if (!m_file.is_open())
    {
        std::cerr << "Cannot open file: " << filename << std::endl;
        return;
    }

    // File Size
    m_file.seekg(0, std::ios::end);
    m_fileSize = m_file.tellg();
    m_file.seekg(0, std::ios::beg);
}

FileReader::~FileReader()
{
    if (m_file.is_open())
        m_file.close();
}

bool FileReader::readNextBlock(std::vector<char>& buffer, size_t blockSize)
{
    if (!m_file.is_open() || m_eof)
        return false;

    // How much is left to read
    size_t remaining = m_fileSize - m_currentPos;
    size_t toRead = std::min(blockSize, remaining);

    if (toRead == 0)
    {
        m_eof = true;
        return false;
    }

    buffer.resize(toRead);
    m_file.read(buffer.data(), toRead);

    size_t actuallyRead = m_file.gcount();
    if (actuallyRead == 0)
    {
        m_eof = true;
        return false;
    }

    buffer.resize(actuallyRead);

// Normalization of line ends
#ifdef _WIN32
    {
        std::string str(buffer.data(), buffer.size());
        size_t pos = 0;
        while ((pos = str.find("\r\n", pos)) != std::string::npos) {
            str.replace(pos, 2, "\n");
            pos += 1;
        }
        buffer.assign(str.begin(), str.end());
    }
#else
    {
        auto newEnd = std::remove(buffer.begin(), buffer.end(), '\r');
        buffer.erase(newEnd, buffer.end());
    }
#endif

    m_currentPos += actuallyRead;

    if (m_currentPos >= m_fileSize)
        m_eof = true;

    return true;
}

size_t FileReader::getCurrentPos() const
{
    return m_currentPos;
}

size_t FileReader::getFileSize() const
{
    return m_fileSize;
}

bool FileReader::isEof() const
{
    return m_eof;
}
