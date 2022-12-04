#include <array>
#include <vector>
#include <cassert>
#include <functional>

#include "File.h"
#include "Day3.h"

namespace
{
    class Puzzle1Rucksack
    {
    public:
        Puzzle1Rucksack(const char* pParseBuffer, const int bufferSize)
        {
            assert(bufferSize % 2 == 0);    //What do?
            int halfBufferSize = bufferSize / 2;

            for (int i = 0; i < bufferSize; ++i)
            {
                int compartment = i < halfBufferSize ? 0 : 1;
                m_Compartments[compartment].push_back(pParseBuffer[i]);
            }
        }

        char FindCommonItem() const
        {
            for (char c1 : m_Compartments[0])
            {
                for (char c2 : m_Compartments[1])
                {
                    if (c1 == c2)
                    {
                        return c1;
                    }
                }
            }

            assert(0);  //Shouldn't happen.
            return 0;
        }

    private:
        static const int kCompartments = 2;
        std::array<std::vector<char>, kCompartments> m_Compartments;
    };

    class Puzzle2ElfGroup
    {
    public:
        bool GroupFull() const { return m_NumElves == 3; }

        void ParseNewLine(const char* pParseBuffer, const int bufferSize)
        {
            assert(!GroupFull());    //We're full!
            for (int i = 0; i < bufferSize; ++i)
            {
                m_ElvesItems[m_NumElves].push_back(pParseBuffer[i]);
            }

            m_NumElves++;
        }

        char FindCommonItem() const
        {
            for (char c1 : m_ElvesItems[0])
            {
                for (char c2 : m_ElvesItems[1])
                {
                    for (char c3 : m_ElvesItems[2])
                    {
                        if (c1 == c2 && c1 == c3)
                        {
                            return c1;
                        }
                    }
                }
            }

            assert(0);  //Shouldn't happen.
            return 0;
        }

        private:
            int m_NumElves = 0;
            std::array<std::vector<char>, 3> m_ElvesItems;
    };

    using Puzzle1RucksackList = std::vector<Puzzle1Rucksack>;
    using Puzzle2ElfGroupList = std::vector<Puzzle2ElfGroup>;

    void ReadData(const char* pFileName, std::function<void(const char[], int)> parseLineFunc)
    {
        File file(pFileName);

        const int kBufSize = 64;
        char lineBuf[kBufSize];

        while (!file.IsAtEnd())
        {
            if (!file.IsLineEmpty())
            {
                memset(lineBuf, 0, kBufSize);
                file.ReadLine(lineBuf, kBufSize);

                int lineLen = static_cast<int>(strlen(lineBuf));

                parseLineFunc(lineBuf, lineLen);
            }

            file.NextLine();
        }
    }

    int GetPriorityForItem(char item)
    {
        if (item >= 'a' && item <= 'z')
        {
            return (item - 'a') + 1;
        }
        else if (item >= 'A' && item <= 'Z')
        {
            return (item - 'A') + 27;
        }

        assert(0);  //Shouldn't get here.
        return 0;
    }

    template<typename T>
    int SumPriorities(const T& things)
    {
        int sum = 0;

        for (const auto& thing : things)
        {
            char commonItem = thing.FindCommonItem();
            sum += GetPriorityForItem(commonItem);
        }

        return sum;
    }

    void RunPuzzle1(const char* pDataFilename)
    {
        Puzzle1RucksackList rucksacks;

        ReadData(pDataFilename, [&](const char lineBuf[], int lineLen) {
            rucksacks.emplace_back(lineBuf, lineLen);
        });

        int sum = SumPriorities<Puzzle1RucksackList>(rucksacks);

        printf("Total: %d\n", sum);
    }
   
    void RunPuzzle2(const char* pDataFilename)
    {
        Puzzle2ElfGroupList groups;

        ReadData(pDataFilename, [&](const char lineBuf[], int lineLen) {

            if (groups.size() == 0 || groups[groups.size() - 1].GroupFull())
            {
                groups.emplace_back();
            }

            groups[groups.size() - 1].ParseNewLine(lineBuf, lineLen);
        });

        int sum = SumPriorities<Puzzle2ElfGroupList>(groups);

        printf("Total: %d\n", sum);

        int a = 0;
    }
}

void Day3::Run()
{
    //RunPuzzle1("Day3_Sample.txt");
    //RunPuzzle1("Day3_Puzzle.txt");

    //RunPuzzle2("Day3_Sample.txt");
    RunPuzzle2("Day3_Puzzle.txt");
}
