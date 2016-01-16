
// PuzzleGameAIDlg.cpp : implementation file
//

#include "stdafx.h"
#include "PuzzleGameAI.h"
#include "PuzzleGameAIDlg.h"
#include "afxdialogex.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CPuzzleGameAIDlg dialog

const UINT PuzzleIDs[4][4] = { { IDC_PUZZLE_0_0, IDC_PUZZLE_0_1, IDC_PUZZLE_0_2, IDC_PUZZLE_0_3 },
							   { IDC_PUZZLE_1_0, IDC_PUZZLE_1_1, IDC_PUZZLE_1_2, IDC_PUZZLE_1_3 },
							   { IDC_PUZZLE_2_0, IDC_PUZZLE_2_1, IDC_PUZZLE_2_2, IDC_PUZZLE_2_3 },
							   { IDC_PUZZLE_3_0, IDC_PUZZLE_3_1, IDC_PUZZLE_3_2, IDC_PUZZLE_3_3 }};


CPuzzleGameAIDlg::CPuzzleGameAIDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CPuzzleGameAIDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CPuzzleGameAIDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CPuzzleGameAIDlg, CDialogEx)

	ON_BN_CLICKED(IDC_PLAYER_HUMAN, OnHumanSelect)
	ON_BN_CLICKED(IDC_PLAYER_AI_DFS, OnAIBreadth)
	ON_BN_CLICKED(IDC_PLAYER_AI_ASM, OnAIAStarManhattan)
	ON_BN_CLICKED(IDC_PLAYER_AI_ASP, OnAIAStarPattern)

	ON_BN_CLICKED(IDC_SIZE_8, OnSize8Select)
	ON_BN_CLICKED(IDC_SIZE_15, OnSize15Select)

	ON_WM_HSCROLL()
END_MESSAGE_MAP()


// CPuzzleGameAIDlg message handlers

BOOL CPuzzleGameAIDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	CSliderCtrl* pSlider = (CSliderCtrl*)GetDlgItem(IDC_AI_SPEED);
	if(pSlider)
	{
		pSlider->SetRange(0,5000, TRUE);
	}


	CWnd* Lable = GetDlgItem(IDC_SECONDS);
	if (Lable)
	{
		std::wstring text = std::to_wstring(m_AiSpeed) + L" ms";
		Lable->SetWindowText(text.c_str());
	}

	resetBoard();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CPuzzleGameAIDlg::OnHumanSelect()
{
	OnPlayerSelectChange(IDC_PLAYER_HUMAN);
}
void CPuzzleGameAIDlg::OnAIBreadth()
{
	OnPlayerSelectChange(IDC_PLAYER_AI_DFS);
}
void CPuzzleGameAIDlg::OnAIAStarManhattan()
{
	OnPlayerSelectChange(IDC_PLAYER_AI_ASM);
}
void CPuzzleGameAIDlg::OnAIAStarPattern()
{
	OnPlayerSelectChange(IDC_PLAYER_AI_ASP);
}

void CPuzzleGameAIDlg::OnPlayerSelectChange(UINT CtrlID)
{
	SetPlayerSelectRadio(CtrlID);
	switch (CtrlID)
	{
	case IDC_PLAYER_HUMAN:
		
		break;


	default:
		break;
	}
}

void CPuzzleGameAIDlg::SetPlayerSelectRadio(UINT CtrlID)
{
	std::vector<UINT> IDs = {IDC_PLAYER_HUMAN,
							 IDC_PLAYER_AI_DFS,
							 IDC_PLAYER_AI_ASM,
							 IDC_PLAYER_AI_ASP};
	for(auto ID : IDs)
	{
		if(ID== CtrlID)
		{
			CButton* pBtn = (CButton*)GetDlgItem(ID);
			if(pBtn)
			{
				pBtn->SetCheck(TRUE);
			}
		}
		else
		{
			CButton* pBtn = (CButton*)GetDlgItem(ID);
			if (pBtn)
			{
				pBtn->SetCheck(FALSE);
			}
		}

	}
}



void CPuzzleGameAIDlg::OnSize8Select()
{
	OnSizeChange(IDC_SIZE_8);
}
void CPuzzleGameAIDlg::OnSize15Select()
{
	OnSizeChange(IDC_SIZE_15);
}

