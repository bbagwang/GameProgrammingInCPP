#include "BoardState.h"

#include "Random.h"

BoardState::BoardState()
{
	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < 7; j++)
		{
			mBoard[i][j] = Empty;
		}
	}
}

std::vector<BoardState*> BoardState::GetPossibleMoves(SquareState player) const
{
	std::vector<BoardState*> retVal;

	// For each column, find if a move is possible
	for (int col = 0; col < 7; col++)
	{
		for (int row = 5; row >= 0; row--)
		{
			if (mBoard[row][col] == BoardState::Empty)
			{
				retVal.emplace_back(new BoardState(*this));
				retVal.back()->mBoard[row][col] = player;
				break;
			}
		}
	}

	return retVal;
}

bool BoardState::IsTerminal() const
{
	// Is the board full?
	if (IsFull())
	{
		return true;
	}

	// Is there a four-in-a-row?
	int fourInRow = GetFourInARow();
	if (fourInRow != 0)
	{
		return true;
	}

	return false;
}

float BoardState::GetScore() const
{
	// If the board is full, the score is 0
	if (IsFull())
	{
		return 0.0f;
	}

	// Is there a four-in-a-row?
	int fourInRow = GetFourInARow();
	if (fourInRow != 0)
	{
		return static_cast<float>(fourInRow);
	}

	return CalculateHeuristic();
}

bool BoardState::IsFull() const
{
	bool isFull = true;
	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < 7; j++)
		{
			if (mBoard[i][j] == Empty)
			{
				isFull = false;
				break;
			}
		}
	}

	return isFull;
}

int BoardState::GetFourInARow() const
{
	// Returns -1 if yellow wins, 1 if red wins, 0 otherwise

	// Check if there's a row with four in a row
	for (int row = 0; row < 6; row++)
	{
		for (int col = 0; col < 4; col++)
		{
			if (mBoard[row][col] == mBoard[row][col + 1] &&
				mBoard[row][col] == mBoard[row][col + 2] &&
				mBoard[row][col] == mBoard[row][col + 3])
			{
				if (mBoard[row][col] == BoardState::Yellow)
				{
					return -1;
				}
				else if (mBoard[row][col] == BoardState::Red)
				{
					return 1;
				}
			}
		}
	}

	// Check if there's a column with four in a row
	for (int col = 0; col < 7; col++)
	{
		for (int row = 0; row < 3; row++)
		{
			if (mBoard[row][col] == mBoard[row + 1][col] &&
				mBoard[row][col] == mBoard[row + 2][col] &&
				mBoard[row][col] == mBoard[row + 3][col])
			{
				if (mBoard[row][col] == BoardState::Yellow)
				{
					return -1;
				}
				else if (mBoard[row][col] == BoardState::Red)
				{
					return 1;
				}
			}
		}
	}

	// Check if there's a right diagonal four in a row
	for (int col = 0; col < 4; col++)
	{
		for (int row = 0; row < 3; row++)
		{
			if (mBoard[row][col] == mBoard[row + 1][col + 1] &&
				mBoard[row][col] == mBoard[row + 2][col + 2] &&
				mBoard[row][col] == mBoard[row + 3][col + 3])
			{
				if (mBoard[row][col] == BoardState::Yellow)
				{
					return -1;
				}
				else if (mBoard[row][col] == BoardState::Red)
				{
					return 1;
				}
			}
		}
	}

	// Check if there's a left diagonal for in a row
	for (int col = 0; col < 4; col++)
	{
		for (int row = 3; row < 6; row++)
		{
			if (mBoard[row][col] == mBoard[row - 1][col + 1] &&
				mBoard[row][col] == mBoard[row - 2][col + 2] &&
				mBoard[row][col] == mBoard[row - 3][col + 3])
			{
				if (mBoard[row][col] == BoardState::Yellow)
				{
					return -1;
				}
				else if (mBoard[row][col] == BoardState::Red)
				{
					return 1;
				}
			}
		}
	}
	return 0;
}

