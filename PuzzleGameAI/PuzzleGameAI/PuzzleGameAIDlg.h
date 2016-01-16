
// PuzzleGameAIDlg.h : header file
//

#pragma once


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
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
};
