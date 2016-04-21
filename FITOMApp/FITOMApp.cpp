
// FITOMApp.cpp : �A�v���P�[�V�����̃N���X������`���܂��B
//

#include "stdafx.h"
#include "FITOMApp.h"
#include "FITOMAppDlg.h"
#include "FITOM.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CFITOMApp

BEGIN_MESSAGE_MAP(CFITOMApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CFITOMApp �R���X�g���N�V����

CFITOMApp::CFITOMApp() : hTimer(0), hPolling(0), theFitom(0), bRunning(FALSE)
{
	// �ċN���}�l�[�W���[���T�|�[�g���܂�
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: ���̈ʒu�ɍ\�z�p�R�[�h��ǉ����Ă��������B
	// ������ InitInstance ���̏d�v�ȏ��������������ׂċL�q���Ă��������B
}


// �B��� CFITOMApp �I�u�W�F�N�g�ł��B

CFITOMApp theApp;


// CFITOMApp ������

BOOL CFITOMApp::InitInstance()
{
	// �A�v���P�[�V���� �}�j�t�F�X�g�� visual �X�^�C����L���ɂ��邽�߂ɁA
	// ComCtl32.dll Version 6 �ȍ~�̎g�p���w�肷��ꍇ�́A
	// Windows XP �� InitCommonControlsEx() ���K�v�ł��B�����Ȃ���΁A�E�B���h�E�쐬�͂��ׂĎ��s���܂��B
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// �A�v���P�[�V�����Ŏg�p���邷�ׂẴR���� �R���g���[�� �N���X���܂߂�ɂ́A
	// �����ݒ肵�܂��B
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	// �_�C�A���O�ɃV�F�� �c���[ �r���[�܂��̓V�F�� ���X�g �r���[ �R���g���[����
	// �܂܂�Ă���ꍇ�ɃV�F�� �}�l�[�W���[���쐬���܂��B
	CShellManager *pShellManager = new CShellManager;

	// MFC �R���g���[���Ńe�[�}��L���ɂ��邽�߂ɁA"Windows �l�C�e�B�u" �̃r�W���A�� �}�l�[�W���[���A�N�e�B�u��
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));

	// �W��������
	// �����̋@�\���g�킸�ɍŏI�I�Ȏ��s�\�t�@�C����
	// �T�C�Y���k���������ꍇ�́A�ȉ�����s�v�ȏ�����
	// ���[�`�����폜���Ă��������B
	// �ݒ肪�i�[����Ă��郌�W�X�g�� �L�[��ύX���܂��B
	// TODO: ��Ж��܂��͑g�D���Ȃǂ̓K�؂ȕ������
	// ���̕������ύX���Ă��������B
	SetRegistryKey(_T("FITOM"));
#ifdef _DEBUG
	::AllocConsole();
	::freopen("CONOUT$", "w", stdout);
	::freopen("CONOUT$", "w", stderr);
#endif
	FITOMStart();
	CFITOMAppDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: �_�C�A���O�� <OK> �ŏ����ꂽ���̃R�[�h��
		//  �L�q���Ă��������B
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: �_�C�A���O�� <�L�����Z��> �ŏ����ꂽ���̃R�[�h��
		//  �L�q���Ă��������B
	}
	else if (nResponse == -1)
	{
		TRACE(traceAppMsg, 0, "�x��: �_�C�A���O�̍쐬�Ɏ��s���܂����B�A�v���P�[�V�����͗\�������ɏI�����܂��B\n");
		TRACE(traceAppMsg, 0, "�x��: �_�C�A���O�� MFC �R���g���[�����g�p���Ă���ꍇ�A#define _AFX_NO_MFC_CONTROLS_IN_DIALOGS ���w��ł��܂���B\n");
	}
	FITOMStop();

#ifdef _DEBUG
	::FreeConsole();
#endif
	// ��ō쐬���ꂽ�V�F�� �}�l�[�W���[���폜���܂��B
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

	// �_�C�A���O�͕����܂����B�A�v���P�[�V�����̃��b�Z�[�W �|���v���J�n���Ȃ���
	//  �A�v���P�[�V�������I�����邽�߂� FALSE ��Ԃ��Ă��������B
	return FALSE;
}

int CFITOMApp::FITOMStart()
{
	char profilepath[MAX_PATH];
	::GetPrivateProfileString(_T("Profile"), _T("Path"), _T("."), profilepath, MAX_PATH, _T(".\\FITOM.ini"));
	theFitom = CFITOM::GetInstance();
	theFitom->InitInstance(profilepath);
	::timeBeginPeriod(1);
	hTimer = timeSetEvent(10 - 1, 0, TimerProc, (DWORD)this, TIME_PERIODIC | TIME_CALLBACK_FUNCTION);
	hPolling = (HANDLE)_beginthreadex(0, 0, PollingProc, (void*)this, 0, 0);
	bRunning = TRUE;
	return 0;
}

int CFITOMApp::FITOMStop()
{
	bRunning = FALSE;
	timeKillEvent(hTimer);
	timeEndPeriod(1);
	::Sleep(100);
	::TerminateThread(hPolling, 0);
	::Sleep(100);
	theFitom->ExitInstance();
	return 0;
}

void CALLBACK CFITOMApp::TimerProc(UINT uTimerID, UINT uMsg, DWORD dwUser, DWORD dummy1, DWORD dummy2)
{
	CFITOMApp* theFitomApp = (CFITOMApp*)dwUser;
	if (theFitomApp->bRunning && theFitomApp->theFitom) {
		theFitomApp->theFitom->TimerCallBack(theFitomApp->lltickcount);
		theFitomApp->lltickcount++;
	}
}

unsigned int CFITOMApp::PollingProc(void* params)
{
	CFITOMApp* theFitomApp = (CFITOMApp*)params;
	while (theFitomApp->bRunning && theFitomApp->theFitom) {
		theFitomApp->theFitom->PollingCallBack();
		//::Sleep(1);
	}
	return 0;
}

