#include <vector>

#include "File.h"
#include "Day8.h"

namespace
{
    using TreeHeightRow = std::vector<int>;
    using TreeHeightList = std::vector<TreeHeightRow>;

    void ReadDataAndParse(const char* pDataFileName, TreeHeightList& outTreeHeights)
    {
        File file(pDataFileName);

        while (!file.IsAtEnd())
        {
            outTreeHeights.emplace_back();
            TreeHeightRow& thisRow = outTreeHeights[outTreeHeights.size() - 1];

            while (!file.IsLineEmpty())
            {
                char c = file.ReadChar();
                int i = c - '0';
                thisRow.push_back(i);

                file.NextChar();
            }

            file.NextLine();
        }
    }

    bool IsTreeVisible(const TreeHeightList& treeHeights, int x, int y)
    {
        int treeHeight = treeHeights[y][x];

        enum Dir { UP, DOWN, LEFT, RIGHT };

        for (Dir dir = UP; dir <= RIGHT; dir = static_cast<Dir>(dir + 1))
        {
            bool dirVisible = true;

            switch (dir)
            {
                case UP:
                    for (int dy = y-1; dy >= 0; --dy)
                    {
                        if (treeHeights[dy][x] >= treeHeight)
                        {
                            dirVisible = false;
                            break;
                        }
                    }
                    break;

                case DOWN:
                    for (int dy = y+1; dy < treeHeights.size(); ++dy)
                    {
                        if (treeHeights[dy][x] >= treeHeight)
                        {
                            dirVisible = false;
                            break;
                        }
                    }
                    break;

                case LEFT:
                    for (int dx = x-1; dx >= 0; --dx)
                    {
                        if (treeHeights[y][dx] >= treeHeight)
                        {
                            dirVisible = false;
                            break;
                        }
                    }
                    break;

                case RIGHT:
                    for (int dx = x+1; dx < treeHeights.size(); ++dx)
                    {
                        if (treeHeights[y][dx] >= treeHeight)
                        {
                            dirVisible = false;
                            break;
                        }
                    }
                    break;
            }

            if (dirVisible)
            {
                return true;
            }
        }

        return false;
    }

    int FindVisibleTrees(const TreeHeightList& treeHeights)
    {
        int numVisible = 0;

        for (int y = 0; y < treeHeights.size(); ++y)
        {
            for (int x = 0; x < treeHeights[y].size(); ++x)
            {
                if (IsTreeVisible(treeHeights, x, y))
                {
                    numVisible++;
                }
            }
        }

        return numVisible;
    }

    int FindScenicScore(const TreeHeightList& treeHeights, int x, int y)
    {
        int treeHeight = treeHeights[y][x];

        int scenicScore = 1;

        enum Dir { UP, DOWN, LEFT, RIGHT };

        for (Dir dir = UP; dir <= RIGHT; dir = static_cast<Dir>(dir + 1))
        {
            int viewDistance = 0;

            switch (dir)
            {
                case UP:
                    for (int dy = y - 1; dy >= 0; --dy)
                    {
                        viewDistance++;

                        if (treeHeights[dy][x] >= treeHeight)
                        {
                            break;
                        }
                    }
                    break;

                case DOWN:
                    for (int dy = y + 1; dy < treeHeights.size(); ++dy)
                    {
                        viewDistance++;

                        if (treeHeights[dy][x] >= treeHeight)
                        {
                            break;
                        }
                    }
                    break;

                case LEFT:
                    for (int dx = x - 1; dx >= 0; --dx)
                    {
                        viewDistance++;

                        if (treeHeights[y][dx] >= treeHeight)
                        {
                            break;
                        }
                    }
                    break;

                case RIGHT:
                    for (int dx = x + 1; dx < treeHeights.size(); ++dx)
                    {
                        viewDistance++;

                        if (treeHeights[y][dx] >= treeHeight)
                        {
                            break;
                        }
                    }
                    break;
            }

            scenicScore *= viewDistance;
        }

        return scenicScore;
    }

    int FindHighestScenicScore(const TreeHeightList& treeHeights)
    {
        int highestScenicScore = 0;

        for (int y = 0; y < treeHeights.size(); ++y)
        {
            for (int x = 0; x < treeHeights[y].size(); ++x)
            {
                int scenicScore = FindScenicScore(treeHeights, x, y);

                if (scenicScore > highestScenicScore)
                {
                    highestScenicScore = scenicScore;
                }
            }
        }

        return highestScenicScore;
    }

    void PrintVisible(const TreeHeightList& treeHeights)
    {
        for (int y = 0; y < treeHeights.size(); ++y)
        {
            for (int x = 0; x < treeHeights[y].size(); ++x)
            {
                if (IsTreeVisible(treeHeights, x, y))
                {
                    printf("X ");
                }
                else
                {
                    printf("_ ");
                }
            }

            printf("\n");
        }
    }

    void RunPuzzle1(const char* pDataFileName)
    {
        TreeHeightList treeHeights;
        ReadDataAndParse(pDataFileName, treeHeights);

        int visibleTrees = FindVisibleTrees(treeHeights);

        //PrintVisible(treeHeights);

        printf("Visible Trees: %d\n", visibleTrees);
    }

    void RunPuzzle2(const char* pDataFileName)
    {
        TreeHeightList treeHeights;
        ReadDataAndParse(pDataFileName, treeHeights);

        int scenicScore = FindHighestScenicScore(treeHeights);

        printf("Highest Scenic Score: %d\n", scenicScore);
    }
}

void Day8::Run()
{
    //RunPuzzle1("Day8_Sample.txt");
    //RunPuzzle1("Day8_Puzzle.txt");

    //RunPuzzle2("Day8_Sample.txt");
    RunPuzzle2("Day8_Puzzle.txt");
}
