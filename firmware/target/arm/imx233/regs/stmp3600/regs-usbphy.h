/***************************************************************************
 *             __________               __   ___.
 *   Open      \______   \ ____   ____ |  | _\_ |__   _______  ___
 *   Source     |       _//  _ \_/ ___\|  |/ /| __ \ /  _ \  \/  /
 *   Jukebox    |    |   (  <_> )  \___|    < | \_\ (  <_> > <  <
 *   Firmware   |____|_  /\____/ \___  >__|_ \|___  /\____/__/\_ \
 *                     \/            \/     \/    \/            \/
 * This file was automatically generated by headergen, DO NOT EDIT it.
 * headergen version: 2.1.8
 * XML versions: stmp3600:2.3.0
 *
 * Copyright (C) 2013 by Amaury Pouly
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY
 * KIND, either express or implied.
 *
 ****************************************************************************/
#ifndef __HEADERGEN__STMP3600__USBPHY__H__
#define __HEADERGEN__STMP3600__USBPHY__H__

#define REGS_USBPHY_BASE (0x8007c000)

#define REGS_USBPHY_VERSION "2.3.0"

/**
 * Register: HW_USBPHY_PWD
 * Address: 0
 * SCT: yes
*/
#define HW_USBPHY_PWD               (*(volatile unsigned long *)(REGS_USBPHY_BASE + 0x0 + 0x0))
#define HW_USBPHY_PWD_SET           (*(volatile unsigned long *)(REGS_USBPHY_BASE + 0x0 + 0x4))
#define HW_USBPHY_PWD_CLR           (*(volatile unsigned long *)(REGS_USBPHY_BASE + 0x0 + 0x8))
#define HW_USBPHY_PWD_TOG           (*(volatile unsigned long *)(REGS_USBPHY_BASE + 0x0 + 0xc))
#define BP_USBPHY_PWD_RXPWDRX       20
#define BM_USBPHY_PWD_RXPWDRX       0x100000
#define BF_USBPHY_PWD_RXPWDRX(v)    (((v) << 20) & 0x100000)
#define BP_USBPHY_PWD_RXPWDDIFF     19
#define BM_USBPHY_PWD_RXPWDDIFF     0x80000
#define BF_USBPHY_PWD_RXPWDDIFF(v)  (((v) << 19) & 0x80000)
#define BP_USBPHY_PWD_RXPWD1PT1     18
#define BM_USBPHY_PWD_RXPWD1PT1     0x40000
#define BF_USBPHY_PWD_RXPWD1PT1(v)  (((v) << 18) & 0x40000)
#define BP_USBPHY_PWD_RXPWDENV      17
#define BM_USBPHY_PWD_RXPWDENV      0x20000
#define BF_USBPHY_PWD_RXPWDENV(v)   (((v) << 17) & 0x20000)
#define BP_USBPHY_PWD_TXPWDCOMP     14
#define BM_USBPHY_PWD_TXPWDCOMP     0x4000
#define BF_USBPHY_PWD_TXPWDCOMP(v)  (((v) << 14) & 0x4000)
#define BP_USBPHY_PWD_TXPWDVBG      13
#define BM_USBPHY_PWD_TXPWDVBG      0x2000
#define BF_USBPHY_PWD_TXPWDVBG(v)   (((v) << 13) & 0x2000)
#define BP_USBPHY_PWD_TXPWDV2I      12
#define BM_USBPHY_PWD_TXPWDV2I      0x1000
#define BF_USBPHY_PWD_TXPWDV2I(v)   (((v) << 12) & 0x1000)
#define BP_USBPHY_PWD_TXPWDIBIAS    11
#define BM_USBPHY_PWD_TXPWDIBIAS    0x800
#define BF_USBPHY_PWD_TXPWDIBIAS(v) (((v) << 11) & 0x800)
#define BP_USBPHY_PWD_TXPWDFS       10
#define BM_USBPHY_PWD_TXPWDFS       0x400
#define BF_USBPHY_PWD_TXPWDFS(v)    (((v) << 10) & 0x400)

