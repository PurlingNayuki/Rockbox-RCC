/***************************************************************************
 *             __________               __   ___.
 *   Open      \______   \ ____   ____ |  | _\_ |__   _______  ___
 *   Source     |       _//  _ \_/ ___\|  |/ /| __ \ /  _ \  \/  /
 *   Jukebox    |    |   (  <_> )  \___|    < | \_\ (  <_> > <  <
 *   Firmware   |____|_  /\____/ \___  >__|_ \|___  /\____/__/\_ \
 *                     \/            \/     \/    \/            \/
 * This file was automatically generated by headergen, DO NOT EDIT it.
 * headergen version: 2.1.8
 * XML versions: stmp3600:2.4.0
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
#ifndef __HEADERGEN__STMP3600__CLKCTRL__H__
#define __HEADERGEN__STMP3600__CLKCTRL__H__

#define REGS_CLKCTRL_BASE (0x80040000)

#define REGS_CLKCTRL_VERSION "2.4.0"

/**
 * Register: HW_CLKCTRL_PLLCTRL0
 * Address: 0
 * SCT: yes
*/
#define HW_CLKCTRL_PLLCTRL0                                     (*(volatile unsigned long *)(REGS_CLKCTRL_BASE + 0x0 + 0x0))
#define HW_CLKCTRL_PLLCTRL0_SET                                 (*(volatile unsigned long *)(REGS_CLKCTRL_BASE + 0x0 + 0x4))
#define HW_CLKCTRL_PLLCTRL0_CLR                                 (*(volatile unsigned long *)(REGS_CLKCTRL_BASE + 0x0 + 0x8))
#define HW_CLKCTRL_PLLCTRL0_TOG                                 (*(volatile unsigned long *)(REGS_CLKCTRL_BASE + 0x0 + 0xc))
#define BP_CLKCTRL_PLLCTRL0_PLLVCOKSTART                        30
#define BM_CLKCTRL_PLLCTRL0_PLLVCOKSTART                        0x40000000
#define BF_CLKCTRL_PLLCTRL0_PLLVCOKSTART(v)                     (((v) << 30) & 0x40000000)
#define BP_CLKCTRL_PLLCTRL0_PLLCPSHORTLFR                       29
#define BM_CLKCTRL_PLLCTRL0_PLLCPSHORTLFR                       0x20000000
#define BF_CLKCTRL_PLLCTRL0_PLLCPSHORTLFR(v)                    (((v) << 29) & 0x20000000)
#define BP_CLKCTRL_PLLCTRL0_PLLCPDBLIP                          28
#define BM_CLKCTRL_PLLCTRL0_PLLCPDBLIP                          0x10000000
#define BF_CLKCTRL_PLLCTRL0_PLLCPDBLIP(v)                       (((v) << 28) & 0x10000000)
#define BP_CLKCTRL_PLLCTRL0_PLLCPNSEL                           24
#define BM_CLKCTRL_PLLCTRL0_PLLCPNSEL                           0x7000000
#define BV_CLKCTRL_PLLCTRL0_PLLCPNSEL__DEFAULT                  0x0
#define BV_CLKCTRL_PLLCTRL0_PLLCPNSEL__TIMES_15                 0x2
#define BV_CLKCTRL_PLLCTRL0_PLLCPNSEL__TIMES_075                0x3
#define BV_CLKCTRL_PLLCTRL0_PLLCPNSEL__TIMES_05                 0x4
#define BV_CLKCTRL_PLLCTRL0_PLLCPNSEL__TIMES_04                 0x7
#define BF_CLKCTRL_PLLCTRL0_PLLCPNSEL(v)                        (((v) << 24) & 0x7000000)
#define BF_CLKCTRL_PLLCTRL0_PLLCPNSEL_V(v)                      ((BV_CLKCTRL_PLLCTRL0_PLLCPNSEL__##v << 24) & 0x7000000)
#define BP_CLKCTRL_PLLCTRL0_PLLV2ISEL                           20
#define BM_CLKCTRL_PLLCTRL0_PLLV2ISEL                           0x300000
#define BV_CLKCTRL_PLLCTRL0_PLLV2ISEL__NORMAL                   0x0
#define BV_CLKCTRL_PLLCTRL0_PLLV2ISEL__LOWER                    0x1
#define BV_CLKCTRL_PLLCTRL0_PLLV2ISEL__LOWEST                   0x2
#define BV_CLKCTRL_PLLCTRL0_PLLV2ISEL__HIGHEST                  0x3
#define BF_CLKCTRL_PLLCTRL0_PLLV2ISEL(v)                        (((v) << 20) & 0x300000)
#define BF_CLKCTRL_PLLCTRL0_PLLV2ISEL_V(v)                      ((BV_CLKCTRL_PLLCTRL0_PLLV2ISEL__##v << 20) & 0x300000)
#define BP_CLKCTRL_PLLCTRL0_FORCE_FREQ                          19
#define BM_CLKCTRL_PLLCTRL0_FORCE_FREQ                          0x80000
#define BV_CLKCTRL_PLLCTRL0_FORCE_FREQ__FORCE_SAME_FREQ         0x1
#define BV_CLKCTRL_PLLCTRL0_FORCE_FREQ__HONOR_SAME_FREQ_RULE    0x0
#define BF_CLKCTRL_PLLCTRL0_FORCE_FREQ(v)                       (((v) << 19) & 0x80000)
#define BF_CLKCTRL_PLLCTRL0_FORCE_FREQ_V(v)                     ((BV_CLKCTRL_PLLCTRL0_FORCE_FREQ__##v << 19) & 0x80000)
#define BP_CLKCTRL_PLLCTRL0_EN_USB_CLKS                         18
#define BM_CLKCTRL_PLLCTRL0_EN_USB_CLKS                         0x40000
#define BF_CLKCTRL_PLLCTRL0_EN_USB_CLKS(v)                      (((v) << 18) & 0x40000)
#define BP_CLKCTRL_PLLCTRL0_BYPASS                              17
#define BM_CLKCTRL_PLLCTRL0_BYPASS                              0x20000
#define BF_CLKCTRL_PLLCTRL0_BYPASS(v)                           (((v) << 17) & 0x20000)
#define BP_CLKCTRL_PLLCTRL0_POWER                               16
#define BM_CLKCTRL_PLLCTRL0_POWER                               0x10000
#define BF_CLKCTRL_PLLCTRL0_POWER(v)                            (((v) << 16) & 0x10000)
#define BP_CLKCTRL_PLLCTRL0_FREQ                                0
#define BM_CLKCTRL_PLLCTRL0_FREQ                                0x1ff
#define BF_CLKCTRL_PLLCTRL0_FREQ(v)                             (((v) << 0) & 0x1ff)

