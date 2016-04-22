#pragma once
#include "AI.h"
#include <thread>


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
	
	std::thread Thread;

	bool isValid(std::shared_ptr<MoveInfo>);

	std::vector<std::shared_ptr<MoveInfo>> m_MovesDone;
};

