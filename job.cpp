#include "job.h"

/*
-----------------------------------JCB��-----------------------------------------
*/

//���캯��
JCB::JCB()
{
	strcpy(UserName,"zhangying");//�û���,Ĭ��Ϊzhangying 
	JobId = 0;//��ҵ��
	
	//��Դ���� 
	RunTime = 0;//CPU���й���ʱ��
	DeadLine = 0;//��ֹʱ��
	MemorySize = 0;//�����ڴ�����
	
	//��Դʹ����� 
	InTime = 0;//����ϵͳʱ��
	StarTime = 0;//��ʼ����ʱ��
	MemoryAddress = 0;//�ڴ��ַ 
	
	ProId = 0;//��ҵ��Ӧ���� 
	
}

//��ʼ������ 
void JCB::Init(int jobid,int intime,int memorysize)
{
	JobId = jobid;//��ҵ��
	
	//��Դ���� 
	MemorySize = memorysize;//�����ڴ�����
	
	//��Դʹ����� 
	InTime = intime;//����ϵͳʱ��
	//MemoryAddress = memoryaddress;//�ڴ��ַ
	cout<<"���µ���ҵ����ϵͳ.."<<endl; 
	cout<<"��Ϊ����ҵ������ҵ���ƿ飬��ҵIDΪ��"<<jobid<<endl; 
} 

/*
-----------------------------------JCB_Table��-----------------------------------------
*/

//���캯��
JCB_Table::JCB_Table()
{
	jcb = new JCB[20];   //�����ʼ����ҵ��ռ� 
	length = 0;   // JCB������ҵ������ʼ��Ϊ0
} 

void JCB_Table::InsertJCB(JCB x,int i)
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
			jcb[k+1] = jcb[k];
		}	
		jcb[k+1] = x;
	}
	cout<<"��ҵIDΪ"<<x.JobId<<"����ҵ��������ҵ��"<<endl; 
}

//������ҵ�ţ�����JCB���е�ĳ����ҵ�����ظ�ͬ��Ĳ���  
void JCB_Table::SearchJobId(int id,JCB &temp)
{
	int i;
	int flag = 0;
	for(i = 1;i < length + 1;i++)
	{
		if(jcb[i].JobId == id)
		{
			temp = jcb[i];
			flag = 1;
			break;
		}
	}
	if(flag == 0)
	{
		cout<<"JCB����û�и�ID�Ľ���"<<endl;
	}
}

//������ҵ�ţ���JCB����ɾ����ӦJCB
void JCB_Table::DeleteJobId(int id)
{
	int i;
	int flag=0;
	for(i = 1;i < length + 1;i++)
	{
		if(jcb[i].JobId == id)
		{
			int k = i;
			for(k=k+1;k < length + 1;k++)
			{
				jcb[k-1] = jcb[k];
			}
			length--;
			flag = 1;
			break;
		}
	}
	if(flag == 0)
	{
		cout<<"JCB����û�и�ID����ҵ"<<endl;
	}
}

//��ӡ��ҵ�� 
void JCB_Table::PrintTable(ofstream &file)
{
	//ofstream file;
	//file.open(a,ios::trunc); //ios::trunc��ʾ�ڴ��ļ�ǰ���ļ����
	//char UserName[10];//�û���,Ĭ��Ϊzhangying //��Դ���� //int RunTime;//CPU���й���ʱ��//int DeadLine;//��ֹʱ��
	//int MemorySize;//�����ڴ�����(��ҳΪ��λ��10����20ҳ)//��Դʹ����� //int InTime;//����ϵͳʱ��	//int StarTime;//��ʼ����ʱ��
	//int MemoryAddress;//�ڴ��ַ //int ProId;//��ҵ��Ӧ���� 
	file<<"��ҵ��"<<setw(15)<<"�û���"<<setw(20)<<"���̺�"<<setw(15)<<"����ʱ��"<<setw(20)<<"��ֹʱ��"<<setw(20)<<"�����ڴ�"
			<<setw(20)<<"����ϵͳʱ��"<<setw(20)<<"��ʼ����ʱ��"<<setw(20)<<"�ڴ��ַ"<<endl;
	for(int i = 1;i <= length;i++)
	{
		file<<jcb[i].JobId<<setw(15)<<jcb[i].UserName<<setw(20)<<jcb[i].ProId<<setw(15)<<jcb[i].RunTime<<setw(20)<<jcb[i].DeadLine<<setw(20)<<jcb[i].MemorySize
			<<setw(20)<<jcb[i].InTime<<setw(20)<<jcb[i].StarTime<<setw(20)<<jcb[i].MemoryAddress<<endl;
	}
	file<<"\r\n";
//	file.close();
} 

/*
-----------------------------------Pool_Queue(�󱸶���)��-----------------------------------------
*/
  
//���캯��
Pool_Queue::Pool_Queue()
{
	rear = new QNode_J;  //Ϊ���е�ͷβ������ռ�
	front = rear;
	if (!front )  return ;
	front->next = NULL;
}

//�����
void Pool_Queue::EnQueue(JCB e)
{
	QueuePtr_J p;
	p = new QNode_J;
	if (!p) return;
	p->data = e; 
	p->next = NULL;
	rear->next = p;
	rear = p;
	cout<<"��ҵIDΪ"<<e.JobId<<"����ҵ"<<"������󱸶��У�"<<endl;
}

//������
void Pool_Queue::DeQueue(JCB &e)
{
	if(front == rear)
	{
		//cout<<"�ö���Ϊ�ն���"<<endl;
		return ;
	}
	QueuePtr_J p;
	p = front ->next ;
	e = p->data ;
	//printf("��������ɣ���ɾ����Ԫ��Ϊ%d\n",e);
	front ->next = p->next ;
	if (rear == p ) rear = front ; //���������ԭ��ֻ��һ��Ԫ�أ�Ҫͬʱ�޸Ķ�βָ��
	delete p;
}

