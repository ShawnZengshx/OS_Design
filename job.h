#ifndef JOB_H
#define JOB_H

//���ͷ�ļ����� 
#include "utils.h"
#include "process.h"
#include "hardware.h"

//process.h��������� 
class Instruc;//ָ���� 
class Process;//������ 
class PCB_Table;//���̱��� 
class LinkQueue;//�������� 

//job.h��������� 
class JCB;//JCB(��ҵ���ƿ�)
class JCB_Table;//JCB��
class Pool_Queue;//�󱸶���
class Job_Scheduler;//��ҵ���ȳ��� 

//hardware.h��������� 
class Cpu;//cpu
class Memory;//�ڴ�
class Time;
class MMU;


//JCB
class JCB
{
	public:	
		//����: 
		//��ҵ��� 
		char UserName[10];//�û���,Ĭ��Ϊzhangying 
		int JobId;//��ҵ��
		int ProId;//��ҵ��Ӧ����
		//��Դ���� 
		int RunTime;//CPU���й���ʱ��
		int DeadLine;//��ֹʱ��
		int MemorySize;//�����ڴ�����(��ҳΪ��λ��10����20ҳ)
		//��Դʹ����� 
		int InTime;//����ϵͳʱ��
		int StarTime;//��ʼ����ʱ��
		int MemoryAddress;//�ڴ��ַ 
		 	
		//����: 
		JCB(); //���캯��
		void Init(int jobid,int intime,int memorysize);//��ʼ������ 
				
	protected:				
};

//JCB_Table
class JCB_Table
{
	public:	
		//����: 
		JCB *jcb; // �洢�ռ��ַ 
		int length;   //�� 
		
		//����:
		JCB_Table();
		
		void InsertJCB(JCB x,int i);//��JCB��ĵ�i������Ԫ��֮ǰ����һ��JCBx (JCB���0λ�ճ�)
		void SearchJobId(int id,JCB &temp);//������ҵ�ţ�����JCB���е�ĳ����ҵ�����ظ�ͬ��Ĳ���  
		void DeleteJobId(int id);//������ҵ�ţ���JCB����ɾ����ӦJCB
		void PrintTable(ofstream &file);//��ӡ��ҵ�� 
		
				
	protected:				
};

//Pool_Queue(�󱸶���)
//�����н�����Ͷ���
typedef struct QNode_J  
{
	JCB data;
	struct QNode_J *next;
}QNode_J,*QueuePtr_J;

class Pool_Queue
{
	public:	
		//���� 
		QueuePtr_J front; //ͷָ�룬data�����Ԫ��
		QueuePtr_J rear;  //βָ�룬data������һ��Ԫ��
		
		//���� 
		Pool_Queue();

		void EnQueue(JCB e);  //�����
		void DeQueue(JCB &e); //������
		void GetTop(JCB &e);  //ȡ����ͷԪ��
		void PrintQueue(); //��������
		int LenQueue();   //ȡ���г���
		void MinPriority(JCB &e);//ÿ��ѡȡ��С���ȼ��Ľ���
		void sort();
		//void DeleteQueue(int v);
		
		
				
	protected:				
};

/*
2����������ҵ�ĵ��ȣ���ҵ���ȳ����������������
	a.ѡ����ҵ�������ȷ����㷨
	b.������Դ����ҵ���ȳ�����洢����������ͨѶ��Ϊ��ҵ�����������Դ��
	c.�������̡�ÿ��ѡ����ҵ�ҽ���װ���ڴ�ʱ��ϵͳ��Ϊ����ҵ����Ӧ�ý��̣�����PCB����Ӧ����ʵ�壬��Щ���̽��ڵͼ����ȿ�����ռ�ô��������С�����ͬʱװ������ҵ���������У�
	d.����������ҵ���������������ֹʱ����ҵ���ȳ���Ҫ������ҵ��������ƹ��������ӡ�����Ϣ�����ո�����Դ������JCB�ȣ�ͬʱ��������ҵ���ȳ���ѡ������ҵ�����ڴ棻
*/
//Job_Scheduler(��ҵ���ȳ��� 
class Job_Scheduler
{
	public:	
		//���� 
		
		
		//���� 
		Job_Scheduler();
		int go(Pool_Queue &pool_queue,JCB &temp,Memory &memory,Page_Table &page_table,Time &timeq,PCB_Table &pcb_table,LinkQueue &ready,JCB_Table &jcb_table);
		void ChooseJob(Pool_Queue &pool_queue,JCB &temp);//ѡ����ҵ(�����ȷ����㷨),�Ӻ󱸶�����ȡ��һ��JCB��temp 
		int AllocatinResource(Memory &memory,JCB jcb,Page_Table &page_table,Pool_Queue &pool_queue,JCB_Table &jcb_table);//������Դ 
		int CreateProcees(JCB temp,Time &timeq,PCB_Table &pcb_table,LinkQueue &ready);//��������
		int FollowWork();//�������� 
				
	protected:				
};

#endif


