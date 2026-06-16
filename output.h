#ifndef OUTPUT_H
#define OUTPUT_H

#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>

#include "patternsearcher.h"

class Output
{
public:
    Output();

    void printResults(const std::vector<Match>& matches) const;
    bool saveToFile(const std::vector<Match>& matches, const std::string& filename) const;
    std::vector<Match> removeOverlapping(const std::vector<Match>& matches) const;
};

#endif // OUTPUT_H
