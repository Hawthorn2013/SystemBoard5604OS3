/*
 * TestTask.h
 *
 *  Created on: Jan 22, 2015
 *      Author: Jian
 */

#ifndef TESTTASK_H_
#define TESTTASK_H_


#define Test1_TASK_STK_SIZE (512)
#define Test2_TASK_STK_SIZE (512)
#define Test3_TASK_STK_SIZE (512)
#define Test4_TASK_STK_SIZE (512)
#define TEST1_TASK_PRIO (10)
#define TEST2_TASK_PRIO (12)
#define TEST3_TASK_PRIO (13)
#define TEST4_TASK_PRIO (14)


/*防止重复定义,未成功
#ifdef TestTask_GLOBALS
#define TestTask_EXT
#else 
#define TestTak_EXT extern
#endif
*/


extern OS_STK Test1TaskStk[];
extern OS_STK Test2TaskStk[];
extern OS_STK Test3TaskStk[];
extern OS_STK Test4TaskStk[];


extern void Test1Task(void *p_arg);
extern void Test2Task(void *p_arg);
extern void Test3Task(void *p_arg);
extern void Test4Task(void *p_arg);

extern OS_EVENT *Sem1,*Sem2,*Sem3,*Sem4;


#endif /* TESTTASK_H_ */