/**
 * Register: HW_USBPHY_TX
 * Address: 0x10
 * SCT: yes
*/
#define HW_USBPHY_TX                    (*(volatile unsigned long *)(REGS_USBPHY_BASE + 0x10 + 0x0))
#define HW_USBPHY_TX_SET                (*(volatile unsigned long *)(REGS_USBPHY_BASE + 0x10 + 0x4))
#define HW_USBPHY_TX_CLR                (*(volatile unsigned long *)(REGS_USBPHY_BASE + 0x10 + 0x8))
#define HW_USBPHY_TX_TOG                (*(volatile unsigned long *)(REGS_USBPHY_BASE + 0x10 + 0xc))
#define BP_USBPHY_TX_TXCMPOUT_STATUS    23
#define BM_USBPHY_TX_TXCMPOUT_STATUS    0x800000
#define BF_USBPHY_TX_TXCMPOUT_STATUS(v) (((v) << 23) & 0x800000)
#define BP_USBPHY_TX_TXENCAL45DP        21
#define BM_USBPHY_TX_TXENCAL45DP        0x200000
#define BF_USBPHY_TX_TXENCAL45DP(v)     (((v) << 21) & 0x200000)
#define BP_USBPHY_TX_TXCAL45DP          16
#define BM_USBPHY_TX_TXCAL45DP          0x1f0000
#define BF_USBPHY_TX_TXCAL45DP(v)       (((v) << 16) & 0x1f0000)
#define BP_USBPHY_TX_TXENCAL45DN        13
#define BM_USBPHY_TX_TXENCAL45DN        0x2000
#define BF_USBPHY_TX_TXENCAL45DN(v)     (((v) << 13) & 0x2000)
#define BP_USBPHY_TX_TXCAL45DN          8
#define BM_USBPHY_TX_TXCAL45DN          0x1f00
#define BF_USBPHY_TX_TXCAL45DN(v)       (((v) << 8) & 0x1f00)
#define BP_USBPHY_TX_TXCALIBRATE        7
#define BM_USBPHY_TX_TXCALIBRATE        0x80
#define BF_USBPHY_TX_TXCALIBRATE(v)     (((v) << 7) & 0x80)

/**
 * Register: HW_USBPHY_RX
 * Address: 0x20
 * SCT: yes
*/
#define HW_USBPHY_RX                (*(volatile unsigned long *)(REGS_USBPHY_BASE + 0x20 + 0x0))
#define HW_USBPHY_RX_SET            (*(volatile unsigned long *)(REGS_USBPHY_BASE + 0x20 + 0x4))
#define HW_USBPHY_RX_CLR            (*(volatile unsigned long *)(REGS_USBPHY_BASE + 0x20 + 0x8))
#define HW_USBPHY_RX_TOG            (*(volatile unsigned long *)(REGS_USBPHY_BASE + 0x20 + 0xc))
#define BP_USBPHY_RX_RXDBYPASS      22
#define BM_USBPHY_RX_RXDBYPASS      0x400000
#define BF_USBPHY_RX_RXDBYPASS(v)   (((v) << 22) & 0x400000)
#define BP_USBPHY_RX_DISCONADJ      4
#define BM_USBPHY_RX_DISCONADJ      0x30
#define BF_USBPHY_RX_DISCONADJ(v)   (((v) << 4) & 0x30)
#define BP_USBPHY_RX_ENVADJ         0
#define BM_USBPHY_RX_ENVADJ         0x3
#define BF_USBPHY_RX_ENVADJ(v)      (((v) << 0) & 0x3)

