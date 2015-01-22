/*
 * Base.h
 *
 *  Created on: Aug 26, 2014
 *      Author: littlecat
 */

#ifndef __BASE_H__
#define __BASE_H__


/* 定义板载LED */
#if 1
#define BD0 (SIU.GPDO[12].B.PDO)
#define BD1 (SIU.GPDO[13].B.PDO)
#define BD2 (SIU.GPDO[14].B.PDO)
#define BD3 (SIU.GPDO[15].B.PDO)
#endif


/* 定义外部函数 */
extern void Disable_Watchdog(void);
extern void Enable_IRQ(void);
extern void Init_ModesAndClock(void);
extern void Init_LQBoard_LED(void);
extern void Init_Pit(void);
extern void OSTickISR_Out(void);


#endif /* __BASE_H__ */
