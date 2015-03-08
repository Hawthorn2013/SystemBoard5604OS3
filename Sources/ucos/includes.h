/*
*********************************************************************************************************
*                                                uC/OS-II
*                                          The Real-Time Kernel
*                                     Freescale  MPC55xx Specific code
*
*                                 (c) Copyright 2007; Micrium; Weston, FL
*                                           All Rights Reserved
*
* File    : INCLUDES.H
* By      : Fabiano Kovalski
*********************************************************************************************************
*/

#include      <stdarg.h>

#ifndef _TYPEDEF_BYTE_
#define _TYPEDEF_BYTE_
typedef unsigned char BYTE;
#endif

#include      "MPC5604B.h"
#include      "IntcInterrupts.h"
#include      "../Base.h"
#include      "cpu.h"
#include      "app_cfg.h"
#include      "ucos_ii.h"
#include      "bsp.h"
#include	  "../TestTask.h"
#include 	  "../Init.h"
#include 	  "../UART.h"
#include 	  "../React.h"

