// CTetrisDlg.cpp: 구현 파일
//

#include "pch.h"
#include "CGameTetrisDlg.h"
#include "afxdialogex.h"
#include "CTetrisDlg.h"


// CTetrisDlg 대화 상자

IMPLEMENT_DYNAMIC(CTetrisDlg, CDialogEx)


struct PointXY { int x, y; };

PointXY BrickPatOrg[7][4][4] = {

	{{0,0,1,0,2,0,-1,0},         {0,0,0,1,0,-1,0,-2},       {0,0,1,0,2,0,-1,0},          {0,0,0,1,0,-1,0,-2}}, //일자블럭

	{{0,0,1,0,0,1,1,1},          {0,0,1,0,0,1,1,1},         {0,0,1,0,0,1,1,1},            {0,0,1,0,0,1,1,1}},//ㄱㄴ블럭

	{{0,0,-1,0,0,-1,1,-1},       {0,0,0,1,-1,0,-1,-1},      {0,0,-1,0,0,-1,1,-1},         {0,0,0,1,-1,0,-1,-1}},//ㄴㄱ블럭

	{{0,0,-1,-1,0,-1,1,0},       {0,0,-1,0,-1,1,0,-1},      {0,0,-1,-1,0,-1,1,0},         {0,0,-1,0,-1,1,0,-1}},

	{{0,0,-1,0,1,0,-1,-1},       {0,0,0,-1,0,1,-1,1},       {0,0,-1,0,1,0,1,1},            {0,0,0,-1,0,1,1,-1}},

	{{0,0,1,0,-1,0,1,-1},        {0,0,0,1,0,-1,-1,-1},       {0,0,1,0,-1,0,-1,1},         {0,0,0,-1,0,1,1,1}},

	{{0,0,-1,0,1,0,0,1},         {0,0,0,-1,0,1,1,0 },        {0,0,-1,0,1,0,0,-1},         {0,0,-1,0,0,-1,0,1} }
};

CPoint BrickPat[7][4][4];
CPoint BrickPatplayer2[7][4][4];

int CTetrisDlg::GetNo(CPoint xy)
{
	return xy.y * TW_CNT + xy.x;
}

int CTetrisDlg::GetPreNo(CPoint xy)
{
	return xy.y * PREW_CNT + xy.x;
}

void CTetrisDlg::Img_Load(CDC* pDC, CRect rt, CString fpath)
{
	CImage img;
	img.Load(fpath);
	if (img.IsNull()) return;

	pDC->SetStretchBltMode(HALFTONE);
	img.StretchBlt(pDC->GetSafeHdc(), rt.left, rt.top, rt.Width(), rt.Height(), SRCCOPY);
	img.Destroy();
}

CDC* CTetrisDlg::MakeMemDC(CRect rt)
{
	CDC* pDC = new CDC;
	CBitmap* pBit = new CBitmap;
	CWindowDC sdc(NULL);

	pDC->CreateCompatibleDC(&sdc);
	pBit->CreateCompatibleBitmap(&sdc, rt.Width(), rt.Height());
	pDC->SelectObject(pBit);

	return pDC;
}


CDC* CTetrisDlg::DeleteMemDC(CDC* pDC)
{
	CBitmap* pBit = pDC->GetCurrentBitmap();
	pBit->DeleteObject();
	pDC->DeleteDC();

	delete(pBit);
	delete(pDC);
	return pDC;
}



