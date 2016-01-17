
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

	ON_CONTROL_RANGE(BN_CLICKED, IDC_PLAYER_AI_DFS, IDC_PLAYER_AI_ASP, OnPlayerSelectChange)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_SIZE_8, IDC_SIZE_15, OnSizeChange)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_PUZZLE_0_0, IDC_PUZZLE_3_3, OnPuzzleClick)
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

void CPuzzleGameAIDlg::OnPlayerSelectChange(UINT id)
{
	SetPlayerSelectRadio(id);
	switch (id)
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

void CPuzzleGameAIDlg::OnSizeChange(UINT id)
{
	if (id == IDC_SIZE_8)
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
		
		for (int x = 0; x < 4; ++x)
		{
			if(x == 3)
			{
				for (int y = 0; y < 4; ++y)
				{
					CWnd* pWnd = GetDlgItem(PuzzleIDs[x][y]);
					if (pWnd)
						pWnd->ShowWindow(SW_HIDE);
				}
			}
			else
			{

				CWnd* pWnd = GetDlgItem(PuzzleIDs[x][3]);
				if (pWnd)
					pWnd->ShowWindow(SW_HIDE);
			}
		}

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

		for (int x = 0; x < 4; ++x)
		{
			if (x == 3)
			{
				for (int y = 0; y < 4; ++y)
				{
					CWnd* pWnd = GetDlgItem(PuzzleIDs[x][y]);
					if (pWnd)
						pWnd->ShowWindow(SW_SHOW);
				}
			}
			else
			{

				CWnd* pWnd = GetDlgItem(PuzzleIDs[x][3]);
				if (pWnd)
					pWnd->ShowWindow(SW_SHOW);
			}
		}
	}
	RedrawPuzzle();
}

void CPuzzleGameAIDlg::RedrawPuzzle()
{
	if (m_boardValues.size() <2)
		return;

	for (int x = 0; x < 3; ++x)
	{
		for(int y = 0; y < 3; ++y)
		{
			CWnd* pWnd = GetDlgItem(PuzzleIDs[x][y]);
			if (pWnd)
				pWnd->SetWindowText(std::to_wstring(m_boardValues[x][y]).c_str());
		}
	}
	CButton* pBtn = (CButton*)GetDlgItem(IDC_SIZE_8);
	if (pBtn && !pBtn->GetCheck())
	{

		for (int x = 0; x < 4; ++x)
		{
			if (x == 3)
			{
				for (int y = 0; y < 4; ++y)
				{
					CWnd* pWnd = GetDlgItem(PuzzleIDs[x][y]);
					if (pWnd)
						pWnd->SetWindowText(std::to_wstring(m_boardValues[x][y]).c_str());
				}
			}
			else
			{

				CWnd* pWnd = GetDlgItem(PuzzleIDs[x][3]);
				if (pWnd)
					pWnd->SetWindowText(std::to_wstring(m_boardValues[x][3]).c_str());
			}
		}
	}
	SetButtons();
}


void CPuzzleGameAIDlg::OnPuzzleClick(UINT id)
{
	int ClickIndex_X = 0, ClickIndex_Y = 0, 
		EmptyIndex_X = 0, EmptyIndex_Y = 0;
	for (int x = 0; x < 4; ++x)
	{
		for (int y = 0; y <4; ++y)
		{
			if (PuzzleIDs[x][y] == id)
			{
				ClickIndex_X = x;
				ClickIndex_Y = y;
			}

			CMFCButton* pWnd = (CMFCButton*)GetDlgItem(PuzzleIDs[x][y]);
			if (pWnd && pWnd->IsWindowVisible())
			{
				pWnd->EnableWindow(FALSE);
				pWnd->SetFaceColor(GetSysColor(COLOR_BTNFACE));

				if (!m_boardValues[x][y])
				{
					EmptyIndex_X = x;
					EmptyIndex_Y = y;
				}
			}
		}
	}

	m_boardValues[EmptyIndex_X][EmptyIndex_Y] = m_boardValues[ClickIndex_X][ClickIndex_Y];
	m_boardValues[ClickIndex_X][ClickIndex_Y] = 0;

	RedrawPuzzle();

}


void CPuzzleGameAIDlg::SetButtons()
{
	int EmptyIndex_X=0, EmptyIndex_Y=0;

	for(int x = 0; x < 4; ++x)
	{
		for(int y=0; y <4; ++y)
		{
			CMFCButton* pWnd = (CMFCButton*)GetDlgItem(PuzzleIDs[x][y]);
			if (pWnd && pWnd->IsWindowVisible())
			{
				pWnd->EnableWindow(FALSE);
				pWnd->SetFaceColor(GetSysColor(COLOR_BTNFACE));

				if(!m_boardValues[x][y])
				{
					EmptyIndex_X = x;
					EmptyIndex_Y = y;
				}
			}
		}
	}
	if (EmptyIndex_X - 1 >= 0)
	{
		CMFCButton* pWnd = (CMFCButton*)GetDlgItem(PuzzleIDs[EmptyIndex_X - 1][EmptyIndex_Y]);
		if (pWnd && pWnd->IsWindowVisible())
		{
			pWnd->EnableWindow(TRUE);
		}
	}
	if (EmptyIndex_X + 1 < 4)
	{
		CMFCButton* pWnd = (CMFCButton*)GetDlgItem(PuzzleIDs[EmptyIndex_X + 1][EmptyIndex_Y]);
		if (pWnd && pWnd->IsWindowVisible())
		{
			pWnd->EnableWindow(TRUE);
		}
	}
	if (EmptyIndex_Y - 1 >= 0)
	{
		CMFCButton* pWnd = (CMFCButton*)GetDlgItem(PuzzleIDs[EmptyIndex_X][EmptyIndex_Y - 1]);
		if (pWnd && pWnd->IsWindowVisible())
		{
			pWnd->EnableWindow(TRUE);
		}
	}
	if (EmptyIndex_Y + 1 < 4)
	{
		CMFCButton* pWnd = (CMFCButton*)GetDlgItem(PuzzleIDs[EmptyIndex_X][EmptyIndex_Y + 1]);
		if (pWnd && pWnd->IsWindowVisible())
		{
			pWnd->EnableWindow(TRUE);
		}
	}
}
bool CPuzzleGameAIDlg::CheckWin()
{
	bool bGameWon = true;
	if (m_bGameRunning)
	{

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
						if(m_boardValues[col][idx] != 0)
							bGameWon = false;
					}
					else
					{
						if (m_boardValues[col][idx] != Number)
							bGameWon = false;
					}
					++Number;

				}
				m_boardValues.push_back(row);
			}

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
						if (m_boardValues[col][idx] != 0)
							bGameWon = false;
					}
					else
					{
						if (m_boardValues[col][idx] != Number)
							bGameWon = false;
					}
					++Number;
				}
				m_boardValues.push_back(row);
			}
		}
	}

	if(bGameWon)
		m_bGameRunning = false;

	return bGameWon;
}