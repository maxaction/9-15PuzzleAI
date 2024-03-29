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
	for (auto& t : m_Threads)
	{
		if (t.joinable())
			t.join();
	}
}

void CAIAStarMan::Startgame(BoardInfo BoardStart)
{
	CAIBase::Startgame(BoardStart);
	m_Threads.push_back(std::thread([&](){solve(); }));

}

void CAIAStarMan::solve()
{
	bool bSolved = false;

	MyQueue Q(Compare);
	std::map<size_t, std::shared_ptr<MoveInfo>> MovesDone;

	auto start = std::make_shared<MoveInfo>();
	start->Board = m_BoardView;
	start->Click = NULL;
	start->LastMove = NULL;
	start->DistanceGone = 0;
	start->Distance2Go = DistanceLeft(m_BoardView);
	start->Hash = std::hash<BITMASK>()(m_pParent->GenHash(m_BoardView));

	Q.push(start);

	std::chrono::high_resolution_clock::time_point Tp = CHRONO_NOW;
	std::chrono::high_resolution_clock::time_point TotalTime = CHRONO_NOW;

	std::shared_ptr<MoveInfo> Solution;

	while (!bSolved && m_pParent->isGameRunning())
	{
#ifdef TIMING 
		Tp = CHRONO_NOW;
#endif
		if (m_pParent->CheckWinCondition(Q.top()->Hash))
		{
			Solution = Q.top();
			bSolved = true;
			break;
		}

#ifdef TIMING 
		if (CHRONO_DURATION(CHRONO_NOW - Tp).count())
			_cprintf("CheckWinCondition %i \n", CHRONO_DURATION(CHRONO_NOW - Tp).count());


		Tp = CHRONO_NOW;
#endif
		auto Nextmoves = GetNextBoardStates(Q.top());
		Q.pop();

#ifdef TIMING 
		if (CHRONO_DURATION(CHRONO_NOW - Tp).count())
			_cprintf("GetNextBoardStates %i \n", CHRONO_DURATION(CHRONO_NOW - Tp).count());
#endif
		

		for (auto& move : Nextmoves)
		{
#ifdef TIMING 
			Tp = CHRONO_NOW;
#endif
			if (isValid(move, MovesDone))
			{
				Q.push(move);
				MovesDone.insert(std::pair<size_t, std::shared_ptr<MoveInfo>>(move->Hash, move));
			}
#ifdef TIMING 
			if (CHRONO_DURATION(CHRONO_NOW - Tp).count())
				_cprintf("isValid %i \n", CHRONO_DURATION(CHRONO_NOW - Tp).count());


			_cprintf("MovesDone %i \n", MovesDone.size());
			_cprintf("Q %i \n", Q.size());
#endif
		}

#if defined(TIMING) || defined(TIMING_BASIC)
		Tp = CHRONO_NOW;
#endif
		
	}
#if defined(TIMING) || defined(TIMING_BASIC)
	_cprintf("Time Taken: %i ms \n", std::chrono::duration_cast<std::chrono::milliseconds>(CHRONO_NOW - TotalTime).count());
	_cprintf("Moves Done: %i \n", MovesDone.size());
	_cprintf("Queue Size: %i \n", Q.size());
#endif


	std::stack<UINT> MessageStack;
	while (m_pParent->isGameRunning() && Solution && Solution->Click)
	{
		MessageStack.push(Solution->Click);
		Solution = Solution->LastMove;
	}

#if defined(TIMING) || defined(TIMING_BASIC)
	_cprintf("MovestoTake: %i  \n", MessageStack.size());
#endif

	while (m_pParent->isGameRunning() && MessageStack.size())
	{
		SendClick(MessageStack.top());
		MessageStack.pop();
	}
}

bool CAIAStarMan::isValid(const std::shared_ptr<MoveInfo>& move, std::map<size_t, std::shared_ptr<MoveInfo>> & MovesDone)
{
	bool bValid = true;

	std::map<size_t, std::shared_ptr<MoveInfo>>::iterator it;

	it = MovesDone.find(move->Hash);

	if (it != MovesDone.end())
	{
		if (it->second->Hash == move->Hash)
		{
			if (move->DistanceGone < it->second->DistanceGone)
			{
				MovesDone.erase(it);
			}
			else
			{
				bValid = false;
			}
		}
	}

	return bValid;
}


std::vector<std::shared_ptr<CAIAStarMan::MoveInfo>> CAIAStarMan::GetNextBoardStates(const std::shared_ptr<MoveInfo>& LastMove)
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
		newState->Hash = std::hash<BITMASK>()(m_pParent->GenHash(newState->Board));

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

