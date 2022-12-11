#include <vector>
#include <cassert>
#include <functional>

#include "File.h"
#include "Utils.h"
#include "Day10.h"

namespace
{
    enum class OpType
    {
        NOOP,
        ADDX
    };

    struct Register
    {
        int X = 1;
    } Registers;

    using OpPair = std::pair<OpType, int>;
    using ProgramList = std::vector<OpPair>;

    void ReadDataAndParse(const char* pDataFileName, ProgramList& outProgramList)
    {
        File file(pDataFileName);

        while (!file.IsAtEnd())
        {
            const int kBufSize = 16;
            char lineBuf[kBufSize];
            file.ReadLine(lineBuf, kBufSize);

            const int kSplitBufSize = 8;
            char opBuf[kSplitBufSize];
            char paramBuf[kSplitBufSize];

            Utils::SplitString(lineBuf, " ", opBuf, kSplitBufSize, paramBuf, kSplitBufSize);

            if (strcmp(opBuf, "noop") == 0)
            {
                outProgramList.push_back({ OpType::NOOP,0 });
            }
            else if (strcmp(opBuf, "addx") == 0)
            {
                int param = atoi(paramBuf);
                outProgramList.push_back({ OpType::ADDX, param });
            }
            else
            {
                assert(0);  //Unhandled!
            }

            file.NextLine();
        }
    }

    using OnCycleFunc = std::function<void(int)>;

    void RunProgram(const ProgramList& programList, const OnCycleFunc& onCycleFunc)
    {      
        for (int pc = 0; pc < programList.size(); ++pc)
        {
            const OpPair& opPair = programList[pc];

            switch (opPair.first)
            {
                case OpType::NOOP:
                    onCycleFunc(1);
                    break;
                case OpType::ADDX:
                    onCycleFunc(2);
                    Registers.X += opPair.second;
                    break;
            }
        }
    }

    void RunPuzzle1(const char* pDataFileName)
    {
        ProgramList programList;
        ReadDataAndParse(pDataFileName, programList);

        int numCycles = 0;
        int sumSignalStrength = 0;

        RunProgram(programList, [&](int cycles)
        {
            int startCycles = 20;

            for (int i = 0; i < cycles; ++i)
            {
                numCycles++;

                if (numCycles == 20 || ((numCycles - startCycles) % 40) == 0)
                {
                    int signalStrength = (numCycles * Registers.X);
                    sumSignalStrength += signalStrength;
                }
            }
        });

        printf("Signal Strength: %d\n", sumSignalStrength);
    }

    void RunPuzzle2(const char* pDataFileName)
    {
        ProgramList programList;
        ReadDataAndParse(pDataFileName, programList);

        int numCycles = 0;
        int scanLinePos = 0;
        const int kLineRes = 40;

        RunProgram(programList, [&](int cycles)
        {
            for (int i = 0; i < cycles; ++i)
            {
                numCycles++;

                int pixLeft = scanLinePos - 1;
                int pixRight = scanLinePos + 1;

                if (Registers.X >= pixLeft && Registers.X <= pixRight)
                {
                    printf("#");
                }
                else
                {
                    printf(".");
                }

                scanLinePos++;
                if (scanLinePos >= kLineRes)
                {
                    scanLinePos = 0;
                    printf("\n");
                }
            }
        });
    }
}

void Day10::Run()
{
    //RunPuzzle1("Day10_Sample1.txt");
    //RunPuzzle1("Day10_Sample2.txt");
    //RunPuzzle1("Day10_Puzzle.txt");

    //RunPuzzle2("Day10_Sample1.txt");
    //RunPuzzle2("Day10_Sample2.txt");
    RunPuzzle2("Day10_Puzzle.txt");
}
