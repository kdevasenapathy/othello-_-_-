#ifndef __PLAYER_H__
#define __PLAYER_H__

#include <iostream>
#include "common.hpp"
#include "board.hpp"
#include <vector>
#define ENDGAME 1000000
#define DEPTH 2
using namespace std;

class Player {
private:
	Board* boardState;
	Side ourSide;
	int calculateWeight(int x, int y);
	int calculateBoard(Move* move);
	int naiveHeuristic(Move* move, Board* state);
	int minimax(Board* current, Side side, int currentDepth);

public:
    Player(Side side);
    ~Player();

    Move *doMove(Move *opponentsMove, int msLeft);
    void setBoard(char data[]);

    // Flag to tell if the player is running within the test_minimax context
    bool testingMinimax;
};

#endif
