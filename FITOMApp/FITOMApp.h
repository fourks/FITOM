
// FITOMApp.h : PROJECT_NAME �A�v���P�[�V�����̃��C�� �w�b�_�[ �t�@�C���ł��B
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH �ɑ΂��Ă��̃t�@�C�����C���N���[�h����O�� 'stdafx.h' ���C���N���[�h���Ă�������"
#endif

#include "resource.h"		// ���C�� �V���{��


// CFITOMApp:
// ���̃N���X�̎����ɂ��ẮAFITOMApp.cpp ���Q�Ƃ��Ă��������B
//

class CFITOM;

class CFITOMApp : public CWinApp
{
protected:
	UINT hTimer;
	HANDLE hPolling;
	volatile UINT32 lltickcount;
	volatile BOOL bRunning;
	CFITOM* theFitom;

public:
	CFITOMApp();
	int FITOMStart();
	int FITOMStop();
	CFITOM* GetFitom() { return theFitom; };

//�X�^�e�B�b�N
	static void CALLBACK TimerProc(UINT uTimerID, UINT uMsg, DWORD dwUser, DWORD dummy1, DWORD dummy2);
	static unsigned int CALLBACK PollingProc(void* params);

// �I�[�o�[���C�h
public:
	virtual BOOL InitInstance();

// ����

	DECLARE_MESSAGE_MAP()
};

extern CFITOMApp theApp;
