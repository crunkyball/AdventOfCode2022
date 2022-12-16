#include <vector>
#include <cassert>

#include "File.h"
#include "Utils.h"
#include "Day14.h"

namespace
{
    enum class CavePoint
    {
        EMPTY,
        ROCK,
        SAND
    };

    using CaveMap = std::vector<std::vector<CavePoint>>;

    void ReadDataAndParse(const char* pDataFileName, CaveMap& outCaveMap)
    {
        File file(pDataFileName);

        while (!file.IsAtEnd())
        {
            const int kBufSize = 512;
            char buf[kBufSize];
            file.ReadLine(buf, kBufSize);

            const int kTokenBufSize = 16;
            const int kNumTokensSize = 32;
            char tokens[kNumTokensSize][kTokenBufSize];

            std::vector<std::pair<int, int>> lineCoords;

            int numTokens = Utils::Tokenise(buf, " -> ", &tokens[0][0], kNumTokensSize, kTokenBufSize);
            for (int i = 0; i < numTokens; ++i)
            {
                const int kCoordsBufSize = 8;
                char xBuf[kCoordsBufSize];
                char yBuf[kCoordsBufSize];

                Utils::SplitString(tokens[i], ",", xBuf, kCoordsBufSize, yBuf, kCoordsBufSize);

                int xPos = atoi(xBuf);
                int yPos = atoi(yBuf);

                assert(yPos <= outCaveMap.size());
                assert(xPos <= outCaveMap[yPos].size());

                lineCoords.push_back({ xPos, yPos });
            }

            for (int i = 1; i < lineCoords.size(); ++i)
            {
                bool horizontal = lineCoords[i-1].second == lineCoords[i].second;
                assert(horizontal || lineCoords[i-1].first == lineCoords[i].first);

                if (horizontal)
                {
                    int dir = lineCoords[i].first < lineCoords[i-1].first ? -1 : 1;
                    for (int x = lineCoords[i-1].first; ; x += dir)
                    {
                        assert(lineCoords[i-1].second == lineCoords[i].second);
                        outCaveMap[lineCoords[i].second][x] = CavePoint::ROCK;

                        if (x == lineCoords[i].first)
                        {
                            break;
                        }
                    }
                }
                else
                {
                    int dir = lineCoords[i].second < lineCoords[i-1].second ? -1 : 1;
                    for (int y = lineCoords[i-1].second; ; y += dir)
                    {
                        assert(lineCoords[i-1].first == lineCoords[i].first);
                        outCaveMap[y][lineCoords[i].first] = CavePoint::ROCK;

                        if (y == lineCoords[i].second)
                        {
                            break;
                        }
                    }
                }
            }

            file.NextLine();
        }
    }

    void DrawMap(const CaveMap& caveMap, int xOffset, int yOffest, int width, int height)
    {
        for (int y = yOffest; y < yOffest + height; ++y)
        {
            for (int x = xOffset; x < xOffset + width; ++x)
            {
                printf("%s", caveMap[y][x] == CavePoint::ROCK ? "#" : caveMap[y][x] == CavePoint::EMPTY ? "." : "O");
            }

            printf("\n");
        }
    }

    int SimulateSand(CaveMap& caveMap, int sourceX, int sourceY, int rockBottom)
    {
        for (int i = 0; ; ++i)
        {
            int currentX = sourceX;
            int currentY = sourceY;

            while (1)
            {
                int nextY = currentY + 1;

                if (nextY > rockBottom)
                {
                    return i;
                }

                if (caveMap[nextY][currentX] == CavePoint::EMPTY)
                {
                    currentY = nextY;
                    continue;
                }
                else if ((currentX - 1) >= 0 && caveMap[nextY][currentX - 1] == CavePoint::EMPTY)
                {
                    currentY = nextY;
                    currentX = currentX - 1;
                    continue;
                }
                else if ((currentX + 1) < caveMap[nextY].size() && caveMap[nextY][currentX + 1] == CavePoint::EMPTY)
                {
                    currentY = nextY;
                    currentX = currentX + 1;
                    continue;
                }

                //Can't move anymore.
                caveMap[currentY][currentX] = CavePoint::SAND;

                break;
            }
        }

        return -1;
    }

    void RunPuzzle1(const char* pDataFileName)
    {
        static const int kCaveWidth = 1024;
        static const int kCaveHeight = 256;

        CaveMap caveMap;

        for (int y = 0; y < kCaveHeight; ++y)
        {
            caveMap.emplace_back();

            for (int x = 0; x < kCaveWidth; ++x)
            {
                caveMap[caveMap.size() - 1].push_back(CavePoint::EMPTY);
            }
        }

        ReadDataAndParse(pDataFileName, caveMap);

        int rockBottom = 0;

        for (int y = 0; y < caveMap.size(); ++y)
        {
            for (int x = 0; x < caveMap[y].size(); ++x)
            {
                if (caveMap[y][x] == CavePoint::ROCK && y > rockBottom)
                {
                    rockBottom = y;
                }
            }
        }

        int stepsToEnd = SimulateSand(caveMap, 500, 0, rockBottom);

        printf("Steps to end: %d\n", stepsToEnd);

        //DrawMap(caveMap, 494, 0, 10, 10);
    }

    void RunPuzzle2(const char* pDataFileName)
    {
    }
}

void Day14::Run()
{
    //RunPuzzle1("Day14_Sample.txt");
    //RunPuzzle1("Day14_Puzzle.txt");

    RunPuzzle2("Day14_Sample.txt");
    //RunPuzzle2("Day14_Puzzle.txt");
}
