#pragma once

#include <vector>


class BoardState
{
public:
	enum SquareState { Empty, Red, Yellow };

	BoardState();

	std::vector<BoardState*> GetPossibleMoves(SquareState player) const;

	bool IsTerminal() const;
	float GetScore() const;

	SquareState mBoard[6][7];

	bool IsFull() const;
	int GetFourInARow() const;
	float CalculateHeuristic() const;
};

struct BoardGTNode
{
	// Children nodes
	std::vector<BoardGTNode*> mChildren;
	// State of game
	BoardState mState;
};

float AlphaBetaMin(const BoardState* node, int Depth, float alpha, float beta);

float AlphaBetaMax(const BoardState* node, int Depth, float alpha, float beta);

BoardState* AlphaBetaDecide(const BoardState* root, int Depth);

// Try to place the player's piece
bool TryPlayerMove(class BoardState* state, int column);

// Make the next CPU move
float CPUMove(class BoardState* state, int Depth);