void CPuzzleGameAIDlg::OnSizeChange(UINT CtrlID)
{
	if (CtrlID == IDC_SIZE_8)
	{
		CButton* pBtn = (CButton*)GetDlgItem(IDC_SIZE_8);
		if (pBtn)
		{
			pBtn->SetCheck(TRUE);
		}

		pBtn = (CButton*)GetDlgItem(IDC_SIZE_15);
		if (pBtn)
		{
			pBtn->SetCheck(FALSE);
		}
	}
	else
	{
		CButton* pBtn = (CButton*)GetDlgItem(IDC_SIZE_8);
		if (pBtn)
		{
			pBtn->SetCheck(FALSE);
		}

		pBtn = (CButton*)GetDlgItem(IDC_SIZE_15);
		if (pBtn)
		{
			pBtn->SetCheck(TRUE);
		}
	}

	resetBoard();
}

void CPuzzleGameAIDlg::SetSize()
{
}




void CPuzzleGameAIDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: Add your message handler code here and/or call default

	CSliderCtrl* pSliderCtrl = (CSliderCtrl*)pScrollBar;

	if(pSliderCtrl)
	{
		m_AiSpeed = nPos;
		CWnd* Lable = GetDlgItem(IDC_SECONDS);
		if(Lable)
		{
			std::wstring text = std::to_wstring(m_AiSpeed) + L" ms";
			Lable->SetWindowText(text.c_str());
		}
	}

	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}


void CPuzzleGameAIDlg::resetBoard()
{
	for(auto &Row : m_boardValues)
	{
		Row.clear();
	}

	m_boardValues.clear();

	CButton* pBtn = (CButton*)GetDlgItem(IDC_SIZE_8);

	if (pBtn && pBtn->GetCheck())
	{
		int Number = 1;
		for (int col = 0; col < 3; ++col)
		{
			std::vector<int> row;
			for (int idx = 0; idx < 3; ++idx)
			{
				if (idx == 2 && col == 2)
				{
					row.push_back(0);
				}
				else
				{
					row.push_back(Number);
				}
				++Number;
				
			}
			m_boardValues.push_back(row);
		}
		
		CWnd* pWnd = GetDlgItem(IDC_PUZZLE_0_3);
		if (pWnd)
			pWnd->ShowWindow(SW_HIDE);

		pWnd = GetDlgItem(IDC_PUZZLE_1_3);
		if (pWnd)
			pWnd->ShowWindow(SW_HIDE);

		pWnd = GetDlgItem(IDC_PUZZLE_2_3);
		if (pWnd)
			pWnd->ShowWindow(SW_HIDE);

		pWnd = GetDlgItem(IDC_PUZZLE_3_0);
		if (pWnd)
			pWnd->ShowWindow(SW_HIDE);

		pWnd = GetDlgItem(IDC_PUZZLE_3_1);
		if (pWnd)
			pWnd->ShowWindow(SW_HIDE);

		pWnd = GetDlgItem(IDC_PUZZLE_3_2);
		if (pWnd)
			pWnd->ShowWindow(SW_HIDE);

		pWnd = GetDlgItem(IDC_PUZZLE_3_3);
		if (pWnd)
			pWnd->ShowWindow(SW_HIDE);



	}
	else
		
	{
		int Number = 1;
		for (int col = 0; col < 4; ++col)
		{
			std::vector<int> row;
			for (int idx = 0; idx < 4; ++idx)
			{
				if (idx == 3 && col == 3)
				{
					row.push_back(0);
				}
				else
				{
					row.push_back(Number);
				}
				++Number;
			}
			m_boardValues.push_back(row);
		}
		CWnd* pWnd = GetDlgItem(IDC_PUZZLE_0_3);
		if (pWnd)
			pWnd->ShowWindow(SW_SHOW);

		pWnd = GetDlgItem(IDC_PUZZLE_1_3);
		if (pWnd)
			pWnd->ShowWindow(SW_SHOW);

		pWnd = GetDlgItem(IDC_PUZZLE_2_3);
		if (pWnd)
			pWnd->ShowWindow(SW_SHOW);

		pWnd = GetDlgItem(IDC_PUZZLE_3_0);
		if (pWnd)
			pWnd->ShowWindow(SW_SHOW);

		pWnd = GetDlgItem(IDC_PUZZLE_3_1);
		if (pWnd)
			pWnd->ShowWindow(SW_SHOW);

		pWnd = GetDlgItem(IDC_PUZZLE_3_2);
		if (pWnd)
			pWnd->ShowWindow(SW_SHOW);

		pWnd = GetDlgItem(IDC_PUZZLE_3_3);
		if (pWnd)
			pWnd->ShowWindow(SW_SHOW);
	}
	RedrawPuzzle();
}

