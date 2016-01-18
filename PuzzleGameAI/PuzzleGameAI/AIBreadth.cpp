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
		for (auto& move : GetNextBoardStates(Q.front()))
		{
			Q.push(move);
		}
		Q.pop();
	}

	std::stack<UINT> MessageStack;
	while (Solution->Click && m_pParent->isGameRunning());
	{
		MessageStack.push(Solution->Click);
		Solution = Solution->LastMove;
	}

	while (MessageStack.top() && m_pParent->isGameRunning())
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

	for (UINT x = 0; x < LastMove->Board.size(); ++x)
	{
		for (UINT y = 0; y <LastMove->Board[x].size(); ++y)
		{
			if (!LastMove->Board[x][y])
			{
				EmptyIndex_X = x;
				EmptyIndex_Y = y;
			}
			
		}
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
	if (EmptyIndex_X + 1 < 4)
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
	if (EmptyIndex_Y + 1 < 4)
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
