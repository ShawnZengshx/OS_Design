#ifndef PAGE_H
#define PAGE_H

//���ͷ�ļ����� 
#include "utils.h"

//page.h��������� 
class Block;//�����
class Block_Table;//������ 
class Page;//ҳ����
class Page_Table;//ҳ�� 

//-----------------------------------------ҳ������--------------------------------------------------------- 
class Page
{
	public:
		//���� 
		int PageId;//ҳ��,��0��ʼ���α�� 
		int BlockId;//�ڴ��ţ���ҳ���С��ȣ���0���˿�ʼ��� 
		int Dwell;//פ����־λ��0��ʾ�����ڴ棬1��ʾ���ڴ�
		int Yinyong;//����λ
		int Modifi;//�޸�λ��0��ʾδ���޸ģ�1��ʾ���޸� 
		
		//���� 
		Page();
		void Init(int id,int dwell);//��ʼ������ 
		
				
	protected:				
};

//-----------------------------------------ҳ����--------------------------------------------------------- 
class Page_Table
{
	public:
		//���� 
		Page *page; //��ַ 
		int length;   //�� 
		
		//���� 
		Page_Table();
		
		void InsertPage(Page x,int i);//��Page_Table��ĵ�i������Ԫ��֮ǰ����һ��ҳ����x (ҳ���0λ�ճ�)
		void SearchPageId(int id,Page &temp);//����ҳ�ţ�����ҳ���е�ĳ��ҳ���� �����ظ�ͬ��Ĳ���  
		void DeletePageId(int id);//����ҳ�ţ���Pҳ����ɾ����Ӧҳ���� 
		void PrintTable(char *a);//��ӡ�� 
						
	protected:				
};

//-----------------------------------------�������--------------------------------------------------------- 
class Block
{
	public:
		//���� 
		int BlockId;//�����ID
		int BlockState;//��������״̬��0��ʾ���У�1��ʾռ�У�
		int OwnerPro;//������Ľ���ID 
		
		//���� 
		Block();
		
		void InitBlock(int id);//������ʼ�� 
				
	protected:				
};

//-----------------------------------------��������----------------------------------------------- 
class Block_Table
{
	public:
		//���� 
		
		Block *block; // ��ַ 
		int length;   //�� 
		
		//���� 
		Block_Table(); //���캯�� 
		
		void InsertBlock(Block x,int i);//��Block_Table��ĵ�i������Ԫ��֮ǰ����һ��Block x (Block_Table���0λ�ճ�)
		void SearchBlockID(int id,Block &temp);//���������ID�������������е�ĳ������飬���ظ�ͬ��Ĳ���  
		void DeleteBlockID(int id);//���������ID������������ɾ����Ӧ����� 
		
				
	protected:
};


#endif
