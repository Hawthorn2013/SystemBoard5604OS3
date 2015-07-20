/*
 * TestTask.c
 *
 *  Created on: Jan 22, 2015
 *      Author: Jian
 */




#include "includes.h"

OS_STK Test1TaskStk[Test1_TASK_STK_SIZE-1];
OS_STK Test2TaskStk[Test2_TASK_STK_SIZE-1];
OS_STK Test3TaskStk[Test3_TASK_STK_SIZE-1];
OS_STK Test4TaskStk[Test4_TASK_STK_SIZE-1];


/*void Test1Task(void *p_arg)
{
	volatile int i = 0;
	
	
   (void) p_arg;              //转换成空指针，防止编译器错误
   while(1)
   {
       BD1 = ~BD1;
       OSTimeDly(3);
   }
}


void Test2Task(void *p_arg)
{
    volatile int i = 0;
    
    
   (void) p_arg;              //转换成空指针，防止编译器错误
   while(1)
   {
       BD2 = ~BD2;
       OSTimeDly(3);
   }
}*/


//流水灯
/*OS_EVENT *Sem1,*Sem2,*Sem3,*Sem4;

void Test1Task(void *p_arg)
{
	(void) p_arg;
	Sem1=OSSemCreate(0);
	Sem2=OSSemCreate(0);
	Sem3=OSSemCreate(0);
	Sem4=OSSemCreate(0);
	OSTaskCreate(  Test2Task,
	    		   (void *)0,
	               &Test2TaskStk[Test2_TASK_STK_SIZE-1],
	               TEST2_TASK_PRIO );
	OSTaskCreate(  Test3Task,
	    		   (void *)0,
	    		   &Test3TaskStk[Test3_TASK_STK_SIZE-1],
	    		   TEST3_TASK_PRIO );
	OSTaskCreate(  Test4Task,
		    	   (void *)0,
		    	   &Test4TaskStk[Test4_TASK_STK_SIZE-1],
		    	   TEST4_TASK_PRIO );
	BD0=1;			//初始灯不亮
	
	while(1)
	{
		INT8U err;
		OSSemPend(Sem4,0,&err);
		BD0=0;
		OSTimeDly(OS_TICKS_PER_SEC/10);
		BD0=1;
		OSSemPost(Sem1);
	}
}

void Test2Task(void *p_arg)
{
	(void) p_arg;
	
	BD1=1;			//初始灯不亮
	
	while(1)
	{
		INT8U err;
		OSSemPend(Sem1,0,&err);
		BD1=0;
		OSTimeDly(OS_TICKS_PER_SEC/10);
		BD1=1;
		OSSemPost(Sem2);
	}
}

void Test3Task(void *p_arg)
{
	(void) p_arg;
	
	BD2=1;			//初始灯不亮
	while(1)
	{
		INT8U err;
		OSSemPend(Sem2,0,&err);
		BD2=0;
		OSTimeDly(OS_TICKS_PER_SEC/10);
		BD2=1;
		OSSemPost(Sem3);
	}
}

void Test4Task(void *p_arg)
{
	(void) p_arg;
	
	BD3=1;
	while(1)
	{
		INT8U err;
		OSSemPend(Sem3,0,&err);
		BD3=0;
		OSTimeDly(OS_TICKS_PER_SEC/10);
		BD3=1;
		OSSemPost(Sem4);
	}
}*/

//OS_EVENT *Sem;
/*void Test1Task(void *p_arg)		//按键任务
{
	(void) p_arg;
	
	OSTaskCreate(Test2Task,
				 (void *)0,
				 &Test2TaskStk[Test2_TASK_STK_SIZE-1],
				 TEST2_TASK_PRIO);
	//Sem=OSSemCreate(0);
	while(1)
	{
		OSTimeDly(2);
		if(SIU.GPDI[1].B.PDI!=1)
			continue;
		else
		{
			BD1=~BD1;
			while(1)
			{
				if(SIU.GPDI[1].B.PDI==0)
					break;
				OSTimeDly(2);
			}
			//OSSemPost(Sem);
		}
	}
}

void Test2Task(void *p_arg)		
{
	(void) p_arg;
	while(1)
	{
		//INT8U err;
		//OSSemPend(Sem,0,&err);
		BD0=~BD0;
		OSTimeDly(OS_TICKS_PER_SEC);
	}
}*/

void Test1Task(void *p_arg)
{
    int cnt = 0;
    (void) p_arg;
    Enable_IRQ();
    while(1)
    {
        LED1 = ~LED1;
        Delay_ms(30);
        if (++cnt >= 30)
        {
            cnt = 0;
            OSTimeDly(100);
        }
	}
}

void Test2Task(void *p_arg)
{
	(void) p_arg;
	while(1)
	{
        LED2 = ~LED2;
        Delay_ms(60);
	}
}
