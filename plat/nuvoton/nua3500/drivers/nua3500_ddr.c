/*
 * Copyright (C) 2020 Nuvoton Technology Corp. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <assert.h>
#include <stdint.h>
#include <string.h>

#include <lib/mmio.h>

#include <plat/common/platform.h>
#include <platform_def.h>

#define RUN_PZ1


void nua3500_ddr_init(void)
{
	//Set TAHBCKEN,CM4CKEN,CA35CKEN,DDR6CKEN,GFXCKEN,VC8KCKEN,DCUCKEN,GMAC0CKEN,GMAC1CKEN,CAP0CKEN,CAP1CKEN
	outp32(CLK_BA + 0x04, (inp32(CLK_BA + 0x04) | 0x7F000037));
	outp32(CLK_BA + 0x0C, (inp32(CLK_BA + 0x0C) | 0x40000000));

	/* DDR control register clock gating disable */
	outp32(SYS_BA + 0x70, (inp32(SYS_BA + 0x70) | 0x00800000));
	/* de-assert presetn of MCTL2 */
	outp32(SYS_BA + 0x20, (inp32(SYS_BA + 0x20) & 0xafffffff));
	while((inp32(SYS_BA + 0x20) & 0x50000000) != 0x00000000);
	//set MCTLCRST to 1
	outp32(SYS_BA + 0x20, (inp32(SYS_BA + 0x20) | 0x20000000));


