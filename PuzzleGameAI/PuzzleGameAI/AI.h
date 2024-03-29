#pragma once
#include<vector>
#include<memory>
#include<bitset>



#define BITMASK std::bitset<128>



class CPuzzleGameAIDlg;

class CAIBase
{
public:
	CAIBase(CPuzzleGameAIDlg* parent);
	~CAIBase();
	typedef std::vector<std::vector<int>> BoardInfo;
	virtual void Startgame(BoardInfo BoardStart) { m_BoardView = BoardStart;}

	void UpdateBoardView(BoardInfo Board) { m_BoardView = Board; }

	void SetAISpeed(int milliseconds) { m_nSpeed = milliseconds;}

protected:
	
	void SendClick(UINT ClickID);

	CPuzzleGameAIDlg* m_pParent;

	BoardInfo m_BoardView;

	int m_nSpeed = 0;
};

