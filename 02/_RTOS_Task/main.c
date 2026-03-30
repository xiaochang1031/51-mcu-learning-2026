#include <REGX52.H>
#include <INTRINS.H>

#define MAX_TASKS 2 //表示有两个任务
#define MAX_TASK_DEP 32//表示栈的深度

unsigned char idata task_sp[MAX_TASKS];//任务的堆栈指针（存任务的）
unsigned char idata task_stack[MAX_TASKS][MAX_TASK_DEP];//每个task任务的堆栈
unsigned char task_id;//当前任务号
unsigned char task_state[MAX_TASKS];//任务状态：0=挂起，1=运行
unsigned int task_delay[MAX_TASKS];//每个任务的延时时间

//void Delay1000ms(void);		
void task_switch(void);
void task0(void);
void task1(void);
void task_load(unsigned int fn,unsigned char tid);
void task_suspend(void);
void task_resume(unsigned char tid);
void Timer0_Init(void);
void task_delay_ms(unsigned int ms);

void main()
{
	Timer0_Init();		//初始化定时器，提供1ms系统节拍
	//加载任务：初始化任务堆栈
	task_load((unsigned int)task0,0);
	task_load((unsigned int)task1,1);
	//初始化任务状态为运行
	task_state[0] = 1;
	task_state[1] = 1;

	task_id = 0;		//从任务0开始运行
	SP=task_sp[0];		//设置CPU堆栈指针

	task0();			//启动第一个任务
}

//void Delay1000ms()		
//{
//	unsigned char i, j, k;
//
//	_nop_();
//	i = 8;
//	j = 154;
//	k = 122;
//	do
//	{
//		do
//		{
//			while (--k);
//		} while (--j);
//	} while (--i);
//}

void task_switch()
{
   task_sp[task_id]=SP;//保存当前的任务的SP
	//查找下一个可运行非挂起任务
   do
   {
	   task_id=task_id+1;
		if(task_id==MAX_TASKS)
		{
		  task_id=0;
		}
   } while (task_state[task_id]==0);//跳过挂起的任务

	
	SP=task_sp[task_id];//恢复下一个任务
}
//P2_0每500ms闪烁一次
void task0()
{
	P2_0=0;
	while(1)
	{
		task_delay_ms(500);
		P2_0=~P2_0;
	}
}
//P2_7每1000ms闪烁一次
void task1()
{
	P2_7=0;
	while(1)
	{
	  task_delay_ms(1000);
		P2_7=~P2_7;
		
	}
}
	
void task_load(unsigned int fn,unsigned char tid)
{
  //task_sp[tid]=  task_stack[tid]+1;//跳到下一个任务
	task_stack[tid][1] = (unsigned char)(fn >> 8); 
    task_stack[tid][0] = (unsigned char)fn; 
	//设置该任务的栈顶指针
	task_sp[tid] = (unsigned char)&task_stack[tid][1];
}

void task_suspend(void)
{
	task_state[task_id] = 0;//状态设为挂起
	task_switch();			//立刻切换走
}
//恢复指定任务
void task_resume(unsigned char tid)
{
	task_state[tid] = 1;	//切换为可运行
}

//void task_init(void)
//{
//	task_state[0] = 1; //任务0 运行
//	task_state[1] = 1; //任务1 运行
//}

void task_delay_ms(unsigned int ms)
{
	task_delay[task_id] = ms;	//给当前任务设置延时时间
	task_state[task_id] = 0;	//挂起当前任务
	task_switch();				//切换到别的任务
}
// 定时器初始化
void Timer0_Init(void)
{
	TMOD = 0x01;
	TH0 = 0xFC;
	TL0 = 0x18;
	ET0 = 1;
	EA = 1;
	TR0 = 1;
}

void Timer0_ISR(void) interrupt 1
{
	unsigned char i;
	TH0 = 0xFC;				//重装初值
	TL0 = 0x18;
	//遍历所有任务，自动恢复任务
	for (i = 0; i < MAX_TASKS; i++)
	{
		if (task_delay[i] > 0)
		{
			task_delay[i]--;
			//时间到了，恢复任务
			if (task_delay[i] == 0)
			{
				task_state[i] = 1;
			}
		}
	}
}

