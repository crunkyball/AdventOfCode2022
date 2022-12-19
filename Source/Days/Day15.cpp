#include <vector>
#include <set>
#include <memory>
#include <cassert>

#include "File.h"
#include "Day15.h"

namespace
{
    struct Vector2D
    {
        int X;
        int Y;

        float Length()
        {
            float fLen = sqrt((float)X * X + Y * Y);
            return fLen;
        }

        int ManhattanDistance(const Vector2D& rOther) const
        {
            return std::abs(X - rOther.X) + std::abs(Y - rOther.Y);
        }

        Vector2D operator -(const Vector2D& rOther) const
        {
            Vector2D vec;
            vec.X = X - rOther.X;
            vec.Y = Y - rOther.Y;
            return vec;
        }
    };

    struct Sensor
    {
        Vector2D Pos;
        struct Beacon* ClosestBeacon = nullptr;
    };

    struct Beacon
    {
        Vector2D Pos;
        std::vector<Sensor*> ClosestSensors;
    };

    using SensorList = std::vector<Sensor>;
    using BeaconList = std::vector<Beacon>;

    struct Map
    {
        enum class Point
        {
            EMPTY,
            SENSOR,
            BEACON,
            BLOCKED,
            MISSING_BEACON,
            PERIMETER
        };

        int XOffest;
        int YOffset;
        
        std::vector<std::vector<Point>> MapPoints;

        void Initialise(int xOffset, int yOffset, int width, int height)
        {
            XOffest = xOffset;
            YOffset = yOffset;
            
            for (int y = 0; y < height + 1; ++y)
            {
                MapPoints.emplace_back();
                std::vector<Point>& row = MapPoints[MapPoints.size() - 1];

                for (int x = 0; x < width + 1; ++x)
                {
                    row.push_back(Point::EMPTY);
                }
            }
        }

        void SetPoint(int x, int y, Point point)
        {
            if (MapPoints[y - YOffset][x - XOffest] == Point::EMPTY)
            {
                MapPoints[y - YOffset][x - XOffest] = point;
            }
        }
    };

    void ReadDataAndParse(const char* pDataFileName, SensorList& outSensorList, BeaconList& outBeaconList)
    {
        File file(pDataFileName);

        while (!file.IsAtEnd())
        {
            int xStrPos = file.LineFind("x=");
            assert(xStrPos >= 0);
            file.NextChar(xStrPos + 2);
            int xVal = file.ReadLineAsNumber();

            int yStrPos = file.LineFind("y=");
            assert(yStrPos >= 0);
            file.NextChar(yStrPos + 2);
            int yVal = file.ReadLineAsNumber();

            outSensorList.push_back({ xVal, yVal });

            xStrPos = file.LineFind("x=");
            assert(xStrPos >= 0);
            file.NextChar(xStrPos + 2);
            xVal = file.ReadLineAsNumber();

            yStrPos = file.LineFind("y=");
            assert(yStrPos >= 0);
            file.NextChar(yStrPos + 2);
            yVal = file.ReadLineAsNumber();

            outBeaconList.push_back({ xVal, yVal });

            file.NextLine();
        }
    }

    void FindClosestBeacons(SensorList& sensorList, BeaconList& beaconList)
    {
        for (Sensor& sensor : sensorList)
        {
            Beacon* pClosestBeacon = nullptr;
            int closestBeaconDist = INT_MAX;

            for (Beacon& beacon : beaconList)
            {
                /*Vector2D diff = beacon.Pos - sensor.Pos;
                float diffLen = diff.Length();
                float dist = std::abs(diffLen);*/

                int dist = beacon.Pos.ManhattanDistance(sensor.Pos);

                if (dist < closestBeaconDist)
                {
                    closestBeaconDist = dist;
                    pClosestBeacon = &beacon;
                }
            }

            assert(pClosestBeacon != nullptr);
            sensor.ClosestBeacon = pClosestBeacon;
            pClosestBeacon->ClosestSensors.push_back(&sensor);
        }
    }

    void PopulateMap(Map& outMap, const SensorList& sensorList, const BeaconList& beaconList)
    {
        for (const auto& sensor : sensorList)
        {
            outMap.SetPoint(sensor.Pos.X, sensor.Pos.Y, Map::Point::SENSOR);
        }

        for (const auto& beacon : beaconList)
        {
            outMap.SetPoint(beacon.Pos.X, beacon.Pos.Y, Map::Point::BEACON);
        }

        //int i = 6;
        for (int i = 0; i < sensorList.size(); ++i)
        {
            const Sensor& sensor = sensorList[i];

            int dist = sensor.Pos.ManhattanDistance(sensor.ClosestBeacon->Pos);

            for (int x = sensor.Pos.X - dist; x <= sensor.Pos.X + dist; ++x)
            {
                for (int y = sensor.Pos.Y - dist; y <= sensor.Pos.Y + dist; ++y)
                {
                    int pointDist = sensor.Pos.ManhattanDistance({ x, y });

                    if (pointDist <= dist)
                    {
                        outMap.SetPoint(x, y, Map::Point::BLOCKED);
                    }
                }
            }
        }
    }

    void DrawMap(const Map& map, int fromY = -1, int toY = -1)
    {
        fromY -= fromY != -1 ? map.YOffset : 0;        
        toY -= toY != -1 ? map.YOffset : 0;

        for (int y = (fromY != -1 ? fromY : 0); y < (toY != -1 ? toY : map.MapPoints.size()); ++y)
        {
            for (int x = 0; x < map.MapPoints[y].size(); ++x)
            {
                switch (map.MapPoints[y][x])
                {
                    case Map::Point::EMPTY: printf("."); break;
                    case Map::Point::SENSOR: printf("S"); break;
                    case Map::Point::BEACON: printf("B"); break;
                    case Map::Point::BLOCKED: printf("#"); break;
                    case Map::Point::MISSING_BEACON: printf("!"); break;
                    case Map::Point::PERIMETER: printf("*"); break;
                }
            }

            printf("\n");
        }
    }

