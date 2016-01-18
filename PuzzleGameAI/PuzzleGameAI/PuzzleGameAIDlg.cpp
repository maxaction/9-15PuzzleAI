
// PuzzleGameAIDlg.cpp : implementation file
//

#include "stdafx.h"
#include "PuzzleGameAI.h"
#include "PuzzleGameAIDlg.h"
#include "afxdialogex.h"
#include <random>
#include "AIBreadth.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CPuzzleGameAIDlg dialog

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
	ON_MESSAGE(AI_BTN_CLICK, OnAIClick)
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_SUFFLE, &CPuzzleGameAIDlg::OnBnClickedSuffle)
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

	OnSizeChange(IDC_SIZE_15);

	OnPlayerSelectChange(IDC_PLAYER_HUMAN);

	resetBoard();

	srand((UINT)time(NULL));

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
			if(ID == IDC_PLAYER_HUMAN)
			{
				m_bAIPlaying = false;
			}
			else
			{
				m_bAIPlaying = true;
			}

			if(ID == IDC_PLAYER_AI_DFS)
			{
				m_pAI = std::make_shared<CAIBreadth>(this);
			}
			if (ID == IDC_PLAYER_AI_ASM)
			{
				//TODO: add AI for A* manhattan
			}
			if (ID == IDC_PLAYER_AI_ASP)
			{
				//TODO: add AI for A* pattarn
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

	if(pScrollBar->m_hWnd == GetDlgItem(IDC_AI_SPEED)->m_hWnd)
	{
		m_AiSpeed = nPos;
		CWnd* Lable = GetDlgItem(IDC_SECONDS);
		if(Lable)
		{
			std::wstring text = std::to_wstring(m_AiSpeed) + L" ms";
			Lable->SetWindowText(text.c_str());
		}
		if (m_bAIPlaying && m_pAI)
		{
			m_pAI->SetAISpeed(m_AiSpeed);
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

	for (UINT x = 0; x < m_boardValues.size(); ++x)
	{
		for (UINT y = 0; y < m_boardValues[x].size(); ++y)
		{
			CWnd* pWnd = GetDlgItem(PuzzleIDs[x][y]);
			if (pWnd)
				pWnd->SetWindowText(std::to_wstring(m_boardValues[x][y]).c_str());
		}
	}

	SetButtons();
}


void CPuzzleGameAIDlg::OnPuzzleClick(UINT id)
{
	if(!m_bGameRunning)
		return;

	int ClickIndex_X = 0, ClickIndex_Y = 0, 
		EmptyIndex_X = 0, EmptyIndex_Y = 0;

	for (UINT x = 0; x < m_boardValues.size(); ++x)
	{
		for (UINT y = 0; y <m_boardValues[x].size(); ++y)
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

	CheckWin();

	if (m_bAIPlaying && m_pAI)
	{
		m_pAI->UpdateBoardView(m_boardValues);
	}

}


void CPuzzleGameAIDlg::SetButtons()
{
	int EmptyIndex_X=0, EmptyIndex_Y=0;

	for (UINT x = 0; x < m_boardValues.size(); ++x)
	{
		for (UINT y = 0; y <m_boardValues[x].size(); ++y)
		{
			CWnd* pWnd = GetDlgItem(PuzzleIDs[x][y]);
			if (pWnd && pWnd->IsWindowVisible())
			{
				pWnd->EnableWindow(FALSE);

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
		CWnd* pWnd = GetDlgItem(PuzzleIDs[EmptyIndex_X - 1][EmptyIndex_Y]);
		if (pWnd && pWnd->IsWindowVisible())
		{
			pWnd->EnableWindow(TRUE);
		}
	}
	if (EmptyIndex_X + 1 < 4)
	{
		CWnd* pWnd = GetDlgItem(PuzzleIDs[EmptyIndex_X + 1][EmptyIndex_Y]);
		if (pWnd && pWnd->IsWindowVisible())
		{
			pWnd->EnableWindow(TRUE);
		}
	}
	if (EmptyIndex_Y - 1 >= 0)
	{
		CWnd* pWnd = GetDlgItem(PuzzleIDs[EmptyIndex_X][EmptyIndex_Y - 1]);
		if (pWnd && pWnd->IsWindowVisible())
		{
			pWnd->EnableWindow(TRUE);
		}
	}
	if (EmptyIndex_Y + 1 < 4)
	{
		CWnd* pWnd = GetDlgItem(PuzzleIDs[EmptyIndex_X][EmptyIndex_Y + 1]);
		if (pWnd && pWnd->IsWindowVisible())
		{
			pWnd->EnableWindow(TRUE);
		}
	}
}
bool CPuzzleGameAIDlg::CheckWinCondition(BoardInfo& board)
{

	bool bGameWon = true;

	int Number = 1;
	for (UINT col = 0; col < board.size(); ++col)
	{
		std::vector<int> row;
		for (UINT idx = 0; idx < board[col].size(); ++idx)
		{
			if (idx == 2 && col == 2)
			{
				if (board[col][idx] != 0)
					bGameWon = false;
			}
			else
			{
				if (board[col][idx] != Number)
					bGameWon = false;
			}
			++Number;

		}
		board.push_back(row);
	}

	return bGameWon;

}


void CPuzzleGameAIDlg::CheckWin()
{
	bool bGameWon = false;;
	if (m_bGameRunning)
	{
		bGameWon = CheckWinCondition(m_boardValues);
	}

	if (bGameWon)
	{
		m_bGameRunning = false;

		MessageBox(L"Complete");
		EndGame();
	}
}

void CPuzzleGameAIDlg::OnBnClickedSuffle()
{
	if (m_bGameRunning)
	{
		EndGame();
	}
	else
	{
		StartGame();
	}
}

void CPuzzleGameAIDlg::StartGame()
{
	for (UINT x = 0; x < m_boardValues.size(); ++x)
	{
		for (UINT y = 0; y <m_boardValues[x].size(); ++y)
		{
			
			CMFCButton* pWnd = (CMFCButton*)GetDlgItem(PuzzleIDs[x][y]);
			if (pWnd && pWnd->IsWindowVisible())
			{
				int CurrentValue = m_boardValues[x][y],
					randX = rand() % m_boardValues.size(),
					randY = rand() % m_boardValues[x].size();

				m_boardValues[x][y] = m_boardValues[randX][randY];
				m_boardValues[randX][randY] = CurrentValue;
			}
		}
	}

	CWnd* pWnd = GetDlgItem(IDC_SIZE_8);
	if (pWnd)
	{
		pWnd->EnableWindow(FALSE);
	}
	pWnd = GetDlgItem(IDC_SIZE_15);
	if (pWnd)
	{
		pWnd->EnableWindow(FALSE);
	}
	pWnd = GetDlgItem(IDC_PLAYER_AI_ASM);
	if (pWnd)
	{
		pWnd->EnableWindow(FALSE);
	}
	pWnd = GetDlgItem(IDC_PLAYER_AI_ASP);
	if (pWnd)
	{
		pWnd->EnableWindow(FALSE);
	}
	pWnd = GetDlgItem(IDC_PLAYER_AI_DFS);
	if (pWnd)
	{
		pWnd->EnableWindow(FALSE);
	}
	pWnd = GetDlgItem(IDC_PLAYER_HUMAN);
	if (pWnd)
	{
		pWnd->EnableWindow(FALSE);
	}
	pWnd = GetDlgItem(IDC_AI_SPEED);
	if (pWnd)
	{
		if (m_bAIPlaying)
		{
			pWnd->EnableWindow(TRUE);
		}
		else
		{
			pWnd->EnableWindow(FALSE);
		}
	}

	pWnd = GetDlgItem(IDC_SUFFLE);
	if (pWnd)
	{
		pWnd->SetWindowText(L"Stop/End");
	}
	RedrawPuzzle();

	m_bGameRunning = true;

	if (m_bAIPlaying && m_pAI)
	{
		m_pAI->Startgame(m_boardValues);
	}
}

void CPuzzleGameAIDlg::EndGame()
{
	CWnd* pWnd = GetDlgItem(IDC_SIZE_8);
	if (pWnd)
	{
		pWnd->EnableWindow(TRUE);
	}
	pWnd = GetDlgItem(IDC_SIZE_15);
	if (pWnd)
	{
		pWnd->EnableWindow(TRUE);
	}
	pWnd = GetDlgItem(IDC_PLAYER_AI_ASM);
	if (pWnd)
	{
		pWnd->EnableWindow(TRUE);
	}
	pWnd = GetDlgItem(IDC_PLAYER_AI_ASP);
	if (pWnd)
	{
		pWnd->EnableWindow(TRUE);
	}
	pWnd = GetDlgItem(IDC_PLAYER_AI_DFS);
	if (pWnd)
	{
		pWnd->EnableWindow(TRUE);
	}
	pWnd = GetDlgItem(IDC_PLAYER_HUMAN);
	if (pWnd)
	{
		pWnd->EnableWindow(TRUE);
	}
	pWnd = GetDlgItem(IDC_AI_SPEED);
	if (pWnd)
	{
		pWnd->EnableWindow(TRUE);
	}

	pWnd = GetDlgItem(IDC_SUFFLE);
	if (pWnd)
	{
		pWnd->SetWindowText(L"Suffle");
	}

	m_bGameRunning = false;

	resetBoard();
}

LRESULT CPuzzleGameAIDlg::OnAIClick(WPARAM wParam, LPARAM lParam)
{
	UINT ClickID = static_cast<UINT>(wParam);


	OnPuzzleClick(ClickID);

	return 0;
}