#ifndef HARDWARE_H
#define HARDWARE_H
//���ͷ�ļ����� 
#include "utils.h"
#include "process.h"
#include "page.h" 
#define _CRT_SECURE_NO_WARNINGS 0
#pragma warning( disable : 4996 )


//process.hͷ�ļ������������ 
class Instruc; 
class PCB_Table;
class LinkQueue;
class Process;

//page.hͷ�ļ���������� 
class Block;//������
class Block_Table;//������� 
class Page;//ҳ����
class Page_Table;//ҳ�� 
class BlockQueue; //��������� 

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
		int NowProcessID;   //��������� 
		int CurrentInsID;   //��ǰָ��ID 
		int PSW;  //״̬�Ĵ���
		
		//���� 
		Cpu();    //���캯�� 
		int Sceneprotection(Process &e);//CPU�ֳ���������
		int SceneRecover(Process e);//CPU�ֳ��ָ����� 
	protected:
		 
};

//�ڴ�(�û���)
class Memory
{
	public:
	//����
	//�ڴ������Ϣ 
	int SpaceSize;//�ڴ�ռ��С (Ĭ��100*1000B)
	int BlockNum;//������������100��
	int BlockSize;//�������С��1000�� 
	
	//�ڴ������Ϣ
	Block_Table block_table;//�ڴ�������� 
	int FreeSpace;//ʣ��ռ�
	int UsedSpace;//���ÿռ� 
		
	//���� 
	Memory();
	int AllocationSpace(int size,int JobId,Page_Table &page_table, BlockQueue &block_queue);//������Դ
	int RecycleSpace(int size,int ProId);//������Դ 
	void Print(ofstream &file);//���������������ļ� 
}; 


//MMU
class MMU
{
	public:
		//���� 
		Page_Table *page_table_addr;//Ӳ��ҳ����ַ�Ĵ��� 
		
		//���� 
		MMU();    //���캯�� 
		void go(ofstream &file,Page_Table &page_table,int addr,int &pageid,int &offset,int &paddr, BlockQueue &cur_block_queue);
		void PageTableAddr(Page_Table &page_table);//����ҳ����ַ�Ĵ��� 
		void BreakAddr(int addr,int &pageid,int &offset);//�ֽ��߼���ַ
		void VisitPageTable(ofstream &file,int pageid,int offset,int &paddr, BlockQueue &cur_block_queue);//����ҳ��
		void MissingPage(int id,BlockQueue &cur_block_queue);//�����쳣 
		
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

