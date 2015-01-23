/*
 * Base.c
 *
 *  Created on: Aug 26, 2014
 *      Author: littlecat
 */


#include "ucos/includes.h"


/*
 * ���ÿ��Ź�
 */
void Disable_Watchdog(void)
{
	SWT.SR.R = 0x0000c520;	/* rite keys to clear soft lock bit */
	SWT.SR.R = 0x0000d928;
	SWT.CR.R = 0x8000010A;	/* Clear watchdog enable (WEN) */
}


/*
 * ��ʼ����Ƭ��
 */
void Init_ModesAndClock(void) 
{
    ME.MER.R = 0x0000001D;	/* Enable DRUN, RUN0, SAFE, RESET modes */
	/* ����sysclk */
    //CGM.FMPLL_CR.R = 0x02400100;	/* 8 MHz xtal: Set PLL0 to 64 MHz */
    CGM.FMPLL_CR.R = 0x01280000;	/* 8 MHz xtal: Set PLL0 to 80 MHz */
    //CGM.FMPLL_CR.R = 0x013C0000;	/* 8 MHz xtal: Set PLL0 to 120 MHz */ 
    ME.RUN[0].R = 0x001F0064;	/* RUN0 cfg: 16MHzIRCON,OSC0ON,PLL0ON,syclk=PLL     sysclkѡ�����໷ʱ�� */
    ME.RUNPC[0].R = 0x00000010;	/* Peri. Cfg. 1 settings: only run in RUN0 mode      ѡ��RUN0ģʽ */
  
	/* PCTL[?] ѡ����Ҫʱ��ģ��(Ĭ�ϼ��ɣ���������) */
	//ME.PCTL[32].R = 0x00;	/* MPC56xxB/P/S ADC 0: select ME.RUNPC[0] */
    ME.PCTL[32].B.DBG_F = 0;
	/* Mode Transition to enter RUN0 mode */
    ME.MCTL.R = 0x40005AF0;	/* Enter RUN0 Mode & Key */
    ME.MCTL.R = 0x4000A50F;	/* Enter RUN0 Mode & Inverted Key */
    
    while (ME.GS.B.S_MTRANS) {}	/* Wait for mode transition to complete �ȴ�ģʽת����� */
    while(ME.GS.B.S_CURRENTMODE != 4) {} /* Verify RUN0 is the current mode �ȴ�ѡ��RUN0ģʽ */
  
	/* ��peri0��1��2 */
	/* ����ʱ������ �����ڷ�Ƶ */
	CGM.SC_DC[0].R = 0x84;	/* LIN */
	CGM.SC_DC[1].R = 0x80;	/* FLEXCAN,DSPI */
    CGM.SC_DC[2].R = 0x80;	/* eMIOS,CTU,ADC */
}


/*
 * ʹ���ⲿ�ж�
 */
void Enable_IRQ(void)
{
	INTC.CPR.B.PRI = 0;	/* Single Core: Lower INTC's current priority */
	asm(" wrteei 1");	/* Enable external interrupts */
}


/*
 * ��ʼ���������LED
 */
void Init_LQBoard_LED(void)
{
 	SIU.PCR[12].R = 0x0203;	/* PA12 */
  	SIU.PCR[13].R = 0x0203;
 	SIU.PCR[14].R = 0x0203; 
	SIU.PCR[15].R = 0x0203;	/* PA15 */
 	
	SIU.GPDO[12].R = 0;	/* 1=Ϩ�� */
	SIU.GPDO[13].R = 0;
	SIU.GPDO[14].R = 0;
	SIU.GPDO[15].R = 0;

}


/*
 * ��ʼ��PIT�ж�
 * 10ms
 */
void Init_Pit(void)
{
	/* NOTE:  DIVIDER FROM SYSCLK TO PIT ASSUMES DEFAULT DIVIDE BY 1 */
	PIT.PITMCR.R = 0x00000001;	/* Enable PIT and configure timers to stop in debug modem */
	PIT.CH[1].LDVAL.R = 1600000;	/* 800000==10ms */
	PIT.CH[1].TCTRL.R = 0x00000003;	/* Enable PIT1 interrupt and make PIT active to count */
	INTC_InstallINTCInterruptHandler(OSTickISR_Out, 60, 1);	/* PIT 1 interrupt vector with priority 1 */
}


void OSTickISR_Out(void)
{
    PIT.CH[1].TFLG.B.TIF = 1;   // MPC56xxB/P/S: Clear PIT 1 flag by writing 1
    BD3 = ~BD3;
    OSTickISR();
}
