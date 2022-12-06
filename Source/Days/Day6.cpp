#include <cstring>
#include <cassert>
#include <deque>

#include "File.h"
#include "Day6.h"

namespace
{
    const char* kSample1 = "mjqjpqmgbljsphdztnvjfqwrcgsmlb";
    const char* kSample2 = "bvwbjplbgvbhsrlpgdmjqwftvncz";
    const char* kSample3 = "nppdvjthqldpwncqszvftbrmjlhg";
    const char* kSample4 = "nznrnfrfntjfmvfwmzdfjlvtqnbhcprsg";
    const char* kSample5 = "zcfzfwzzqfrljwzlrfnpqdbhtmscgvjw";

    int FindMarker(const char* pStr, int numDistinctChars)
    {
        int len = static_cast<int>(strlen(pStr));

        for (int i = numDistinctChars - 1; i < len; ++i)
        {
            int letters[26];
            memset(letters, 0, sizeof(letters));

            for (int j = 0; j < numDistinctChars; ++j)
            {
                char c = pStr[i - j];
                int letterIdx = c - 'a';
                letters[letterIdx]++;
            }

            bool bDupe = false;

            for (int num : letters)
            {
                if (num > 1)
                {
                    bDupe = true;
                    break;
                }
            }

            if (!bDupe)
            {
                return i + 1;
            }
        }

        assert(0);
        return -1;
    }

    void RunPuzzle1(const char* pStr)
    {
        int marker = FindMarker(pStr, 4);
        printf("Marker: %d\n", marker);
    }

    void RunPuzzle2(const char* pStr)
    {
        int marker = FindMarker(pStr, 14);
        printf("Marker: %d\n", marker);
    }

    void RunPuzzle1File(const char* pDataFileName)
    {
        const int kPuzzleInputSize = 4096;
        char puzzleInputBuf[kPuzzleInputSize];

        File file(pDataFileName);

        file.ReadLine(puzzleInputBuf, kPuzzleInputSize);

        RunPuzzle1(puzzleInputBuf);
    }

    void RunPuzzle2File(const char* pDataFileName)
    {
        const int kPuzzleInputSize = 4096;
        char puzzleInputBuf[kPuzzleInputSize];

        File file(pDataFileName);

        file.ReadLine(puzzleInputBuf, kPuzzleInputSize);

        RunPuzzle2(puzzleInputBuf);
    }
}

void Day6::Run()
{  
    /*RunPuzzle1(kSample1);
    RunPuzzle1(kSample2);
    RunPuzzle1(kSample3);
    RunPuzzle1(kSample4);
    RunPuzzle1(kSample5);*/
    
    //RunPuzzle1File("Day6_Puzzle.txt");

    /*RunPuzzle2(kSample1);
    RunPuzzle2(kSample2);
    RunPuzzle2(kSample3);
    RunPuzzle2(kSample4);
    RunPuzzle2(kSample5);*/

    RunPuzzle2File("Day6_Puzzle.txt");
}