CTetrisDlg::CTetrisDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CTetrisDlg, pParent)
	, m_ScoreEdit(0)
{
	m_CellRt = CRect(0, 0, TCW, TCW);
	m_SecondCellRt = CRect(0, 0, TCW, TCW);

	m_BoardRt = m_BackRt = m_PanRt = CRect(0, 0, m_CellRt.Width() * TW_CNT,
		m_CellRt.Height() * TH_CNT);
	m_SecondBoardRt = m_SecondBackRt = m_SecondPanRt = CRect(0, 0, m_SecondCellRt.Width() * TW_CNT,
		m_SecondCellRt.Height() * TH_CNT);

	m_BoardRt.OffsetRect(BoardOrg);
	m_SecondBoardRt.OffsetRect(BoardOrg);

	m_ImgRt = CRect(0, 0, m_CellRt.Width() * 7, m_CellRt.Width());
	m_SecondImgRt = CRect(0, 0, m_CellRt.Width() * 7, m_CellRt.Width());

	m_PreviewRt = m_PreMemRt = CRect(0, 0, m_CellRt.Width() * PREW_CNT,
		m_CellRt.Height() * PREH_CNT);

	m_SecondPreviewRt = m_SecondPreMemRt = CRect(0, 0, m_SecondCellRt.Width() * PREW_CNT,
		m_SecondCellRt.Height() * PREH_CNT);

	m_PreviewRt.MoveToXY(PreviewOrg);
	m_SecondPreviewRt.MoveToXY(PreviewOrgPlayer2);

	for (int y = 0; y < 7; y++) {
		for (int x = 0; x < 4; x++) {
			for (int k = 0; k < 4; k++) {
				BrickPat[y][x][k].x = BrickPatOrg[y][x][k].x;
				BrickPat[y][x][k].y = BrickPatOrg[y][x][k].y;

				BrickPatplayer2[y][x][k].x = BrickPatOrg[y][x][k].x;
				BrickPatplayer2[y][x][k].y = BrickPatOrg[y][x][k].y;
			}
		}
	}

}
CTetrisDlg::~CTetrisDlg()
{
	DeleteMemDC(m_PanDC);
	DeleteMemDC(m_BackDC);
	DeleteMemDC(m_ImgDC);
	DeleteMemDC(m_PreMemDC);
	
	
	DeleteMemDC(m_ImgDCplayer2);
	DeleteMemDC(m_PreMemDCplayer2);
}



void CTetrisDlg::Proglnit()
{
	CWindowDC sdc(NULL);

	m_PanDC = MakeMemDC(m_PanRt);
	m_BackDC = MakeMemDC(m_BackRt);
	m_ImgDC = MakeMemDC(m_ImgRt);
	m_PreMemDC = MakeMemDC(m_PreMemRt);

	m_PanDCplayer2 = MakeMemDC(m_SecondPanRt);
	m_BackDCplayer2 = MakeMemDC(m_SecondBackRt);
	m_ImgDCplayer2 = MakeMemDC(m_SecondImgRt);
	m_PreMemDCplayer2 = MakeMemDC(m_SecondPreviewRt);

	CRect rt1 = CRect(0, 0, m_PanRt.Width() / 2, m_PanRt.Height() / 2);
	CRect rt2(rt1), rt3(rt1), rt4(rt1);

	CRect rt5 = CRect(0, 0, m_SecondPanRt.Width() / 2, m_SecondPanRt.Height() / 2);
	CRect rt6(rt5), rt7(rt5), rt8(rt5);

	rt2.OffsetRect(rt1.Width(), 0);
	rt3.OffsetRect(0, rt1.Height());
	rt4.OffsetRect(rt1.Width(), rt1.Height());

	rt6.OffsetRect(rt5.Width(), 0);
	rt7.OffsetRect(0, rt5.Height());
	rt8.OffsetRect(rt5.Width(), rt5.Height());

	Img_Load(m_BackDC, rt1, _T("res\\pan.png"));
	Img_Load(m_BackDC, rt2, _T("res\\pan.png"));
	Img_Load(m_BackDC, rt3, _T("res\\pan.png"));
	Img_Load(m_BackDC, rt4, _T("res\\pan.png"));

	Img_Load(m_BackDCplayer2, rt5, _T("res\\pan.png"));
	Img_Load(m_BackDCplayer2, rt6, _T("res\\pan.png"));
	Img_Load(m_BackDCplayer2, rt7, _T("res\\pan.png"));
	Img_Load(m_BackDCplayer2, rt8, _T("res\\pan.png"));

	m_PanDC->BitBlt(m_PanRt.left, m_PanRt.top, m_PanRt.Width(), m_PanRt.Height(),
		m_BackDC, m_PanRt.left, m_PanRt.top, SRCCOPY);

	m_PanDCplayer2->BitBlt(m_SecondPanRt.left, m_SecondPanRt.top, m_SecondPanRt.Width(), m_SecondPanRt.Height(),
		m_BackDCplayer2, m_SecondPanRt.left, m_SecondPanRt.top, SRCCOPY);

	Img_Load(m_ImgDC, m_ImgRt, _T("res\\block.png"));
	Img_Load(m_ImgDCplayer2, m_SecondImgRt, _T("res\\block.png"));

	for (int no = 0; no < TBOARD_BDCNT; no++) {
		m_tCell[no].Initialize(no, TW_CNT, m_CellRt, m_PanDC, m_BackDC, m_ImgDC);
	}
	for (int no = 0; no < PRE_BDCNT; no++) {
		m_PreCell[no].Initialize(no, PREW_CNT, m_CellRt, m_PreMemDC, m_BackDC, m_ImgDC);
	}
	for (int no = 0; no < PRE_BDCNT; no++) {
		m_SecondPreCell[no].InitializePlayer2(no, PREW_CNT, m_SecondCellRt, m_PreMemDCplayer2, m_BackDCplayer2, m_ImgDCplayer2);
	}
}

