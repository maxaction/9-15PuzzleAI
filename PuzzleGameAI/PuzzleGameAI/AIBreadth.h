#pragma once
#include "AI.h"
class CAIBreadth :
	public CAIBase
{
public:
	CAIBreadth(CPuzzleGameAIDlg* parent);
	~CAIBreadth();

	virtual void Startgame(BoardInfo BoardStart);
};

