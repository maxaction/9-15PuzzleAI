
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


	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();

	afx_msg void OnSize8Select();
	afx_msg void OnSize15Select();

	void OnSizeChange(UINT CtrlID);
	
	void SetSize();

	afx_msg void OnHumanSelect();
	afx_msg void OnAIBreadth();
	afx_msg void OnAIAStarManhattan();
	afx_msg void OnAIAStarPattern();

	void OnPlayerSelectChange(UINT CtrlID);
	void SetPlayerSelectRadio(UINT CtrlID);

	void resetBoard();
	void RedrawPuzzle();

	

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);



private:
	int m_AiSpeed = 0;
	
	std::vector<std::vector<int>> m_boardValues;
	
};