/**
 * Register: HW_CLKCTRL_PLLCTRL1
 * Address: 0x10
 * SCT: yes
*/
#define HW_CLKCTRL_PLLCTRL1                 (*(volatile unsigned long *)(REGS_CLKCTRL_BASE + 0x10 + 0x0))
#define HW_CLKCTRL_PLLCTRL1_SET             (*(volatile unsigned long *)(REGS_CLKCTRL_BASE + 0x10 + 0x4))
#define HW_CLKCTRL_PLLCTRL1_CLR             (*(volatile unsigned long *)(REGS_CLKCTRL_BASE + 0x10 + 0x8))
#define HW_CLKCTRL_PLLCTRL1_TOG             (*(volatile unsigned long *)(REGS_CLKCTRL_BASE + 0x10 + 0xc))
#define BP_CLKCTRL_PLLCTRL1_LOCK            31
#define BM_CLKCTRL_PLLCTRL1_LOCK            0x80000000
#define BF_CLKCTRL_PLLCTRL1_LOCK(v)         (((v) << 31) & 0x80000000)
#define BP_CLKCTRL_PLLCTRL1_FORCE_LOCK      30
#define BM_CLKCTRL_PLLCTRL1_FORCE_LOCK      0x40000000
#define BF_CLKCTRL_PLLCTRL1_FORCE_LOCK(v)   (((v) << 30) & 0x40000000)
#define BP_CLKCTRL_PLLCTRL1_LOCK_COUNT      0
#define BM_CLKCTRL_PLLCTRL1_LOCK_COUNT      0xffff
#define BF_CLKCTRL_PLLCTRL1_LOCK_COUNT(v)   (((v) << 0) & 0xffff)

