#include "output.h"


Output::Output()
{
}

// Eliminating overlapping occurrences
std::vector<Match> Output::removeOverlapping(const std::vector<Match>& matches) const
{
    if (matches.size() <= 1)
        return matches;

    std::vector<Match> result;

    for (size_t i = 0; i < matches.size(); ++i)
    {
        const Match& current = matches[i];

        // Checking to see if it overlaps with the ones that have already been added
        bool overlapping = false;
        for (const auto& added : result)
        {
            // If both lines and positions overlap in the same block
            if (current.blockIndex == added.blockIndex &&
                current.lineInBlock == added.lineInBlock)
            {

                int currentEnd = current.posInLine + current.data.size();
                int addedEnd = added.posInLine + added.data.size();

                // Checking for an intersection
                if (!(currentEnd <= added.posInLine || current.posInLine >= addedEnd))
                {
                    overlapping = true;
                    break;
                }
            }
        }

        if (!overlapping)
            result.push_back(current);
    }
    return result;
}

void Output::printResults(const std::vector<Match>& matches) const
{
    // Removing overlapping occurrences
    std::vector<Match> nonOverlapping = removeOverlapping(matches);

    // Sorting by the order in the file
    std::sort(nonOverlapping.begin(), nonOverlapping.end(),
              [](const Match& a, const Match& b)
              {
                  if (a.blockIndex != b.blockIndex) {
                      return a.blockIndex < b.blockIndex;
                  }
                  if (a.lineInBlock != b.lineInBlock) {
                      return a.lineInBlock < b.lineInBlock;
                  }
                  return a.posInLine < b.posInLine;
              });

    // Output quantity
    std::cout << nonOverlapping.size() << std::endl;

    for (const auto& match : nonOverlapping)
    {
        std::cout << (match.lineInBlock + 1) << " "
                  << (match.posInLine + 1) << " "
                  << match.data << std::endl;
    }
}

bool Output::saveToFile(const std::vector<Match>& matches, const std::string& filename) const
{
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Cannot open file for writing: " << filename << std::endl;
        return false;
    }

    std::vector<Match> nonOverlapping = removeOverlapping(matches);

    std::sort(nonOverlapping.begin(), nonOverlapping.end(),
              [](const Match& a, const Match& b) {
                  if (a.blockIndex != b.blockIndex) return a.blockIndex < b.blockIndex;
                  if (a.lineInBlock != b.lineInBlock) return a.lineInBlock < b.lineInBlock;
                  return a.posInLine < b.posInLine;
              });

    file << nonOverlapping.size() << std::endl;
    for (const auto& match : nonOverlapping) {
        file << (match.lineInBlock + 1) << " "
             << (match.posInLine + 1) << " "
             << match.data << std::endl;
    }

    file.close();
    return true;
}