/**
 * Register: HW_USBPHY_CTRL
 * Address: 0x30
 * SCT: yes
*/
#define HW_USBPHY_CTRL                          (*(volatile unsigned long *)(REGS_USBPHY_BASE + 0x30 + 0x0))
#define HW_USBPHY_CTRL_SET                      (*(volatile unsigned long *)(REGS_USBPHY_BASE + 0x30 + 0x4))
#define HW_USBPHY_CTRL_CLR                      (*(volatile unsigned long *)(REGS_USBPHY_BASE + 0x30 + 0x8))
#define HW_USBPHY_CTRL_TOG                      (*(volatile unsigned long *)(REGS_USBPHY_BASE + 0x30 + 0xc))
#define BP_USBPHY_CTRL_SFTRST                   31
#define BM_USBPHY_CTRL_SFTRST                   0x80000000
#define BF_USBPHY_CTRL_SFTRST(v)                (((v) << 31) & 0x80000000)
#define BP_USBPHY_CTRL_CLKGATE                  30
#define BM_USBPHY_CTRL_CLKGATE                  0x40000000
#define BF_USBPHY_CTRL_CLKGATE(v)               (((v) << 30) & 0x40000000)
#define BP_USBPHY_CTRL_UTMI_SUSPENDM            29
#define BM_USBPHY_CTRL_UTMI_SUSPENDM            0x20000000
#define BF_USBPHY_CTRL_UTMI_SUSPENDM(v)         (((v) << 29) & 0x20000000)
#define BP_USBPHY_CTRL_RESUME_IRQ               10
#define BM_USBPHY_CTRL_RESUME_IRQ               0x400
#define BF_USBPHY_CTRL_RESUME_IRQ(v)            (((v) << 10) & 0x400)
#define BP_USBPHY_CTRL_ENIRQRESUMEDETECT        9
#define BM_USBPHY_CTRL_ENIRQRESUMEDETECT        0x200
#define BF_USBPHY_CTRL_ENIRQRESUMEDETECT(v)     (((v) << 9) & 0x200)
#define BP_USBPHY_CTRL_ENOTGIDDETECT            7
#define BM_USBPHY_CTRL_ENOTGIDDETECT            0x80
#define BF_USBPHY_CTRL_ENOTGIDDETECT(v)         (((v) << 7) & 0x80)
#define BP_USBPHY_CTRL_ENDEVPLUGINDETECT        4
#define BM_USBPHY_CTRL_ENDEVPLUGINDETECT        0x10
#define BF_USBPHY_CTRL_ENDEVPLUGINDETECT(v)     (((v) << 4) & 0x10)
#define BP_USBPHY_CTRL_HOSTDISCONDETECT_IRQ     3
#define BM_USBPHY_CTRL_HOSTDISCONDETECT_IRQ     0x8
#define BF_USBPHY_CTRL_HOSTDISCONDETECT_IRQ(v)  (((v) << 3) & 0x8)
#define BP_USBPHY_CTRL_ENIRQHOSTDISCON          2
#define BM_USBPHY_CTRL_ENIRQHOSTDISCON          0x4
#define BF_USBPHY_CTRL_ENIRQHOSTDISCON(v)       (((v) << 2) & 0x4)
#define BP_USBPHY_CTRL_ENHOSTDISCONDETECT       1
#define BM_USBPHY_CTRL_ENHOSTDISCONDETECT       0x2
#define BF_USBPHY_CTRL_ENHOSTDISCONDETECT(v)    (((v) << 1) & 0x2)
#define BP_USBPHY_CTRL_ENHSPRECHARGEXMIT        0
#define BM_USBPHY_CTRL_ENHSPRECHARGEXMIT        0x1
#define BF_USBPHY_CTRL_ENHSPRECHARGEXMIT(v)     (((v) << 0) & 0x1)

/**
 * Register: HW_USBPHY_STATUS
 * Address: 0x40
 * SCT: no
*/
#define HW_USBPHY_STATUS                            (*(volatile unsigned long *)(REGS_USBPHY_BASE + 0x40))
#define BP_USBPHY_STATUS_RESUME_STATUS              10
#define BM_USBPHY_STATUS_RESUME_STATUS              0x400
#define BF_USBPHY_STATUS_RESUME_STATUS(v)           (((v) << 10) & 0x400)
#define BP_USBPHY_STATUS_OTGID_STATUS               8
#define BM_USBPHY_STATUS_OTGID_STATUS               0x100
#define BF_USBPHY_STATUS_OTGID_STATUS(v)            (((v) << 8) & 0x100)
#define BP_USBPHY_STATUS_DEVPLUGIN_STATUS           6
#define BM_USBPHY_STATUS_DEVPLUGIN_STATUS           0x40
#define BF_USBPHY_STATUS_DEVPLUGIN_STATUS(v)        (((v) << 6) & 0x40)
#define BP_USBPHY_STATUS_HOSTDISCONDETECT_STATUS    3
#define BM_USBPHY_STATUS_HOSTDISCONDETECT_STATUS    0x8
#define BF_USBPHY_STATUS_HOSTDISCONDETECT_STATUS(v) (((v) << 3) & 0x8)

