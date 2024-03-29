#include "stdafx.h"
#include "AI.h"
#include "PuzzleGameAIDlg.h"

CAIBase::CAIBase(CPuzzleGameAIDlg* parent)
{
	m_pParent = parent;
}


CAIBase::~CAIBase()
{
}

void CAIBase::SendClick(UINT ClickID)
{
	if (m_nSpeed)
		Sleep(m_nSpeed);
	if(m_pParent)
		m_pParent->PostMessage(AI_BTN_CLICK, ClickID);
}