BOOL CTetrisDlg::OnInitDialog()
{
	// TODO: 여기에 구현 코드 추가.
	CDialogEx::OnInitDialog();
	Proglnit();
	
	/*SetTimer(1, 1000, NULL);
	SetTimer(2, 800, NULL);*/

	m_SecondPreviewRt = CRect(400, 250, 550, 395);

	return TRUE;
}

void CTetrisDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	// 그리기 메시지에 대해서는 CDialogEx::OnPaint()을(를) 호출하지 마십시오.
	dc.BitBlt(m_BoardRt.left, m_BoardRt.top, m_BoardRt.Width(), m_BoardRt.Height(),
		m_PanDC, 0, 0, SRCCOPY);

	dc.BitBlt(m_PreviewRt.left, m_PreviewRt.top, m_PreviewRt.Width(), m_PreviewRt.Height(),
		m_PreMemDC, 0, 0, SRCCOPY);

	dc.BitBlt(m_SecondPreviewRt.left, m_SecondPreviewRt.top, m_SecondPreviewRt.Width(), m_SecondPreviewRt.Height(),
		m_PreMemDCplayer2, 0, 0, SRCCOPY);
}

void CTetrisDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	bool player1 = MoveCheck(DOWN);
	bool player2 = MoveCheckPlayer2(DOWN);
		if (player1) ViewPattern();
		else  ArriveBottom();
		if (player2) ViewPatternPlayer2();
		else ArriveBottomPlayer2();
	
	//블럭1만 하강하고 블럭2는 하강x
	//if (nIDEvent == 1) {
	//	if (MoveCheck(DOWN))ViewPattern();
	//	else ArriveBottom();
	//}
	//else if (nIDEvent == 2) {
	//	if (MoveCheck(DOWN))ViewPatternPlayer2();
	//	else ArriveBottomPlayer2();
	//}

	CEdit* scoreEdit = (CEdit*)GetDlgItem(IDC_EDIT_SCORE);
	CString strscore;
	strscore.Format(_T("%d"), m_nScore);
	scoreEdit->SetWindowText(strscore);

	m_nSpeed += 0.005f;//속도 조절

	SetSpeed();

	if (m_nScore >= 1000000) {
		Stop();
		CString dm;
		dm.Format(_T("승리"));
		if (MessageBox(dm, _T("메시지"), MB_YESNO) == IDYES) {
			Reset(); Start();
		}
		else {
			return CDialogEx::OnOK();
		}
	}
	CDialogEx::OnTimer(nIDEvent);
}


CTetrisCell* CTetrisDlg::GetCell(CPoint xy)
{
	if (xy.x < 0 || xy.x >= TW_CNT || xy.y < 0 || xy.y >= TH_CNT) return NULL;
	return &m_tCell[GetNo(xy)];
}

CTetrisCell* CTetrisDlg::GetPreCell(CPoint xy)
{
	if (xy.x < 0 || xy.x >= PREW_CNT || xy.y < 0 || xy.y >= PREH_CNT) return NULL;
	return &m_PreCell[GetPreNo(xy)];
}

CTetrisCell* CTetrisDlg::GetPreCellPlayer2(CPoint xy)
{
	if (xy.x < 0 || xy.x >= PREW_CNT || xy.y < 0 || xy.y >= PREH_CNT) return NULL;
	return &m_SecondPreCell[GetPreNo(xy)];
}

void CTetrisDlg::OnClickedButtonStart()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	Start();
}

void CTetrisDlg::Start()
{
	m_nScore = 0;
	m_nSpeed = 5;

	Refresh();
	srand((UINT)time(0));

	m_PrePatNo = Random(7);
	m_PreRotNo = Random(4);

	m_SecondPrePatNo = Random(7);
	m_SecondPreRotNo = Random(4);

	MakePattern();
	MakePatternPlayer2();
	SetSpeed();
}

