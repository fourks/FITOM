
// FITOMAppDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "FITOMApp.h"
#include "FITOMAppDlg.h"
#include "afxdialogex.h"
#include "FITOM.h"
#include "MidiMonitor.h"
#include "DevMonitor.h"
#include "VoiceEditDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// アプリケーションのバージョン情報に使われる CAboutDlg ダイアログ

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// ダイアログ データ
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

// 実装
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CFITOMAppDlg ダイアログ



CFITOMAppDlg::CFITOMAppDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CFITOMAppDlg::IDD, pParent)
	, bMidMon(FALSE)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CFITOMAppDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK_START, btnStart);
	DDX_Control(pDX, IDC_CHECK_MIDIMON, btnMidiMon);
	DDX_Check(pDX, IDC_CHECK_MIDIMON, bMidMon);
}

BEGIN_MESSAGE_MAP(CFITOMAppDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	ON_COMMAND(IDC_CHECK_MIDIMON, OnMidiMon)
	ON_COMMAND(IDC_BUTTON_DEVMON, OnDevMon)
	ON_COMMAND(IDC_CHECK_START, OnStartStop)
	ON_COMMAND(IDC_BUTTON_CONFIG, OnConfig)
	ON_COMMAND(IDC_BUTTON_VEDIT, OnVEdit)
END_MESSAGE_MAP()


// CFITOMAppDlg メッセージ ハンドラー

BOOL CFITOMAppDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// "バージョン情報..." メニューをシステム メニューに追加します。

	// IDM_ABOUTBOX は、システム コマンドの範囲内になければなりません。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// このダイアログのアイコンを設定します。アプリケーションのメイン ウィンドウがダイアログでない場合、
	//  Framework は、この設定を自動的に行います。
	SetIcon(m_hIcon, TRUE);			// 大きいアイコンの設定
	SetIcon(m_hIcon, FALSE);		// 小さいアイコンの設定

	// TODO: 初期化をここに追加します。
	theFitom = ((CFITOMApp*)AfxGetApp())->GetFitom();
	dlgMidi = new CMidiMonitor(theFitom, this);

	return TRUE;  // フォーカスをコントロールに設定した場合を除き、TRUE を返します。
}

void CFITOMAppDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// ダイアログに最小化ボタンを追加する場合、アイコンを描画するための
//  下のコードが必要です。ドキュメント/ビュー モデルを使う MFC アプリケーションの場合、
//  これは、Framework によって自動的に設定されます。

void CFITOMAppDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 描画のデバイス コンテキスト

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// クライアントの四角形領域内の中央
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// アイコンの描画
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// ユーザーが最小化したウィンドウをドラッグしているときに表示するカーソルを取得するために、
//  システムがこの関数を呼び出します。
HCURSOR CFITOMAppDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CFITOMAppDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: ここにメッセージ ハンドラー コードを追加するか、既定の処理を呼び出します。

	CDialogEx::OnTimer(nIDEvent);
}

void CFITOMAppDlg::OnOK()
{
	// TODO: ここに特定なコードを追加するか、もしくは基底クラスを呼び出してください。
	dlgMidi->CloseWindow();

	CDialogEx::OnOK();
}



void CFITOMAppDlg::OnDevMon()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	CDevMonitor* pdlg = new CDevMonitor(theFitom, this);
	pdlg->ShowWindow(SW_SHOW);
}


void CFITOMAppDlg::OnConfig()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
}


void CFITOMAppDlg::OnMidiMon()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	UpdateData(1);
	dlgMidi->ShowWindow(bMidMon ? SW_SHOW : SW_HIDE);
}


void CFITOMAppDlg::OnStartStop()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
}


void CFITOMAppDlg::OnVEdit()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	CVoiceEditDlg dlg(this);
	dlg.SetDevice(theFitom->GetInstDeviceIDFromIndex(0));
	dlg.SetBank(0);
	dlg.SetProg(0);
	if (dlg.DoModal() == IDOK) {

	}
}


BOOL CFITOMAppDlg::OnCommand(WPARAM wParam, LPARAM lParam)
{
	// TODO: ここに特定なコードを追加するか、もしくは基底クラスを呼び出してください。
	return CDialogEx::OnCommand(wParam, lParam);
}
