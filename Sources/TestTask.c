/*
 * TestTask.c
 *
 *  Created on: Jan 22, 2015
 *      Author: Jian
 */



#include "ucos/includes.h"


OS_STK Test1TaskStk[Test1_TASK_STK_SIZE-1];


void Test1Task(void *p_arg)
{
	volatile int i = 0;
	
	
   (void) p_arg;              //×ª»»³É¿ÕÖ¸Õë£¬·ÀÖ¹±àÒëÆ÷´íÎó
   while(1)
   {
	   i++;
   }
}
