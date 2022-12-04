#include <vector>
#include <functional>

#include "File.h"
#include "Utils.h"
#include "Day4.h"

namespace
{
    class Elf
    {
    public:
        Elf(int assignmentStart, int assignmentEnd) :
            m_AssignmentStart(assignmentStart),
            m_AssignmentEnd(assignmentEnd)
        {}

        bool AssignmentContains(const Elf& other) const
        {
            return m_AssignmentStart <= other.m_AssignmentStart && m_AssignmentEnd >= other.m_AssignmentEnd;
        }

        bool AssignmentOverlaps(const Elf& other) const
        {
            return m_AssignmentStart <= other.m_AssignmentEnd && other.m_AssignmentStart <= m_AssignmentEnd;
        }

    private:
        int m_AssignmentStart;
        int m_AssignmentEnd;
    };

    using ElfPair = std::pair<Elf, Elf>;
    using ElfPairList = std::vector<ElfPair>;

    void ReadDataAndParse(const char* pFileName, ElfPairList& outElfPairList)
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

                const int kElfPairStrSize = 16;

                char firstElf[kElfPairStrSize];
                char secondElf[kElfPairStrSize];

                Utils::SplitString(lineBuf, ",", firstElf, kElfPairStrSize, secondElf, kElfPairStrSize);

                const int kElfRangeStrSize = 8;

                char firstElfRangesStr[2][kElfRangeStrSize];
                char secondElfRangesStr[2][kElfRangeStrSize];

                Utils::SplitString(firstElf, "-", firstElfRangesStr[0], kElfRangeStrSize, firstElfRangesStr[1], kElfRangeStrSize);
                Utils::SplitString(secondElf, "-", secondElfRangesStr[0], kElfRangeStrSize, secondElfRangesStr[1], kElfRangeStrSize);

                ElfPair pair = { {atoi(firstElfRangesStr[0]), atoi(firstElfRangesStr[1])}, {atoi(secondElfRangesStr[0]), atoi(secondElfRangesStr[1])} };
                outElfPairList.push_back(std::move(pair));
            }

            file.NextLine();
        }
    }

    int SumContainedAssignments(const ElfPairList& elfPairList)
    {
        int sum = 0;

        for (const ElfPair& elfPair : elfPairList)
        {
            if (elfPair.first.AssignmentContains(elfPair.second) ||
                elfPair.second.AssignmentContains(elfPair.first))
            {
                sum++;
            }
        }

        return sum;
    }

    int SumOverlappedAssignments(const ElfPairList& elfPairList)
    {
        int sum = 0;

        for (const ElfPair& elfPair : elfPairList)
        {
            if (elfPair.first.AssignmentOverlaps(elfPair.second))
            {
                sum++;
            }
        }

        return sum;
    }

    void RunPuzzle1(const char* pDataFilename)
    {
        ElfPairList elfPairList;
        ReadDataAndParse(pDataFilename, elfPairList);

        int containedAssignments = SumContainedAssignments(elfPairList);

        printf("Num Contained Assignments: %d\n", containedAssignments);
    }
   
    void RunPuzzle2(const char* pDataFilename)
    {
        ElfPairList elfPairList;
        ReadDataAndParse(pDataFilename, elfPairList);

        int overlappedAssignments = SumOverlappedAssignments(elfPairList);

        printf("Num Overlapped Assignments: %d\n", overlappedAssignments);
    }
}

void Day4::Run()
{
    //RunPuzzle1("Day4_Sample.txt");
    //RunPuzzle1("Day4_Puzzle.txt");

    //RunPuzzle2("Day4_Sample.txt");
    RunPuzzle2("Day4_Puzzle.txt");
}