void CTetrisDlg::MakePattern()
{
	m_PatNo = m_PrePatNo;
	m_RotNo = m_PreRotNo;


	m_PrePatNo = Random(7);
	m_PreRotNo = Random(4);

	m_CurXY = CPoint(3, 2);

	if (ViewCheck(m_RotNo, m_CurXY)) {
		DrawPreview();
		ViewPattern();
	}
	else {
		 Stop();
		CString str;
		str.Format(_T("게임종료 : 점수(%d)\n 게임을 다시 시작하시겠습니까"), m_nScore);
		if (MessageBox(str, _T("알림"), MB_YESNO) == IDYES) {
			Reset(); Start();
		}
		else {
			return CDialogEx::OnOK();
		}	
	}
}

void CTetrisDlg::MakePatternPlayer2()
{
	m_SecondPatNo = m_SecondPrePatNo;
	m_SecondRotNo = m_SecondPreRotNo;

	m_SecondPrePatNo = Random(7);
	m_SecondPreRotNo = Random(4);

	m_CurXY2 = CPoint(9, 2);

	if (ViewCheckPlayer2(m_SecondPreRotNo, m_CurXY2)) {
		DrawPreviewPlayer2();
		ViewPatternPlayer2();
	}
	else {
		Stop();
		CString str;
		str.Format(_T("2게임종료 : 점수(%d)\n 게임을 다시 시작하시겠습니까"), m_nScore);
		if (MessageBox(str, _T("알림"), MB_YESNO) == IDYES) {
			Reset(); Start();
		}
		else {
			return CDialogEx::OnOK();
		}
	}
}


void CTetrisDlg::ViewPattern()
{
	ErasePattern(m_BeforePatCell4Vec);
	DrawPattern(m_PatCell4Vec);
}

void CTetrisDlg::ViewPatternPlayer2()
{
	ErasePattern(m_SecondBeforePatCell4Vec);
	DrawPatternPlayer2(m_SecondPatCell4Vec);
}

void CTetrisDlg::DrawPattern(vector<CTetrisCell*> v)
{
	for each (CTetrisCell * pCell in v) {
		pCell->DrawBrick(m_PatNo);
	}
	Invalidate(FALSE);
}

void CTetrisDlg::DrawPatternPlayer2(vector<CTetrisCell*> v)//DrawBrickPlayer2로 수정하기(액세스 위반 해결해야 함)
{
	for each (CTetrisCell * pell in v) {
		pell->DrawBrickPlayer2(m_SecondPatNo);
	}
	Invalidate(FALSE);
}

void CTetrisDlg::ErasePattern(vector<CTetrisCell*> v)
{
	for each (CTetrisCell * pCell in v) {
		pCell->EraseBrick();
	}
	Invalidate(FALSE);
}

void CTetrisDlg::ErasePatternPlayer2(vector<CTetrisCell*> v)
{
	for each (CTetrisCell * pCell in v) {
		pCell->EraseBrickPlayer2();
	}
	Invalidate(FALSE);
}


void CTetrisDlg::SetPattern(vector<CTetrisCell*> v)
{
	for each (CTetrisCell * pCell in v) {
		pCell->SetBrick();
	}
	Invalidate(FALSE);
}

void CTetrisDlg::SetPatternPlayer2(vector<CTetrisCell*> v)
{
	for each (CTetrisCell * pCell in v) {
		pCell->SetBrickPlayer2();
	}
	Invalidate(FALSE);
}



bool CTetrisDlg::ViewCheck(int rot, CPoint xy)
{
	vector<CTetrisCell*> v;
	for (int k = 0; k < 4; k++) {
		CPoint pt = xy + BrickPat[m_PatNo][rot][k];
		CTetrisCell* pCell = GetCell(pt);
		if (pCell != NULL) {
			if (pCell->IsSet()) return false;
			v.push_back(pCell);
		}
	}
	if (v.size() != 4) return false;

	for (CTetrisCell* cell : m_SecondPatCell4Vec) {
		for (CTetrisCell* newCell : v) {
			if (cell->m_NO == newCell->m_NOPlayer2) return false;
			}
	}

	bool Bottom = false; 

	for (CTetrisCell* cell : m_PatCell4Vec) {
		for (CTetrisCell* newCell : v) {
			if (newCell->m_NOPlayer2 >= TH_CNT)
				Bottom = true;
		}
	}

	/*for (CTetrisCell* newCell : v) {
		if (newCell->m_NOPlayer2 >= TH_CNT) {
			for (CTetrisCell* cell : m_SecondPatCell4Vec) {
				if (cell->m_NO == newCell->m_NOPlayer2) return false;
			}
		}
	}*/
	
	
	m_BeforePatCell4Vec = m_PatCell4Vec;
	m_PatCell4Vec = v;
	return true;
}

