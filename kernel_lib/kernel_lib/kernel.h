//���ͷ�ļ����� 
#include "utils.h"
#include "windows.h"  //ʹ��ϵͳ���߳�
#include "process.h"
#include "hardware.h" 
#include "job.h"


//�����̲߳����Ľṹ�� 
typedef struct node {
	Memory memory;//�����ڴ� 
	JCB_Table jcb_table;//������ҵ��
	Pool_Queue pool_queue;//�����󱸶��� 
	Job_Scheduler job_scheduler;//������ҵ������ 
	PCB_Table pcb_table;//����PCB��
	Page_Table page_table[JOB_NUM+1];//����JOB_NUM��ҳ�� 
	LinkQueue ready;//��������
	LinkQueue run;//���ж��� 
	LinkQueue block;//��������
	LinkQueue finish;//��ɶ��� 
	ofstream fmemory;//�ڴ��ļ� 
	ofstream fjob;//��ҵ�ļ� 
	ofstream frun;//������������ļ� 
	MMU mmu;
	Time time;//ʱ�ӣ���������ɣ� 
	int k; //�ڴ滥���� 
}DATA;

//�߳����庯�� 
DWORD WINAPI ThreadFun(LPVOID pM);


int kernel_main();