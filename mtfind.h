#ifndef MTFIND_H
#define MTFIND_H

#include <vector>
#include <string>
#include <mutex>
#include <future>
#include <thread>
#include <chrono>

#include "patternsearcher.h"
#include "filereader.h"
#include "output.h"


class MtFind
{
public:
    explicit MtFind();
    bool loadAndSearch(const std::string& filename, const std::string& pattern);

    void setThreadCount(int count);

    const std::vector<Match>& getMatches() const;

    void printResults() const;
    void saveResults(const std::string& filename) const;

private:
    std::vector<Match> m_matches;
    Output m_output;
    int m_threadCount;

    size_t BLOCK_SIZE = 64 * 1024 * 1024;  // 64 MB
    const size_t MAX_PATTERN_SIZE = 100000;
    int DEFAULT_THREAD_COUNT = 2;
};

#endif // MTFIND_H
