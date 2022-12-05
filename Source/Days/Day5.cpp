#include <stack>
#include <vector>
#include <cassert>

#include "File.h"
#include "Utils.h"
#include "Day5.h"

namespace
{
    using SupplyStack = std::stack<char>;
    using SupplyStackList = std::vector<SupplyStack>;

    struct Move
    {
        int NumToMove;
        int FromStack;
        int ToStack;
    };

    using MoveList = std::vector<Move>;

    void ReadDataAndParse(const char* pFileName, SupplyStackList& outStackList, MoveList& outMoveList)
    {
        File file(pFileName);

        //First find the split point between stacks and instructions.
        int splitLineNum = 0;
        while (!file.IsAtEnd())
        {
            if (file.IsLineEmpty())
            {
                break;
            }

            file.NextLine();
            splitLineNum++;
        }

        //Then populate the stacks.
        file.SetLine(splitLineNum - 2);

        for (int lineIdx = 0; lineIdx < splitLineNum - 1; ++lineIdx)
        {
            int currentStack = 0;

            while (!file.IsLineEmpty())
            {
                const int kStackCharsSize = 3;
                char stackBuf[kStackCharsSize + 1]; //+1 for NTC.

                file.ReadLineChars(stackBuf, kStackCharsSize);

                assert(outStackList.size() >= currentStack);
                if (outStackList.size() == currentStack)
                {
                    outStackList.emplace_back();
                }

                if (file.ReadChar() == '[')
                {
                    file.NextChar();
                    char c = file.ReadChar();
                    outStackList[currentStack].push(c);

                    file.NextChar(3);
                }
                else
                {
                    file.NextChar(kStackCharsSize + 1);
                }

                currentStack++;
            }

            file.PreviousLine();
        }

        //Then populate the move instructions
        file.SetLine(splitLineNum + 1);

        const int kMoveStrLen = static_cast<int>(strlen("move "));

        const int kLineBufSize = 64;
        char lineBuf[kLineBufSize];

        while (!file.IsAtEnd())
        {
            memset(lineBuf, 0, kLineBufSize);
            file.ReadLine(lineBuf, kLineBufSize);

            const int kMoveStrBufSize = 16;

            char moveStrBuf[kMoveStrBufSize];
            char stacksStrBuf[kMoveStrBufSize];

            Utils::SplitString(lineBuf, " from ", moveStrBuf, kMoveStrBufSize, stacksStrBuf, kMoveStrBufSize);

            int numToMove = atoi(moveStrBuf + kMoveStrLen);

            const int kStackStrBufSize = 8;
            char fromStrBuf[kStackStrBufSize];
            char toStrBuf[kStackStrBufSize];

            Utils::SplitString(stacksStrBuf, " to ", fromStrBuf, kStackStrBufSize, toStrBuf, kStackStrBufSize);

            int fromStack = atoi(fromStrBuf) - 1;
            int toStack = atoi(toStrBuf) - 1;

            outMoveList.push_back({ numToMove, fromStack, toStack});

            file.NextLine();
        }
    }

    void ApplyMoveListToSupply(const MoveList& moveList, SupplyStackList& supplyStackList, bool bKeepOrder)
    {
        for (const Move& move : moveList)
        {
            std::vector<char> crateMove;

            SupplyStack& fromStack = supplyStackList[move.FromStack];
            SupplyStack& toStack = supplyStackList[move.ToStack];

            for (int i = 0; i < move.NumToMove; ++i)
            {   
                char c = fromStack.top();
                fromStack.pop();

                crateMove.push_back(c);
            }

            if (bKeepOrder)
            {
                for (auto itr = crateMove.rbegin(); itr != crateMove.rend(); ++itr)
                {
                    toStack.push(*itr);
                }
            }
            else
            {
                for (char c : crateMove)
                {
                    toStack.push(c);
                }
            }
        }
    }

    void FindSupplyHeads(const SupplyStackList& supplyStackList, char charBuf[], int bufSize)
    {
        memset(charBuf, 0, bufSize);

        assert(supplyStackList.size() < (bufSize - 1));

        for (int i = 0; i < supplyStackList.size(); ++i)
        {
            charBuf[i] = supplyStackList[i].top();
        }
    }

    void RunPuzzle(const char* pDataFileName, bool bKeepOrder)
    {
        SupplyStackList supplyStackList;
        MoveList moveList;
        ReadDataAndParse(pDataFileName, supplyStackList, moveList);

        ApplyMoveListToSupply(moveList, supplyStackList, bKeepOrder);

        const int kHeadBufSize = 16;
        char headBuf[kHeadBufSize];
        FindSupplyHeads(supplyStackList, headBuf, kHeadBufSize);

        printf("Supply heads: %s\n", headBuf);
    }

    void RunPuzzle1(const char* pDataFileName)
    {
        RunPuzzle(pDataFileName, false);
    }

    void RunPuzzle2(const char* pDataFileName)
    {
        RunPuzzle(pDataFileName, true);
    }
}

void Day5::Run()
{
    //RunPuzzle1("Day5_Sample.txt");
    //RunPuzzle1("Day5_Puzzle.txt");

    //RunPuzzle2("Day5_Sample.txt");
    RunPuzzle2("Day5_Puzzle.txt");
}
