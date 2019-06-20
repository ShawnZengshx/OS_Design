#include "page.h" 

//-----------------------------------------ҳ������----------------------------------------------------------
//���캯��
Page::Page()
{
	PageId = -1;//ҳ��,��0��ʼ���α�� 
	BlockId = -1;//�ڴ��ţ���ҳ���С��ȣ���0���˿�ʼ��� 
	Dwell = 0;//פ����־λ��0��ʾ�����ڴ棬1��ʾ���ڴ�
	Yinyong = 0;//����λ
	Modifi = 0;//�޸�λ��0��ʾδ���޸ģ�1��ʾ���޸� 
} 

void Page::Init(int id,int dwell)
{
	PageId = id;
	Dwell = dwell;
}
//------------------------------------------ҳ���ຯ��ʵ��ģ��--------------------------------------------------------------------------
//���캯�� 
Page_Table::Page_Table()
{
	page = new Page[25];   //��ʼ��ҳ��ռ�(��ʼ25) 
	length = 0;   // PCB�н��̸�����ʼ��Ϊ0
}

//��Page_Table��ĵ�i������Ԫ��֮ǰ����һ��ҳ����x (ҳ���0λ�ճ�)
void Page_Table::InsertPage(Page x,int i)
{
	int k;
	if(i < 1 || i > length + 1)  //�жϲ���λ���Ƿ�Ϸ������Ϸ����������ʾ
	{
		cout<<"����λ�ô��󣡣�"<<endl;
	}
	else
	{
		length++;
		for(k = length -1;k > i - 1;k--)
		{
			page[k+1] = page[k];
		}	
		page[k+1] = x;
	}
} 

//����ҳ�ţ���Pҳ����ɾ����Ӧҳ���� 
void Page_Table::DeletePageId(int id)
{
	int i;
	int flag=0;
	for(i = 1;i < length + 1;i++)
	{
		if(page[i].PageId == id)
		{
			int k = i;
			for(k=k+1;k < length + 1;k++)
			{
				page[k-1] = page[k];
			}
			length--;
			flag = 1;
			break;
		}
	}
	if(flag == 0)
	{
		cout<<"ҳ����û�и�ҳ��"<<endl;
	}
} 

//����ҳ�ţ�����ҳ���е�ĳ��ҳ���� �����ظ�ͬ��Ĳ��� 
void Page_Table::SearchPageId(int id,Page &temp)
{
	int i;
	int flag = 0;
	for(i = 1;i < length + 1;i++)
	{
		if(page[i].PageId == id)
		{
			temp = page[i];
			flag = 1;
			break;
		}
	}
	if(flag == 0)
	{
		cout<<"ҳ����û��ҳ��"<<endl;
	}
} 

void Page_Table::PrintTable(char *a)
{
	ofstream file;
	file.open(a,ios::trunc); //ios::trunc��ʾ�ڴ��ļ�ǰ���ļ����
		//int PageId;//ҳ��,��0��ʼ���α�� 
	//	int BlockId;//�ڴ��ţ���ҳ���С��ȣ���0���˿�ʼ��� 
	//	int Dwell;//פ����־λ��0��ʾ�����ڴ棬1��ʾ���ڴ�
	//	int Yinyong;//����λ
	//	int Modifi;//�޸�λ��0��ʾδ���޸ģ�1��ʾ���޸� 
	file<<"PageId"<<setw(15)<<"BlockId"<<setw(15)<<"Dwell"<<setw(15)<<"Yinyong"<<setw(15)<<"Modifi";
	for(int i = 1;i <= length;i++)
	{
		file<<"\r\n"<<page[i].PageId<<setw(15)<<page[i].BlockId<<setw(15)<<page[i].Dwell<<setw(15)
		<<page[i].Yinyong<<setw(15)<<page[i].Modifi<<setw(15);
	}
	file.close();
}

//-----------------------------------------�������---------------------------------------------------------
//���캯��
Block::Block()
{
	BlockId = -1;//�����ID
	BlockState = 0;//��������״̬��0��ʾ���У�1��ʾռ�У�
	OwnerPro = -1;//������Ľ���ID 
} 

//������ʼ�� 
void Block::InitBlock(int id)
{
	BlockId = id;
} 


//------------------------------------------Block_Table�ຯ��ʵ��ģ��--------------------------------------------------------------------------
//���캯�� 
Block_Table::Block_Table()
{
	block = new Block[200];   //��ʼ��Block_Tabel�ռ�(��ʼ200��) 
	length = 0;   // PCB�н��̸�����ʼ��Ϊ0
}

//��Block_Table��ĵ�i������Ԫ��֮ǰ����һ��Block x (Block_Table���0λ�ճ�)
void Block_Table::InsertBlock(Block x,int i)
{
	int k;
	if(i < 1 || i > length + 1)  //�жϲ���λ���Ƿ�Ϸ������Ϸ����������ʾ
	{
		cout<<"����λ�ô��󣡣�"<<endl;
	}
	else
	{
		length++;
		for(k = length -1;k > i - 1;k--)
		{
			block[k+1] = block[k];
		}	
		block[k+1] = x;
	}
} 

//���������ID������������ɾ����Ӧ����� 
void Block_Table::DeleteBlockID(int id)
{
	int i;
	int flag=0;
	for(i = 1;i < length + 1;i++)
	{
		if(block[i].BlockId == id)
		{
			int k = i;
			for(k=k+1;k < length + 1;k++)
			{
				block[k-1] = block[k];
			}
			length--;
			flag = 1;
			break;
		}
	}
	if(flag == 0)
	{
		cout<<"Block_Table����û�и�ID�������"<<endl;
	}
} 

//���������ID�������������е�ĳ������飬���ظ�ͬ��Ĳ���
void Block_Table::SearchBlockID(int id,Block &temp)
{
	int i;
	int flag = 0;
	for(i = 1;i < length + 1;i++)
	{
		if(block[i].BlockId == id)
		{
			temp = block[i];
			flag = 1;
			break;
		}
	}
	if(flag == 0)
	{
		cout<<"Block_Table����û�и�ID�������"<<endl;
	}
} 


