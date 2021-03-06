#ifndef __OPM_H__
#define __OPM_H__

#include "SoundDev.H"

class COPM : public CSoundDevice {
protected:
	virtual FNUM GetFnumber(UINT8 ch);

	virtual void UpdateVolExp(UINT8 ch);
	virtual void UpdateVoice(UINT8 ch);
	virtual void UpdateFreq(UINT8 ch, const FNUM* fnum);
	virtual void UpdatePanpot(UINT8 ch);
	virtual void UpdateLFO() {};
	virtual void UpdateSustain(UINT8 ch);
	virtual void UpdateTL(UINT8 ch, UINT8 op, UINT8 lev);
	virtual void UpdateKey(UINT8 ch, UINT8 keyon);
	virtual void ResetLFO(UINT8 ch, UINT8 lfonum) {};

	static UINT8 KeyCode[];
	static UINT8 map[];
	static UINT8 carmsk[];
	UINT8 lfos;
	SINT16 MasterTune;
	LFORESOURCE* lfores;
public:
	COPM(CPort* pt, UINT8 fsamp=2);
	virtual UINT8 QueryCh(CMidiCh* parent, FMVOICE* voice, int mode);
	virtual void SetLFOParam(FMVOICE* voice, int update) {};
	virtual void SetLFOMode(UINT8 ch, UINT8 mode, int update) {};
	virtual void EnableDevPM(UINT8 ch, UINT8 on);
	virtual void EnableDevAM(UINT8 ch, UINT8 on);
	virtual void SetDevPMDepth(UINT8 ch, UINT8 dep);
	virtual void SetDevAMDepth(UINT8 ch, UINT8 dep);
	virtual void SetDevPMRate(UINT8 ch, UINT8 rate);
	virtual void SetDevAMRate(UINT8 ch, UINT8 rate);
};

class COPP : public COPM {
protected:
	virtual void UpdateLFO() {};
public:
	COPP(CPort* pt, UINT8 fsamp=2);
};

class COPZ : public COPM {
protected:
	virtual void UpdateVolExp(UINT8 ch);
	virtual void UpdateVoice(UINT8 ch);
	virtual void UpdatePanpot(UINT8 ch);
	UINT8 lfos;
public:
	COPZ(CPort* pt, UINT8 fsamp=2);
};

#endif
