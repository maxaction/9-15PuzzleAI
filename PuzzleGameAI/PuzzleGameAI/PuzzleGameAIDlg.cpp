
// PuzzleGameAIDlg.cpp : implementation file
//

#include "stdafx.h"
#include "PuzzleGameAI.h"
#include "PuzzleGameAIDlg.h"
#include "afxdialogex.h"
#include <random>
#include "AIBreadth.h"
#include "AIAStarMan.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

static const int MAX_SUFFLES = 0;
static const int MIN_SUFFLES = 20;


const int Worst9[3][3] = {{ 8, 6, 7 }, 
						  { 2, 5, 4 }, 
						  { 3, 0, 1 }};

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

	m_pAIBreadth = std::make_shared<CAIBreadth>(this);

	m_pAIStar = std::make_shared<CAIAStarMan>(this);

	OnPlayerSelectChange(IDC_PLAYER_HUMAN);

	resetBoard();

	srand((UINT)time(NULL));
	//srand(NULL);

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
							 IDC_PLAYER_AI_ASM};
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
				m_pAI = m_pAIBreadth;
			}
			if (ID == IDC_PLAYER_AI_ASM)
			{
				m_pAI = m_pAIStar;
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

		CWnd* pWnd = GetDlgItem(IDC_WORST);
		if (pWnd)
		{
			pWnd->EnableWindow(TRUE);
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

		CWnd* pWnd = GetDlgItem(IDC_WORST);
		if (pWnd)
		{
			pWnd->EnableWindow(FALSE);
		}
	}

	resetBoard();

}

void CPuzzleGameAIDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: Add your message handler code here and/or call default

	if(pScrollBar->m_hWnd == GetDlgItem(IDC_AI_SPEED)->m_hWnd)
	{
		CSliderCtrl* pSlider = (CSliderCtrl*)GetDlgItem(IDC_AI_SPEED);


		m_AiSpeed = pSlider->GetPos();
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

	m_CompleteHash = std::hash<BITMASK>()(GenHash(m_boardValues));

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
			{
				if (m_boardValues[x][y])
				{
					pWnd->SetWindowText(std::to_wstring(m_boardValues[x][y]).c_str());
				}
				else
					pWnd->SetWindowText(L" ");
			}
		}
	}

	SetButtons();
}


void CPuzzleGameAIDlg::OnPuzzleClick(UINT id)
{
	if(!m_bGameRunning)
		return;

	UpdateState(id,m_boardValues);

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
		if (pWnd && pWnd->IsWindowVisible() && !m_bAIPlaying)
		{
			pWnd->EnableWindow(TRUE);
		}
	}
	if (EmptyIndex_X + 1 < 4)
	{
		CWnd* pWnd = GetDlgItem(PuzzleIDs[EmptyIndex_X + 1][EmptyIndex_Y]);
		if (pWnd && pWnd->IsWindowVisible() && !m_bAIPlaying)
		{
			pWnd->EnableWindow(TRUE);
		}
	}
	if (EmptyIndex_Y - 1 >= 0)
	{
		CWnd* pWnd = GetDlgItem(PuzzleIDs[EmptyIndex_X][EmptyIndex_Y - 1]);
		if (pWnd && pWnd->IsWindowVisible() && !m_bAIPlaying)
		{
			pWnd->EnableWindow(TRUE);
		}
	}
	if (EmptyIndex_Y + 1 < 4)
	{
		CWnd* pWnd = GetDlgItem(PuzzleIDs[EmptyIndex_X][EmptyIndex_Y + 1]);
		if (pWnd && pWnd->IsWindowVisible() && !m_bAIPlaying)
		{
			pWnd->EnableWindow(TRUE);
		}
	}
}
bool CPuzzleGameAIDlg::CheckWinCondition(size_t Hash)
{
	return Hash == m_CompleteHash;

}