/**
 * Register: HW_CLKCTRL_CPU
 * Address: 0x20
 * SCT: no
*/
#define HW_CLKCTRL_CPU                      (*(volatile unsigned long *)(REGS_CLKCTRL_BASE + 0x20))
#define BP_CLKCTRL_CPU_WAIT_PLL_LOCK        30
#define BM_CLKCTRL_CPU_WAIT_PLL_LOCK        0x40000000
#define BF_CLKCTRL_CPU_WAIT_PLL_LOCK(v)     (((v) << 30) & 0x40000000)
#define BP_CLKCTRL_CPU_BUSY                 29
#define BM_CLKCTRL_CPU_BUSY                 0x20000000
#define BF_CLKCTRL_CPU_BUSY(v)              (((v) << 29) & 0x20000000)
#define BP_CLKCTRL_CPU_INTERRUPT_WAIT       12
#define BM_CLKCTRL_CPU_INTERRUPT_WAIT       0x1000
#define BF_CLKCTRL_CPU_INTERRUPT_WAIT(v)    (((v) << 12) & 0x1000)
#define BP_CLKCTRL_CPU_DIV                  0
#define BM_CLKCTRL_CPU_DIV                  0x3ff
#define BF_CLKCTRL_CPU_DIV(v)               (((v) << 0) & 0x3ff)

/**
 * Register: HW_CLKCTRL_HBUS
 * Address: 0x30
 * SCT: no
*/
#define HW_CLKCTRL_HBUS                         (*(volatile unsigned long *)(REGS_CLKCTRL_BASE + 0x30))
#define BP_CLKCTRL_HBUS_WAIT_PLL_LOCK           30
#define BM_CLKCTRL_HBUS_WAIT_PLL_LOCK           0x40000000
#define BF_CLKCTRL_HBUS_WAIT_PLL_LOCK(v)        (((v) << 30) & 0x40000000)
#define BP_CLKCTRL_HBUS_BUSY                    29
#define BM_CLKCTRL_HBUS_BUSY                    0x20000000
#define BF_CLKCTRL_HBUS_BUSY(v)                 (((v) << 29) & 0x20000000)
#define BP_CLKCTRL_HBUS_EMI_BUSY_FAST           27
#define BM_CLKCTRL_HBUS_EMI_BUSY_FAST           0x8000000
#define BF_CLKCTRL_HBUS_EMI_BUSY_FAST(v)        (((v) << 27) & 0x8000000)
#define BP_CLKCTRL_HBUS_APBHDMA_BUSY_FAST       26
#define BM_CLKCTRL_HBUS_APBHDMA_BUSY_FAST       0x4000000
#define BF_CLKCTRL_HBUS_APBHDMA_BUSY_FAST(v)    (((v) << 26) & 0x4000000)
#define BP_CLKCTRL_HBUS_APBXDMA_BUSY_FAST       25
#define BM_CLKCTRL_HBUS_APBXDMA_BUSY_FAST       0x2000000
#define BF_CLKCTRL_HBUS_APBXDMA_BUSY_FAST(v)    (((v) << 25) & 0x2000000)
#define BP_CLKCTRL_HBUS_TRAFFIC_JAM_FAST        24
#define BM_CLKCTRL_HBUS_TRAFFIC_JAM_FAST        0x1000000
#define BF_CLKCTRL_HBUS_TRAFFIC_JAM_FAST(v)     (((v) << 24) & 0x1000000)
#define BP_CLKCTRL_HBUS_TRAFFIC_FAST            23
#define BM_CLKCTRL_HBUS_TRAFFIC_FAST            0x800000
#define BF_CLKCTRL_HBUS_TRAFFIC_FAST(v)         (((v) << 23) & 0x800000)
#define BP_CLKCTRL_HBUS_CPU_DATA_FAST           22
#define BM_CLKCTRL_HBUS_CPU_DATA_FAST           0x400000
#define BF_CLKCTRL_HBUS_CPU_DATA_FAST(v)        (((v) << 22) & 0x400000)
#define BP_CLKCTRL_HBUS_CPU_INSTR_FAST          21
#define BM_CLKCTRL_HBUS_CPU_INSTR_FAST          0x200000
#define BF_CLKCTRL_HBUS_CPU_INSTR_FAST(v)       (((v) << 21) & 0x200000)
#define BP_CLKCTRL_HBUS_AUTO_SLOW_MODE          20
#define BM_CLKCTRL_HBUS_AUTO_SLOW_MODE          0x100000
#define BF_CLKCTRL_HBUS_AUTO_SLOW_MODE(v)       (((v) << 20) & 0x100000)
#define BP_CLKCTRL_HBUS_SLOW_DIV                16
#define BM_CLKCTRL_HBUS_SLOW_DIV                0x30000
#define BV_CLKCTRL_HBUS_SLOW_DIV__BY1           0x0
#define BV_CLKCTRL_HBUS_SLOW_DIV__BY2           0x1
#define BV_CLKCTRL_HBUS_SLOW_DIV__BY4           0x2
#define BV_CLKCTRL_HBUS_SLOW_DIV__BY8           0x3
#define BF_CLKCTRL_HBUS_SLOW_DIV(v)             (((v) << 16) & 0x30000)
#define BF_CLKCTRL_HBUS_SLOW_DIV_V(v)           ((BV_CLKCTRL_HBUS_SLOW_DIV__##v << 16) & 0x30000)
#define BP_CLKCTRL_HBUS_DIV                     0
#define BM_CLKCTRL_HBUS_DIV                     0x1f
#define BF_CLKCTRL_HBUS_DIV(v)                  (((v) << 0) & 0x1f)

