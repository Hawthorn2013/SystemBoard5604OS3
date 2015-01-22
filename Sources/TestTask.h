/*
 * TestTask.h
 *
 *  Created on: Jan 22, 2015
 *      Author: Jian
 */

#ifndef TESTTASK_H_
#define TESTTASK_H_


#define Test1_TASK_STK_SIZE (1024)
#define Test2_TASK_STK_SIZE (1024)
#define TEST1_TASK_PRIO (10)
#define TEST2_TASK_PRIO (12)


extern OS_STK Test1TaskStk[];
extern OS_STK Test2TaskStk[];


extern void Test1Task(void *p_arg);
extern void Test2Task(void *p_arg);


#endif /* TESTTASK_H_ */