#ifdef RUN_PZ1
   //set DBG1
   outp32(UMCTL2_BA + 0x304,0x00000001);

   //set PWRCTL
   outp32(UMCTL2_BA + 0x030,0x00000000);  //add

   //set MSTR
   outp32(UMCTL2_BA + 0x000,0x03040001);

   //set MRCTRL0
   outp32(UMCTL2_BA + 0x010,0x0000d030);

   //set MRCTRL1
   outp32(UMCTL2_BA + 0x014,0x00000000);

   //set PWRTMG
   outp32(UMCTL2_BA + 0x034,0x00400010);  //mod 20200421

   //set HWLPCTL
   outp32(UMCTL2_BA + 0x038,0x000a0003);  //mod 20200121

   //set RFSHCTL0
   outp32(UMCTL2_BA + 0x050,0x00210000);

   //set RFSHCTL1
   outp32(UMCTL2_BA + 0x054,0x003c003c);

   //set RFSHCTL3
   outp32(UMCTL2_BA + 0x060,0x00000000);  //mod 20191211

   //set RFSHTMG
   outp32(UMCTL2_BA + 0x064,0x00410046);

   //set CRCPARCTL0
   outp32(UMCTL2_BA + 0x0c0,0x00000000);

   //set INIT0
   outp32(UMCTL2_BA + 0x0d0,0x00020083);  //mod 20200505

   //set INIT1
   outp32(UMCTL2_BA + 0x0d4,0x00350002);  //mod 20200505

   //set INIT3
   outp32(UMCTL2_BA + 0x0dc,0x1b400006);  //mod

   //set INIT4
   outp32(UMCTL2_BA + 0x0e0,0x00080000);

   //set INIT5
   outp32(UMCTL2_BA + 0x0e4,0x00090000);

   //set DIMMCTL
   outp32(UMCTL2_BA + 0x0f0,0x00000000);

   //set RANKCTL
   outp32(UMCTL2_BA + 0x0f4,0x0000032f);

   //set DRAMTMG0
   outp32(UMCTL2_BA + 0x100,0x090d130a);  //mod 20200103

   //set DRAMTMG1
   outp32(UMCTL2_BA + 0x104,0x0003020e);  //mod 20200103

   //set DRAMTMG2
   outp32(UMCTL2_BA + 0x108,0x00000408);  //mod 20200103

   //set DRAMTMG3
   outp32(UMCTL2_BA + 0x10c,0x00002006);

   //set DRAMTMG4
   outp32(UMCTL2_BA + 0x110,0x04020205);  //mod 20200103

   //set DRAMTMG5
   outp32(UMCTL2_BA + 0x114,0x03030202);  //mod 20200103

   //set DRAMTMG8
   outp32(UMCTL2_BA + 0x120,0x00000a04);

   //set DRAMTMG15
   outp32(UMCTL2_BA + 0x13c,0x80000032);  //mod 20200225

   //set ZQCTL0
   outp32(UMCTL2_BA + 0x180,0x00800020);

   //set ZQCTL1
   outp32(UMCTL2_BA + 0x184,0x00000100);  //mod 20200505

   //set DFITMG0
   outp32(UMCTL2_BA + 0x190,0x04020101);

   //set DFITMG1
   outp32(UMCTL2_BA + 0x194,0x00060101);

   //set DFILPCFG0
   outp32(UMCTL2_BA + 0x198,0x0700b030);  //mod 20200421

   //set DFIUPD0
   outp32(UMCTL2_BA + 0x1a0,0x00400005);  //mod 20200303

   //set DFIUPD1
   outp32(UMCTL2_BA + 0x1a4,0x00170066);  //mod 20200425

   //set DFIUPD2
   outp32(UMCTL2_BA + 0x1a8,0x80000000);  //mod 20200309

   //set DFIMISC
   outp32(UMCTL2_BA + 0x1b0,0x00000010);  //mod 20200104

   //set DFIPHYMSTR
   outp32(UMCTL2_BA + 0x1c4,0x00000000);

   //set ADDRMAP0
   outp32(UMCTL2_BA + 0x200,0x00000016);

   //set ADDRMAP1
   outp32(UMCTL2_BA + 0x204,0x00080808);

   //set ADDRMAP2
   outp32(UMCTL2_BA + 0x208,0x00000000);

   //set ADDRMAP3
   outp32(UMCTL2_BA + 0x20c,0x00000000);

   //set ADDRMAP4
   outp32(UMCTL2_BA + 0x210,0x00001f1f);

   //set ADDRMAP5
   outp32(UMCTL2_BA + 0x214,0x070f0707);

   //set ADDRMAP6
   outp32(UMCTL2_BA + 0x218,0x0f070707);

   //set ADDRMAP9
   outp32(UMCTL2_BA + 0x224,0x07070707);

   //set ADDRMAP10
   outp32(UMCTL2_BA + 0x228,0x07070707);

   //set ADDRMAP11
   outp32(UMCTL2_BA + 0x22c,0x00000007);

   //set ODTCFG
   outp32(UMCTL2_BA + 0x240,0x06000608);

   //set ODTMAP
   outp32(UMCTL2_BA + 0x244,0x00000303);  //mod

   //set SCHED
   outp32(UMCTL2_BA + 0x250,0x00f51f00);

   //set SCHED1
   outp32(UMCTL2_BA + 0x254,0x00000000);

   //set PERFHPR1
   outp32(UMCTL2_BA + 0x25c,0x0f000001);

   //set PERFLPR1
   outp32(UMCTL2_BA + 0x264,0x0f00007f);

   //set PERFWR1
   outp32(UMCTL2_BA + 0x26c,0x0f00007f);

   //set DBG0
   outp32(UMCTL2_BA + 0x300,0x00000000);

   //set DBG1
   outp32(UMCTL2_BA + 0x304,0x00000000);

   //set DBGCMD
   outp32(UMCTL2_BA + 0x30c,0x00000000);

   //set SWCTL
   outp32(UMCTL2_BA + 0x320,0x00000001);

   //set SWCTLSTATIC
   outp32(UMCTL2_BA + 0x328,0x00000000);

   //set POISONCFG
   outp32(UMCTL2_BA + 0x36c,0x00110011);

   //set PCTRL_0
   outp32(UMCTL2_BA + 0x490,0x00000001);

   //set PCTRL_1
   outp32(UMCTL2_BA + 0x540,0x00000000);  //mod 20200323

   //set PCTRL_2
   outp32(UMCTL2_BA + 0x5f0,0x00000000);  //mod 20200323

   //set PCTRL_3
   outp32(UMCTL2_BA + 0x6a0,0x00000000);  //mod 20200323

   //set PCTRL_4
   outp32(UMCTL2_BA + 0x750,0x00000000);  //mod 20200323

   //set PCTRL_5
   outp32(UMCTL2_BA + 0x800,0x00000000);  //mod 20200323

   //set PCTRL_6
   outp32(UMCTL2_BA + 0x8b0,0x00000001);

   //set PCTRL_7
   outp32(UMCTL2_BA + 0x960,0x00000001);

   //set PCCFG
   outp32(UMCTL2_BA + 0x400,0x00000000);

   //set PCFGR_0
   outp32(UMCTL2_BA + 0x404,0x0001500f);  //mod 20200408

   //set PCFGR_1
   outp32(UMCTL2_BA + 0x4b4,0x0001500f);  //mod 20200408

   //set PCFGR_2
   outp32(UMCTL2_BA + 0x564,0x0001500f);  //mod 20200408

   //set PCFGR_3
   outp32(UMCTL2_BA + 0x614,0x0001500f);  //mod 20200408

   //set PCFGR_4
   outp32(UMCTL2_BA + 0x6c4,0x0001500f);  //mod 20200408

   //set PCFGR_5
   outp32(UMCTL2_BA + 0x774,0x0001500f);  //mod 20200408

   //set PCFGR_6
   outp32(UMCTL2_BA + 0x824,0x0001500f);  //mod 20200408

   //set PCFGR_7
   outp32(UMCTL2_BA + 0x8d4,0x0001500f);  //mod 20200408

   //set PCFGW_0
   outp32(UMCTL2_BA + 0x408,0x0000500f);  //mod 20200408

   //set PCFGW_1
   outp32(UMCTL2_BA + 0x4b8,0x0000500f);  //mod 20200408

   //set PCFGW_2
   outp32(UMCTL2_BA + 0x568,0x0000500f);  //mod 20200408

   //set PCFGW_3
   outp32(UMCTL2_BA + 0x618,0x0000500f);  //mod 20200408

   //set PCFGW_4
   outp32(UMCTL2_BA + 0x6c8,0x0000500f);  //mod 20200408

   //set PCFGW_5
   outp32(UMCTL2_BA + 0x778,0x0000500f);  //mod 20200408

   //set PCFGW_6
   outp32(UMCTL2_BA + 0x828,0x0000500f);  //mod 20200408

   //set PCFGW_7
   outp32(UMCTL2_BA + 0x8d8,0x0000500f);  //mod 20200408

   //set SARBASE0
   outp32(UMCTL2_BA + 0xf04,0x00000008);

   //set SARSIZE0
   outp32(UMCTL2_BA + 0xf08,0x00000003);

   //de-assert reset signals of DDR memory controller
   outp32(SYS_BA+0x20,(inp32(SYS_BA+0x20) & 0x8fffffff));
   while( (inp32(SYS_BA+0x20) & 0x20000000) != 0x00000000);

   //=====================================================================
   //                  DDR PHY initialization
   //=====================================================================


   //---------------------------------------------------------------------
   //set SWCTL
   outp32(UMCTL2_BA + 0x320,0x00000000);

   //set DFIMISC
   outp32(UMCTL2_BA + 0x1b0,0x00000011);  //add 20200104

   //set SWCTL
   outp32(UMCTL2_BA + 0x320,0x00000001);

   //polling MCTL2 SWSTAT to 0x00000001
   while((inp32(UMCTL2_BA + 0x324) & 0x00000001) != 0x00000001);

   //polling MCTL2 STAT to 0x00000001, ddr umctl2 is normal state
   while((inp32(UMCTL2_BA + 0x004) & 0x00000003) != 0x00000001);

   //polling MCTL2 STAT to 0x00000001
   while((inp32(UMCTL2_BA + 0x004) & 0x00000003) != 0x00000001);

