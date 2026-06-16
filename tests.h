#ifndef TESTS_H
#define TESTS_H

#include <iostream>
#include <string>
#include <vector>

#include "patternsearcher.h"
#include "output.h"
#include "filereader.h"

using namespace std;


#define ASSERT_EQUAL(a, b) \
if ((a) != (b)) { \
        cerr << "ASSERT_EQUAL failed: " << #a << " != " << #b \
        << " (" << (a) << " != " << (b) << ")" << endl; \
        abort(); \
}

#define ASSERT_TRUE(expr) \
if (!(expr)) { \
        cerr << "ASSERT_TRUE failed: " << #expr << " is false" << endl; \
        abort(); \
}

#define ASSERT_FALSE(expr) \
if (expr) { \
        cerr << "ASSERT_FALSE failed: " << #expr << " is true" << endl; \
        abort(); \
}

#define RUN_TEST(func) \
cerr << "Running " << #func << "... "; \
    func(); \
    cerr << "OK" << endl;

// Tests for PatternSearcher


void TestPatternSearcherExactMatch()
{
    Block block;
    block.data = "Hello world\nThis is a test\n";
    block.blockIndex = 0;
    block.globalOffset = 0;

    PatternSearcher searcher(block, "world");
    ASSERT_TRUE(searcher.run());

    auto matches = searcher.getMatches();
    ASSERT_EQUAL(matches.size(), 1u);
    ASSERT_EQUAL(matches[0].lineInBlock, 0);
    ASSERT_EQUAL(matches[0].posInLine, 6);
    ASSERT_EQUAL(matches[0].data, "world");
}

void TestPatternSearcherWithWildcard()
{
    Block block;
    block.data = "bad mad had sad\n";
    block.blockIndex = 0;
    block.globalOffset = 0;

    PatternSearcher searcher(block, "?ad");
    ASSERT_TRUE(searcher.run());

    auto matches = searcher.getMatches();
    ASSERT_EQUAL(matches.size(), 4u);
    ASSERT_EQUAL(matches[0].data, "bad");
    ASSERT_EQUAL(matches[1].data, "mad");
    ASSERT_EQUAL(matches[2].data, "had");
    ASSERT_EQUAL(matches[3].data, "sad");
}

void TestPatternSearcherMultipleLines()
{
    Block block;
    block.data = "I live in Moscow.\nMoscow is a beautiful city.\n";
    block.blockIndex = 0;
    block.globalOffset = 0;

    PatternSearcher searcher(block, "?oscow");  // ищем "Moscow" (M = любой символ)
    ASSERT_TRUE(searcher.run());

    auto matches = searcher.getMatches();
    ASSERT_EQUAL(matches.size(), 2u);  // "Moscow" в обеих строках
    ASSERT_EQUAL(matches[0].lineInBlock, 0);
    ASSERT_EQUAL(matches[0].data, "Moscow");
    ASSERT_EQUAL(matches[1].lineInBlock, 1);
    ASSERT_EQUAL(matches[1].data, "Moscow");
}

void TestPatternSearcherNoMatch()
{
    Block block;
    block.data = "Hello world\nThis is a test\n";
    block.blockIndex = 0;
    block.globalOffset = 0;

    PatternSearcher searcher(block, "xyz");
    ASSERT_FALSE(searcher.run());

    auto matches = searcher.getMatches();
    ASSERT_EQUAL(matches.size(), 0u);
}

void TestPatternSearcherEmptyPattern()
{
    Block block;
    block.data = "Hello world\n";
    block.blockIndex = 0;
    block.globalOffset = 0;

    PatternSearcher searcher(block, "");
    ASSERT_FALSE(searcher.run());
}

void TestPatternSearcherLongPattern()
{
    Block block;
    block.data = "My dog and my cat are best friends.\n";
    block.blockIndex = 0;
    block.globalOffset = 0;

    PatternSearcher searcher(block, "dog and my cat");
    ASSERT_TRUE(searcher.run());

    auto matches = searcher.getMatches();
    ASSERT_EQUAL(matches.size(), 1u);
    ASSERT_EQUAL(matches[0].data, "dog and my cat");
}

