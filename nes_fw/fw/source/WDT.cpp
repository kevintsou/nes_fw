
#include "Var_Def.h"
#include "MCU_Reg.h"
#include "WDT_Reg.h"


//RTC bug, this version RTS fail
void En_Real_Timer()
{
    WDTREG[REAL_TIMER1_H] = 0;
    WDTREG[REAL_TIMER1_L] = 0;
    WDTREG[WDT_CON] |= RTC1_EN;
}



void Dis_Real_Timer()
{
    WDTREG[WDT_CON] &= ~(RTC1_EN);
    WDTREG[REAL_TIMER1_H] = 0;
    WDTREG[REAL_TIMER1_L] = 0;
}


SWORD Rd_Real_Timer()
{
    TWORD aw_Timer;
    aw_Timer.HLMODE.H = WDTREG[REAL_TIMER1_H];
    aw_Timer.HLMODE.L = WDTREG[REAL_TIMER1_L];
    return (aw_Timer.Word);

}
