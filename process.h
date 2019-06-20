#ifndef PROCESS_H
#define PROCESS_H

//���ͷ�ļ����� 
#include "utils.h"
#include "hardware.h"

//hardware.hͷ�ļ��е���������� 
class Cpu;//cpu
class Memory;//�ڴ�
class Time;

//process.h��������� 
class Instruc;//ָ���� 
class Process;//������ 
class PCB_Table;//���̱��� 
class LinkQueue;//�������� 

//------------------------------------------ָ����-------------------------------------------------- 
class Instruc
{
	public:
		int ID;//ָ��ID
		int State;//ָ��״̬��־��1Ϊ�û�̫������2ΪI/O���� 
		int time;//����ָ������ʱ�� 
		int addr;//ָ���ַ,ID*1000+Random[0,999] 
		
		Instruc();//���캯�� 
		void Init(int id,Time &timeq);//���캯�� 
		
		void changeID(int temp);  //�޸�ָ��ID 
		void changeState(int temp);  //�޸�ָ��״̬ 

	protected:		
};


//-----------------------------------------������--------------------------------------------------------- 
class Process
{
	public:	
		//���� 
		int ProID;//���̱�� 
		int JobId;//���̶�Ӧ��ҵID 
		int Priority;//���������� 
		int InTimes;//���̴���ʱ�� 
		int ProState;//����״̬ 
		int RunTimes;//��������ʱ�� 
		int PSW;//����ý��̵�ǰִ�е�ָ���� 
		int InstrucNum;//���̰�����ָ����Ŀ 
		Instruc *instruc_arry;//���̰�����ָ������ָ�� 
		int alltime;//������תʱ�� 
		
		//���� 
		Process();
		Process(int id,int intime,int instrucnum,Time &timeq);
		void proinit(int m_ID,int m_priority,int m_intimes);//���̵ĳ�ʼ�� 
		int profork(PCB_Table &L,LinkQueue &ready);//���̵Ĵ��� 
		int prokill(PCB_Table &L,LinkQueue &finish);//���̵���ֹ
		int problock(PCB_Table &L,LinkQueue &finish,Cpu CPU);//���̵�����
		int procall(PCB_Table &L,LinkQueue &ready,LinkQueue &block);//���̵Ļ���
		//int prorun(PCB_Table &L,LinkQueue &ready,LinkQueue &block);//���̵����� 
		
		//void init_instrucID(); //����ָ��ID 
				
	protected:				
};

//-----------------------------------------PCB����----------------------------------------------- 
class PCB_Table
{
	public:
		//���� 	
		Process *process; // 
		int length;   //�� 
		
		//���� 
		PCB_Table(); //���캯�� 
		
		void InsertPCB(Process x,int i);//��PCB��ĵ�i������Ԫ��֮ǰ����һ������x (PCB���0λ�ճ�)
		void SearchProID(int id,Process &temp);//���ݽ���ID������PCB���е�ĳ������ �����ظ�ͬ��Ĳ���  
		void DeleteProID(int id);//���ݽ���ID����PCB����ɾ����Ӧ���� 
		void PrintTable(char *a);//��ӡ�� 
				
	protected:
};


//-----------------------------------------��������-------------------------------------------------
//�����н�����Ͷ���
typedef struct QNode  
{
	Process data;
	struct QNode *next;
}QNode,*QueuePtr;

class LinkQueue
{
	public:
		//����		
		QueuePtr front; //ͷָ�룬data�����Ԫ��
		QueuePtr rear;  //βָ�룬data������һ��Ԫ��
		
		//���� 
		LinkQueue();

		void EnQueue(Process e);  //�����
		void DeQueue(Process &e); //������
		void GetTop(Process &e);  //ȡ����ͷԪ��
		void PrintQueue(); //��������
		int LenQueue();   //ȡ���г���
		void MinPriority(Process &e);//ÿ��ѡȡ��С���ȼ��Ľ���
		void sort();
		//void DeleteQueue(int v);
		
	protected:
};


#endif

