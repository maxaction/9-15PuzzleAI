#include "stdafx.h"
#include "AIBreadth.h"


CAIBreadth::CAIBreadth(CPuzzleGameAIDlg* parent): CAIBase(parent)
{
}


CAIBreadth::~CAIBreadth()
{
}

void CAIBreadth::Startgame(BoardInfo BoardStart)
{
	m_BoardView = BoardStart;
}
