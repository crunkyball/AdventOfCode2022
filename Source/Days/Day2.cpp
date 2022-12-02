#include <vector>
#include <unordered_map>
#include <functional>

#include "File.h"
#include "Day2.h"

namespace
{
    //Value is score.
    enum class MoveType
    {
        ROCK = 1,
        PAPER = 2,
        SCISSORS = 3
    };

    struct Move
    {
        MoveType LosesTo;
        MoveType Beats;
    };

    //Value is score.
    enum class Result
    {
        LOSS = 0,
        DRAW = 3,
        WIN = 6
    };

    using MoveParseMap = std::unordered_map<char, MoveType>;
    using ResultParseMap = std::unordered_map<char, Result>;

    const std::unordered_map<MoveType, Move> kMoves = {
        { MoveType::ROCK, { MoveType::PAPER, MoveType::SCISSORS } },
        { MoveType::PAPER, { MoveType::SCISSORS, MoveType::ROCK } },
        { MoveType::SCISSORS, { MoveType::ROCK, MoveType::PAPER } }
    };

    struct Game
    {
        MoveType Player1;
        MoveType Player2;
    };

    using GameList = std::vector<Game>;

    void ReadData(const char* pFileName, std::function<void(char, char)> parseGameFunc)
    {
        File file(pFileName);

        while (!file.IsAtEnd())
        {
            if (!file.IsLineEmpty())
            {
                char player1Move = file.ReadChar();
                file.NextChar(2);
                char player2Move = file.ReadChar();

                parseGameFunc(player1Move, player2Move);
            }

            file.NextLine();
        }
    }

    Result GetGameResult(const Game& game)
    {
        const Move& move = kMoves.at(game.Player2);
        return move.Beats == game.Player1 ? Result::WIN : move.LosesTo == game.Player1 ? Result::LOSS : Result::DRAW;
    }

    MoveType GetMoveForResult(const MoveType& moveType, const Result& result)
    {
        const Move& move = kMoves.at(moveType);
        return result == Result::WIN ? move.LosesTo : result == Result::LOSS ? move.Beats : moveType;
    }

    int SumGames(const GameList& games)
    {
        int score = 0;

        for (const Game& game : games)
        {
            //Score for shape selected.
            score += static_cast<int>(game.Player2);

            //Score for the outcome.
            Result result = GetGameResult(game);
            score += static_cast<int>(result);
        }

        return score;
    }

    void RunPuzzle1(const char* pDataFilename)
    {
        MoveParseMap moveParseMap = {
            {'A', MoveType::ROCK},
            {'B', MoveType::PAPER},
            {'C', MoveType::SCISSORS},
            {'X', MoveType::ROCK},
            {'Y', MoveType::PAPER},
            {'Z', MoveType::SCISSORS}
        };

        GameList games;
        ReadData(pDataFilename, [&](char player1MoveChar, char player2MoveChar) {
            Game game = {
                moveParseMap.at(player1MoveChar),
                moveParseMap.at(player2MoveChar)
            };

            games.push_back(game);
        });

        int score = SumGames(games);
        printf("Score: %d\n", score);
    }

    void RunPuzzle2(const char* pDataFilename)
    {
        MoveParseMap moveParseMap = {
            {'A', MoveType::ROCK},
            {'B', MoveType::PAPER},
            {'C', MoveType::SCISSORS}
        };

        ResultParseMap resultParseMap{
            {'X', Result::LOSS},
            {'Y', Result::DRAW},
            {'Z', Result::WIN}
        };

        GameList games;
        ReadData(pDataFilename, [&](char player1MoveChar, char player2Result) {

            MoveType p1MoveType = moveParseMap.at(player1MoveChar);
            Result p2Result = resultParseMap.at(player2Result);

            MoveType p2MoveType = GetMoveForResult(p1MoveType, p2Result);

            games.push_back({ p1MoveType, p2MoveType });
        });

        int score = SumGames(games);
        printf("Score: %d\n", score);
    }
}

void Day2::Run()
{
    //RunPuzzle1("Day2_Sample.txt");
    //RunPuzzle1("Day2_Puzzle.txt");

    //RunPuzzle2("Day2_Sample.txt");
    RunPuzzle2("Day2_Puzzle.txt");
}
