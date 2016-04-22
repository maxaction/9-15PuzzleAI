#include "stdafx.h"
#include "AIBreadth.h"
#include "PuzzleGameAIDlg.h"
#include <queue>
#include <stack>
#include <thread>
#include <chrono>

#include <conio.h>


#define CHRONO_NOW std::chrono::high_resolution_clock::now()
#define CHRONO_DURATION std::chrono::duration_cast<std::chrono::microseconds>

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
	CAIBase::Startgame(BoardStart);
	m_MovesDone.clear();
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
		if(m_pParent->CheckWinCondition(Q.front()->Hash))
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
				Q.push(move);
				m_MovesDone.insert(std::pair<size_t, std::shared_ptr<MoveInfo>>(move->Hash, move));
			}
#ifdef TIMING 
			if (CHRONO_DURATION(CHRONO_NOW - Tp).count())
				_cprintf("isValid %i \n", CHRONO_DURATION(CHRONO_NOW - Tp).count());


			_cprintf("m_movesDone %i \n", m_MovesDone.size());
			_cprintf("Q %i \n", Q.size());
#endif
		}
		Q.pop();
	}

#if defined(TIMING) || defined(TIMING_BASIC)

	_cprintf("Time Taken: %i ms \n", std::chrono::duration_cast<std::chrono::milliseconds>(CHRONO_NOW - TotalTime).count());
	_cprintf("Moves Done: %i \n", m_MovesDone.size());
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

bool CAIBreadth::isValid(std::shared_ptr<MoveInfo> move)
{
	bool bValid = true;

	std::map<size_t, std::shared_ptr<MoveInfo>>::iterator it;

	it = m_MovesDone.find(move->Hash);

	if (it != m_MovesDone.end())
	{
		if (it->second->Hash == move->Hash)
		{
			bValid = false;
		}
	}

	return bValid;
}

std::vector<std::shared_ptr<CAIBreadth::MoveInfo>> CAIBreadth::GetNextBoardStates(std::shared_ptr<MoveInfo> LastMove)
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

		newState->Hash = std::hash<BITMASK>()(m_pParent->GenHash(newState->Board));

		newStates.push_back(newState);
	}

	return newStates;

}