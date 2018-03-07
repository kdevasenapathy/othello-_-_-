#ifndef __PLAYER_H__
#define __PLAYER_H__

#include <iostream>
#include "common.hpp"
#include "board.hpp"
#include <cstdlib>
#include <vector>
#define ENDGAME 1000000
using namespace std;

class Player {
private:
	Board* boardState;
	Side ourSide;
	int calculateWeight(int x, int y);
	int calculateBoard(Move* move);

public:
    Player(Side side);
    ~Player();

    Move *doMove(Move *opponentsMove, int msLeft);

    // Flag to tell if the player is running within the test_minimax context
    bool testingMinimax;
};

#endif
