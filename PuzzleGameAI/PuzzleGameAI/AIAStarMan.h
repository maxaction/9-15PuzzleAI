#pragma once
#include "AI.h"
#include <thread>

class CAIAStarMan :
	public CAIBase
{
public:
	CAIAStarMan(CPuzzleGameAIDlg* parent);
	~CAIAStarMan();

	virtual void Startgame(BoardInfo BoardStart);

protected:

	struct MoveInfo
	{
		BoardInfo Board;
		UINT Click;
		std::shared_ptr<MoveInfo> LastMove;
		int DistanceGone = 0, Distance2Go = 0;
		int TotalDist() {return DistanceGone + Distance2Go;}
		BITMASK Hash = 0;
	};

	std::vector<std::shared_ptr<MoveInfo>> GetNextBoardStates(std::shared_ptr<MoveInfo> LastMove);

	void solve();

	std::thread m_Thread;

	bool isValid(std::shared_ptr<MoveInfo>);

	int DistanceLeft(BoardInfo& Board);

	std::vector<std::shared_ptr<MoveInfo>> m_MovesDone;
};

