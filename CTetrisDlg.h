#pragma once
#include "afxdialogex.h"

#include "CTetrisCell.h"
#include <vector>
using namespace std;


// CTetrisDlg 대화 상자

class CTetrisDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CTetrisDlg)

private:
	CRect m_CellRt, m_BoardRt, m_PanRt, m_BackRt;
	CRect m_ImgRt, m_PreMemRt, m_PreviewRt;

	CRect m_SecondCellRt, m_SecondBoardRt, m_SecondPanRt, m_SecondBackRt;
	CRect m_SecondImgRt, m_SecondPreMemRt, m_SecondPreviewRt;

	vector<CTetrisCell*> m_BeforePatCell4Vec;
	vector<CTetrisCell*>m_PatCell4Vec;

	vector<CTetrisCell*> m_SecondBeforePatCell4Vec;
	vector<CTetrisCell*>m_SecondPatCell4Vec;

	CPoint m_CurXY;
	CPoint m_CurXY2;

	int m_PatNo, m_RotNo;
	int m_PrePatNo, m_PreRotNo;

	int m_SecondPatNo, m_SecondRotNo;
	int m_SecondPrePatNo, m_SecondPreRotNo;

	CDC* m_PanDC, * m_BackDC, * m_ImgDC, * m_PreMemDC;
	CDC* m_PanDCplayer2, * m_BackDCplayer2, * m_ImgDCplayer2, * m_PreMemDCplayer2;

	CTetrisCell m_tCell[TBOARD_BDCNT];
	CTetrisCell m_PreCell[PRE_BDCNT];
	CTetrisCell* m_CurCell;

	CTetrisCell m_SecondPreCell[PRE_BDCNT];//두번째
	CTetrisCell* m_SecondCurCell;

public:
	CTetrisDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.

	virtual ~CTetrisDlg();

	int GetNo(CPoint xy);
	int GetPreNo(CPoint xy);
	void Img_Load(CDC* pDC, CRect rt, CString fpath);
	CDC* MakeMemDC(CRect rt);
	CDC* DeleteMemDC(CDC* pDC);


	void Start();//7
	void Proglnit();//6
	CTetrisCell* GetCell(CPoint xy);//7
	CTetrisCell* GetPreCell(CPoint xy);//7
	CTetrisCell* GetPreCellPlayer2(CPoint xy);

	void MakePattern();//7
	void MakePatternPlayer2();

	void ViewPattern();//8
	void ViewPatternPlayer2();
	void DrawPattern(vector<CTetrisCell*> v);//8
	void DrawPatternPlayer2(vector<CTetrisCell*> v);

	void ErasePattern(vector<CTetrisCell* > v);//8
	void ErasePatternPlayer2(vector<CTetrisCell*> v);

	void SetPattern(vector<CTetrisCell*>v);//8
	void SetPatternPlayer2(vector<CTetrisCell*>v);

	bool ViewCheck(int rot, CPoint xy);//9
	bool ViewCheckPlayer2(int rot, CPoint xy);

	bool MoveCheck(int dir);//9
	bool MoveCheckPlayer2(int dir);

	void ArriveBottom();//9
	void ArriveBottomPlayer2();

	void SetSpeed();//10
	void Reset();//10
	void Stop();//10
	void DrawPreview();//11
	void DrawPreviewPlayer2();
	
	float m_nSpeed;//원래는 int
	int m_nScore;
	int BlockCheck();//11
	void BlockDelete(int delY);//11
	void Refresh();//11
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnPaint();
	afx_msg void OnClickedButtonStart();


// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CTetrisDlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	int m_ScoreEdit;
	afx_msg void OnBnClickedButtonGuide();
};