/**
 * Register: HW_USBPHY_DEBUG
 * Address: 0x50
 * SCT: yes
*/
#define HW_USBPHY_DEBUG                         (*(volatile unsigned long *)(REGS_USBPHY_BASE + 0x50 + 0x0))
#define HW_USBPHY_DEBUG_SET                     (*(volatile unsigned long *)(REGS_USBPHY_BASE + 0x50 + 0x4))
#define HW_USBPHY_DEBUG_CLR                     (*(volatile unsigned long *)(REGS_USBPHY_BASE + 0x50 + 0x8))
#define HW_USBPHY_DEBUG_TOG                     (*(volatile unsigned long *)(REGS_USBPHY_BASE + 0x50 + 0xc))
#define BP_USBPHY_DEBUG_CLKGATE                 30
#define BM_USBPHY_DEBUG_CLKGATE                 0x40000000
#define BF_USBPHY_DEBUG_CLKGATE(v)              (((v) << 30) & 0x40000000)
#define BP_USBPHY_DEBUG_SQUELCHRESETLENGTH      25
#define BM_USBPHY_DEBUG_SQUELCHRESETLENGTH      0x1e000000
#define BF_USBPHY_DEBUG_SQUELCHRESETLENGTH(v)   (((v) << 25) & 0x1e000000)
#define BP_USBPHY_DEBUG_ENSQUELCHRESET          24
#define BM_USBPHY_DEBUG_ENSQUELCHRESET          0x1000000
#define BF_USBPHY_DEBUG_ENSQUELCHRESET(v)       (((v) << 24) & 0x1000000)
#define BP_USBPHY_DEBUG_SQUELCHRESETCOUNT       16
#define BM_USBPHY_DEBUG_SQUELCHRESETCOUNT       0x1f0000
#define BF_USBPHY_DEBUG_SQUELCHRESETCOUNT(v)    (((v) << 16) & 0x1f0000)
#define BP_USBPHY_DEBUG_ENTX2RXCOUNT            12
#define BM_USBPHY_DEBUG_ENTX2RXCOUNT            0x1000
#define BF_USBPHY_DEBUG_ENTX2RXCOUNT(v)         (((v) << 12) & 0x1000)
#define BP_USBPHY_DEBUG_TX2RXCOUNT              8
#define BM_USBPHY_DEBUG_TX2RXCOUNT              0xf00
#define BF_USBPHY_DEBUG_TX2RXCOUNT(v)           (((v) << 8) & 0xf00)
#define BP_USBPHY_DEBUG_ENHSTPULLDOWN           4
#define BM_USBPHY_DEBUG_ENHSTPULLDOWN           0x30
#define BF_USBPHY_DEBUG_ENHSTPULLDOWN(v)        (((v) << 4) & 0x30)
#define BP_USBPHY_DEBUG_HSTPULLDOWN             2
#define BM_USBPHY_DEBUG_HSTPULLDOWN             0xc
#define BF_USBPHY_DEBUG_HSTPULLDOWN(v)          (((v) << 2) & 0xc)
#define BP_USBPHY_DEBUG_DEBUG_INTERFACE_HOLD    1
#define BM_USBPHY_DEBUG_DEBUG_INTERFACE_HOLD    0x2
#define BF_USBPHY_DEBUG_DEBUG_INTERFACE_HOLD(v) (((v) << 1) & 0x2)
#define BP_USBPHY_DEBUG_OTGIDPIOLOCK            0
#define BM_USBPHY_DEBUG_OTGIDPIOLOCK            0x1
#define BF_USBPHY_DEBUG_OTGIDPIOLOCK(v)         (((v) << 0) & 0x1)

