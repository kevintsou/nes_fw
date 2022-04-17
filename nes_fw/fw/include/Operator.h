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
// 猔種ㄆ兜:
//          1. 磷把计盿 ++ -- 单单笲衡Α. 硑Θ狡笲衡
//=====================================================================================================================

//眔﹚竊┪
#define  MEM_B( x )  ( *( (BYTE*) (x) ) )
#define  MEM_W( x )  ( *( (WORD*) (x) ) )

//―程㎝程
#define  MAX( x, y ) ( ((x) > (y)) ? (x) : (y) )
#define  MIN( x, y ) ( ((x) < (y)) ? (x) : (y) )

//ゑX程钡8计
#define RND8( x )       ((((x) + 7) / 8 ) * 8 )

//盢ダ锣传糶
#define  UPCASE( c ) ( ((c) >= 'a' && (c) <= 'z') ? ((c) - 0x20) : (c) )

//耞才琌ぃ琌10秈计
#define  DECCHK( c ) ((c) >= '0' && (c) <= '9')

//耞才琌ぃ琌16秈计
#define  HEXCHK( c ) ( ((c) >= '0' && (c) <= '9') ||\
                       ((c) >= 'A' && (c) <= 'F') ||\
                       ((c) >= 'a' && (c) <= 'f') )
//ňゎ犯よ猭
#define  INC_SAT( val )  (val = ((val)+1 > (val)) ? (val)+1 : (val))

//礚才腹计nЮ
#define MOD_BY_POWER_OF_TWO(X,n)=X%(2^n)

//癸IO丁琈甮纗丁挡篶块块矪瞶
#define inp(port)         (*((volatile BYTE*) (port)))
#define inpw(port)        (*((volatile WORD*) (port)))
#define inpdw(port)       (*((volatile DWORD*)(port)))

#define outp(port, val)   (*((volatile BYTE*) (port)) = ((BYTE) (val)))
#define outpw(port, val)  (*((volatile WORD*) (port)) = ((WORD) (val)))
#define outpdw(port, val) (*((volatile DWORD*) (port)) = ((DWORD) (val)))


#define abs(x) (((x)>=0)?(x):-(x))

#endif // #ifndef _OPERATOR_H_
