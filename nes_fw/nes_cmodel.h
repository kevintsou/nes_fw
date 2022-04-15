#pragma once

class REG_R16_MR_QINFO
{
private:
	int _inv;
	unsigned char (REG_R16_MR_QINFO::* _pOPRFunc)(unsigned char);

	unsigned char callfundef(unsigned char v);
	unsigned char callfun1(unsigned char v);
	unsigned char callfun2(unsigned char v);

public:
	REG_R16_MR_QINFO(int inv);
	~REG_R16_MR_QINFO();

	//assign value to register
	unsigned char operator=(unsigned char v);
	//use signed int as read operator
	unsigned char operator[](int regaddr);
	//use unsigned int as write operator
	REG_R16_MR_QINFO& operator[](unsigned int regaddr);

};