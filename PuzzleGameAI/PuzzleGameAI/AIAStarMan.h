#pragma once
#include "AI.h"
#include <thread>
#include <queue>
#include <map>


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
		size_t Hash = 0;
	};

	std::function<bool(const std::shared_ptr<MoveInfo>, const std::shared_ptr<MoveInfo>)> Compare = [](const std::shared_ptr<MoveInfo> Move1, const std::shared_ptr<MoveInfo> Move2){return Move1->TotalDist() > Move2->TotalDist(); };
	typedef std::priority_queue<std::shared_ptr<MoveInfo>, std::vector<std::shared_ptr<MoveInfo>>, decltype(Compare)> MyQueue;

	std::vector<std::shared_ptr<MoveInfo>> GetNextBoardStates(const std::shared_ptr<MoveInfo>& LastMove);

	void solve();

	std::vector<std::thread> m_Threads;

	bool isValid(const std::shared_ptr<MoveInfo>& Move, std::map<size_t, std::shared_ptr<MoveInfo>>& MovesDone);

	int DistanceLeft(BoardInfo& Board);

	
};