#else	// real chip

   //set DBG1
   outp32(UMCTL2_BA + 0x304,0x00000001);

   //set PWRCTL
   outp32(UMCTL2_BA + 0x030,0x00000000);  //add

   //set MSTR
   outp32(UMCTL2_BA + 0x000,0x01040001);  //mod 20200507

   //set MRCTRL0
   outp32(UMCTL2_BA + 0x010,0x0000d010);  //mod 20200507

   //set MRCTRL1
   outp32(UMCTL2_BA + 0x014,0x00000000);

   //set PWRTMG
   outp32(UMCTL2_BA + 0x034,0x00400010);  //mod 20200421

   //set HWLPCTL
   outp32(UMCTL2_BA + 0x038,0x000a0003);  //mod 20200121

   //set RFSHCTL0
   outp32(UMCTL2_BA + 0x050,0x00210000);

   //set RFSHCTL1
   outp32(UMCTL2_BA + 0x054,0x003c003c);

   //set RFSHCTL3
   outp32(UMCTL2_BA + 0x060,0x00000000);  //mod 20191211

   //set RFSHTMG
   outp32(UMCTL2_BA + 0x064,0x00410046);

   //set CRCPARCTL0
   outp32(UMCTL2_BA + 0x0c0,0x00000000);

   //set INIT0
   outp32(UMCTL2_BA + 0x0d0,0x40020083);  //mod 20200505

   //set INIT1
   outp32(UMCTL2_BA + 0x0d4,0x00350002);  //mod 20200505

   //set INIT3
   outp32(UMCTL2_BA + 0x0dc,0x1b400006);  //mod

   //set INIT4
   outp32(UMCTL2_BA + 0x0e0,0x00080000);

   //set INIT5
   outp32(UMCTL2_BA + 0x0e4,0x00090000);

   //set DIMMCTL
   outp32(UMCTL2_BA + 0x0f0,0x00000000);

   //set RANKCTL
   outp32(UMCTL2_BA + 0x0f4,0x0000032f);

   //set DRAMTMG0
   outp32(UMCTL2_BA + 0x100,0x090d130a);  //mod 20200103

   //set DRAMTMG1
   outp32(UMCTL2_BA + 0x104,0x0003020e);  //mod 20200103

   //set DRAMTMG2
   outp32(UMCTL2_BA + 0x108,0x00000408);  //mod 20200103

   //set DRAMTMG3
   outp32(UMCTL2_BA + 0x10c,0x00002006);

   //set DRAMTMG4
   outp32(UMCTL2_BA + 0x110,0x04020205);  //mod 20200103

   //set DRAMTMG5
   outp32(UMCTL2_BA + 0x114,0x03030202);  //mod 20200103

   //set DRAMTMG8
   outp32(UMCTL2_BA + 0x120,0x00000a04);

   //set DRAMTMG15
   outp32(UMCTL2_BA + 0x13c,0x80000032);  //mod 20200225

   //set ZQCTL0
   outp32(UMCTL2_BA + 0x180,0x00800020);

   //set ZQCTL1
   outp32(UMCTL2_BA + 0x184,0x00000100);  //mod 20200505

   //set DFITMG0
   outp32(UMCTL2_BA + 0x190,0x04020101);

   //set DFITMG1
   outp32(UMCTL2_BA + 0x194,0x00060101);

   //set DFILPCFG0
   outp32(UMCTL2_BA + 0x198,0x0700b131);  //mod 20200417

   //set DFIUPD0
   outp32(UMCTL2_BA + 0x1a0,0x00400005);  //mod 20200303

   //set DFIUPD1
   outp32(UMCTL2_BA + 0x1a4,0x00170066);  //mod 20200425

   //set DFIUPD2
   outp32(UMCTL2_BA + 0x1a8,0x80000000);  //mod 20200309

   //set DFIMISC
   outp32(UMCTL2_BA + 0x1b0,0x00000011);  //mod

   //set DFIPHYMSTR
   outp32(UMCTL2_BA + 0x1c4,0x00000000);

   //set ADDRMAP0
   outp32(UMCTL2_BA + 0x200,0x0000001f);  //mod 20200507

   //set ADDRMAP1
   outp32(UMCTL2_BA + 0x204,0x00080808);

   //set ADDRMAP2
   outp32(UMCTL2_BA + 0x208,0x00000000);

   //set ADDRMAP3
   outp32(UMCTL2_BA + 0x20c,0x00000000);

   //set ADDRMAP4
   outp32(UMCTL2_BA + 0x210,0x00001f1f);

   //set ADDRMAP5
   outp32(UMCTL2_BA + 0x214,0x070f0707);

   //set ADDRMAP6
   outp32(UMCTL2_BA + 0x218,0x0f070707);

   //set ADDRMAP9
   outp32(UMCTL2_BA + 0x224,0x07070707);

   //set ADDRMAP10
   outp32(UMCTL2_BA + 0x228,0x07070707);

   //set ADDRMAP11
   outp32(UMCTL2_BA + 0x22c,0x00000007);

   //set ODTCFG
   outp32(UMCTL2_BA + 0x240,0x06000608);

   //set ODTMAP
   outp32(UMCTL2_BA + 0x244,0x00000101);  //mod 20200508

   //set SCHED
   outp32(UMCTL2_BA + 0x250,0x00f51f00);

   //set SCHED1
   outp32(UMCTL2_BA + 0x254,0x00000000);

   //set PERFHPR1
   outp32(UMCTL2_BA + 0x25c,0x0f000001);

   //set PERFLPR1
   outp32(UMCTL2_BA + 0x264,0x0f00007f);

   //set PERFWR1
   outp32(UMCTL2_BA + 0x26c,0x0f00007f);

   //set DBG0
   outp32(UMCTL2_BA + 0x300,0x00000000);

   //set DBG1
   outp32(UMCTL2_BA + 0x304,0x00000000);

   //set DBGCMD
   outp32(UMCTL2_BA + 0x30c,0x00000000);

   //set SWCTL
   outp32(UMCTL2_BA + 0x320,0x00000001);

   //set SWCTLSTATIC
   outp32(UMCTL2_BA + 0x328,0x00000000);

   //set POISONCFG
   outp32(UMCTL2_BA + 0x36c,0x00110011);

   //set PCTRL_0
   outp32(UMCTL2_BA + 0x490,0x00000001);

   //set PCTRL_1
   outp32(UMCTL2_BA + 0x540,0x00000000);  //mod 20200323

   //set PCTRL_2
   outp32(UMCTL2_BA + 0x5f0,0x00000000);  //mod 20200323

   //set PCTRL_3
   outp32(UMCTL2_BA + 0x6a0,0x00000000);  //mod 20200323

   //set PCTRL_4
   outp32(UMCTL2_BA + 0x750,0x00000000);  //mod 20200323

   //set PCTRL_5
   outp32(UMCTL2_BA + 0x800,0x00000000);  //mod 20200323

   //set PCTRL_6
   outp32(UMCTL2_BA + 0x8b0,0x00000001);

   //set PCTRL_7
   outp32(UMCTL2_BA + 0x960,0x00000001);

   //set PCCFG
   outp32(UMCTL2_BA + 0x400,0x00000000);

   //set PCFGR_0
   outp32(UMCTL2_BA + 0x404,0x0001500f);  //mod 20200408

   //set PCFGR_1
   outp32(UMCTL2_BA + 0x4b4,0x0001500f);  //mod 20200408

   //set PCFGR_2
   outp32(UMCTL2_BA + 0x564,0x0001500f);  //mod 20200408

   //set PCFGR_3
   outp32(UMCTL2_BA + 0x614,0x0001500f);  //mod 20200408

   //set PCFGR_4
   outp32(UMCTL2_BA + 0x6c4,0x0001500f);  //mod 20200408

   //set PCFGR_5
   outp32(UMCTL2_BA + 0x774,0x0001500f);  //mod 20200408

   //set PCFGR_6
   outp32(UMCTL2_BA + 0x824,0x0001500f);  //mod 20200408

   //set PCFGR_7
   outp32(UMCTL2_BA + 0x8d4,0x0001500f);  //mod 20200408

   //set PCFGW_0
   outp32(UMCTL2_BA + 0x408,0x0000500f);  //mod 20200408

   //set PCFGW_1
   outp32(UMCTL2_BA + 0x4b8,0x0000500f);  //mod 20200408

   //set PCFGW_2
   outp32(UMCTL2_BA + 0x568,0x0000500f);  //mod 20200408

   //set PCFGW_3
   outp32(UMCTL2_BA + 0x618,0x0000500f);  //mod 20200408

   //set PCFGW_4
   outp32(UMCTL2_BA + 0x6c8,0x0000500f);  //mod 20200408

   //set PCFGW_5
   outp32(UMCTL2_BA + 0x778,0x0000500f);  //mod 20200408

   //set PCFGW_6
   outp32(UMCTL2_BA + 0x828,0x0000500f);  //mod 20200408

   //set PCFGW_7
   outp32(UMCTL2_BA + 0x8d8,0x0000500f);  //mod 20200408

   //set SARBASE0
   outp32(UMCTL2_BA + 0xf04,0x00000008);

   //set SARSIZE0
   outp32(UMCTL2_BA + 0xf08,0x00000001);  //mod 20200507

   //de-assert reset signals of DDR memory controller
   outp32(SYS_BA+0x20,(inp32(SYS_BA+0x20) & 0x8fffffff));
   while( (inp32(SYS_BA+0x20) & 0x20000000) != 0x00000000);

   //=====================================================================
   //                  DDR PHY initialization
   //=====================================================================

   //set DSGCR,addr=16
   outp32(DDRPHY_BA + 0x040,0xf004649f);

   //set PGCR1,addr=3
   outp32(DDRPHY_BA + 0x00c,0x0200c461);

   //set PGCR2,addr=35
   outp32(DDRPHY_BA + 0x08c,0x00f00eae);  //mod 20200304

   //set PTR0,addr=7
   outp32(DDRPHY_BA + 0x01c,0x0c806403);  //mod 20200306 for pclk=100mhz

   //set PTR1,addr=8
   outp32(DDRPHY_BA + 0x020,0x27100385);  //mod 20200306 for pclk=100mhz

   //set PTR2,addr=9
   outp32(DDRPHY_BA + 0x024,0x00083def);

   //set PTR3,addr=10
   outp32(DDRPHY_BA + 0x028,0x0904111d);  //mod 20200504

   //set PTR4,addr=11
   outp32(DDRPHY_BA + 0x02c,0x0801a072);  //mod 20200508

   //set MR0,addr=21
   outp32(DDRPHY_BA + 0x054,0x00001b40);  //mod

   //set MR1,addr=22
   outp32(DDRPHY_BA + 0x058,0x00000006);

   //set MR2,addr=23
   outp32(DDRPHY_BA + 0x05c,0x00000008);

   //set MR3,addr=24
   outp32(DDRPHY_BA + 0x060,0x00000000);

   //set DTPR0,addr=18
   outp32(DDRPHY_BA + 0x048,0x71568855);

   //set DTPR1,addr=19
   outp32(DDRPHY_BA + 0x04c,0x16846320);

   //set DTPR2,addr=20
   outp32(DDRPHY_BA + 0x050,0x30023e00);

   //polling PGSR0 (addr=4) to 0x0000000f
   while((inp32(DDRPHY_BA + 0x010) & 0x0000000f) != 0x0000000f);

   //set DCR,addr=17
   outp32(DDRPHY_BA + 0x044,0x0000040b);

   //set DTCR,addr=26
   outp32(DDRPHY_BA + 0x068,0x910035c7);  //mod 20200507

   //set PIR,addr=1
   outp32(DDRPHY_BA + 0x004,0x0000ff81);

   //polling PGSR0 (addr=4) to 0x00000fff
   while((inp32(DDRPHY_BA + 0x010) & 0x00000fff) != 0x00000fff);

   //polling MCTL2 STAT to 0x00000001
   while((inp32(UMCTL2_BA + 0x004) & 0x00000003) != 0x00000001);

#endif

	outp32((void *)UMCTL2_BA+0x490, 0x1);
	outp32((void *)UMCTL2_BA+0x8b0, 0x1);
	outp32((void *)UMCTL2_BA+0x960, 0x1);

	outp32((void *)UMCTL2_BA+0x540, 0x1);
	outp32((void *)UMCTL2_BA+0x5f0, 0x1);
	outp32((void *)UMCTL2_BA+0x6a0, 0x1);
	outp32((void *)UMCTL2_BA+0x750, 0x1);
	outp32((void *)UMCTL2_BA+0x800, 0x1);

	outp32(SYS_BA + 0x70,(inp32(SYS_BA + 0x70) & ~0x00800000));	/* DDR control register clock gating enable */
	outp32(CLK_BA + 0x04, 0x35);

}

