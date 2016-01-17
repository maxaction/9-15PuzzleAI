
// PuzzleGameAIDlg.h : header file
//
#pragma once
#include <vector>
#include <string>


// CPuzzleGameAIDlg dialog
class CPuzzleGameAIDlg : public CDialogEx
{
// Construction
public:
	CPuzzleGameAIDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_PUZZLEGAMEAI_DIALOG };

	bool CheckWin();


	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();

	afx_msg void OnSizeChange(UINT id);
	
	void SetSize();

	afx_msg void OnPlayerSelectChange(UINT id);

	afx_msg void OnPuzzleClick(UINT id);
	void SetPlayerSelectRadio(UINT CtrlID);

	void resetBoard();
	void RedrawPuzzle();

	void SetButtons();
	

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);



private:
	int m_AiSpeed = 0;
	
	std::vector<std::vector<int>> m_boardValues;

	bool m_bGameRunning = false;
	
};
