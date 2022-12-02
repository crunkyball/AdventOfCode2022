#include <vector>
#include <algorithm>
#include <cassert>

#include "File.h"
#include "Day1.h"

namespace
{
    void ReadDataAndSort(const char* pFileName, std::vector<int>& outData)
    {
        File file(pFileName);

        outData.push_back(0);

        while (!file.IsAtEnd())
        {
            if (!file.IsLineEmpty())
            {
                int num = file.ReadLineAsNumber();
                outData[outData.size() - 1] += num;
            }
            else
            {
                outData.push_back(0);
            }

            file.NextLine();
        }

        std::sort(outData.begin(), outData.end());
    }

    int SumTopN(const std::vector<int>& arr, int num)
    {
        assert(arr.size() >= num);

        int sum = 0;

        for (int i = 0; i < num; ++i)
        {
            sum += arr[arr.size() - (i + 1)];
        }

        return sum;
    }

    void RunPuzzle1(const char* pDataFileName)
    {
        std::vector<int> elfCarryWeight;
        ReadDataAndSort(pDataFileName, elfCarryWeight);
        printf("Most calories carried: %d\n", SumTopN(elfCarryWeight, 1));
    }

    void RunPuzzle2(const char* pDataFileName)
    {
        std::vector<int> elfCarryWeight;
        ReadDataAndSort(pDataFileName, elfCarryWeight);
        printf("Combined top three: %d\n", SumTopN(elfCarryWeight, 3));
    }
}

void Day1::Run()
{
    //RunPuzzle1("Day1_Sample.txt");
    //RunPuzzle1("Day1_Puzzle.txt");

    //RunPuzzle2("Day1_Sample.txt");
    RunPuzzle2("Day1_Puzzle.txt");
}
