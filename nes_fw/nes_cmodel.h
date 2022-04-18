#pragma once

#define _OUT (int)
#define _IN	(unsigned int)

#define u64_t unsigned long
#define u32_t unsigned int
#define u16_t unsigned short
#define u8_t  unsigned char

#ifdef __cplusplus
class REG
{
private:
	int (REG::* _pOPRFunc)(int);
	virtual int defCallback(int v);
public:
	virtual REG& operator[](unsigned int addr);
	virtual int operator[](int regaddr);

	char* _pReg_mem;
	int	regIdx;
	int regBar;
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

class C_DMAREG : public REG
{
private:
	int _inv;
	unsigned char (C_DMAREG::* _pOPRFunc)(unsigned char);

	int defCallback(int v);
	unsigned char funcDmaCtrl(unsigned char v);
	unsigned char callfun1(unsigned char v);
	unsigned char callfun2(unsigned char v);

public:
	C_DMAREG(int inv);
	~C_DMAREG();

	int operator[](int regaddr);
	C_DMAREG& operator[](unsigned int addr);

	int operator=(int v);
	REG& operator|=(REG b);
	REG& operator|=(int i);
	REG& operator&=(REG b);
	REG& operator&=(int i);
};
#else
typedef struct REG REG;
typedef struct REG_R16 REG_R16;
typedef struct C_DMAREG C_DMAREG;
#endif
/*
	global variable declaration
*/
extern REG_R16 R16_MR_QINFO;   // for test 
extern C_DMAREG DMAREG;

/*
	function declaration
*/
extern int initCmodelMem();
extern int initCmodelReg();