/**
 * Register: HW_USBPHY_DEBUG0_STATUS
 * Address: 0x60
 * SCT: no
*/
#define HW_USBPHY_DEBUG0_STATUS                             (*(volatile unsigned long *)(REGS_USBPHY_BASE + 0x60))
#define BP_USBPHY_DEBUG0_STATUS_SQUELCH_COUNT               26
#define BM_USBPHY_DEBUG0_STATUS_SQUELCH_COUNT               0xfc000000
#define BF_USBPHY_DEBUG0_STATUS_SQUELCH_COUNT(v)            (((v) << 26) & 0xfc000000)
#define BP_USBPHY_DEBUG0_STATUS_UTMI_RXERROR_FAIL_COUNT     16
#define BM_USBPHY_DEBUG0_STATUS_UTMI_RXERROR_FAIL_COUNT     0x3ff0000
#define BF_USBPHY_DEBUG0_STATUS_UTMI_RXERROR_FAIL_COUNT(v)  (((v) << 16) & 0x3ff0000)
#define BP_USBPHY_DEBUG0_STATUS_LOOP_BACK_FAIL_COUNT        0
#define BM_USBPHY_DEBUG0_STATUS_LOOP_BACK_FAIL_COUNT        0xffff
#define BF_USBPHY_DEBUG0_STATUS_LOOP_BACK_FAIL_COUNT(v)     (((v) << 0) & 0xffff)

/**
 * Register: HW_USBPHY_DEBUG1_STATUS
 * Address: 0x70
 * SCT: no
*/
#define HW_USBPHY_DEBUG1_STATUS                 (*(volatile unsigned long *)(REGS_USBPHY_BASE + 0x70))
#define BP_USBPHY_DEBUG1_STATUS_UTMI_TX_DATA    16
#define BM_USBPHY_DEBUG1_STATUS_UTMI_TX_DATA    0xffff0000
#define BF_USBPHY_DEBUG1_STATUS_UTMI_TX_DATA(v) (((v) << 16) & 0xffff0000)
#define BP_USBPHY_DEBUG1_STATUS_UTMI_RX_DATA    0
#define BM_USBPHY_DEBUG1_STATUS_UTMI_RX_DATA    0xffff
#define BF_USBPHY_DEBUG1_STATUS_UTMI_RX_DATA(v) (((v) << 0) & 0xffff)

