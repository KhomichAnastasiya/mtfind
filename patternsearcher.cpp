#include "patternsearcher.h"

PatternSearcher::PatternSearcher(Block& block, const std::string& pattern)
    : m_block(block), m_pattern(pattern)
{
}

bool PatternSearcher::run()
{
    m_matchList.clear();

    if (m_pattern.empty())
        return false;

    std::string data = m_block.data;
    int lineIndex = 0;

    while (!data.empty())
    {
        int pos = data.find('\n');
        std::string line;

        if (pos == -1)
        {
            line = data;
            data.clear();
        }
        else
        {
            line = data.substr(0, pos);
            data.erase(0, pos + 1);
        }

        checkPatternInLine(line, lineIndex);
        lineIndex++;
    }
    return !m_matchList.empty();
}

void PatternSearcher::checkPatternInLine(const std::string& line, int lineIndex)
{
    if (m_pattern.size() > line.size())
        return;

    for (size_t indexLine = 0; indexLine <= line.size() - m_pattern.size(); ++indexLine)
    {
        bool match = true;

        for (size_t indexPattern = 0; indexPattern < m_pattern.size(); ++indexPattern)
        {
            if (m_pattern[indexPattern] != '?' && m_pattern[indexPattern] != line[indexLine + indexPattern])
            {
                match = false;
                break;
            }
        }

        if (match)
        {
            Match m;
            m.blockIndex = m_block.blockIndex;
            m.lineInBlock = lineIndex;
            m.posInLine = indexLine;
            m.data = line.substr(indexLine, m_pattern.length());
            m_matchList.push_back(m);
        }
    }
}

const std::vector<Match>& PatternSearcher::getMatches() const
{
    return m_matchList;
}
