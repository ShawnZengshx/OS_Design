#include "hardware.h"


//--------------------------------------------------Cpu�ຯ��------------------------------------------------------------ 

//���캯�� 
Cpu::Cpu() 
{
	PC = 1;//���������
	IR = 0;//ָ��Ĵ��� 
	PSW = 0;//0Ϊ���У�1Ϊæµ ,//״̬�Ĵ���
}

//CPU�ֳ���������
int Cpu::Sceneprotection(Process &e)
{
	e.PSW = IR;
	PSW = 0;
	return 0;
}

//CPU�ֳ��ָ�����
int Cpu::Scenerevover(Process e)
{
	PC = e.ProID;
	IR = e.PSW;
	PSW = 1;
	return 0;
}


//---------------------------------------------Memory�ຯ���ڴ�(�û���)------------------------------------------------------------ 

//���캯�� 
Memory::Memory()
{
	//�ڴ������Ϣ 
	SpaceSize = 100*1000;//��λB 
	BlockNum = 100;
	BlockSize = 1000;
	
	//�ڴ������Ϣ
	Block *temp = new Block[100]; 
	for(int i = 0; i < 100;i++)//��ʼ���ڴ�������
	{
		temp[i].InitBlock(i);
		block_table.InsertBlock(temp[i],i+1);
	} 
	FreeSpace = 100;
	UsedSpace = 0; 
}

//������Դ
/*
����ֵ��1 ����>����ɹ�
		0 ����> ����ʧ�� 
*/ 
int Memory::AllocationSpace(int size,int JobId,Page_Table &page_table)
{
	if((size-5) <= FreeSpace)//������п������㣬����� 
	{
		//��ʼ��ҳ��
		for(int k = 0;k < size;k++)
		{
			Page page;
			if(k < size -5)
			{
				page.Init(k,1);//ѡ����ҵ�Ĳ��ֵ����ڴ�
				page_table.InsertPage(page,k+1); 
			}
			else
			{
				page.Init(k,0);
				page_table.InsertPage(page,k+1); 
			}
		} 
		
		int ii = 1;
		int j = 1;
		size = size -5;
		while(size > 0)
		{
			if(block_table.block[ii].BlockState == 0)//�ҳ����еĿ� 
			{
				block_table.block[ii].BlockState = 1;//��ռ�ñ�־ 
				block_table.block[ii].OwnerPro = JobId;//�ÿ���������ҵ 
				page_table.page[j].BlockId = block_table.block[ii].BlockId;//���ҵ���λ�ÿ�ż����Ӧҳ��ţ�������̵�ҳ�� 
				FreeSpace--;//�ӿ��п����м�ȥ����ռ�ÿ��� 
				UsedSpace++;
				size--;
				j++;
				//��Ӧ��ҳ��Ӧ 
			}
			ii++;
		} 
		return 1;//����ɹ� 
	}
	else//������п�������������̵ȴ� 
	{
		//���̵ȴ� 
		return 0;//����ʧ�� 
	} 
}

//������Դ 
int Memory::RecycleSpace(int size,int ProId)
{
	int i = 0;
	while(size > 0)
	{
		if(block_table.block[i].OwnerPro == ProId)//�ҳ���Ҫ�黹�Ŀ� 
		{
			block_table.block[i].BlockState = 0;//��ռ�ñ�־Ϊ0 
			block_table.block[i].OwnerPro = -1;//���ռ�н��̱�־λ 
			FreeSpace++;//�ӿ��п����м�ȥ����ռ�ÿ��� 
			UsedSpace--;
			size--;
		}
		i++;
	} 
}

//��ӡ�ڴ�
void Memory::Print(ofstream &file)
{
	file<<"ʣ��ռ䣨�飩��"<<FreeSpace<<"\r\n"<<"���ÿռ䣨�飩��"<<UsedSpace<<"\r\n";
	file<<"�ڴ�������������һ������0Ϊ���У�1Ϊռ��;�ڶ�������-1��ʾδ������κν��̣��������ֱ�ʾռ�иĿ�Ľ���ID��:"<<"\r\n";
	for(int i = 1;i <= block_table.length;i++)
	{
		file<<block_table.block[i].BlockId<<"("<<block_table.block[i].BlockState<<","<<block_table.block[i].OwnerPro<<")";
		file<<"    ";
		if(i % 10 == 0)
		{
			file<<"\r\n";
		}
	} 
	file<<"\r\n";
} 


//--------------------------------------------------MMU�ຯ��------------------------------------------------------------ 

//���캯�� 
MMU::MMU() 
{
	page_table_addr = NULL;	
}

void MMU::go(ofstream &file,Page_Table &page_table,int addr,int &pageid,int &pianyi,int &paddr)
{
	
	PageTableAddr(page_table);
	BreakAddr(addr,pageid,pianyi);	
	cout<<"�߼���ַ�ֽ����:ҳ��Ϊ"<<pageid<<",ƫ�Ƶ�ַΪ"<<pianyi<<endl; 
	file<<"�߼���ַ�ֽ����:ҳ��Ϊ"<<pageid<<",ƫ�Ƶ�ַΪ"<<pianyi<<endl; 
	VisitPageTable(file,pageid,pianyi,paddr);
}
//����ҳ���ַ�Ĵ��� 
//(�����̱����ȵ�CPU������ʱ������ϵͳ�Զ��Ѵ˽���PCB�е�ҳ����ʼ��ַװ��Ӳ��ҳ���ַ�Ĵ�����
//�˺󣬽��̿�ʼ���в�����ĳ���߼���ַ��MMU��ʼ����)
void MMU::PageTableAddr(Page_Table &page_table)
{
	page_table_addr = &page_table;
}

//�ֽ��߼���ַ
void MMU::BreakAddr(int addr,int &pageid,int &pianyi)
{
	pageid = addr/1000;
	pianyi = addr%1000;
}

//����ҳ��
void MMU::VisitPageTable(ofstream &file,int pageid,int pianyi,int &paddr)
{
	Page temp;
	page_table_addr->SearchPageId(pageid,temp);//ҳ��Ϊ��������ҳ��
	if(temp.Dwell == 1)//���ҳ������,���ͳ�ҳ��ţ�����ҳ��λ��ƴ�ӳ������ַ
	{
		paddr = temp.BlockId * 1000 + pianyi;
		cout<<"ҳ�����У������ַΪ"<<paddr<<endl;
		file<<"ҳ�����У������ַΪ"<<paddr<<endl;
	}
	else
	{
		MissingPage(pageid);//ȱҳ�쳣 
		cout<<"ҳ��δ���У�����ȱҳ�жϣ�"<<endl;
		cout<<"�滻�ɹ���"<<endl;
		cout<<"���������ж�ָ�"<<endl;
		file<<"ҳ��δ���У�����ȱҳ�жϣ�"<<endl;
		file<<"�滻�ɹ���"<<endl;
		file<<"�ж�ָ�����гɹ���"<<endl;
	}
}

//�����쳣
void MMU::MissingPage(int id)
{
	//page_table_addr->SearchPageId(id,temp);//����ҳ�ţ�������ҳ�� 
	page_table_addr->page[id].Dwell = 1;//�����ڴ� 
	page_table_addr->page[id].BlockId = id*987%100;
}