/**
 * Register: HW_CLKCTRL_XBUS
 * Address: 0x40
 * SCT: no
*/
#define HW_CLKCTRL_XBUS         (*(volatile unsigned long *)(REGS_CLKCTRL_BASE + 0x40))
#define BP_CLKCTRL_XBUS_BUSY    31
#define BM_CLKCTRL_XBUS_BUSY    0x80000000
#define BF_CLKCTRL_XBUS_BUSY(v) (((v) << 31) & 0x80000000)
#define BP_CLKCTRL_XBUS_DIV     0
#define BM_CLKCTRL_XBUS_DIV     0x3ff
#define BF_CLKCTRL_XBUS_DIV(v)  (((v) << 0) & 0x3ff)

/**
 * Register: HW_CLKCTRL_XTAL
 * Address: 0x50
 * SCT: no
*/
#define HW_CLKCTRL_XTAL                         (*(volatile unsigned long *)(REGS_CLKCTRL_BASE + 0x50))
#define BP_CLKCTRL_XTAL_UART_CLK_GATE           31
#define BM_CLKCTRL_XTAL_UART_CLK_GATE           0x80000000
#define BF_CLKCTRL_XTAL_UART_CLK_GATE(v)        (((v) << 31) & 0x80000000)
#define BP_CLKCTRL_XTAL_FILT_CLK24M_GATE        30
#define BM_CLKCTRL_XTAL_FILT_CLK24M_GATE        0x40000000
#define BF_CLKCTRL_XTAL_FILT_CLK24M_GATE(v)     (((v) << 30) & 0x40000000)
#define BP_CLKCTRL_XTAL_PWM_CLK24M_GATE         29
#define BM_CLKCTRL_XTAL_PWM_CLK24M_GATE         0x20000000
#define BF_CLKCTRL_XTAL_PWM_CLK24M_GATE(v)      (((v) << 29) & 0x20000000)
#define BP_CLKCTRL_XTAL_DRI_CLK24M_GATE         28
#define BM_CLKCTRL_XTAL_DRI_CLK24M_GATE         0x10000000
#define BF_CLKCTRL_XTAL_DRI_CLK24M_GATE(v)      (((v) << 28) & 0x10000000)
#define BP_CLKCTRL_XTAL_DIGCTRL_CLK1M_GATE      27
#define BM_CLKCTRL_XTAL_DIGCTRL_CLK1M_GATE      0x8000000
#define BF_CLKCTRL_XTAL_DIGCTRL_CLK1M_GATE(v)   (((v) << 27) & 0x8000000)
#define BP_CLKCTRL_XTAL_TIMROT_CLK32K_GATE      26
#define BM_CLKCTRL_XTAL_TIMROT_CLK32K_GATE      0x4000000
#define BF_CLKCTRL_XTAL_TIMROT_CLK32K_GATE(v)   (((v) << 26) & 0x4000000)
#define BP_CLKCTRL_XTAL_EXRAM_CLK16K_GATE       25
#define BM_CLKCTRL_XTAL_EXRAM_CLK16K_GATE       0x2000000
#define BF_CLKCTRL_XTAL_EXRAM_CLK16K_GATE(v)    (((v) << 25) & 0x2000000)
#define BP_CLKCTRL_XTAL_LRADC_CLK2K_GATE        24
#define BM_CLKCTRL_XTAL_LRADC_CLK2K_GATE        0x1000000
#define BF_CLKCTRL_XTAL_LRADC_CLK2K_GATE(v)     (((v) << 24) & 0x1000000)

