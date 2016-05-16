
// PuzzleGameAIDlg.h : header file
//
#pragma once
#include <vector>
#include <string>
#include <memory>
#include "AI.h"
#include "resource.h"

#define AI_BTN_CLICK  (WM_USER + 100)


const static UINT PuzzleIDs[4][4] = { { IDC_PUZZLE_0_0, IDC_PUZZLE_0_1, IDC_PUZZLE_0_2, IDC_PUZZLE_0_3 },
{ IDC_PUZZLE_1_0, IDC_PUZZLE_1_1, IDC_PUZZLE_1_2, IDC_PUZZLE_1_3 },
{ IDC_PUZZLE_2_0, IDC_PUZZLE_2_1, IDC_PUZZLE_2_2, IDC_PUZZLE_2_3 },
{ IDC_PUZZLE_3_0, IDC_PUZZLE_3_1, IDC_PUZZLE_3_2, IDC_PUZZLE_3_3 } };


class CAIBreadth;
class CAIAStarMan;

// CPuzzleGameAIDlg dialog
class CPuzzleGameAIDlg : public CDialogEx
{
// Construction
public:
	CPuzzleGameAIDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_PUZZLEGAMEAI_DIALOG };

	typedef std::vector<std::vector<int>> BoardInfo;

	bool CheckWinCondition(size_t Hash);

	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnPuzzleClick(UINT id);
	void UpdateState(UINT id, BoardInfo& CurrentState);
	std::vector<UINT> GetAvailableMoves(BoardInfo& CurrentState);
	afx_msg void OnBnClickedSuffle();

	BITMASK GenHash(BoardInfo& CurrentState);
	
	bool isGameRunning() {return m_bGameRunning;}

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	virtual BOOL OnInitDialog();

	afx_msg void OnSizeChange(UINT id);

	afx_msg void OnPlayerSelectChange(UINT id);

	afx_msg LRESULT OnAIClick(WPARAM wParam, LPARAM lParam);

	void SetPlayerSelectRadio(UINT CtrlID);

	void resetBoard();
	void RedrawPuzzle();
	void SetButtons();

	void StartGame();
	void CheckWin();
	void EndGame();

private:
	int m_AiSpeed = 0;

	BoardInfo m_boardValues;

	size_t m_CompleteHash;

	bool m_bGameRunning = false;

	bool m_bAIPlaying = false;

	std::shared_ptr<CAIBreadth> m_pAIBreadth;

	std::shared_ptr<CAIAStarMan> m_pAIStar;

	std::shared_ptr<CAIBase> m_pAI; 

	DECLARE_MESSAGE_MAP()
	
};