void CPuzzleGameAIDlg::CheckWin()
{
	bool bGameWon = false;
	if (m_bGameRunning)
	{
		bGameWon = CheckWinCondition(std::hash<BITMASK>()(GenHash(m_boardValues)));
	}

	if (bGameWon)
	{
		m_bGameRunning = false;

		//MessageBox(L"Complete");
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
	bool suffled = false; 
	CButton* pBtn = (CButton*)GetDlgItem(IDC_WORST);
	if (pBtn && pBtn->GetCheck())
	{
		CButton* pBtn = (CButton*)GetDlgItem(IDC_SIZE_8);

		if (pBtn && pBtn->GetCheck())
		{
			m_boardValues.clear();
		
			for (int col = 0; col < 3; ++col)
			{
				std::vector<int> row;
				for (int idx = 0; idx < 3; ++idx)
				{
					row.push_back(Worst9[col][idx]);
				}
				m_boardValues.push_back(row);
			}
			suffled = true;
		}
	}
	if (!suffled)
	{
		int numberOfMoves = (rand() % (MAX_SUFFLES + MIN_SUFFLES)) + MIN_SUFFLES;
		for (int idx = 0; idx < numberOfMoves; ++idx)
		{
			auto Moves = GetAvailableMoves(m_boardValues);
			UpdateState(Moves[rand() % Moves.size()], m_boardValues);
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

void CPuzzleGameAIDlg::UpdateState(UINT id, BoardInfo& CurrentState)
{

	int ClickIndex_X = 0, ClickIndex_Y = 0,
		EmptyIndex_X = 0, EmptyIndex_Y = 0;

	for (UINT x = 0; x < CurrentState.size(); ++x)
	{
		for (UINT y = 0; y <CurrentState[x].size(); ++y)
		{
			if (PuzzleIDs[x][y] == id)
			{
				ClickIndex_X = x;
				ClickIndex_Y = y;
			}

			if (!CurrentState[x][y])
			{
				EmptyIndex_X = x;
				EmptyIndex_Y = y;
			}
			
		}
	}

	CurrentState[EmptyIndex_X][EmptyIndex_Y] = CurrentState[ClickIndex_X][ClickIndex_Y];
	CurrentState[ClickIndex_X][ClickIndex_Y] = 0;

}


std::vector<UINT> CPuzzleGameAIDlg::GetAvailableMoves(BoardInfo& CurrentState)
{
	int EmptyIndex_X = 0, EmptyIndex_Y = 0;

	std::vector<UINT> AvailableMoves;
	bool found = false;
	for (UINT x = 0, lengthX = CurrentState.size(); x < lengthX; ++x)
	{
		for (UINT y = 0, length = CurrentState[x].size(); y < length; ++y)
		{
			if (!CurrentState[x][y])
			{
				EmptyIndex_X = x;
				EmptyIndex_Y = y;
				found = true;
				break;
			}

		}
		if (found)
			break;
	}

	if (EmptyIndex_X - 1 >= 0)
	{
		AvailableMoves.push_back(PuzzleIDs[EmptyIndex_X - 1][EmptyIndex_Y]);
	}
	if (EmptyIndex_X + 1 < CurrentState.size())
	{
		AvailableMoves.push_back(PuzzleIDs[EmptyIndex_X + 1][EmptyIndex_Y]);

	}
	if (EmptyIndex_Y - 1 >= 0)
	{
		AvailableMoves.push_back(PuzzleIDs[EmptyIndex_X][EmptyIndex_Y-1]);

	}
	if (EmptyIndex_Y + 1 < CurrentState[EmptyIndex_X].size())
	{
		AvailableMoves.push_back(PuzzleIDs[EmptyIndex_X][EmptyIndex_Y+1]);

	}

	return AvailableMoves;
}

BITMASK CPuzzleGameAIDlg::GenHash(BoardInfo& CurrentState)
{
	BITMASK hash = 0;

	for (int x = 0; x < CurrentState.size(); ++x)
	{
		for(int y = 0; y < CurrentState[x].size(); ++y)
		{
			hash = hash << 1+ (int)log2(CurrentState.size() * CurrentState.size());
			hash |= CurrentState[x][y] ;
		}
	}
	return hash;
}