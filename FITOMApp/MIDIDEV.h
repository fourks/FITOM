#ifndef __MIDIDEV_H__
#define __MIDIDEV_H__

#include "Port.h"

class CMidiIn {
public:
	CMidiIn() {};
	~CMidiIn() {};
	virtual UINT8	IsReceived() = 0;
	virtual UINT8	Read() = 0;
	virtual const char* GetDescriptor() = 0;
};

class CMidiOut {
	UINT8 buf[4096];
	int	len;
public:
	CMidiOut() : len(0) {};
	~CMidiOut() {};
	virtual UINT8 IsTxBusy() = 0;
	virtual void Send(UINT8 data) = 0;
	virtual void Send(void* msg, size_t len) = 0;
	virtual const char* GetDescriptor() = 0;
};

class CMPU401 : public CMidiIn, public CMidiOut
{
protected:
	char desc[32];
public:
	CMPU401(UINT16 dtport, UINT16 stport);
	~CMPU401();
	virtual UINT8	IsReceived();
	virtual UINT8	IsTxBusy();
	virtual UINT8	Read();
	virtual void Send(UINT8 data);
	virtual void Send(void* msg, size_t len);
	UINT8	GetStat();
	void	SetStat(UINT8 data);
	virtual const char* GetDescriptor() { return desc; };
protected:
	UINT16 portd;
	UINT16 ports;
};

class CYMZ263Midi : public CMidiIn, public CMidiOut
{
protected:
	char desc[32];
public:
	CYMZ263Midi(CPort* devport);
	~CYMZ263Midi();
	virtual UINT8	IsReceived();
	virtual UINT8	Read();
	virtual UINT8	IsTxBusy();
	virtual void Send(UINT8 data);
	virtual void Send(void* msg, size_t len);
	virtual const char* GetDescriptor() { return desc; };
protected:
	CPort* port;
};

class CRSMidi : public CMidiIn, public CMidiOut
{
protected:
	char desc[32];
public:
	CRSMidi();
	~CRSMidi();
	virtual UINT8	IsReceived();
	virtual UINT8	IsTxBusy();
	virtual UINT8	Read();
	virtual void Send(UINT8 data);
	virtual void Send(void* msg, size_t len);
	virtual const char* GetDescriptor() { return desc; };
};


#ifdef _WIN32

#define RING_MAX	8192

class CW32MidiIn : public CMidiIn
{
public:
	CW32MidiIn(char* name);
	~CW32MidiIn();
	virtual UINT8	IsReceived();
	virtual UINT8	Read();
	static void CALLBACK MidiInProc(HMIDIIN hMidiIn, UINT wMsg, DWORD dwInstance, DWORD dwParam1, DWORD dwParam2);
	virtual const char* GetDescriptor() { return desc; };
protected:
	int AddBuffer(DWORD msg);
	HMIDIIN hIn;
	MIDIHDR mhdr;
	UINT8 buf[RING_MAX];
	DWORD rpt;
	DWORD wpt;
	char desc[32];
};

class CW32MidiOut : public CMidiOut
{
public:
	CW32MidiOut(char* name);
	~CW32MidiOut();
	virtual UINT8 IsTxBusy();
	virtual void Send(UINT8 data);
	virtual void Send(void* msg, size_t len);
	virtual const char* GetDescriptor() { return desc; };
	static void CALLBACK MidiOutProc(HMIDIOUT hMidiOut, UINT wMsg, DWORD dwInstance, DWORD dwParam1, DWORD dwParam2);
protected:
	HMIDIOUT hOut;
	UINT32 txout;
	char desc[32];
};

class CW32RsMidi : public CMidiIn, public CMidiOut
{
protected:
	HANDLE hCom;
	DCB dcb;
	char desc[32];
public:
	CW32RsMidi(char* name);
	~CW32RsMidi();
	virtual UINT8	IsReceived();
	virtual UINT8	Read();
	virtual UINT8	IsTxBusy();
	virtual void Send(UINT8 data);
	virtual void Send(void* msg, size_t len);
	virtual const char* GetDescriptor();
};
#endif

#endif
