#pragma once
#include "AI.h"
#include <thread>
#include <map>


class CAIBreadth :
	public CAIBase
{
public:
	CAIBreadth(CPuzzleGameAIDlg* parent);
	~CAIBreadth();

	virtual void Startgame(BoardInfo BoardStart);

protected:

	struct MoveInfo
	{
		BoardInfo Board;
		UINT Click;
		std::shared_ptr<MoveInfo> LastMove;
		size_t Hash = 0;
	};

	std::vector<std::shared_ptr<MoveInfo>> GetNextBoardStates(std::shared_ptr<MoveInfo> LastMove);


	void solve();
	
	std::vector<std::thread> m_Threads;

	bool isValid(const std::shared_ptr<MoveInfo>& Move, std::map<size_t, std::shared_ptr<MoveInfo>>& MovesDone);
};

