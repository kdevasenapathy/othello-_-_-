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
    if (!testingMinimax)
    {
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

        unsigned int best = 0;
        int bestWeight = calculateBoard(possible[best]);
        for (unsigned int i = 1; i < possible.size(); i++){
            if (bestWeight < calculateBoard(possible[i])){
                best = i;
                bestWeight = calculateBoard(possible[best]);
            }
        }

        for (unsigned int i = 0; i < possible.size(); i++){
            if (i != best){
                delete possible[i];
            }
        }

        boardState->doMove(possible[best], ourSide);
        return possible[best];
    }
    else
    {
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

        unsigned int best = 0;
        int bestWeight = -1 * ENDGAME;
        for (unsigned int i = 0; i < possible.size(); i++)
        {
            Board* copy = boardState->copy();
            copy->doMove(possible[i], ourSide);
            int minimaxer = minimax(copy, ourSide, 0);
            if (bestWeight < minimaxer){
                best = i;
                bestWeight = minimaxer;
            }
            delete copy;
        }
        for (unsigned int i = 0; i < possible.size(); i++){
            if (i != best){
                delete possible[i];
            }
        }

        boardState->doMove(possible[best], ourSide);
        return possible[best];
    }
}

int Player::calculateBoard(Move* move){
    Board* copy = boardState->copy();
    copy->doMove(move, ourSide);

    int sum = 0;
    for (unsigned int i = 0; i < 8; i++){
        for (unsigned int j = 0; j < 8; j++){
            if(copy->publicGet(ourSide, i, j)){
                sum += calculateWeight(i, j);
            }
        }
    }

    if (!copy->hasMoves((ourSide == BLACK) ? WHITE : BLACK))
    {
        sum = ENDGAME;
    }

    delete copy;

    return sum;
}

int Player::calculateWeight(int x, int y){
    int weight = 1;
    if(x % 7 == 0 || y % 7 == 0){ // edges
        weight = 2;
    }

    if(x % 7 == 0 && y % 7 == 0){ // corners
        weight = 9;
    }

    if(y % 7 == 0 && (x == 1 || x == 6)){ // next to corners
        weight = -2;
    }

    if(x % 7 == 0 && (y == 1 || y == 6)){ // next to corners
        weight = -2;
    }

    if((y == 1 || y == 6) && (x == 1 || x == 6)){ //very bad next to diag
        weight = -6;
    }

    return weight;
}

void Player::setBoard(char data[])
{
    if (testingMinimax)
    {
        boardState->setBoard(data);
    }
}

int Player::naiveHeuristic(Move* move, Board* state)
{
    Board* copy = state->copy();
    copy->doMove(move, ourSide);
    Side other = (ourSide == BLACK) ? WHITE : BLACK;
    int naive = copy->count(ourSide) - copy->count(other);

    delete copy;
    return naive;
}

int Player::minimax(Board* current, Side side, int currentDepth)
{
    if (side == ourSide)
    {
        vector<Move*> possible;

        for (int i = 0; i < 8; i++)
        {
            for (int j = 0; j < 8; j++)
            {
                Move* move = new Move(i, j);
                if (current->checkMove(move, side))
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
            return -1 * ENDGAME;
        }

        if (currentDepth == DEPTH)
        {
            int bestWeight = naiveHeuristic(possible[0], current);
            for (unsigned int i = 1; i < possible.size(); i++)
            {
                if (bestWeight < naiveHeuristic(possible[i], current))
                {
                    bestWeight = naiveHeuristic(possible[i], current);
                }
                delete possible[i];
            }
            return bestWeight;
        }
        else
        {
            int bestWeight = -1* ENDGAME;
            for (unsigned int i = 0; i < possible.size(); i++)
            {
                Board* copy = current->copy();
                copy->doMove(possible[i], side);
                int max = minimax(copy, (side == BLACK) ? WHITE : BLACK, currentDepth + 1);
                if (bestWeight < max)
                {
                    bestWeight = max;
                }
                delete copy;
                delete possible[i];
            }
            return bestWeight;
        }
    }
    else
    {
        vector<Move*> possible;

        for (int i = 0; i < 8; i++)
        {
            for (int j = 0; j < 8; j++)
            {
                Move* move = new Move(i, j);
                if (current->checkMove(move, side))
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
            return ENDGAME;
        }

        if (currentDepth == DEPTH)
        {
            int worstWeight = naiveHeuristic(possible[0], current);
            for (unsigned int i = 1; i < possible.size(); i++)
            {
                if (worstWeight > naiveHeuristic(possible[i], current))
                {
                    worstWeight = naiveHeuristic(possible[i], current);
                }
                delete possible[i];
            }
            return worstWeight;
        }
        else
        {
            int worstWeight = ENDGAME;
            for (unsigned int i = 0; i < possible.size(); i++)
            {
                Board* copy = current->copy();
                copy->doMove(possible[i], side);
                int min = minimax(copy, (side == BLACK) ? WHITE : BLACK, currentDepth + 1);
                if (worstWeight < min)
                {
                    worstWeight = min;
                }
                delete copy;
                delete possible[i];
            }
            return worstWeight;
        }
    }
}
