
// PuzzleGameAIDlg.h : header file
//
#pragma once
#include <vector>
#include <string>
#include <memory>
#include "AI.h"
#include "resource.h"


const static UINT PuzzleIDs[4][4] = { { IDC_PUZZLE_0_0, IDC_PUZZLE_0_1, IDC_PUZZLE_0_2, IDC_PUZZLE_0_3 },
{ IDC_PUZZLE_1_0, IDC_PUZZLE_1_1, IDC_PUZZLE_1_2, IDC_PUZZLE_1_3 },
{ IDC_PUZZLE_2_0, IDC_PUZZLE_2_1, IDC_PUZZLE_2_2, IDC_PUZZLE_2_3 },
{ IDC_PUZZLE_3_0, IDC_PUZZLE_3_1, IDC_PUZZLE_3_2, IDC_PUZZLE_3_3 } };

// CPuzzleGameAIDlg dialog
class CPuzzleGameAIDlg : public CDialogEx
{
// Construction
public:
	CPuzzleGameAIDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_PUZZLEGAMEAI_DIALOG };

	typedef std::vector<std::vector<int>> BoardInfo;

	bool CheckWinCondition(BoardInfo& board);

	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnPuzzleClick(UINT id);
	afx_msg void OnBnClickedSuffle();


	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	virtual BOOL OnInitDialog();

	afx_msg void OnSizeChange(UINT id);

	afx_msg void OnPlayerSelectChange(UINT id);
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

	bool m_bGameRunning = false;

	bool m_bAIPlaying = false;

	std::shared_ptr<CAIBase> AI; 

	DECLARE_MESSAGE_MAP()
	
};