/**
 * Register: HW_CLKCTRL_OCRAM
 * Address: 0x60
 * SCT: no
*/
#define HW_CLKCTRL_OCRAM            (*(volatile unsigned long *)(REGS_CLKCTRL_BASE + 0x60))
#define BP_CLKCTRL_OCRAM_CLKGATE    31
#define BM_CLKCTRL_OCRAM_CLKGATE    0x80000000
#define BF_CLKCTRL_OCRAM_CLKGATE(v) (((v) << 31) & 0x80000000)
#define BP_CLKCTRL_OCRAM_BUSY       30
#define BM_CLKCTRL_OCRAM_BUSY       0x40000000
#define BF_CLKCTRL_OCRAM_BUSY(v)    (((v) << 30) & 0x40000000)
#define BP_CLKCTRL_OCRAM_DIV        0
#define BM_CLKCTRL_OCRAM_DIV        0x3ff
#define BF_CLKCTRL_OCRAM_DIV(v)     (((v) << 0) & 0x3ff)

/**
 * Register: HW_CLKCTRL_UTMI
 * Address: 0x70
 * SCT: no
*/
#define HW_CLKCTRL_UTMI                         (*(volatile unsigned long *)(REGS_CLKCTRL_BASE + 0x70))
#define BP_CLKCTRL_UTMI_UTMI_CLK120M_GATE       31
#define BM_CLKCTRL_UTMI_UTMI_CLK120M_GATE       0x80000000
#define BF_CLKCTRL_UTMI_UTMI_CLK120M_GATE(v)    (((v) << 31) & 0x80000000)
#define BP_CLKCTRL_UTMI_UTMI_CLK30M_GATE        30
#define BM_CLKCTRL_UTMI_UTMI_CLK30M_GATE        0x40000000
#define BF_CLKCTRL_UTMI_UTMI_CLK30M_GATE(v)     (((v) << 30) & 0x40000000)

/**
 * Register: HW_CLKCTRL_SSP
 * Address: 0x80
 * SCT: no
*/
#define HW_CLKCTRL_SSP                  (*(volatile unsigned long *)(REGS_CLKCTRL_BASE + 0x80))
#define BP_CLKCTRL_SSP_CLKGATE          31
#define BM_CLKCTRL_SSP_CLKGATE          0x80000000
#define BF_CLKCTRL_SSP_CLKGATE(v)       (((v) << 31) & 0x80000000)
#define BP_CLKCTRL_SSP_WAIT_PLL_LOCK    30
#define BM_CLKCTRL_SSP_WAIT_PLL_LOCK    0x40000000
#define BF_CLKCTRL_SSP_WAIT_PLL_LOCK(v) (((v) << 30) & 0x40000000)
#define BP_CLKCTRL_SSP_BUSY             29
#define BM_CLKCTRL_SSP_BUSY             0x20000000
#define BF_CLKCTRL_SSP_BUSY(v)          (((v) << 29) & 0x20000000)
#define BP_CLKCTRL_SSP_DIV              0
#define BM_CLKCTRL_SSP_DIV              0x1ff
#define BF_CLKCTRL_SSP_DIV(v)           (((v) << 0) & 0x1ff)

/**
 * Register: HW_CLKCTRL_GPMI
 * Address: 0x90
 * SCT: no
*/
#define HW_CLKCTRL_GPMI                     (*(volatile unsigned long *)(REGS_CLKCTRL_BASE + 0x90))
#define BP_CLKCTRL_GPMI_CLKGATE             31
#define BM_CLKCTRL_GPMI_CLKGATE             0x80000000
#define BF_CLKCTRL_GPMI_CLKGATE(v)          (((v) << 31) & 0x80000000)
#define BP_CLKCTRL_GPMI_WAIT_PLL_LOCK       30
#define BM_CLKCTRL_GPMI_WAIT_PLL_LOCK       0x40000000
#define BF_CLKCTRL_GPMI_WAIT_PLL_LOCK(v)    (((v) << 30) & 0x40000000)
#define BP_CLKCTRL_GPMI_BUSY                29
#define BM_CLKCTRL_GPMI_BUSY                0x20000000
#define BF_CLKCTRL_GPMI_BUSY(v)             (((v) << 29) & 0x20000000)
#define BP_CLKCTRL_GPMI_DIV                 0
#define BM_CLKCTRL_GPMI_DIV                 0x3ff
#define BF_CLKCTRL_GPMI_DIV(v)              (((v) << 0) & 0x3ff)

