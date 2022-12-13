#include <vector>
#include <unordered_map>
#include <algorithm>
#include <queue>

#include "File.h"
#include "Day12.h"

namespace
{
    struct MapCoords
    {
        int X = 0;
        int Y = 0;

        bool operator ==(const MapCoords& rOther)
        {
            return X == rOther.X && Y == rOther.Y;
        }
    };

    struct MapEntry
    {
        int Height = 0;
        bool Visited = false;
        MapCoords Parent;
    };

    using Map = std::vector<std::vector<MapEntry>>;

    int mapWidth = 0;
    int mapHeight = 0;

    void ReadDataAndParse(const char* pDataFileName, Map& outMap, MapCoords& outStart, MapCoords& outEnd)
    {
        File file(pDataFileName);

        int x = 0;
        int y = 0;

        while (!file.IsAtEnd())
        {
            outMap.emplace_back();
            std::vector<MapEntry>& row = outMap[outMap.size() - 1];

            while (!file.IsLineEmpty())
            {
                char c = file.ReadChar();

                if (c == 'S')
                {
                    outStart = { x, y };
                    c = 'a';    //Same height as 'a'.
                }
                else if (c == 'E')
                {
                    outEnd = { x, y };
                    c = 'z';    //Same height as 'z'.
                }

                int val = c - 'a';

                row.emplace_back();
                row[row.size() - 1].Height = val;

                file.NextChar();
                
                x++;
            }

            file.NextLine();

            y++;
            x = 0;
        }
    }

    bool RunBFS(Map& map, MapCoords start, MapCoords end)
    {
        enum Direction
        {
            UP,
            DOWN,
            LEFT,
            RIGHT,
        };

        MapEntry& rootNode = map[start.Y][start.X];
        rootNode.Visited = true;

        MapCoords v;

        std::queue<MapCoords> routeQueue;
        routeQueue.push(start);

        while (!routeQueue.empty())
        {
            v = routeQueue.front();
            routeQueue.pop();

            if (v == end)
            {
                return true;
            }

            MapEntry& vEntry = map[v.Y][v.X];

            bool bHandled = false;

            //For each direction.
            for (int dir = UP; dir <= RIGHT; ++dir)
            {
                MapCoords w;

                switch (dir)
                {
                    case UP: w = { v.X, v.Y - 1 }; break;
                    case DOWN: w = { v.X, v.Y + 1 }; break;
                    case LEFT: w = { v.X - 1, v.Y }; break;
                    case RIGHT: w = { v.X + 1, v.Y }; break;
                }

                if (w.Y >= 0 && w.Y < map.size() &&
                    w.X >= 0 && w.X < map[w.Y].size())
                {
                    MapEntry& wEntry = map[w.Y][w.X];

                    if (!wEntry.Visited && wEntry.Height <= (vEntry.Height + 1))
                    {
                        bHandled = true;

                        wEntry.Visited = true;
                        wEntry.Parent = { v.X, v.Y };

                        if (w == end)
                        {
                            int a = 0;
                        }

                        routeQueue.push(w);
                    }
                }
            }

            if (!bHandled)
            {
                int a = 0;
            }
        }

        return false;
    }

    int TraverseRoute(const Map& map, MapCoords start, MapCoords end)
    {
        int numSteps = 0;

        for (MapCoords currNode = end; !(currNode == start); currNode = map[currNode.Y][currNode.X].Parent)
        {
            numSteps++;
        }

        return numSteps;
    }

    void ResetMap(Map& map)
    {
        for (std::vector<MapEntry>& row : map)
        {
            for (MapEntry& entry : row)
            {
                entry.Visited = false;
            }
        }
    }

    void RunPuzzle1(const char* pDataFileName)
    {
        Map map;
        MapCoords start, end;
        ReadDataAndParse(pDataFileName, map, start, end);

        RunBFS(map, start, end);
        int numSteps = TraverseRoute(map, start, end);

        printf("Took %d steps!\n", numSteps);
    }

    void RunPuzzle2(const char* pDataFileName)
    {
        Map map;
        MapCoords start, end;
        ReadDataAndParse(pDataFileName, map, start, end);

        //First find a list of all the 'a's.
        std::vector<MapCoords> startPositions;

        for (int y = 0; y < map.size(); ++y)
        {
            for (int x = 0; x < map[y].size(); ++x)
            {
                if (map[y][x].Height == 0)
                {
                    startPositions.push_back( {x, y} );
                }
            }
        }

        int shortestPath = INT_MAX;

        for (MapCoords thisStart : startPositions)
        {
            bool routeFound = RunBFS(map, thisStart, end);

            if (routeFound)
            {
                int numSteps = TraverseRoute(map, thisStart, end);

                if (numSteps < shortestPath)
                {
                    shortestPath = numSteps;
                }
            }

            ResetMap(map);
        }

        printf("Shortest path: %d\n", shortestPath);
    }
}

void Day12::Run()
{
    //RunPuzzle1("Day12_Sample.txt");
    //RunPuzzle1("Day12_Puzzle.txt");

    //RunPuzzle2("Day12_Sample.txt");
    RunPuzzle2("Day12_Puzzle.txt");
}
