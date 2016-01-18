#pragma once
#include "AI.h"
#include <memory>
#include <thread>

class CAIBreadth :
	public CAIBase
{
public:
	CAIBreadth(CPuzzleGameAIDlg* parent);
	~CAIBreadth();

	virtual void Startgame(BoardInfo BoardStart);

protected:

	void solve();
	struct MoveInfo;
	struct MoveInfo
	{
		BoardInfo Board;
		UINT Click;
		std::shared_ptr<MoveInfo> LastMove;
	};

	std::thread Thread;

	std::vector<std::shared_ptr<MoveInfo>> GetNextBoardStates(std::shared_ptr<MoveInfo> LastMove);
};

