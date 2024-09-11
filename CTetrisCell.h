#pragma once

#define TCW 25
#define TW_CNT 13
#define TH_CNT 24
#define TBOARD_BDCNT (TW_CNT*TH_CNT)

#define PREW_CNT 6
#define PREH_CNT 6
#define PRE_BDCNT (PREW_CNT * PREH_CNT)

#define PreviewOrg CPoint(400,70)
#define PreviewOrgPlayer2 CPoint(400, 150)

#define BoardOrg CPoint(50,50)

#define Random(x) (rand() % (x))
#define Left 0 
#define Up 1
#define RIGHT 2
#define DOWN 3
#define ROT 4
#define FALL 5



class CTetrisCell
{
public:
	CPoint m_XY;
	CRect m_Rt;
	CDC* m_pDrawDC, * m_pBackDC, * m_pImgDC;
	CRect m_ImgRt[7];

	CPoint m_XYPlayer2;
	CRect m_RtPlayer2;
	CDC* m_pDrawDCPlayer2, * m_pBackDCPlayer2, * m_pImgDCPlayer2;
	CRect m_ImgRtPlayer2[7];

	int m_NO;
	int m_Dol;
	int m_PatNo;

	int m_NOPlayer2;
	int m_DolPlayer2;
	int m_PatNoPlayer2;

	CTetrisCell();
	~CTetrisCell();

	void Initialize(int no, int wcnt, CRect rt, CDC* pDC, CDC* bDC, CDC* iDC);
	void InitializePlayer2(int no, int wcnt, CRect rt, CDC* pDC, CDC* bDC, CDC* iDC);

	void DrawBrick(int no);
	void DrawBrickPlayer2(int not);

	void EraseBrick();
	void EraseBrickPlayer2();

	void SetBrick();
	void SetBrickPlayer2();

	int IsSet();
	int IsSetPlayer2();
	

	void MoveFrom(CTetrisCell* pUpCell);
	void MoveFromPlayer2(CTetrisCell* pUpCell);

	void Refresh();
	void RefreshPlayer2();

	CPoint GetPosition() const {
		return m_XY;
	}

};

/*
CPoint GetPosition() const {
		return m_XY;
	}

	if (cell->GetPosition() == newCell->GetPosition()) return false;
	위에 코드처럼 x
	if (cell->m_NO == newCell->m_NO) return false;

CPoint m_XY;
int m_NO;
*/