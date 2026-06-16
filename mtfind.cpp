#include "mtfind.h"


static bool isAscii7bit(const std::string& str)
{
    for (unsigned char c : str)
    {
        if (c > 127)
            return false;
    }
    return true;
}


MtFind::MtFind()
 : m_matches(), m_threadCount(std::thread::hardware_concurrency())
{
    if (m_threadCount == 0)
        m_threadCount = DEFAULT_THREAD_COUNT;
}

void MtFind::setThreadCount(int count)
{
    m_threadCount = (count > 0) ? count : 1;
}

bool MtFind::loadAndSearch(const std::string& filename, const std::string& pattern)
{
    if (pattern.size() > MAX_PATTERN_SIZE)
    {
        std::cerr << "Pattern is too long. Maximum length is "
                  << MAX_PATTERN_SIZE << " characters." << std::endl;
        return false;
    }

    if (pattern.find('\n') != std::string::npos)
    {
        std::cerr << "Pattern cannot contain newline character." << std::endl;
        return false;
    }

    if (pattern.empty())
    {
        std::cerr << "Pattern cannot be empty." << std::endl;
        return false;
    }

    if (!isAscii7bit(pattern))
    {
        std::cerr << "Pattern contains non-ASCII characters." << std::endl;
        return false;
    }

    FileReader reader(filename);
    if (reader.getFileSize() == 0)
    {
        std::cerr << "Cannot open or file is empty: " << filename << std::endl;
        return false;
    }

    m_matches.clear();

    std::vector<std::future<void>> futures;
    std::mutex mutex;
    std::vector<char> buffer;
    int blockIndex = 0;

    int activeThreads = 0;
    const int maxThreads = m_threadCount;

    while (reader.readNextBlock(buffer, BLOCK_SIZE))
    {
        size_t offset = reader.getCurrentPos() - buffer.size();

        // lim — waiting until a place is available
        while (activeThreads >= maxThreads)
        {
            for (auto it = futures.begin(); it != futures.end(); )
            {
                if (it->wait_for(std::chrono::milliseconds(0)) == std::future_status::ready)
                {
                    it = futures.erase(it);
                    activeThreads--;
                }
                else
                    ++it;
            }
            // Sleep if there are no free threads
            if (activeThreads >= maxThreads)
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }

        // Starting a new thread
        futures.push_back(std::async(std::launch::async,
                                     [this, &pattern, &mutex, buffer = std::move(buffer), offset, blockIndex]()
                                     {
                                         Block block;
                                         block.data.assign(buffer.data(), buffer.size());
                                         block.globalOffset = offset;
                                         block.blockIndex = blockIndex;

                                         PatternSearcher searcher(block, pattern);
                                         if (searcher.run())
                                         {
                                             auto matches = searcher.getMatches();
                                             std::lock_guard<std::mutex> lock(mutex);
                                             m_matches.insert(m_matches.end(), matches.begin(), matches.end());
                                         }
                                     }));

        activeThreads++;
        blockIndex++;
        buffer.clear();
    }

    // Waiting for all threads to complete
    for (auto& f : futures)
        f.wait();

    return !m_matches.empty();
}

const std::vector<Match>& MtFind::getMatches() const
{
    return m_matches;
}

void MtFind::printResults() const
{
    m_output.printResults(m_matches);
}

void MtFind::saveResults(const std::string& filename) const
{
    m_output.saveToFile(m_matches, filename);
}