/**
 * Register: HW_CLKCTRL_SPDIF
 * Address: 0xa0
 * SCT: no
*/
#define HW_CLKCTRL_SPDIF            (*(volatile unsigned long *)(REGS_CLKCTRL_BASE + 0xa0))
#define BP_CLKCTRL_SPDIF_CLKGATE    31
#define BM_CLKCTRL_SPDIF_CLKGATE    0x80000000
#define BF_CLKCTRL_SPDIF_CLKGATE(v) (((v) << 31) & 0x80000000)
#define BP_CLKCTRL_SPDIF_BUSY       30
#define BM_CLKCTRL_SPDIF_BUSY       0x40000000
#define BF_CLKCTRL_SPDIF_BUSY(v)    (((v) << 30) & 0x40000000)
#define BP_CLKCTRL_SPDIF_DIV        0
#define BM_CLKCTRL_SPDIF_DIV        0x7
#define BF_CLKCTRL_SPDIF_DIV(v)     (((v) << 0) & 0x7)

/**
 * Register: HW_CLKCTRL_EMI
 * Address: 0xb0
 * SCT: no
*/
#define HW_CLKCTRL_EMI                  (*(volatile unsigned long *)(REGS_CLKCTRL_BASE + 0xb0))
#define BP_CLKCTRL_EMI_CLKGATE          31
#define BM_CLKCTRL_EMI_CLKGATE          0x80000000
#define BF_CLKCTRL_EMI_CLKGATE(v)       (((v) << 31) & 0x80000000)
#define BP_CLKCTRL_EMI_WAIT_PLL_LOCK    30
#define BM_CLKCTRL_EMI_WAIT_PLL_LOCK    0x40000000
#define BF_CLKCTRL_EMI_WAIT_PLL_LOCK(v) (((v) << 30) & 0x40000000)
#define BP_CLKCTRL_EMI_BUSY             29
#define BM_CLKCTRL_EMI_BUSY             0x20000000
#define BF_CLKCTRL_EMI_BUSY(v)          (((v) << 29) & 0x20000000)
#define BP_CLKCTRL_EMI_DIV              0
#define BM_CLKCTRL_EMI_DIV              0x7
#define BF_CLKCTRL_EMI_DIV(v)           (((v) << 0) & 0x7)

/**
 * Register: HW_CLKCTRL_IR
 * Address: 0xc0
 * SCT: no
*/
#define HW_CLKCTRL_IR                   (*(volatile unsigned long *)(REGS_CLKCTRL_BASE + 0xc0))
#define BP_CLKCTRL_IR_CLKGATE           31
#define BM_CLKCTRL_IR_CLKGATE           0x80000000
#define BF_CLKCTRL_IR_CLKGATE(v)        (((v) << 31) & 0x80000000)
#define BP_CLKCTRL_IR_WAIT_PLL_LOCK     30
#define BM_CLKCTRL_IR_WAIT_PLL_LOCK     0x40000000
#define BF_CLKCTRL_IR_WAIT_PLL_LOCK(v)  (((v) << 30) & 0x40000000)
#define BP_CLKCTRL_IR_AUTO_DIV          29
#define BM_CLKCTRL_IR_AUTO_DIV          0x20000000
#define BF_CLKCTRL_IR_AUTO_DIV(v)       (((v) << 29) & 0x20000000)
#define BP_CLKCTRL_IR_IR_BUSY           28
#define BM_CLKCTRL_IR_IR_BUSY           0x10000000
#define BF_CLKCTRL_IR_IR_BUSY(v)        (((v) << 28) & 0x10000000)
#define BP_CLKCTRL_IR_IROV_BUSY         27
#define BM_CLKCTRL_IR_IROV_BUSY         0x8000000
#define BF_CLKCTRL_IR_IROV_BUSY(v)      (((v) << 27) & 0x8000000)
#define BP_CLKCTRL_IR_IROV_DIV          16
#define BM_CLKCTRL_IR_IROV_DIV          0x1ff0000
#define BF_CLKCTRL_IR_IROV_DIV(v)       (((v) << 16) & 0x1ff0000)
#define BP_CLKCTRL_IR_IR_DIV            0
#define BM_CLKCTRL_IR_IR_DIV            0x3ff
#define BF_CLKCTRL_IR_IR_DIV(v)         (((v) << 0) & 0x3ff)

#endif /* __HEADERGEN__STMP3600__CLKCTRL__H__ */
