#include "pch.h"
#include "CTetrisCell.h"

CTetrisCell::CTetrisCell()
{
	m_Dol = 0;
	m_PatNo = -1;

	m_DolPlayer2 = 0;
	m_PatNoPlayer2 = -2;
}

CTetrisCell::~CTetrisCell()
{
	
}

void CTetrisCell::Initialize(int no, int wcnt, CRect rt, CDC* pDC, CDC* bDC, CDC* iDC)
{
	m_NO = no;
	m_XY.x = no % wcnt;
	m_XY.y = no / wcnt;

	rt.OffsetRect(TCW * m_XY.x, TCW * m_XY.y);
	m_Rt = rt;

	m_pDrawDC = pDC;
	m_pBackDC = bDC;
	m_pImgDC = iDC;

	rt.MoveToXY(0, 0);
	for (int x = 0; x < 7; x++) {
		m_ImgRt[x] = rt;
		rt.OffsetRect(rt.Width(), 0);
	}
}

void CTetrisCell::InitializePlayer2(int no, int wcnt, CRect rt, CDC* pDC, CDC* bDC, CDC* iDC)
{
	m_NOPlayer2 = no;
	m_XYPlayer2.x = no % wcnt;
	m_XYPlayer2.y = no / wcnt;
	
	rt.OffsetRect(TCW * m_XYPlayer2.x, TCW * m_XYPlayer2.y);
	m_RtPlayer2 = rt;
	
	m_pDrawDCPlayer2 = pDC;
	m_pBackDCPlayer2 = bDC;
	m_pImgDCPlayer2 = iDC;
	
	rt.MoveToXY(0, 0);
	for (int x = 0; x < 7; x++) {
		m_ImgRtPlayer2[x] = rt;
		rt.OffsetRect(rt.Width(), 0);
		}

}

void CTetrisCell::DrawBrick(int no)
{
	m_pDrawDC->BitBlt(m_Rt.left, m_Rt.top, m_Rt.Width(), m_Rt.Height(),
		m_pImgDC, m_ImgRt[no].left, m_ImgRt[no].top, SRCCOPY);
	m_PatNo = no;
}

void CTetrisCell::DrawBrickPlayer2(int not)
{
	m_pDrawDCPlayer2->BitBlt(m_RtPlayer2.left, m_RtPlayer2.top, m_RtPlayer2.Width(), m_RtPlayer2.Height(),
		m_pImgDCPlayer2, m_ImgRtPlayer2[not].left, m_ImgRtPlayer2[not].top, SRCCOPY);
	m_PatNoPlayer2 = not;
}
//0x00007FF9B431129D(mfc140d.dll)에(CGameTetrisDlg.exe의) 처리되지 않은 예외가 있습니다. 0xC0000005: 0xFFFFFFFFFFFFFFFF 위치를 읽는 동안 액세스 위반이 발생했습니다..
//0x00007FF9C6880E4D(mfc140ud.dll)에(CGameTetrisDlg.exe의) 처리되지 않은 예외가 있습니다. 0xC0000005: 0x0000000000000008 위치를 읽는 동안 액세스 위반이 발생했습니다..
void CTetrisCell::EraseBrick()
{
	m_pDrawDC->BitBlt(m_Rt.left, m_Rt.top, m_Rt.Width(), m_Rt.Height(),
		m_pBackDC, m_Rt.left, m_Rt.top, SRCCOPY);
	m_Dol = false;
}

void CTetrisCell::EraseBrickPlayer2()
{
	m_pDrawDCPlayer2->BitBlt(m_RtPlayer2.left, m_RtPlayer2.top, m_RtPlayer2.Width(), m_RtPlayer2.Height(),
		m_pBackDCPlayer2, m_RtPlayer2.left, m_RtPlayer2.top, SRCCOPY);
	m_DolPlayer2 = false;
}

int CTetrisCell::IsSet()
{
	return m_Dol;
}

int CTetrisCell::IsSetPlayer2()
{
	return m_DolPlayer2;
}

void CTetrisCell::SetBrick()
{
	m_Dol = 1;
}

void CTetrisCell::SetBrickPlayer2()
{
	m_DolPlayer2 = 1;
}

void CTetrisCell::MoveFrom(CTetrisCell* pUpCell)
{
	m_Dol = pUpCell->m_Dol;
	m_PatNo = pUpCell->m_PatNo;
	pUpCell->m_Dol = 0;
}

void CTetrisCell::MoveFromPlayer2(CTetrisCell* pUpCell)
{
	m_DolPlayer2 = pUpCell->m_DolPlayer2;
	m_PatNoPlayer2 = pUpCell->m_PatNoPlayer2;
	pUpCell->m_DolPlayer2 = 0;
}



void CTetrisCell::Refresh()
{
	if (this->IsSet()) DrawBrick(m_PatNo);
	else EraseBrick();
}


void CTetrisCell::RefreshPlayer2()
{
	if (this->IsSetPlayer2())DrawBrick(m_PatNoPlayer2);
	else EraseBrickPlayer2();
}


