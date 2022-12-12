#include <vector>
#include <algorithm>
#include <cassert>

#include "File.h"
#include "Utils.h"
#include "Day11.h"

namespace
{
    using ItemList = std::vector<int>;

    enum class OpType
    {
        ADD_SELF,
        MULTIPLY_SELF,
        ADD_VALUE,
        MULTIPLY_VALUE
    };

    using OpPair = std::pair<OpType, int>;

    struct DivisibilityTest
    {
        int TestValue;
        int TrueMonkey;
        int FalseMonkey;
    };

    struct Monkey
    {
        ItemList Items;
        OpPair Operation;
        DivisibilityTest Test;

        int InspectedCount = 0;
    };

    using MonkeyList = std::vector<Monkey>;

    void ReadDataAndParse(const char* pFileName, MonkeyList& outMonkeyList)
    {
        File file(pFileName);

        const char* kMonkeyStr = "Monkey";
        const int kMonkeyStrLen = static_cast<int>(strlen(kMonkeyStr));

        const char* kItemsStr = "  Starting items: ";
        const int kItemsStrLen = static_cast<int>(strlen(kItemsStr));

        const char* kOperationStr = "  Operation: new = old ";
        const int kOperationStrLen = static_cast<int>(strlen(kOperationStr));

        const char* kTestStr = "  Test: divisible by ";
        const int kTestStrLen = static_cast<int>(strlen(kTestStr));

        const char* kIfTrueStr = "    If true: throw to monkey ";
        const int kIfTrueStrLen = static_cast<int>(strlen(kIfTrueStr));

        const char* kIfFalseStr = "    If false: throw to monkey ";
        const int kIfFalseStrLen = static_cast<int>(strlen(kIfFalseStr));

        while (!file.IsAtEnd())
        {
            const int kLineBufSize = 64;
            char lineBuf[kLineBufSize];

            bool bMonkeyLine = file.LineStartsWith(kMonkeyStr);
            assert(bMonkeyLine);
            //file.NextChar(kMonkeyStrLen);
            file.NextLine();

            outMonkeyList.emplace_back();
            Monkey& monkey = outMonkeyList[outMonkeyList.size() - 1];

            //Starting items
            bool bItemsLine = file.LineStartsWith(kItemsStr);
            assert(bItemsLine);
            file.NextChar(kItemsStrLen);
            file.ReadLine(lineBuf, kLineBufSize);

            char items[8][8];
            int numItems = Utils::Tokenise(lineBuf, ", ", &items[0][0], 8, 8);
            for (int i = 0; i < numItems; ++i)
            {
                int item = atoi(items[i]);
                monkey.Items.push_back(item);
            }

            file.NextLine();

            //Operation
            bool bOperationLine = file.LineStartsWith(kOperationStr);
            assert(bOperationLine);
            file.NextChar(kOperationStrLen);
            
            char opChar = file.ReadChar();
            file.NextChar(2);

            bool self = false;
            int val = 0;

            if (file.LineStartsWith("old"))
            {
                self = true;
            }
            else
            {
                val = file.ReadLineAsNumber();
            }

            OpType opType;

            switch (opChar)
            {
                case '+':
                    opType = self ? OpType::ADD_SELF : OpType::ADD_VALUE;
                    break;
                case '*':
                    opType = self ? OpType::MULTIPLY_SELF : OpType::MULTIPLY_VALUE;
                    break;
                default:
                    assert(0);  //Unhandled!
            }

            monkey.Operation = { opType, val };

            file.NextLine();

            //Test
            bool bTestLine = file.LineStartsWith(kTestStr);
            assert(bTestLine);
            file.NextChar(kTestStrLen);
            
            int testValue = file.ReadLineAsNumber();
            monkey.Test.TestValue = testValue;

            file.NextLine();

            //If true
            bool bIfTrueLine = file.LineStartsWith(kIfTrueStr);
            assert(bIfTrueLine);
            file.NextChar(kIfTrueStrLen);

            monkey.Test.TrueMonkey = file.ReadLineAsNumber();
            
            file.NextLine();

            //If false
            bool bIfFalseLine = file.LineStartsWith(kIfFalseStr);
            assert(bIfFalseLine);
            file.NextChar(kIfFalseStrLen);
            
            monkey.Test.FalseMonkey = file.ReadLineAsNumber();

            file.NextLine(2);   //2 for the blank line.
        }
    }

    int FindMonkeyBusiness(MonkeyList& monkeyList)
    {
        int numRounds = 20;

        for (int i = 0; i < numRounds; ++i)
        {
            for (Monkey& monkey : monkeyList)
            {
                const OpPair& opPair = monkey.Operation;

                for (int& worryLevel : monkey.Items)
                {
                    monkey.InspectedCount++;

                    switch (opPair.first)
                    {
                        case OpType::ADD_SELF:
                            worryLevel += worryLevel;
                            break;
                        case OpType::MULTIPLY_SELF:
                            worryLevel *= worryLevel;
                            break;
                        case OpType::ADD_VALUE:
                            worryLevel += opPair.second;
                            break;
                        case OpType::MULTIPLY_VALUE:
                            worryLevel *= opPair.second;
                            break;
                    }

                    worryLevel /= 3;

                    if ((worryLevel % monkey.Test.TestValue) == 0)
                    {
                        monkeyList[monkey.Test.TrueMonkey].Items.push_back(worryLevel);
                    }
                    else
                    {
                        monkeyList[monkey.Test.FalseMonkey].Items.push_back(worryLevel);
                    }
                }

                monkey.Items.clear();
            }
        }

        std::vector<int> inspectLevels;
        for (const Monkey& monkey : monkeyList)
        {
            inspectLevels.push_back(monkey.InspectedCount);
        }

        std::sort(inspectLevels.begin(), inspectLevels.end(), std::greater<>());

        return inspectLevels[0] * inspectLevels[1];
    }

    void RunPuzzle1(const char* pDataFileName)
    {
        MonkeyList monkeyList;
        ReadDataAndParse(pDataFileName, monkeyList);

        int monkeyBusiness = FindMonkeyBusiness(monkeyList);

        printf("Monkey Business: %d\n", monkeyBusiness);
    }

    void RunPuzzle2(const char* pDataFileName)
    {
    }
}

void Day11::Run()
{
    //RunPuzzle1("Day11_Sample.txt");
    RunPuzzle1("Day11_Puzzle.txt");

    //RunPuzzle2("Day11_Sample.txt");
    //RunPuzzle2("Day11_Puzzle.txt");
}