bool CTetrisDlg::ViewCheckPlayer2(int rot, CPoint xy)
{
	vector<CTetrisCell*> v;
	for (int k = 0; k < 4; k++) {
		CPoint pt = xy + BrickPatplayer2[m_SecondPatNo][rot][k];
		CTetrisCell* pCell = GetCell(pt);
		if (pCell != NULL) {
			if (pCell->IsSet()) return false;
			v.push_back(pCell);
		}
	}
	if (v.size() != 4) return false;

	for (CTetrisCell* cell : m_PatCell4Vec) {
		for (CTetrisCell* newCell : v) {
			if (cell->m_NO == newCell->m_NO) return false;
		}
	}

	
	m_SecondBeforePatCell4Vec = m_SecondPatCell4Vec;
	m_SecondPatCell4Vec = v;
	return true;
}

bool CTetrisDlg::MoveCheck(int dir)
{
	vector<CTetrisCell*> v;

	int rot = m_RotNo;
	CPoint xy(m_CurXY);
	switch (dir) {
	case Left: xy.Offset(-1, 0); break;
	case RIGHT: xy.Offset(1, 0); break;
	case DOWN: xy.Offset(0, 1); break;
	case ROT:rot = (rot == 3) ? 0 : rot + 1; break;
	}


	if (ViewCheck(rot, xy)) {
		if (ViewCheckPlayer2(m_SecondRotNo, m_CurXY2))
			m_CurXY = xy;
			m_RotNo = rot;
			return true;
	}

	
	return false;
}

bool CTetrisDlg::MoveCheckPlayer2(int dir)
{
	int mov = m_SecondRotNo;
	CPoint po(m_CurXY2);
	switch (dir) {
	case Left: po.Offset(-1, 0); break;
	case RIGHT: po.Offset(1, 0); break;
	case DOWN: po.Offset(0, 1); break;
	case ROT: mov = (mov == 3) ? 0 : mov + 1; break;
	}

	if (ViewCheckPlayer2(mov, po)) {
		m_CurXY2 = po;
		m_SecondRotNo = mov;
		return true;
	}

	return false;
}

void CTetrisDlg::ArriveBottom()
{
	SetPattern(m_PatCell4Vec);
	m_BeforePatCell4Vec.clear();
	m_PatCell4Vec.clear();

	for (int y, cnt = 1; y = BlockCheck(); cnt++) {
		m_nScore += cnt * cnt * 1000;
		UpdateData(FALSE);
		MessageBeep(0xFFFFFFFF);
		BlockDelete(y);
	}
	Refresh();
	MakePattern();
}

void CTetrisDlg::ArriveBottomPlayer2()
{
	SetPattern(m_SecondPatCell4Vec);
	m_SecondBeforePatCell4Vec.clear();
	m_SecondPatCell4Vec.clear();

	for (int y, cnt = 1; y = BlockCheck(); cnt++) {
		m_nScore += cnt * cnt * 1000;
		UpdateData(FALSE);
		MessageBeep(0xFFFFFFFF);
		BlockDelete(y);
	}
	Refresh();
	MakePatternPlayer2();
}


void CTetrisDlg::SetSpeed()
{
	Stop();
	UpdateData(TRUE);
	int timer = static_cast<int>(3000 / m_nSpeed);
	SetTimer(1, timer, NULL);
}

