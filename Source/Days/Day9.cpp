#include <vector>
#include <cassert>
#include <cmath>
#include <unordered_set>

#include "File.h"
#include "Day9.h"

namespace
{
    enum class Direction
    {
        UP,
        DOWN,
        LEFT,
        RIGHT
    };

    struct Vector2D
    {
        int X;
        int Y;

        float Length()
        {
            float fLen = sqrt((float)X * X + Y * Y);
            return fLen;
        }

        void Normalise()
        {
            float fLen = Length();

            //The proper way, but we're dealing with integers.
            //X = std::round(X / fLen);
            //Y = std::round(Y / fLen);

            //I imagine there's a better way of doing this...
            if (X > 0.f)
            {
                X = std::ceil(X / fLen);
            }
            else if (X < 0.f)
            {
                X = std::floor(X / fLen);
            }

            if (Y > 0.f)
            {
                Y = std::ceil(Y / fLen);
            }
            else if (Y < 0.f)
            {
                Y = std::floor(Y / fLen);
            }
        }

        Vector2D& operator =(const Vector2D& rOther)
        {
            X = rOther.X;
            Y = rOther.Y;
            return *this;
        }

        bool operator ==(const Vector2D& rOther) const
        {
            return X == rOther.X && Y == rOther.Y;
        }

        bool operator !=(const Vector2D& rOther) const
        {
            return X != rOther.X || Y != rOther.Y;
        }

        Vector2D operator +(const Vector2D& rOther) const
        {
            Vector2D vec;
            vec.X = X + rOther.X;
            vec.Y = Y + rOther.Y;
            return vec;
        }

        Vector2D operator -(const Vector2D& rOther) const
        {
            Vector2D vec;
            vec.X = X - rOther.X;
            vec.Y = Y - rOther.Y;
            return vec;
        }

        Vector2D operator +=(const Vector2D& rOther)
        {
            X += rOther.X;
            Y += rOther.Y;
            return *this;
        }

        Vector2D operator -=(const Vector2D& rOther)
        {
            X -= rOther.X;
            Y -= rOther.Y;
            return *this;
        }

        Vector2D operator *(const int val)
        {
            Vector2D vec;
            vec.X = X * val;
            vec.Y = Y * val;
            return vec;
        }
    };

    const Vector2D DirectionVectors[] = {
        { 0, 1 },
        { 0, -1 },
        { -1, 0 },
        { 1, 0}
    };

    class Vector2DHash
    {
    public:
        int operator()(const Vector2D& rVec) const
        {
            return rVec.X + rVec.Y * 100;   //This should do.
        }
    };

    using InstructionPair = std::pair<Direction, int>;
    using InstructionList = std::vector<InstructionPair>;
    using PositionList = std::unordered_set<Vector2D, Vector2DHash>;

    void ReadDataAndParse(const char* pFileName, InstructionList& outList)
    {
        File file(pFileName);

        while (!file.IsAtEnd())
        {
            char dirChar = file.ReadChar();
            Direction dir = 
                dirChar == 'U' ? Direction::UP : 
                dirChar == 'D' ? Direction::DOWN : 
                dirChar == 'L' ? Direction::LEFT : 
                Direction::RIGHT;
            assert(dir != Direction::RIGHT || dirChar == 'R');
            
            file.NextChar(2);
            
            const int kAmountBufSize = 8;
            char amountBuf[kAmountBufSize];
            file.ReadLine(amountBuf, kAmountBufSize);

            int amount = atoi(amountBuf);

            outList.push_back({ dir, amount });

            file.NextLine();
        }
    }

    void ProcessInstructionList(const InstructionList& instructionList, int numNodes, PositionList& outPositions)
    {
        //Vector2D head = { 0, 0 };
        std::vector<Vector2D> nodes;
        for (int i = 0; i < numNodes; ++i)
        {
            nodes.push_back({ 0, 0 });
        }

        for (const InstructionPair& pair : instructionList)
        {
            //head += DirectionVectors[static_cast<int>(pair.first)] * pair.second;

            for (int i = 0; i < pair.second; ++i)
            {
                //Move the head.
                nodes[0] += DirectionVectors[static_cast<int>(pair.first)];

                for (int n = 1; n < numNodes; ++n)
                {
                    //Move the tail.
                    Vector2D diff = nodes[n-1] - nodes[n];
                    if (diff.Length() >= 2.0f)
                    {
                        diff.Normalise();   //Get direction.
                        nodes[n] += diff;
                    }
                }

                outPositions.insert(nodes[numNodes-1]);
            }
        }
    }

    void RunPuzzle(const char* pDataFileName, int numNodes)
    {
        InstructionList instructionList;

        ReadDataAndParse(pDataFileName, instructionList);

        PositionList visitedPositions;
        ProcessInstructionList(instructionList, numNodes, visitedPositions);

        printf("Visited Positions: %d\n", visitedPositions.size());
    }

    void RunPuzzle1(const char* pDataFileName)
    {
        RunPuzzle(pDataFileName, 2);
    }

    void RunPuzzle2(const char* pDataFileName)
    {
        RunPuzzle(pDataFileName, 10);
    }
}

void Day9::Run()
{
    //RunPuzzle1("Day9_Sample.txt");
    //RunPuzzle1("Day9_Puzzle.txt");

    //RunPuzzle2("Day9_Sample.txt");
    RunPuzzle2("Day9_Puzzle.txt");
}
