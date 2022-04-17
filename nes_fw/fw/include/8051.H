#pragma once

#ifndef D_8051_H
#define D_8051_H

#ifdef __cplusplus
extern "C" {
#endif
#include "nes.h"

#define tmod_gate1_on     0xff
#define tmod_gate1_off    0x7f
#define tmod_counter1_sel 0xff
#define tmod_timer1_sel   0xbf
#define tmod_mode0_of_t1  0xcf
#define tmod_mode1_of_t1  0xdf
#define tmod_mode2_of_t1  0xef
#define tmod_mode3_of_t1  0xff
#define tmod_gate0_on     0xff
#define tmod_gate0_off    0xf7
#define tmod_counter0_sel 0xff
#define tmod_timer0_sel   0xfb
#define tmod_mode0_of_t0  0xfc
#define tmod_mode1_of_t0  0xfd
#define tmod_mode2_of_t0  0xfe
#define tmod_mode3_of_t0  0xff


#define ie_ea_on      0xbf
#define ie_ea_off     0x3f
#define ie_et2_on     0xbf
#define ie_et2_off  0x9f
#define ie_es_on      0xbf
#define ie_es_off     0xaf
#define ie_et1_on     0xbf
#define ie_et1_off   0xb7
#define ie_ex1_on     0xbf
#define ie_ex1_off  0xbb
#define ie_et0_on     0xbf
#define ie_et0_off  0xbd
#define ie_ex0_on     0xbf
#define ie_ex0_off  0xbe


#define ip_pt2_on     0x3f
#define ip_pt2_off    0x1f
#define ip_ps_on      0x3f
#define ip_ps_off     0x2f
#define ip_pt1_on     0x3f
#define ip_pt1_off    0x37
#define ip_px1_on     0x3f
#define ip_px1_off    0x3b
#define ip_pt0_on     0x3f
#define ip_pt0_off    0x3d
#define ip_px0_on     0x3f
#define ip_px0_off    0x3e


#define tcon_tf1_on   0xff
#define tcon_tf1_off  0x7f
#define tcon_tr1_on   0xff
#define tcon_tr1_off  0xbf
#define tcon_tf0_on   0xff
#define tcon_tf0_off  0xdf
#define tcon_tr0_on   0xff
#define tcon_tr0_off  0xef
#define tcon_ie1_on   0xff
#define tcon_ie1_off  0xf7
#define tcon_it1_on   0xff
#define tcon_it1_off  0xfb
#define tcon_ie0_on   0xff
#define tcon_ie0_off  0xfd
#define tcon_it0_on   0xff
#define tcon_it0_off  0xfe

	sfr CKCON;// = 0x8E;
sfr BKCON;// = 0xBB;
sfr T2MOD;// = 0xC9;
sfr EIE;//   = 0xE8;
sfr MD0;//   = 0xE9;
sfr MD1;//   = 0xEA;
sfr MD2;//   = 0xEB;
sfr MD3;//   = 0xEC;
sfr MD4;//   = 0xED;
sfr MD5;//   = 0xEE;
sfr ARCON;// = 0xEF;

sfr TXFFCTL;// = 0x9E;
#define TXFF_RST    0x80
#define TXFF_EN     0x40
#define DMA_EN      0x20

sfr RXFFCTL;// = 0x9C;
#define RXFF_RST    0x80
#define RXFF_EN     0x40

sfr TXFFSPACE;// = 0x9F;
#ifdef __cplusplus
}
#endif
#endif
