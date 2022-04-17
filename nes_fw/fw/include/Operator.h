#ifndef _OPERATOR_H_
#define _OPERATOR_H_

#define HIGHBYTE(x)             ((unsigned char)( (x) >> 8))
#define LOWBYTE(x)              ((unsigned char)( (x) ))
#define BYTE2INT(x,y)           ((unsigned int)(((unsigned int)x<<8) + (unsigned int)y))
#define BYTE2LONG(a,b,c,d)      ((unsigned long)(((unsigned long)a<<24) + ((unsigned long)b<<16) + ((unsigned long)c<<8) + (unsigned long)d))

#define INT_BYTE0(x)            ((unsigned char)( (x) ))        // LSB
#define INT_BYTE1(x)            ((unsigned char)( (x) >> 8))
#define INT_BYTE2(x)            ((unsigned char)( (x) >> 16))
#define INT_BYTE3(x)            ((unsigned char)( (x) >> 24))   // MSB


#define MaxNum_To_MaxAddr(x)    ( (x)-1 )
#define MaxAddr_To_MaxNum(x)    ( (x)+1 )
#define LOGIC_TEST(x)           ((x)?1:0)
#define _Align_Num(var,align_num)   (((var + align_num-1)/align_num)*align_num)

#define _setbit(var, value)  (var |= (value))
#define _clrbit(var, value)  (var &= (~(value)))
#define _testbit(var, value) (((var) & (value)) == (value))

//=====================================================================================================================
//                                                  [ Define Library ]
// �`�N�ƶ�:
//          1. �קK�ѼƱa�J ++ -- �������B�⦡. �y�����ƹB��
//=====================================================================================================================

//�o����w�a�}�W���@�Ӧr�`�Φr
#define  MEM_B( x )  ( *( (BYTE*) (x) ) )
#define  MEM_W( x )  ( *( (WORD*) (x) ) )

//�D�̤j�ȩM�̤p��
#define  MAX( x, y ) ( ((x) > (y)) ? (x) : (y) )
#define  MIN( x, y ) ( ((x) < (y)) ? (x) : (y) )

//��^�@�Ӥ�X�j���̱���8������
#define RND8( x )       ((((x) + 7) / 8 ) * 8 )

//�N�@�Ӧr���ഫ���j�g
#define  UPCASE( c ) ( ((c) >= 'a' && (c) <= 'z') ? ((c) - 0x20) : (c) )

//�P�_�r�ŬO���O10�i�Ȫ��Ʀr
#define  DECCHK( c ) ((c) >= '0' && (c) <= '9')

//�P�_�r�ŬO���O16�i�Ȫ��Ʀr
#define  HEXCHK( c ) ( ((c) >= '0' && (c) <= '9') ||\
                       ((c) >= 'A' && (c) <= 'F') ||\
                       ((c) >= 'a' && (c) <= 'f') )
//����X���@�Ӥ�k
#define  INC_SAT( val )  (val = ((val)+1 > (val)) ? (val)+1 : (val))

//��^�@�ӵL�Ÿ���n������
#define MOD_BY_POWER_OF_TWO(X,n)=X%(2^n)

//���IO�Ŷ��M�g�b�s�x�Ŷ������c�A��J��X�B�z
#define inp(port)         (*((volatile BYTE*) (port)))
#define inpw(port)        (*((volatile WORD*) (port)))
#define inpdw(port)       (*((volatile DWORD*)(port)))

#define outp(port, val)   (*((volatile BYTE*) (port)) = ((BYTE) (val)))
#define outpw(port, val)  (*((volatile WORD*) (port)) = ((WORD) (val)))
#define outpdw(port, val) (*((volatile DWORD*) (port)) = ((DWORD) (val)))


#define abs(x) (((x)>=0)?(x):-(x))

#endif // #ifndef _OPERATOR_H_
