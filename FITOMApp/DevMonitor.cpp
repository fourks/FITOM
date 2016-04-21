// DevMonitor.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "FITOM.h"
#include "SoundDev.h"
#include "FITOMApp.h"
#include "DevMonitor.h"
#include "afxdialogex.h"


// CDevMonitor �_�C�A���O

IMPLEMENT_DYNAMIC(CDevMonitor, CDialogEx)

CDevMonitor::CDevMonitor(CFITOM* pFitom, CWnd* pParent /*=NULL*/)
	: CDialogEx(CDevMonitor::IDD, pParent), theFitom(pFitom)
{
	Create(IDD, pParent);
}

CDevMonitor::~CDevMonitor()
{
}

void CDevMonitor::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_DEVICE, cmbDevice);
	DDX_Control(pDX, IDC_LIST_DEVCH, lstDevice);
}


BEGIN_MESSAGE_MAP(CDevMonitor, CDialogEx)
	ON_CBN_SELENDOK(IDC_COMBO_DEVICE, &CDevMonitor::OnSelendokComboDevice)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_RESET, &CDevMonitor::OnBnClickedButtonReset)
END_MESSAGE_MAP()


// CDevMonitor ���b�Z�[�W �n���h���[


void CDevMonitor::OnSelendokComboDevice()
{
	// TODO: �����ɃR���g���[���ʒm�n���h���[ �R�[�h��ǉ����܂��B
	UpdateList(TRUE);
}


void CDevMonitor::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �����Ƀ��b�Z�[�W �n���h���[ �R�[�h��ǉ����邩�A����̏������Ăяo���܂��B
	if (this->IsWindowVisible()) {
		UpdateList(FALSE);
	}
	CDialogEx::OnTimer(nIDEvent);
}


void CDevMonitor::PostNcDestroy()
{
	// TODO: �����ɓ���ȃR�[�h��ǉ����邩�A�������͊��N���X���Ăяo���Ă��������B
	::Sleep(100);
	delete this;
}


void CDevMonitor::OnOK()
{
	// TODO: �����ɓ���ȃR�[�h��ǉ����邩�A�������͊��N���X���Ăяo���Ă��������B
	OnCancel();
}


void CDevMonitor::OnCancel()
{
	// TODO: �����ɓ���ȃR�[�h��ǉ����邩�A�������͊��N���X���Ăяo���Ă��������B
	KillTimer(hTimer);
	DestroyWindow();
}


BOOL CDevMonitor::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO: �����ɏ�������ǉ����Ă�������
	cmbDevice.ResetContent();
	for (int i = 0; i < theFitom->GetInstDevs(); i++) {
		TCHAR tmp[256];
		CSoundDevice* pDev = theFitom->GetInstDeviceFromIndex(i);
		int devid = pDev->GetDevice();
		wsprintf(tmp, _T("%02X:%s"), devid, theFitom->GetDeviceNameFromID(devid));
		cmbDevice.AddString(tmp);
		cmbDevice.SetItemData(i, devid);
	}
	lstDevice.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	lstDevice.InsertColumn(0, _T("CH"), LVCFMT_LEFT, 28);
	lstDevice.InsertColumn(1, _T("F-Num"), LVCFMT_LEFT, 60);
	lstDevice.InsertColumn(2, _T("Voice"), LVCFMT_LEFT, 160);
	lstDevice.InsertColumn(3, _T("Volume"), LVCFMT_LEFT, 32);
	cmbDevice.SetCurSel(0);
	hTimer = SetTimer(0, 10, 0);
	UpdateList(TRUE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// ��O : OCX �v���p�e�B �y�[�W�͕K�� FALSE ��Ԃ��܂��B
}


BOOL CDevMonitor::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �����ɓ���ȃR�[�h��ǉ����邩�A�������͊��N���X���Ăяo���Ă��������B
	if (WM_KEYDOWN == pMsg->message)
	{
		switch (pMsg->wParam)
		{
		case VK_RETURN:
			return FALSE;
		case VK_ESCAPE:
			return FALSE;
		default:
			break;
		}
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}

void CDevMonitor::UpdateList(BOOL binit)
{
	if (binit) {
		lstDevice.DeleteAllItems();
	}
	CSoundDevice* pdev = theFitom->GetInstDeviceFromID(UINT8(cmbDevice.GetItemData(cmbDevice.GetCurSel())));
	if (pdev) {
		for (int i = 0; i < pdev->GetChs(); i++) {
			TCHAR tmp[80];
			if (binit) {
				wsprintf(tmp, _T("%i"), i);
				lstDevice.InsertItem(i, tmp);
			}
			ISoundDevice::FNUM fnum = pdev->GetCurrentFnum(i);
			wsprintf(tmp, _T("%02X:%04X"), fnum.block, fnum.fnum);
			if (lstrcmp(tmp, lstDevice.GetItemText(i, 1))) {
				lstDevice.SetItemText(i, 1, tmp);
			}
			FMVOICE* pvoice = pdev->GetCurrentVoice(i);
			TCHAR name[17];
			memcpy_s(LPVOID(name), 17, pvoice->name, 16);
			name[16] = 0;
			wsprintf(tmp, _T("%08X:%s"), pvoice->ID, name);
			if (lstrcmp(tmp, lstDevice.GetItemText(i, 2))) {
				lstDevice.SetItemText(i, 2, tmp);
			}
			int evol = ::CalcVolExpVel(pdev->GetChAttribute(i)->volume, pdev->GetChAttribute(i)->velocity, pdev->GetChAttribute(i)->express);
			wsprintf(tmp, _T("%i"), evol);
			if (lstrcmp(tmp, lstDevice.GetItemText(i, 3))) {
				lstDevice.SetItemText(i, 3, tmp);
			}
		}
	}
}

void CDevMonitor::OnBnClickedButtonReset()
{
	// TODO: �����ɃR���g���[���ʒm�n���h���[ �R�[�h��ǉ����܂��B
	CSoundDevice* pdev = theFitom->GetInstDeviceFromID(UINT8(cmbDevice.GetItemData(cmbDevice.GetCurSel())));
	if (pdev) {
		pdev->Reset();
	}
}
