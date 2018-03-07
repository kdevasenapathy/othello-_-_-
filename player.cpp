#include "player.hpp"

/*
 * Constructor for the player; initialize everything here. The side your AI is
 * on (BLACK or WHITE) is passed in as "side". The constructor must finish
 * within 30 seconds.
 */
Player::Player(Side side) {
    // Will be set to true in test_minimax.cpp.
    testingMinimax = false;    

    /*
     * TODO: Do any initialization you need to do here (setting up the board,
     * precalculating things, etc.) However, remember that you will only have
     * 30 seconds.
     */
    boardState = new Board;
    ourSide = side;
    srand(time(NULL));


}

/*
 * Destructor for the player.
 */
Player::~Player() {
    delete boardState;
}

/*
 * Compute the next move given the opponent's last move. Your AI is
 * expected to keep track of the board on its own. If this is the first move,
 * or if the opponent passed on the last move, then opponentsMove will be
 * nullptr.
 *
 * msLeft represents the time your AI has left for the total game, in
 * milliseconds. doMove() must take no longer than msLeft, or your AI will
 * be disqualified! An msLeft value of -1 indicates no time limit.
 *
 * The move returned must be legal; if there are no valid moves for your side,
 * return nullptr.
 */
Move *Player::doMove(Move *opponentsMove, int msLeft) {
    /*
     * TODO: Implement how moves your AI should play here. You should first
     * process the opponent's opponents move before calculating your own move
     */
    vector<Move*> possible;

    if (opponentsMove != nullptr)
    {
        Side other = (ourSide == BLACK) ? WHITE : BLACK;
        boardState->doMove(opponentsMove, other);
    }

    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            Move* move = new Move(i, j);
            if (boardState->checkMove(move, ourSide))
            {
                possible.push_back(move);
            }
            else
            {
                delete move;
            }
        }
    }

    if (possible.size() == 0)
    {
        return nullptr;
    }

    unsigned int index = rand() % (possible.size());

    boardState->doMove(possible[index], ourSide);

    for(unsigned int i = 0; i < possible.size(); i++)
    {
        if (i != index)
        {
            delete possible[i];
        }
    }

    return possible[index];
}
