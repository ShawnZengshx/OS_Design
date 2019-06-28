//相关头文件声明 
#include "utils.h"
#include "windows.h"  //使用系统级线程
#include "process.h"
#include "hardware.h" 
#include "job.h"


//传入线程参数的结构体 
typedef struct node {
	Memory memory;//建立内存 
	JCB_Table jcb_table;//建立作业表
	Pool_Queue pool_queue;//建立后备队列 
	Job_Scheduler job_scheduler;//建立作业调度器 
	PCB_Table pcb_table;//建立PCB表
	Page_Table page_table[JOB_NUM+1];//建立JOB_NUM张页表 
	LinkQueue ready;//就绪队列
	LinkQueue run;//运行队列 
	LinkQueue block;//阻塞队列
	LinkQueue finish;//完成队列 
	ofstream fmemory;//内存文件 
	ofstream fjob;//作业文件 
	ofstream frun;//运行情况保存文件 
	MMU mmu;
	Time time;//时钟（随机数生成） 
	int k; //内存互斥锁 
}DATA;

//线程主体函数 
DWORD WINAPI ThreadFun(LPVOID pM);


int kernel_main();