BOOL CTetrisDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN) {
		switch (pMsg->wParam) {
		case 'W':
			if (MoveCheck(ROT)) ViewPattern();
			return TRUE;

		case 'A':
			if (MoveCheck(Left))
				if (!ViewCheckPlayer2(m_SecondRotNo, m_CurXY2)) break;
				ViewPattern();
			return TRUE;

		case 'D':
			if (MoveCheck(RIGHT))	
				if (!ViewCheckPlayer2(m_SecondRotNo, m_CurXY2)) break;
			ViewPattern();
			return TRUE;

		case 'S':
			if (MoveCheck(DOWN)) ViewPattern();
			return TRUE;

		case VK_SPACE:
			while (MoveCheck(DOWN)) 
				if (!ViewCheckPlayer2(m_SecondRotNo, m_CurXY2)) break;
					ViewPattern(); 
			
			return TRUE;

		case VK_UP:
			if (MoveCheckPlayer2(ROT))ViewPatternPlayer2();
			return TRUE;
		case VK_LEFT:
			if (MoveCheckPlayer2(Left))ViewPatternPlayer2();
			return TRUE;
		case VK_RIGHT:
			if (MoveCheckPlayer2(RIGHT))ViewPatternPlayer2();
			return TRUE;
		case VK_DOWN:
			if (MoveCheckPlayer2(DOWN))ViewPatternPlayer2();
			return TRUE;
		case VK_RETURN:  //Enter
			while (MoveCheckPlayer2(DOWN))ViewPatternPlayer2();
			return TRUE;
			
		case 'T':
			KillTimer(1);
			return true;
		case 'G':
			SetSpeed();
			return true;
		case 'P' :
			m_nScore += 1000;
			return true;
		}
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}
void CTetrisDlg::Reset()
{
	for (int no = 0; no < TBOARD_BDCNT; no++) {
		CTetrisCell* pCell = &m_tCell[no];
		pCell->EraseBrick();
	}
}

void CTetrisDlg::Stop()
{
	KillTimer(1);
	//KillTimer(2);
}


int CTetrisDlg::BlockCheck()
{
	for (int y = TH_CNT - 1; y >= 0; y--) {
		int sf = 1;
		for (int x = 0; x < TW_CNT; x++) {
			CTetrisCell* pCell = GetCell(CPoint(x, y));
			if (!pCell->IsSet())sf = 0;
		}
		if (sf == 1) return y;
	}
	return 0;
}

void CTetrisDlg::BlockDelete(int delY)
{
	for (int y = delY; y > 1; y--) {
		for (int x = 0; x < TW_CNT; x++) {
			CTetrisCell* pUpCell = GetCell(CPoint(x, y - 1));
			CTetrisCell* pMe = GetCell(CPoint(x, y));
			pMe->MoveFrom(pUpCell);
		}
	}
}

void CTetrisDlg::Refresh()
{
	for (int no = 0; no <= TBOARD_BDCNT; no++) {
		m_tCell[no].Refresh();
	}
}

void CTetrisDlg::DrawPreview()
{
	for (int no = 0; no < PRE_BDCNT; no++) {
		m_PreCell[no].EraseBrick();
	}

	for (int k = 0; k < 4; k++) {
		CPoint pt = CPoint(2, 3) + BrickPat[m_PrePatNo][m_PreRotNo][k];
		CTetrisCell* pCell = GetPreCell(pt);
		if (pCell)pCell->DrawBrick(m_PrePatNo);
	}
}

void CTetrisDlg::DrawPreviewPlayer2()
{
	for (int no = 0; no < PRE_BDCNT; no++) {
		m_SecondPreCell[no].EraseBrickPlayer2();
	}
	for (int k = 0; k < 4; k++) {
		CPoint pt = CPoint(2, 3) + BrickPatplayer2[m_SecondPrePatNo][m_SecondPreRotNo][k];
		CTetrisCell* pCell = GetPreCellPlayer2(pt);
		if (pCell)pCell->DrawBrickPlayer2(m_SecondPrePatNo);
	}
}


void CTetrisDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_SCORE, m_ScoreEdit);
}






BEGIN_MESSAGE_MAP(CTetrisDlg, CDialogEx)
	ON_WM_TIMER()
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BUTTON_START, &CTetrisDlg::OnClickedButtonStart)
	ON_BN_CLICKED(IDC_BUTTON_GUIDE, &CTetrisDlg::OnBnClickedButtonGuide)
END_MESSAGE_MAP()


// CTetrisDlg 메시지 처리기



void CTetrisDlg::OnBnClickedButtonGuide()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString gui;

	gui.Format(_T("a,s,d,→,←,↓ : 방향조작\nw,↑ : 블럭회전\nG : 시작 T : 멈춤"));
	MessageBox(gui, _T("조작키"), MB_OK | MB_ICONINFORMATION);
}




/*  
//http://www.soen.kr/xe/question/15115
//https://kldp.org/node/163111

수정된 부분
PreTranslateMessage(MSG* pMsg)
a,d if문 추가

*/