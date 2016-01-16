
// PuzzleGameAIDlg.cpp : implementation file
//

#include "stdafx.h"
#include "PuzzleGameAI.h"
#include "PuzzleGameAIDlg.h"
#include "afxdialogex.h"
#include <vector>
#include <string>

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
