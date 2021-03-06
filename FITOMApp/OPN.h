#ifndef __OPN_H__
#define __OPN_H__

#include "MultiDev.h"

class COPN : public CSoundDevice {
	friend class COPN2;
protected:
	virtual void UpdateVolExp(UINT8 ch);
	virtual void UpdateVoice(UINT8 ch);
	virtual void UpdateFreq(UINT8 ch, const FNUM* fnum);
	virtual void UpdatePanpot(UINT8 ch);
	virtual void UpdateSustain(UINT8 ch);
	virtual void UpdateKey(UINT8 ch, UINT8 keyon);
	virtual void UpdateTL(UINT8 ch, UINT8 op, UINT8 lev);

	static UINT8 map[4];
	static UINT8 carmsk[8];
public:
	COPN(CPort* pt, UINT8 fsamp=0, UINT8 devtype=DEVICE_OPN);
};

class COPN2 : public CSpanDevice {
protected:
	virtual void UpdateVoice(UINT8 ch);
	virtual void UpdateKey(UINT8 ch, UINT8 keyon);
	LFORESOURCE* lfores;
public:
	COPN2(CPort* pt1, CPort* pt2, UINT8 fsamp=0, UINT8 devtype=DEVICE_OPN2);
	~COPN2();
	virtual void EnableDevPM(UINT8 ch, UINT8 on);
	virtual void EnableDevAM(UINT8 ch, UINT8 on);
	virtual void SetDevPMDepth(UINT8 ch, UINT8 dep);
	virtual void SetDevAMDepth(UINT8 ch, UINT8 dep);
	virtual void SetDevPMRate(UINT8 ch, UINT8 rate);
	virtual void SetDevAMRate(UINT8 ch, UINT8 rate);
};

class COPNC : public COPN {
public:
	COPNC(CPort* pt, UINT8 fsamp = 0, UINT8 devtype = DEVICE_OPNC);
};

class COPN2C : public COPN2 {
public:
	COPN2C(CPort* pt1, CPort* pt2, UINT8 fsamp = 0, UINT8 devtype = DEVICE_OPN2C);
};

class COPN2L : public COPN2 {
public:
	COPN2L(CPort* pt1, CPort* pt2, UINT8 fsamp = 0, UINT8 devtype = DEVICE_OPN2L);
};

class COPNB : public COPN2 {
public:
	COPNB(CPort* pt1, CPort* pt2, UINT8 fsamp = 0, UINT8 devtype = DEVICE_OPNB);
};

class CF286 : public COPNB {
public:
	CF286(CPort* pt1, CPort* pt2, UINT8 fsamp = 0, UINT8 devtype = DEVICE_F286);
};

class C2610B : public COPNB {
public:
	C2610B(CPort* pt1, CPort* pt2, UINT8 fsamp = 0, UINT8 devtype = DEVICE_2610B);
};

class COPNA : public COPN2 {
protected:
	virtual void UpdateRhythmVol();
public:
	COPNA(CPort* pt1, CPort* pt2, UINT8 fsamp = 0, UINT8 devtype = DEVICE_OPNA);
	virtual void RhythmOn(UINT8 num, UINT8 vel, SINT8 pan, FMVOICE* rv, FNUM* fnum);
};

class COPN3L : public COPNA {
public:
	COPN3L(CPort* pt1, CPort* pt2, UINT8 fsamp = 0, UINT8 devtype = DEVICE_OPN3L);
};

#endif

