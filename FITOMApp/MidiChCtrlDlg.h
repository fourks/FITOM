#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// CMidiChCtrlDlg ダイアログ

class CMidiChCtrlDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CMidiChCtrlDlg)

public:
	CMidiChCtrlDlg(CFITOM* pFitom, int ifid, int ch, CWnd* pParent = NULL);   // 標準コンストラクター
	virtual ~CMidiChCtrlDlg();

// ダイアログ データ
	enum { IDD = IDD_MIDI_CH_MONITOR };
protected:
	static const int numMultiCtrl = 11;
	static const int numChecker = 6;
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
	virtual void OnCancel();
	virtual void OnOK();
	UINT_PTR hTimer;
	int iIF;
	int iCH;
	CFITOM* theFitom;
	CMidiInst* theInst;
	CMidiCh* theCh;
	void InitDevice();
	void SetDevice();
	void SetBank();
	void SetProg();
public:
	virtual BOOL OnInitDialog();
	virtual void PostNcDestroy();
	afx_msg void OnSelendokComboDevice();
	afx_msg void OnSelendokComboBank();
	afx_msg void OnSelendokComboProg();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	void SetVolume(int val) { theCh->SetVolume(val); };
	void SetExpr(int val) { theCh->SetExpress(val); };
	void SetModur(int val) { theCh->SetModulation(val); };
	void SetFoot(int val) { theCh->SetFootCtrl(val); };
	void SetPorta(int val) { theCh->SetPortTime(val); };
	void SetRange(int val) { theCh->SetBendRange(val); };
	void SetBend(int val) { theCh->SetPitchBend(val+8192); };
	void SetTune(int val) { theCh->SetFineTune(val + 8192); };
	void SetPoly(int val) { theCh->SetPoly(val); };
	void SetPhyCh(int val) { theCh->SetPhyCh(val); };
	void SetPan(int val) { theCh->SetPanpot(val); };
	void SetLegato(int val) { theCh->SetLegato(val ? 64 : 0); };
	void SetPortOn(int val) { theCh->SetPortamento(val ? 64 : 0); };
	void SetMono(int val) { theCh->SetMono(val ? 1 : 0); };
	void SetSustain(int val) { theCh->SetSustain(val ? 64 : 0); };
	void SetDamp(int val) { theCh->SetForceDamp(val ? 64 : 0); };
	void SetSoste(int val) { theCh->SetSostenuto(val ? 64 : 0); };
	int GetVolume() { return theCh->GetTrackVolume(); };
	int GetExpr() { return theCh->GetExpress(); };
	int GetModur() { return theCh->GetModulation(); };
	int GetFoot() { return theCh->GetFootCtrl(); };
	int GetPorta() { return theCh->GetPortTime(); };
	int GetRange() { return theCh->GetBendRange(); };
	int GetBend() { return theCh->GetPitchBend()-8192; };
	int GetTune() { return theCh->GetFineTune()-8192; };
	int GetPoly() { return theCh->GetPoly(); };
	int GetPhyCh() { return theCh->GetPhyCh(); };
	int GetPan() { return theCh->GetPanpot(); };
	int GetLegato() { return theCh->GetLegato() != 0; };
	int GetPortOn() { return theCh->GetPortamento() != 0; };
	int GetMono() { return theCh->GetPoly() <= 1 ; };
	int GetSustain() { return theCh->GetSustain() != 0; };
	int GetDamp() { return theCh->GetForceDamp() != 0; };
	int GetSoste() { return theCh->GetSostenuto() != 0; };
	CEdit edtMidiIfCh;
	CComboBox cmbDevice;
	CComboBox cmbBank;
	CComboBox cmbProg;
	class CChecker {
	protected:
		CButton Btn;
		CMidiChCtrlDlg* pParent;
		int (CMidiChCtrlDlg::*pGetter)();
		void (CMidiChCtrlDlg::*pSetter)(int val);
	public:
		CChecker() : pParent(0), pGetter(0), pSetter(0){};
		CChecker(CMidiChCtrlDlg* par, int id, int(CMidiChCtrlDlg::*pget)(), void(CMidiChCtrlDlg::*pset)(int)) { Attach(par, id, pget, pset); };
		void Attach(CMidiChCtrlDlg* par, int id, int(CMidiChCtrlDlg::*pget)(), void(CMidiChCtrlDlg::*pset)(int));
		BOOL IsMember(int id) { return (id == Btn.GetDlgCtrlID()); };
		void OnClicked() { (pParent->*pSetter)(Checked()); };
		BOOL Checked() { return Btn.GetCheck(); };
		void UpdateState() { Btn.SetCheck((pParent->*pGetter)()!=0); };
	};
	class CMultiCtrl {
	protected:
		int value;
		CMidiChCtrlDlg* pParent;
		int (CMidiChCtrlDlg::*pGetter)();
		void (CMidiChCtrlDlg::*pSetter)(int val);
	public:
		CEdit Edit;
		CSpinButtonCtrl Spin;
		CSliderCtrl Slider;
		BOOL IsMember(int id);
		void SetValue();
		int GetValue() { return value; };
		void SetRange(int min, int max);
		BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
		CMultiCtrl();
		CMultiCtrl(CMidiChCtrlDlg* par, int idedt, int idspn, int idsld, int(CMidiChCtrlDlg::*pget)(), void(CMidiChCtrlDlg::*pset)(int), int min, int max);
		void Attach(CMidiChCtrlDlg* par, int idedt, int idspn, int idsld, int(CMidiChCtrlDlg::*pget)(), void(CMidiChCtrlDlg::*pset)(int));
	};
	CMultiCtrl* pMulCtrl[numMultiCtrl];
	CChecker* pChecker[numChecker];
	afx_msg void OnBnClickedBtnVedit();
	afx_msg void OnBnClickedButtonPick();
	afx_msg void OnBnClickedCheckLegato();
	CSpinButtonCtrl spnPoly;
	CSpinButtonCtrl spnPhyCh;
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnEnChangeEdit(UINT id);
	afx_msg void OnDeltaposSpin(UINT id, NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedCheck(UINT id);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