void CPuzzleGameAIDlg::RedrawPuzzle()
{
	if (m_boardValues.size() <2)
		return;

	CWnd* pWnd = GetDlgItem(IDC_PUZZLE_0_0);
	if(pWnd)
		pWnd->SetWindowText(std::to_wstring(m_boardValues[0][0]).c_str());

	 pWnd = GetDlgItem(IDC_PUZZLE_0_1);
		if (pWnd)
			pWnd->SetWindowText(std::to_wstring(m_boardValues[0][1]).c_str());

	 pWnd = GetDlgItem(IDC_PUZZLE_0_2);
		if (pWnd)
			pWnd->SetWindowText(std::to_wstring(m_boardValues[0][2]).c_str());

	 pWnd = GetDlgItem(IDC_PUZZLE_1_0);
		if (pWnd)
			pWnd->SetWindowText(std::to_wstring(m_boardValues[1][0]).c_str());

	 pWnd = GetDlgItem(IDC_PUZZLE_1_1);
		if (pWnd)
			pWnd->SetWindowText(std::to_wstring(m_boardValues[1][1]).c_str());

	 pWnd = GetDlgItem(IDC_PUZZLE_1_2);
		if (pWnd)
			pWnd->SetWindowText(std::to_wstring(m_boardValues[1][2]).c_str());

	 pWnd = GetDlgItem(IDC_PUZZLE_2_0);
		if (pWnd)
			pWnd->SetWindowText(std::to_wstring(m_boardValues[2][0]).c_str());

	 pWnd = GetDlgItem(IDC_PUZZLE_2_1);
		if (pWnd)
			pWnd->SetWindowText(std::to_wstring(m_boardValues[2][1]).c_str());

	 pWnd = GetDlgItem(IDC_PUZZLE_2_2);
		if (pWnd)
			pWnd->SetWindowText(std::to_wstring(m_boardValues[2][2]).c_str());


	CButton* pBtn = (CButton*)GetDlgItem(IDC_SIZE_8);

	if (pBtn && !pBtn->GetCheck())
	{
		 pWnd = GetDlgItem(IDC_PUZZLE_0_3);
			if (pWnd)
				pWnd->SetWindowText(std::to_wstring(m_boardValues[0][3]).c_str());

		 pWnd = GetDlgItem(IDC_PUZZLE_1_3);
			if (pWnd)
				pWnd->SetWindowText(std::to_wstring(m_boardValues[1][3]).c_str());

		 pWnd = GetDlgItem(IDC_PUZZLE_2_3);
			if (pWnd)
				pWnd->SetWindowText(std::to_wstring(m_boardValues[2][3]).c_str());

		 pWnd = GetDlgItem(IDC_PUZZLE_3_0);
			if (pWnd)
				pWnd->SetWindowText(std::to_wstring(m_boardValues[3][0]).c_str());

		 pWnd = GetDlgItem(IDC_PUZZLE_3_1);
			if (pWnd)
				pWnd->SetWindowText(std::to_wstring(m_boardValues[3][1]).c_str());

		 pWnd = GetDlgItem(IDC_PUZZLE_3_2);
			if (pWnd)
				pWnd->SetWindowText(std::to_wstring(m_boardValues[3][2]).c_str());

		 pWnd = GetDlgItem(IDC_PUZZLE_3_3);
			if (pWnd)
				pWnd->SetWindowText(std::to_wstring(m_boardValues[3][3]).c_str());
	}

}

