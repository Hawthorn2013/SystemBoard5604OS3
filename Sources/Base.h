/*
 * Base.h
 *
 *  Created on: Aug 26, 2014
 *      Author: littlecat
 */

#ifndef __BASE_H__
#define __BASE_H__


/* 定义外部函数 */
extern void Disable_Watchdog(void);
extern void Enable_IRQ(void);
extern void Init_ModesAndClock(void);
extern void Init_LQBoard_LED(void);
extern void Init_Pit(void);


#endif /* __BASE_H__ */
