#pragma once

#define _OUT (int)
#define _IN	(unsigned int)

class REG
{
private:
	int (REG::* _pOPRFunc)(int);
	virtual int defCallback(int v);
public:
	virtual REG& operator[](unsigned int addr);
	virtual int operator[](int regaddr);

	int* _pR16_mem;
	int	regIdx;
};


class REG_R16 : public REG
{
private:
	int _inv;
	unsigned char (REG_R16::* _pOPRFunc)(unsigned char);

	int defCallback(int v);
	unsigned char callfundef(unsigned char v);
	unsigned char callfun1(unsigned char v);
	unsigned char callfun2(unsigned char v);

public:
	REG_R16(int inv);
	~REG_R16();

	int operator[](int regaddr);
	REG_R16& operator[](unsigned int addr);

	int operator=(int v);
	REG& operator|=(REG b);
	REG& operator|=(int i);
	REG& operator&=(REG b);
	REG& operator&=(int i);
};

/*
	global variable declaration
*/
extern REG_R16 R16_MR_QINFO;

/*
	function declaration
*/
extern int initCmodelMem();
extern int initCmodelReg();