/**
 * Register: HW_USBPHY_DEBUG2_STATUS
 * Address: 0x80
 * SCT: no
*/
#define HW_USBPHY_DEBUG2_STATUS                     (*(volatile unsigned long *)(REGS_USBPHY_BASE + 0x80))
#define BP_USBPHY_DEBUG2_STATUS_UTMI_TXVALIDH       22
#define BM_USBPHY_DEBUG2_STATUS_UTMI_TXVALIDH       0x400000
#define BF_USBPHY_DEBUG2_STATUS_UTMI_TXVALIDH(v)    (((v) << 22) & 0x400000)
#define BP_USBPHY_DEBUG2_STATUS_UTMI_TXVALID        21
#define BM_USBPHY_DEBUG2_STATUS_UTMI_TXVALID        0x200000
#define BF_USBPHY_DEBUG2_STATUS_UTMI_TXVALID(v)     (((v) << 21) & 0x200000)
#define BP_USBPHY_DEBUG2_STATUS_UTMI_TERMSELECT     20
#define BM_USBPHY_DEBUG2_STATUS_UTMI_TERMSELECT     0x100000
#define BF_USBPHY_DEBUG2_STATUS_UTMI_TERMSELECT(v)  (((v) << 20) & 0x100000)
#define BP_USBPHY_DEBUG2_STATUS_UTMI_XCVRSELECT     18
#define BM_USBPHY_DEBUG2_STATUS_UTMI_XCVRSELECT     0xc0000
#define BF_USBPHY_DEBUG2_STATUS_UTMI_XCVRSELECT(v)  (((v) << 18) & 0xc0000)
#define BP_USBPHY_DEBUG2_STATUS_UTMI_OPMODE         16
#define BM_USBPHY_DEBUG2_STATUS_UTMI_OPMODE         0x30000
#define BF_USBPHY_DEBUG2_STATUS_UTMI_OPMODE(v)      (((v) << 16) & 0x30000)
#define BP_USBPHY_DEBUG2_STATUS_UTMI_LINESTATE      6
#define BM_USBPHY_DEBUG2_STATUS_UTMI_LINESTATE      0xc0
#define BF_USBPHY_DEBUG2_STATUS_UTMI_LINESTATE(v)   (((v) << 6) & 0xc0)
#define BP_USBPHY_DEBUG2_STATUS_UTMI_SUSPENDM       5
#define BM_USBPHY_DEBUG2_STATUS_UTMI_SUSPENDM       0x20
#define BF_USBPHY_DEBUG2_STATUS_UTMI_SUSPENDM(v)    (((v) << 5) & 0x20)
#define BP_USBPHY_DEBUG2_STATUS_UTMI_RXVALIDH       4
#define BM_USBPHY_DEBUG2_STATUS_UTMI_RXVALIDH       0x10
#define BF_USBPHY_DEBUG2_STATUS_UTMI_RXVALIDH(v)    (((v) << 4) & 0x10)
#define BP_USBPHY_DEBUG2_STATUS_UTMI_RXVALID        3
#define BM_USBPHY_DEBUG2_STATUS_UTMI_RXVALID        0x8
#define BF_USBPHY_DEBUG2_STATUS_UTMI_RXVALID(v)     (((v) << 3) & 0x8)
#define BP_USBPHY_DEBUG2_STATUS_UTMI_RXACTIVE       2
#define BM_USBPHY_DEBUG2_STATUS_UTMI_RXACTIVE       0x4
#define BF_USBPHY_DEBUG2_STATUS_UTMI_RXACTIVE(v)    (((v) << 2) & 0x4)
#define BP_USBPHY_DEBUG2_STATUS_UTMI_RXERROR        1
#define BM_USBPHY_DEBUG2_STATUS_UTMI_RXERROR        0x2
#define BF_USBPHY_DEBUG2_STATUS_UTMI_RXERROR(v)     (((v) << 1) & 0x2)
#define BP_USBPHY_DEBUG2_STATUS_UTMI_TXREADY        0
#define BM_USBPHY_DEBUG2_STATUS_UTMI_TXREADY        0x1
#define BF_USBPHY_DEBUG2_STATUS_UTMI_TXREADY(v)     (((v) << 0) & 0x1)

/**
 * Register: HW_USBPHY_DEBUG3_STATUS
 * Address: 0x90
 * SCT: no
*/
#define HW_USBPHY_DEBUG3_STATUS                     (*(volatile unsigned long *)(REGS_USBPHY_BASE + 0x90))
#define BP_USBPHY_DEBUG3_STATUS_B_CNT_FSM           28
#define BM_USBPHY_DEBUG3_STATUS_B_CNT_FSM           0x70000000
#define BF_USBPHY_DEBUG3_STATUS_B_CNT_FSM(v)        (((v) << 28) & 0x70000000)
#define BP_USBPHY_DEBUG3_STATUS_SQ_UNLOCK_FSM       23
#define BM_USBPHY_DEBUG3_STATUS_SQ_UNLOCK_FSM       0x3800000
#define BF_USBPHY_DEBUG3_STATUS_SQ_UNLOCK_FSM(v)    (((v) << 23) & 0x3800000)
#define BP_USBPHY_DEBUG3_STATUS_BIT_CNT             12
#define BM_USBPHY_DEBUG3_STATUS_BIT_CNT             0x3ff000
#define BF_USBPHY_DEBUG3_STATUS_BIT_CNT(v)          (((v) << 12) & 0x3ff000)
#define BP_USBPHY_DEBUG3_STATUS_MAIN_HS_RX_FSM      8
#define BM_USBPHY_DEBUG3_STATUS_MAIN_HS_RX_FSM      0xf00
#define BF_USBPHY_DEBUG3_STATUS_MAIN_HS_RX_FSM(v)   (((v) << 8) & 0xf00)
#define BP_USBPHY_DEBUG3_STATUS_UNSTUFF_BIT_CNT     0
#define BM_USBPHY_DEBUG3_STATUS_UNSTUFF_BIT_CNT     0xff
#define BF_USBPHY_DEBUG3_STATUS_UNSTUFF_BIT_CNT(v)  (((v) << 0) & 0xff)

