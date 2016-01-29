#include "stdafx.h"
#include "AIAStarMan.h"
#include "PuzzleGameAIDlg.h"
#include <queue>
#include <stack>
#include <thread>
#include <chrono>
#include <algorithm>

#include <conio.h>

#define CHRONO_NOW std::chrono::high_resolution_clock::now()
#define CHRONO_DURATION std::chrono::duration_cast<std::chrono::microseconds>

CAIAStarMan::CAIAStarMan(CPuzzleGameAIDlg* parent) : CAIBase(parent)
{

}


CAIAStarMan::~CAIAStarMan()
{
	if (m_Thread.joinable())
		m_Thread.join();
}

void CAIAStarMan::Startgame(BoardInfo BoardStart)
{
	CAIBase::Startgame(BoardStart);
	m_MovesDone.clear();
	std::thread t([&](){solve(); });

	if (m_Thread.joinable())
		m_Thread.join();

	m_Thread.swap(t);

}

void CAIAStarMan::solve()
{
	bool bSolved = false;
	std::vector<std::shared_ptr<MoveInfo>> Q;

	auto start = std::make_shared<MoveInfo>();
	start->Board = m_BoardView;
	start->Click = NULL;
	start->LastMove = NULL;
	start->DistanceGone = 0;
	start->Distance2Go = DistanceLeft(m_BoardView);

	Q.push_back(start);

	std::chrono::high_resolution_clock::time_point Tp = CHRONO_NOW;

	std::shared_ptr<MoveInfo> Solution;

	while (!bSolved && m_pParent->isGameRunning())
	{
#ifdef TIMING 
		Tp = CHRONO_NOW;
#endif
		if (m_pParent->CheckWinCondition(Q.front()->Board))
		{
			Solution = Q.front();
			bSolved = true;
			break;
		}

#ifdef TIMING 
		if (CHRONO_DURATION(CHRONO_NOW - Tp).count())
			_cprintf("CheckWinCondition %i \n", CHRONO_DURATION(CHRONO_NOW - Tp).count());


		Tp = CHRONO_NOW;
#endif
		auto Nextmoves = GetNextBoardStates(Q.front());

#ifdef TIMING 
		if (CHRONO_DURATION(CHRONO_NOW - Tp).count())
			_cprintf("GetNextBoardStates %i \n", CHRONO_DURATION(CHRONO_NOW - Tp).count());
#endif
		for (auto& move : Nextmoves)
		{
#ifdef TIMING 
			Tp = CHRONO_NOW;
#endif
			if (isValid(move))
			{
				Q.push_back(move);
				m_MovesDone.push_back(move);
			}
#ifdef TIMING 
			if (CHRONO_DURATION(CHRONO_NOW - Tp).count())
				_cprintf("isValid %i \n", CHRONO_DURATION(CHRONO_NOW - Tp).count());


			_cprintf("m_movesDone %i \n", m_MovesDone.size());
			_cprintf("Q %i \n", Q.size());
#endif
		}
		Q.erase(Q.begin());

		std::sort(Q.begin(), Q.end(),
				  [](std::shared_ptr<MoveInfo> Move1, std::shared_ptr<MoveInfo> Move2)
				  ->bool{return Move1->TotalDist() < Move2->TotalDist(); });
	}

	std::stack<UINT> MessageStack;
	while (m_pParent->isGameRunning() && Solution && Solution->Click)
	{
		MessageStack.push(Solution->Click);
		Solution = Solution->LastMove;
	}

	while (m_pParent->isGameRunning() && MessageStack.size())
	{
		SendClick(MessageStack.top());
		MessageStack.pop();
	}

}

bool CAIAStarMan::isValid(std::shared_ptr<MoveInfo> move)
{
	bool bValid = true;



	for (int idx = m_MovesDone.size() - 1; idx >= 0; --idx)
	{
		bool bisSame = true;

		for (int x = 0, lengthX = move->Board.size(); x < lengthX; ++x)
		{
			for (int y = 0, length = move->Board[x].size(); y < length; ++y)
			{
				if (m_MovesDone[idx]->Board[x][y] != move->Board[x][y])
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

			if(move->TotalDist() < m_MovesDone[idx]->TotalDist())
			{
				m_MovesDone.erase(m_MovesDone.begin() + idx);
			}
			else
			{
				bValid = false;
			}
			break;
		}
	}

	

	return bValid;
}


std::vector<std::shared_ptr<CAIAStarMan::MoveInfo>> CAIAStarMan::GetNextBoardStates(std::shared_ptr<MoveInfo> LastMove)
{

	std::vector<std::shared_ptr<MoveInfo>> newStates;
	auto availableMoves = m_pParent->GetAvailableMoves(LastMove->Board);

	for (auto ID : availableMoves)
	{
		std::shared_ptr<MoveInfo> newState = std::make_shared<MoveInfo>();
		newState->Board = LastMove->Board;

		newState->Click = ID;

		m_pParent->UpdateState(ID, newState->Board);

		newState->LastMove = LastMove;

		newState->DistanceGone = LastMove->DistanceGone + 1;

		newState->Distance2Go = DistanceLeft(newState->Board);

		newStates.push_back(newState);
	}

	return newStates;

}

int CAIAStarMan::DistanceLeft(BoardInfo& Board)
{
	int dist = 0;
	for (int Number = 0; Number < Board.size()*Board[0].size(); ++Number)
	{
		bool bfound = false;
		for (int x = 0; x < Board.size(); ++x)
		{
			for (int y = 0; y < Board[x].size(); ++y)
			{
				if (Number == 0 && Board[x][y] == Number)
				{
					int locX = Board.size()-1;
					int locY = Board[Board.size() - 1].size();

					dist += abs(locX - x);
					dist += abs(locY - y);

					bfound = true;
					break;
				}
				
				if (Board[x][y] == Number)
				{
					int locX = (Number - 1) % Board.size();
					int locY = (Number - 1) / Board.size();

					dist += abs(locX - x);
					dist += abs(locY - y);

					bfound = true;
					break;
				}
				
			}
			if (bfound)
				break;
		}
	}

	return dist;
}