    int FindBlockedPointsOnLine(int lineY, const SensorList& sensorList, const BeaconList& beaconList)
    {
        std::set<int> blockedPoints;

        for (const auto& sensor : sensorList)
        {
            int dist = sensor.Pos.ManhattanDistance(sensor.ClosestBeacon->Pos);

            for (int x = sensor.Pos.X - dist; x <= sensor.Pos.X + dist; ++x)
            {
                int pointDist = sensor.Pos.ManhattanDistance({ x, lineY });

                if (pointDist <= dist)
                {
                    blockedPoints.insert(x);
                }
            }
        }

        return blockedPoints.size() - 1;
    }

    bool IsPointCovered(int x, int y, const SensorList& sensorList)
    {
        for (const auto& sensor : sensorList)
        {
            int dist = sensor.Pos.ManhattanDistance(sensor.ClosestBeacon->Pos);
            int pointDist = sensor.Pos.ManhattanDistance({ x, y });

            if (pointDist <= dist)
            {
                return true;
            }
        }

        return false;
    }

    void FindMissingBeacon(int min, int max, const SensorList& sensorList, const BeaconList& beaconList, int& outBeaconX, int& outBeaconY)
    {
        for (const auto& sensor : sensorList)
        {
            int dist = sensor.Pos.ManhattanDistance(sensor.ClosestBeacon->Pos);

            //Check the perimeter. There's got to be a better way of doing this but I'm sick of this puzzle so fuck it.
            for (int y = sensor.Pos.Y + (dist + 1), x = sensor.Pos.X; y > sensor.Pos.Y; --y, ++x)
            {
                if (y >= min && y < max && x >= min && x < max)
                {
                    if (!IsPointCovered(x, y, sensorList))
                    {
                        outBeaconX = x;
                        outBeaconY = y;
                        return;
                    }
                }
            }

            for (int x = sensor.Pos.X + (dist + 1), y = sensor.Pos.Y; x > sensor.Pos.X; --x, --y)
            {
                if (y >= min && y < max && x >= min && x < max)
                {
                    if (!IsPointCovered(x, y, sensorList))
                    {
                        outBeaconX = x;
                        outBeaconY = y;
                        return;
                    }
                }
            }

            for (int y = sensor.Pos.Y - (dist + 1), x = sensor.Pos.X; y < sensor.Pos.Y; ++y, --x)
            {
                if (y >= min && y < max && x >= min && x < max)
                {
                    if (!IsPointCovered(x, y, sensorList))
                    {
                        outBeaconX = x;
                        outBeaconY = y;
                        return;
                    }
                }
            }

            for (int x = sensor.Pos.X - (dist + 1), y = sensor.Pos.Y; x < sensor.Pos.X; ++x, ++y)
            {
                if (y >= min && y < max && x >= min && x < max)
                {
                    if (!IsPointCovered(x, y, sensorList))
                    {
                        outBeaconX = x;
                        outBeaconY = y;
                        return;
                    }
                }
            }
        }

        assert(0);  //Not found!
    }

    void RunPuzzle1(const char* pDataFileName)
    {
        SensorList sensorList;
        BeaconList beaconList;
        ReadDataAndParse(pDataFileName, sensorList, beaconList);

        FindClosestBeacons(sensorList, beaconList);

        int numBlocked = FindBlockedPointsOnLine(2000000, sensorList, beaconList);
        printf("Num Blocked Positions: %d\n", numBlocked);
    }

    void RunPuzzle2(const char* pDataFileName)
    {
        SensorList sensorList;
        BeaconList beaconList;
        ReadDataAndParse(pDataFileName, sensorList, beaconList);

        FindClosestBeacons(sensorList, beaconList);

        int lowestX, lowestY, highestX, highestY;
        lowestX = lowestY = INT_MAX;
        highestX = highestY = INT_MIN;

        for (const auto& sensor : sensorList)
        {
            int closestBeaconDist = sensor.Pos.ManhattanDistance(sensor.ClosestBeacon->Pos);

            if (sensor.Pos.X - closestBeaconDist < lowestX)
            {
                lowestX = sensor.Pos.X - closestBeaconDist;
            }

            if (sensor.Pos.X + closestBeaconDist > highestX)
            {
                highestX = sensor.Pos.X + closestBeaconDist;
            }

            if (sensor.Pos.Y - closestBeaconDist < lowestY)
            {
                lowestY = sensor.Pos.Y - closestBeaconDist;
            }

            if (sensor.Pos.Y + closestBeaconDist > highestY)
            {
                highestY = sensor.Pos.Y + closestBeaconDist;
            }
        }

        int beaconX = -1;
        int beaconY = -1;
        FindMissingBeacon(0, 4000000, sensorList, beaconList, beaconX, beaconY);

        int64_t tuningFreq = static_cast<int64_t>(beaconX) * 4000000 + beaconY;

        printf("Tuning Freq: %llu\n", tuningFreq);

        /*Map map;
        map.Initialise(lowestX, lowestY, highestX - lowestX, highestY - lowestY);

        PopulateMap(map, sensorList, beaconList);

        DrawMap(map);*/
    }
}

void Day15::Run()
{
    //RunPuzzle1("Day15_Sample.txt");
    //RunPuzzle1("Day15_Puzzle.txt");

    //RunPuzzle2("Day15_Sample.txt");
    RunPuzzle2("Day15_Puzzle.txt");
}
