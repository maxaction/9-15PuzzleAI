#include "stdafx.h"
#include "AIBreadth.h"
#include "PuzzleGameAIDlg.h"
#include <queue>
#include <stack>
#include <thread>


CAIBreadth::CAIBreadth(CPuzzleGameAIDlg* parent): CAIBase(parent)
{
}


CAIBreadth::~CAIBreadth()
{
	if(Thread.joinable())
		Thread.join();

}

void CAIBreadth::Startgame(BoardInfo BoardStart)
{
	m_BoardView = BoardStart;
	std::thread t([&](){solve();});

	if(Thread.joinable())
		Thread.join();

	Thread.swap(t);
}

void CAIBreadth::solve()
{
	bool bSolved = false;
	std::queue<std::shared_ptr<MoveInfo>> Q;

	auto start = std::make_shared<MoveInfo>();
	start->Board = m_BoardView;
	start->Click = NULL;
	start->LastMove = NULL;

	Q.push(start);

	std::shared_ptr<MoveInfo> Solution;

	while (!bSolved && m_pParent->isGameRunning())
	{
		if(m_pParent->CheckWinCondition(Q.front()->Board))
		{
			Solution = Q.front();
			bSolved = true;
			break;
		}

		auto Nextmoves = GetNextBoardStates(Q.front());
	
		for (auto& move : Nextmoves)
		{
			if (isValid(move))
			{
				Q.push(move);
				m_MovesDone.push_back(move);
			}
		}
		Q.pop();
	}

	std::stack<UINT> MessageStack;
	while (m_pParent->isGameRunning() && Solution && Solution->Click)
	{
		MessageStack.push(Solution->Click);
		Solution = Solution->LastMove;
	}

	while (m_pParent->isGameRunning() && MessageStack.top())
	{
		SendClick(MessageStack.top());
		MessageStack.pop();
	}
	
}

std::vector<std::shared_ptr<CAIBreadth::MoveInfo>> CAIBreadth::GetNextBoardStates(std::shared_ptr<MoveInfo> LastMove)
{
	int ClickIndex_X = 0, ClickIndex_Y = 0,
		EmptyIndex_X = 0, EmptyIndex_Y = 0;

	std::vector<std::shared_ptr<MoveInfo>> newStates;
	bool found = false;
	for (UINT x = 0; x < LastMove->Board.size(); ++x)
	{
		for (UINT y = 0; y <LastMove->Board[x].size(); ++y)
		{
			if (!LastMove->Board[x][y])
			{
				EmptyIndex_X = x;
				EmptyIndex_Y = y;
				found= true;
				break;
			}
			
		}
		if(found)
			break;
	}

	if (EmptyIndex_X - 1 >= 0)
	{
		std::shared_ptr<MoveInfo> newState = std::make_shared<MoveInfo>();
		newState->Board = LastMove->Board;
		
		newState->Board[EmptyIndex_X][EmptyIndex_Y] = newState->Board[EmptyIndex_X - 1][EmptyIndex_Y];
		newState->Board[EmptyIndex_X - 1][EmptyIndex_Y] = 0;

		newState->Click = PuzzleIDs[EmptyIndex_X - 1][EmptyIndex_Y];

		newState->LastMove = LastMove;


		newStates.push_back(newState);
	}
	if (EmptyIndex_X + 1 < LastMove->Board.size())
	{
		std::shared_ptr<MoveInfo> newState = std::make_shared<MoveInfo>();
		newState->Board = LastMove->Board;

		newState->Board[EmptyIndex_X][EmptyIndex_Y] = newState->Board[EmptyIndex_X + 1][EmptyIndex_Y];
		newState->Board[EmptyIndex_X + 1][EmptyIndex_Y] = 0;

		newState->Click = PuzzleIDs[EmptyIndex_X + 1][EmptyIndex_Y];

		newState->LastMove = LastMove;
		newStates.push_back(newState);
	}
	if (EmptyIndex_Y - 1 >= 0)
	{
		std::shared_ptr<MoveInfo> newState = std::make_shared<MoveInfo>();
		newState->Board = LastMove->Board;

		newState->Board[EmptyIndex_X][EmptyIndex_Y] = newState->Board[EmptyIndex_X][EmptyIndex_Y - 1];
		newState->Board[EmptyIndex_X][EmptyIndex_Y - 1] = 0;

		newState->Click = PuzzleIDs[EmptyIndex_X][EmptyIndex_Y - 1];

		newState->LastMove = LastMove;
		newStates.push_back(newState);
	}
	if (EmptyIndex_Y + 1 < LastMove->Board[EmptyIndex_X].size())
	{
		std::shared_ptr<MoveInfo> newState = std::make_shared<MoveInfo>();
		newState->Board = LastMove->Board;

		newState->Board[EmptyIndex_X][EmptyIndex_Y] = newState->Board[EmptyIndex_X][EmptyIndex_Y + 1];
		newState->Board[EmptyIndex_X][EmptyIndex_Y + 1] = 0;

		newState->Click = PuzzleIDs[EmptyIndex_X][EmptyIndex_Y + 1];

		newState->LastMove = LastMove;
		newStates.push_back(newState);
	}

	return newStates;

}


bool CAIBreadth::isValid(std::shared_ptr<MoveInfo> move)
{
	bool bValid = true;

	

	for (auto& PastMoves : m_MovesDone)
	{
		bool bisSame = true;

		for (int x = 0; x < move->Board.size(); ++x)
		{
			for (int y = 0; y < move->Board[x].size(); ++y)
			{
				if (PastMoves->Board[x][y] != move->Board[x][y])
				{
					bisSame = false;
					break;
				}
			}
			if (!bisSame)
				break;
		}
		if (bisSame)
		{
			bValid = false;
			break;
		}
	}

	return bValid;
}