float BoardState::CalculateHeuristic() const
{
	float BestScore = 0.f;
	//// Check if there's a row with four in a row
	//for (int row = 0; row < 6; row++)
	//{
	//	for (int col = 0; col < 4; col++)
	//	{
	//		if (mBoard[row][col] == mBoard[row][col + 1] &&
	//			mBoard[row][col] == mBoard[row][col + 2] &&
	//			mBoard[row][col] == mBoard[row][col + 3])
	//		{
	//			BestScore = 100;
	//		}
	//		else if (mBoard[row][col] == mBoard[row][col + 1] &&
	//				mBoard[row][col] == mBoard[row][col + 2])
	//		{
	//			BestScore = 75;
	//		}
	//		else if (mBoard[row][col] == mBoard[row][col + 1])
	//		{
	//			BestScore = 50;
	//		}
	//	}
	//}

	//// Check if there's a column with four in a row
	//for (int col = 0; col < 7; col++)
	//{
	//	for (int row = 0; row < 3; row++)
	//	{
	//		if (mBoard[row][col] == mBoard[row + 1][col] &&
	//			mBoard[row][col] == mBoard[row + 2][col] &&
	//			mBoard[row][col] == mBoard[row + 3][col])
	//		{
	//			BestScore = 100;
	//		}
	//		else if (mBoard[row][col] == mBoard[row + 1][col] &&
	//			mBoard[row][col] == mBoard[row + 2][col])
	//		{
	//			BestScore = 75;
	//		}
	//		else if (mBoard[row][col] == mBoard[row + 1][col])
	//		{
	//			BestScore = 50;
	//		}
	//	}
	//}

	//// Check if there's a right diagonal four in a row
	//for (int col = 0; col < 4; col++)
	//{
	//	for (int row = 0; row < 3; row++)
	//	{
	//		if (mBoard[row][col] == mBoard[row + 1][col + 1] &&
	//			mBoard[row][col] == mBoard[row + 2][col + 2] &&
	//			mBoard[row][col] == mBoard[row + 3][col + 3])
	//		{
	//			BestScore = 100;
	//		}
	//		else if (mBoard[row][col] == mBoard[row + 1][col + 1] &&
	//			mBoard[row][col] == mBoard[row + 2][col + 2])
	//		{
	//			BestScore = 75;
	//		}
	//		else if (mBoard[row][col] == mBoard[row + 1][col + 1])
	//		{
	//			BestScore = 50;
	//		}
	//	}
	//}

	//// Check if there's a left diagonal for in a row
	//for (int col = 0; col < 4; col++)
	//{
	//	for (int row = 3; row < 6; row++)
	//	{
	//		if (mBoard[row][col] == mBoard[row - 1][col + 1] &&
	//			mBoard[row][col] == mBoard[row - 2][col + 2] &&
	//			mBoard[row][col] == mBoard[row - 3][col + 3])
	//		{
	//			BestScore = 100;
	//		}
	//		else if (mBoard[row][col] == mBoard[row - 1][col + 1] &&
	//			mBoard[row][col] == mBoard[row - 2][col + 2])
	//		{
	//			BestScore = 75;
	//		}
	//		else if (mBoard[row][col] == mBoard[row - 1][col + 1])
	//		{
	//				BestScore = 50;
	//		}
	//	}
	//}

	return BestScore;
}

float AlphaBetaMin(const BoardState* node, int Depth, float alpha, float beta)
{
	
	if (Depth == 0 || node->IsTerminal())
	{
		return node->GetScore();
	}

	float minValue = std::numeric_limits<float>::infinity();
	// Find the subtree with the minimum value
	for (const BoardState* child : node->GetPossibleMoves(BoardState::Red))
	{
		minValue = std::min(minValue, AlphaBetaMax(child, Depth - 1, alpha, beta));
		if (minValue <= alpha)
		{
			return minValue; // Alpha prune
		}
		beta = std::min(minValue, beta);
	}
	return minValue;
}

float AlphaBetaMax(const BoardState* node, int Depth, float alpha, float beta)
{
	// If this is a leaf, return score
	if (/*node->mChildren.empty() ||*/ Depth == 0 || node->IsTerminal())
	{
		return node->GetScore();
	}

	float maxValue = -std::numeric_limits<float>::infinity();
	// Find the subtree with the maximum value
	for (const BoardState* child : node->GetPossibleMoves(BoardState::Red))
	{
		maxValue = std::max(maxValue, AlphaBetaMin(child, Depth - 1, alpha, beta));
		if (maxValue >= beta)
		{
			return maxValue; // Beta prune
		}
		alpha = std::max(maxValue, alpha);
	}
	return maxValue;
}

BoardState* AlphaBetaDecide(const BoardState* root, int Depth)
{
	// Find the subtree with the maximum value, and save the choice
	BoardState* choice = nullptr;
	float maxValue = -std::numeric_limits<float>::infinity();
	float beta = std::numeric_limits<float>::infinity();
	for (BoardState* child : root->GetPossibleMoves(BoardState::Red))
	{
		float v = AlphaBetaMin(child, Depth, maxValue, beta);
		if (v > maxValue)
		{
			maxValue = v;
			choice = child;
		}
	}
	return choice;
}

bool TryPlayerMove(BoardState* state, int column)
{
	// Find the first row in that column that's available
	// (if any)
	for (int row = 5; row >= 0; row--)
	{
		if (state->mBoard[row][column] == BoardState::Empty)
		{
			state->mBoard[row][column] = BoardState::Yellow;
			return true;
		}
	}

	return false;
}

float CPUMove(BoardState* State, int Depth)
{
	BoardState* Result = AlphaBetaDecide(State, Depth);
	
	*State = *Result;

	return 0.f;
}
