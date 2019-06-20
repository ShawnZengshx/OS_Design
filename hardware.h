#ifndef HARDWARE_H
#define HARDWARE_H

//���ͷ�ļ����� 
#include "utils.h"
#include "process.h"
#include "page.h" 

//process.hͷ�ļ������������ 
class Instruc; 
class PCB_Table;
class LinkQueue;
class Process;

//page.hͷ�ļ���������� 
class Block;//�����
class Block_Table;//������ 
class Page;//ҳ����
class Page_Table;//ҳ�� 

//hardware.h��������� 
class Cpu;//cpu
class Memory;//�ڴ�
class Time;
class MMU;


//CPU
class Cpu
{
	public:
		//���� 
		int PC;   //��������� 
		int IR;   //ָ��Ĵ��� 
		int PSW;  //״̬�Ĵ���
		
		//���� 
		Cpu();    //���캯�� 
		int Sceneprotection(Process &e);//CPU�ֳ���������
		int Scenerevover(Process e);//CPU�ֳ��ָ����� 
	protected:
		 
};

//�ڴ�(�û���)
class Memory
{
	public:
	//����
	//�ڴ������Ϣ 
	int SpaceSize;//�ڴ�ռ��С (Ĭ��100*1000B)
	int BlockNum;//�����������100��
	int BlockSize;//������С��1000�� 
	
	//�ڴ������Ϣ
	Block_Table block_table;//�ڴ������� 
	int FreeSpace;//ʣ��ռ�
	int UsedSpace;//���ÿռ� 
		
	//���� 
	Memory();
	int AllocationSpace(int size,int JobId,Page_Table &page_table);//������Դ
	int RecycleSpace(int size,int ProId);//������Դ 
	void Print(ofstream &file);//��������������ļ� 
}; 


//MMU
class MMU
{
	public:
		//���� 
		Page_Table *page_table_addr;//Ӳ��ҳ���ַ�Ĵ��� 
		
		//���� 
		MMU();    //���캯�� 
		void go(ofstream &file,Page_Table &page_table,int addr,int &pageid,int &pianyi,int &paddr);
		void PageTableAddr(Page_Table &page_table);//����ҳ���ַ�Ĵ��� 
		void BreakAddr(int addr,int &pageid,int &pianyi);//�ֽ��߼���ַ
		void VisitPageTable(ofstream &file,int pageid,int pianyi,int &paddr);//����ҳ��
		void MissingPage(int id);//�����쳣 
		
	protected:
};

//-----------------------------------Time��---------------------------------------------------------
class Time
{
private:
	struct 
	{
		int hour;
		int min;
		int sec;
	}time_hmc;
	public:
		//��ȡ��ǰʱ�亯��
		void GetTime()
		{
			time_t timep;
			struct tm *p;
			time(&timep);  //ת��������
			p = localtime(&timep);  //ʱ�������յȽṹ��
			time_hmc.hour = p->tm_hour;
			time_hmc.min = p->tm_min;
			time_hmc.sec = p->tm_sec;
		}
		
		//���������
		int RandNum(int start,int end)
		{
			
			return start+(rand()%(end - start + 1));
		}
		
		void test()
		{
			cout<<time_hmc.hour<<":"<<time_hmc.min<<":"<<time_hmc.sec<<"\t"<<"\n";  //���߳�֮��ʹ��endl������ջ�������ͻ
		}
};

#endif