void TestPatternSearcherMultipleMatchesSameLine()
{
    Block block;
    block.data = "bad and mad and had\n";
    block.blockIndex = 0;
    block.globalOffset = 0;

    PatternSearcher searcher(block, "?ad");
    ASSERT_TRUE(searcher.run());

    auto matches = searcher.getMatches();
    ASSERT_EQUAL(matches.size(), 3u);
    ASSERT_EQUAL(matches[0].posInLine, 0);
    ASSERT_EQUAL(matches[1].posInLine, 8);
    ASSERT_EQUAL(matches[2].posInLine, 16);
}

// Tests for Output

void TestOutputRemoveOverlapping()
{
    Output output;

    std::vector<Match> matches;

    Match m1;
    m1.blockIndex = 0;
    m1.lineInBlock = 0;
    m1.posInLine = 0;
    m1.data = "abc";

    Match m2;
    m2.blockIndex = 0;
    m2.lineInBlock = 0;
    m2.posInLine = 1;
    m2.data = "bcd";

    matches.push_back(m1);
    matches.push_back(m2);

    std::vector<Match> result = output.removeOverlapping(matches);
    ASSERT_EQUAL(result.size(), 1u);
}

void TestOutputNoOverlapping()
{
    Output output;

    std::vector<Match> matches;

    Match m1;
    m1.blockIndex = 0;
    m1.lineInBlock = 0;
    m1.posInLine = 0;
    m1.data = "abc";

    Match m2;
    m2.blockIndex = 0;
    m2.lineInBlock = 1;
    m2.posInLine = 0;
    m2.data = "xyz";

    matches.push_back(m1);
    matches.push_back(m2);

    std::vector<Match> result = output.removeOverlapping(matches);
    ASSERT_EQUAL(result.size(), 2u);
}

void TestOutputDifferentBlocks()
{
    Output output;

    std::vector<Match> matches;

    Match m1;
    m1.blockIndex = 0;
    m1.lineInBlock = 0;
    m1.posInLine = 0;
    m1.data = "abc";

    Match m2;
    m2.blockIndex = 1;
    m2.lineInBlock = 0;
    m2.posInLine = 0;
    m2.data = "xyz";

    matches.push_back(m1);
    matches.push_back(m2);

    std::vector<Match> result = output.removeOverlapping(matches);
    ASSERT_EQUAL(result.size(), 2u);
}

// Tests for FileReader

void TestFileReaderOpen()
{
    std::ofstream testFile("test_temp.txt");
    testFile << "Hello world\nTest line 2\n";
    testFile.close();

    FileReader reader("test_temp.txt");
    ASSERT_TRUE(reader.getFileSize() > 0);
    ASSERT_FALSE(reader.isEof());
}

void TestFileReaderReadBlock()
{
    std::ofstream testFile("test_temp2.txt");
    testFile << "Line 1\nLine 2\nLine 3\n";
    testFile.close();

    FileReader reader("test_temp2.txt");
    std::vector<char> buffer;

    ASSERT_TRUE(reader.readNextBlock(buffer, 1024));
    ASSERT_TRUE(buffer.size() > 0);
}

// Running all the tests

void TestPatternSearcher()
{
    RUN_TEST(TestPatternSearcherExactMatch);
    RUN_TEST(TestPatternSearcherWithWildcard);
    RUN_TEST(TestPatternSearcherMultipleLines);
    RUN_TEST(TestPatternSearcherNoMatch);
    RUN_TEST(TestPatternSearcherEmptyPattern);
    RUN_TEST(TestPatternSearcherLongPattern);
    RUN_TEST(TestPatternSearcherMultipleMatchesSameLine);
}

void TestOutputFunctions()
{
    RUN_TEST(TestOutputRemoveOverlapping);
    RUN_TEST(TestOutputNoOverlapping);
    RUN_TEST(TestOutputDifferentBlocks);
}

void TestFileReaderFunctions()
{
    RUN_TEST(TestFileReaderOpen);
    RUN_TEST(TestFileReaderReadBlock);
}

void RunAllTests()
{
    cerr << "\n------ Running Tests ------\n" << endl;

    TestPatternSearcher();
    TestOutputFunctions();
    TestFileReaderFunctions();

    cerr << "\n------ All tests passed! ------\n" << endl;
}

#endif // TESTS_H