/**
 * Register: HW_USBPHY_DEBUG4_STATUS
 * Address: 0xa0
 * SCT: no
*/
#define HW_USBPHY_DEBUG4_STATUS             (*(volatile unsigned long *)(REGS_USBPHY_BASE + 0xa0))
#define BP_USBPHY_DEBUG4_STATUS_BYTE_FSM    16
#define BM_USBPHY_DEBUG4_STATUS_BYTE_FSM    0x1fff0000
#define BF_USBPHY_DEBUG4_STATUS_BYTE_FSM(v) (((v) << 16) & 0x1fff0000)
#define BP_USBPHY_DEBUG4_STATUS_SND_FSM     0
#define BM_USBPHY_DEBUG4_STATUS_SND_FSM     0x3fff
#define BF_USBPHY_DEBUG4_STATUS_SND_FSM(v)  (((v) << 0) & 0x3fff)

/**
 * Register: HW_USBPHY_DEBUG5_STATUS
 * Address: 0xb0
 * SCT: no
*/
#define HW_USBPHY_DEBUG5_STATUS                     (*(volatile unsigned long *)(REGS_USBPHY_BASE + 0xb0))
#define BP_USBPHY_DEBUG5_STATUS_MAIN_FSM            24
#define BM_USBPHY_DEBUG5_STATUS_MAIN_FSM            0xf000000
#define BF_USBPHY_DEBUG5_STATUS_MAIN_FSM(v)         (((v) << 24) & 0xf000000)
#define BP_USBPHY_DEBUG5_STATUS_SYNC_FSM            16
#define BM_USBPHY_DEBUG5_STATUS_SYNC_FSM            0x3f0000
#define BF_USBPHY_DEBUG5_STATUS_SYNC_FSM(v)         (((v) << 16) & 0x3f0000)
#define BP_USBPHY_DEBUG5_STATUS_PRECHARGE_FSM       12
#define BM_USBPHY_DEBUG5_STATUS_PRECHARGE_FSM       0x7000
#define BF_USBPHY_DEBUG5_STATUS_PRECHARGE_FSM(v)    (((v) << 12) & 0x7000)
#define BP_USBPHY_DEBUG5_STATUS_SHIFT_FSM           8
#define BM_USBPHY_DEBUG5_STATUS_SHIFT_FSM           0x700
#define BF_USBPHY_DEBUG5_STATUS_SHIFT_FSM(v)        (((v) << 8) & 0x700)
#define BP_USBPHY_DEBUG5_STATUS_SOF_FSM             0
#define BM_USBPHY_DEBUG5_STATUS_SOF_FSM             0x1f
#define BF_USBPHY_DEBUG5_STATUS_SOF_FSM(v)          (((v) << 0) & 0x1f)

/**
 * Register: HW_USBPHY_DEBUG6_STATUS
 * Address: 0xc0
 * SCT: no
*/
#define HW_USBPHY_DEBUG6_STATUS                     (*(volatile unsigned long *)(REGS_USBPHY_BASE + 0xc0))
#define BP_USBPHY_DEBUG6_STATUS_FIRST_EOP_FSM       8
#define BM_USBPHY_DEBUG6_STATUS_FIRST_EOP_FSM       0x700
#define BF_USBPHY_DEBUG6_STATUS_FIRST_EOP_FSM(v)    (((v) << 8) & 0x700)
#define BP_USBPHY_DEBUG6_STATUS_EOP_FSM             0
#define BM_USBPHY_DEBUG6_STATUS_EOP_FSM             0xff
#define BF_USBPHY_DEBUG6_STATUS_EOP_FSM(v)          (((v) << 0) & 0xff)

