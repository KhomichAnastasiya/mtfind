#ifndef PATTERNSEARCHER_H
#define PATTERNSEARCHER_H

#include "filereader.h"


struct Match
{
    int blockIndex;      // Block number (block index in the general list)
    int lineInBlock;     // The number of the sentence/line in the block (starting from 0)
    int posInLine;       // Position in the sentence (starting from 0)
    std::string data;    // The found text that matches the pattern
};


class PatternSearcher
{
public:
    PatternSearcher(Block &block, const std::string& pattern);
    bool run();
    const std::vector<Match>& getMatches() const;

private:
    Block m_block;
    std::string m_pattern;
    std::vector<Match> m_matchList;

    void checkPatternInLine(const std::string& line, int lineIndex);
};

#endif // PATTERNSEARCHER_H
