/*
 * TestTask.h
 *
 *  Created on: Jan 22, 2015
 *      Author: Jian
 */

#ifndef TESTTASK_H_
#define TESTTASK_H_


#define Test1_TASK_STK_SIZE (1024)
#define TEST1_TASK_PRIO (10)


extern OS_STK Test1TaskStk[];


extern void Test1Task(void *p_arg);


#endif /* TESTTASK_H_ */