/**
 * Register: HW_USBPHY_DEBUG7_STATUS
 * Address: 0xd0
 * SCT: no
*/
#define HW_USBPHY_DEBUG7_STATUS                     (*(volatile unsigned long *)(REGS_USBPHY_BASE + 0xd0))
#define BP_USBPHY_DEBUG7_STATUS_FIRST_DATA_FSM      28
#define BM_USBPHY_DEBUG7_STATUS_FIRST_DATA_FSM      0x30000000
#define BF_USBPHY_DEBUG7_STATUS_FIRST_DATA_FSM(v)   (((v) << 28) & 0x30000000)
#define BP_USBPHY_DEBUG7_STATUS_BIT_CNT             24
#define BM_USBPHY_DEBUG7_STATUS_BIT_CNT             0xf000000
#define BF_USBPHY_DEBUG7_STATUS_BIT_CNT(v)          (((v) << 24) & 0xf000000)
#define BP_USBPHY_DEBUG7_STATUS_UNSTUFF_CNT         20
#define BM_USBPHY_DEBUG7_STATUS_UNSTUFF_CNT         0x700000
#define BF_USBPHY_DEBUG7_STATUS_UNSTUFF_CNT(v)      (((v) << 20) & 0x700000)
#define BP_USBPHY_DEBUG7_STATUS_LD_FSM              16
#define BM_USBPHY_DEBUG7_STATUS_LD_FSM              0x30000
#define BF_USBPHY_DEBUG7_STATUS_LD_FSM(v)           (((v) << 16) & 0x30000)
#define BP_USBPHY_DEBUG7_STATUS_FIFO_FSM            8
#define BM_USBPHY_DEBUG7_STATUS_FIFO_FSM            0x3f00
#define BF_USBPHY_DEBUG7_STATUS_FIFO_FSM(v)         (((v) << 8) & 0x3f00)
#define BP_USBPHY_DEBUG7_STATUS_MAIN_FSM            4
#define BM_USBPHY_DEBUG7_STATUS_MAIN_FSM            0xf0
#define BF_USBPHY_DEBUG7_STATUS_MAIN_FSM(v)         (((v) << 4) & 0xf0)
#define BP_USBPHY_DEBUG7_STATUS_EOP_FSM             0
#define BM_USBPHY_DEBUG7_STATUS_EOP_FSM             0xf
#define BF_USBPHY_DEBUG7_STATUS_EOP_FSM(v)          (((v) << 0) & 0xf)

/**
 * Register: HW_USBPHY_DEBUG8_STATUS
 * Address: 0xe0
 * SCT: no
*/
#define HW_USBPHY_DEBUG8_STATUS                     (*(volatile unsigned long *)(REGS_USBPHY_BASE + 0xe0))
#define BP_USBPHY_DEBUG8_STATUS_RX_SIE_FSM          28
#define BM_USBPHY_DEBUG8_STATUS_RX_SIE_FSM          0xf0000000
#define BF_USBPHY_DEBUG8_STATUS_RX_SIE_FSM(v)       (((v) << 28) & 0xf0000000)
#define BP_USBPHY_DEBUG8_STATUS_TX_SIE_FSM          24
#define BM_USBPHY_DEBUG8_STATUS_TX_SIE_FSM          0xf000000
#define BF_USBPHY_DEBUG8_STATUS_TX_SIE_FSM(v)       (((v) << 24) & 0xf000000)
#define BP_USBPHY_DEBUG8_STATUS_SHIFT_FSM           8
#define BM_USBPHY_DEBUG8_STATUS_SHIFT_FSM           0x300
#define BF_USBPHY_DEBUG8_STATUS_SHIFT_FSM(v)        (((v) << 8) & 0x300)
#define BP_USBPHY_DEBUG8_STATUS_FS_TX_MAIN_FSM      0
#define BM_USBPHY_DEBUG8_STATUS_FS_TX_MAIN_FSM      0x7f
#define BF_USBPHY_DEBUG8_STATUS_FS_TX_MAIN_FSM(v)   (((v) << 0) & 0x7f)

#endif /* __HEADERGEN__STMP3600__USBPHY__H__ */