//ȡ����ͷԪ��
void Pool_Queue::GetTop(JCB &e)
{
	if (rear ==  front ) 
	{
		//cout<<"�ö���Ϊ�ն���"<<endl;
		return ;
	}
	QueuePtr_J p;
	p = front ->next ;
	e = p->data;
}

//��������
void Pool_Queue::PrintQueue()
{
	if (front == rear)
	{
		//cout<<"�ö���Ϊ�ն���"<<endl;
		return;
	}
	
	QueuePtr_J p;
	p = front->next ;
	cout<<"�ö����е���ҵID���£�";
	while(p)
	{
		cout<<p->data.JobId<<",";
		p = p->next ;
	}
	cout<<endl;
}

//ȡ���г���
int Pool_Queue::LenQueue()
{
	if (front == rear )
	{
		//cout<<"�ö���Ϊ��"<<endl;
		return 0; 
	}
	int i=0;
	QueuePtr_J p;
	p = front ->next ;
	while(p) 
	{
		i++;
		p = p->next ;
	}
	return i;
}
	
/*
-----------------------------------Job_Scheduler(��ҵ���ȳ�����-----------------------------------------
*/
//���캯��	
Job_Scheduler::Job_Scheduler()
{
	
}
/*
2����������ҵ�ĵ��ȣ���ҵ���ȳ����������������
	a.ѡ����ҵ�������ȷ����㷨
	b.������Դ����ҵ���ȳ�����洢����������ͨѶ��Ϊ��ҵ�����������Դ��
	c.�������̡�ÿ��ѡ����ҵ�ҽ���װ���ڴ�ʱ��ϵͳ��Ϊ����ҵ����Ӧ�ý��̣�����PCB����Ӧ����ʵ�壬��Щ���̽��ڵͼ����ȿ�����ռ�ô��������С�����ͬʱװ������ҵ���������У�
	d.����������ҵ���������������ֹʱ����ҵ���ȳ���Ҫ������ҵ��������ƹ��������ӡ�����Ϣ�����ո�����Դ������JCB�ȣ�ͬʱ��������ҵ���ȳ���ѡ������ҵ�����ڴ棻
*/


int Job_Scheduler::go(Pool_Queue &pool_queue,JCB &temp,Memory &memory,Page_Table &page_table,Time &timeq,PCB_Table &pcb_table,LinkQueue &ready,JCB_Table &jcb_table)
{
	ChooseJob(pool_queue,temp);//a.ѡ����ҵ��
	
	int i = AllocatinResource(memory,temp,page_table,pool_queue,jcb_table);//	b.������Դ��
	if(i == 1)
	{
		Job_Scheduler::CreateProcees(temp,timeq,pcb_table,ready);//c.�������̡�
		return 1;
	}
	else
	{
		return 0;
	}
	
	
}

//ѡ����ҵ(�����ȷ����㷨),�Ӻ󱸶�����ȡ��һ��JCB��temp 
void Job_Scheduler::ChooseJob(Pool_Queue &pool_queue,JCB &temp)
{
	if(pool_queue.front == pool_queue.rear)
	{
		cout<<"�󱸶���Ϊ�ն��У�"<<endl;
	}
	else
	{
		pool_queue.GetTop(temp);//�Ӻ󱸶�����ѡ��һ����ҵ�����ڴ�
		cout<<"��ҵ��Ϊ��"<<temp.JobId<<"����ҵ�ѵ����ڴ棡"<<endl; 
	}
	
}

//������Դ����ҵ���ȳ�����洢����������ͨѶ��Ϊ��ҵ�����������Դ���� 
int Job_Scheduler::AllocatinResource(Memory &memory,JCB jcb,Page_Table &page_table,Pool_Queue &pool_queue,JCB_Table &jcb_table)
{
	int i = memory.AllocationSpace(jcb.MemorySize,jcb.JobId,page_table);
	if(i == 1)
	{
		cout<<"��Ϊ��ҵ"<<jcb.JobId<<"���������ڴ���Դ!"<<endl;
		pool_queue.DeQueue(jcb);
		jcb_table.jcb[jcb.JobId].ProId = jcb.JobId;//�ı�JCB���������Ϣ 
		jcb_table.jcb[jcb.JobId].StarTime = jcb_table.jcb[jcb.JobId].InTime + 10;
		return 1;
	}
	else
	{
		cout<<"��ҵ"<<jcb.JobId<<"���ڻع�!"<<endl;
		cout<<"�ڴ������������ȴ���"<<endl;
		cout<<endl;
		return 0;
	}

	
}

//��������(ÿ��ѡ����ҵ�ҽ���װ���ڴ�ʱ��ϵͳ��Ϊ����ҵ����Ӧ�ý��̣�����PCB����Ӧ����ʵ�壬��Щ���̽��ڵͼ����ȿ�����ռ�ô��������С�)
int Job_Scheduler::CreateProcees(JCB temp,Time &timeq,PCB_Table &pcb_table,LinkQueue &ready)
{
	cout<<"��Ϊ��ҵ"<<temp.JobId<<"������Ӧ����"<<temp.JobId<<"�Լ���Ӧҳ��!"<<endl;
	Process process(temp.JobId,temp.StarTime+10,temp.MemorySize,timeq);//Ϊ����ҵ����Ӧ�ý��̣�����PCB 
	process.profork(pcb_table,ready);//��������������к�PCB��ȴ��ͼ����� 
	return 0;
}

//��������
int Job_Scheduler::FollowWork()
{
	return 0;
} 	
	
	
	
	
	

