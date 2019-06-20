#include "process.h"


//-----------------------------------------ָ���ຯ��ʵ��ģ��-----------------------------------------------------------------------
//���캯�� 
Instruc::Instruc()
{
	ID = 0;       //ID����ʼ��Ϊ0 
	State = 1;       //Instruc_State��ʼ��Ϊ1 
	time = 0;   //��Instruc_State=0��1ʱ, Instruc_Times=����[10-40]֮������10ms����������
	addr = 0;
}

void Instruc::Init(int id,Time &timeq)
{
	ID = id;       //ID��1��ʼ 
	State = timeq.RandNum(1,2);       //Instruc_State��ʼ��Ϊ1��2
	if(State == 1)//��Instruc_State=0��1ʱ, Instruc_Times=����[10-40]֮������10ms������������
	{
		time = timeq.RandNum(1,4)*10;
	}
	else//�� Instruc_State=2 ʱ, Instruc_Times=50���û����̷��� I/O �������󣬲�������� I/O ����ͨ��ʱ��Ϊ 50ms��֮��ɻ���
	{
		time = 50;
	}
	addr = ID*1000 +	timeq.RandNum(0,999); //����ָ���ַ 
}

//�޸�ָ��ID 
void Instruc::changeID(int temp)
{
	ID = temp;
}

//�޸�ָ��״̬ 
void Instruc::changeState(int temp)
{
	State = temp;
}


//--------------------------------------------�����ຯ��ʵ��ģ��---------------------------------------------------------------------
//���캯�� 
Process::Process()
{
	ProID = 0;      //���̱�ţ�ProID��,���û������ID��Ĭ����Ϊ0 
	
	Priority = 5;	//������������Priority��,���û������,Ĭ����Ϊ5
	
	InTimes =0;   //���̴���ʱ�䣨InTimes�����������д�0��ʼ��ʱ����2s(��2000ms)���������ʱ��������С�[�����ID���]
	
	RunTimes = 0;  //��������ʱ�䣨RunTime��:ͳ�Ƽ�¼���̵�ǰ�������˶���ʱ�䣬���ֶο�ʼʱΪ�գ��������й����в��ϱ���ͼ�¼��
	
	InstrucNum = 20; //���̰�����ָ����Ŀ��InstrucNum������[5-20]���ڵ��������������
	
	PSW = 1;  //PSW�б���ý��̵�ǰִ�е�ָ���š����磬1��ʾ����ִ�е�1��ָ�� 
	
	ProState = 1;  //����״̬��ProState��:0Ϊ����̬��1Ϊ����̬��2Ϊ����̬��Ĭ����Ϊ����̬ 
	
	instruc_arry = new Instruc[InstrucNum];  //�������N��ָ�� 
	
	alltime = 0; 
}

Process::Process(int id,int intime,int instrucnum,Time &timeq)
{
	ProID = id;      //���̱�ţ�ProID��,���û������ID��Ĭ����Ϊ0 
	
	JobId = id;
	
	Priority = 5;	//������������Priority��,���û������,Ĭ����Ϊ5
	
	InTimes = intime;   //���̴���ʱ�䣨InTimes�����������д�0��ʼ��ʱ����2s(��2000ms)���������ʱ��������С�[�����ID���]
	
	RunTimes = 0;  //��������ʱ�䣨RunTime��:ͳ�Ƽ�¼���̵�ǰ�������˶���ʱ�䣬���ֶο�ʼʱΪ�գ��������й����в��ϱ���ͼ�¼��
	
	InstrucNum = instrucnum; //���̰�����ָ����Ŀ��InstrucNum������[5-20]���ڵ��������������
	
	PSW = 1;  //PSW�б���ý��̵�ǰִ�е�ָ���š����磬1��ʾ����ִ�е�1��ָ�� 
	
	ProState = 1;  //����״̬��ProState��:0Ϊ����̬��1Ϊ����̬��2Ϊ����̬��Ĭ����Ϊ����̬ 
	
	instruc_arry = new Instruc[InstrucNum];  //����N��ָ�� 
	
	for(int j = 0;j < InstrucNum;j++)//��ʼ������ָ�� 
	{
		instruc_arry[j].Init(j+1,timeq);
	}
	
	alltime = 0; 
}

//���̵ĳ�ʼ�� 
void Process::proinit(int m_ID,int m_priority,int m_intimes)
{
	ProID = m_ID;   
	Priority = m_priority;
	InTimes = m_intimes; 
} 

//���̵Ĵ���
int Process::profork(PCB_Table &L,LinkQueue &ready)
{
	Process temp;
	temp = *this;	
	ready.EnQueue(temp);//�����������
	cout<<"����"<<temp.ProID<<"������������У�"<<endl;	
	//֪ͨĳЩģ�飬������	
	L.InsertPCB(temp,ProID);//����PCB�� 
	cout<<"����"<<temp.ProID<<"������PCB��"<<endl;		
	return 0;	
}

//���̵���ֹ
int Process::prokill(PCB_Table &L,LinkQueue &finish)
{
	Process temp;
	//����Ӧ�Ķ������Ƴ�
	finish.DeQueue(temp);
	//�Ƴ�PCB��
	L.DeleteProID(ProID);
	//ת��ϵͳ���ȹ��� 
	return ProID;
}

//���̵�����
int Process::problock(PCB_Table &L,LinkQueue &block,Cpu CPU)
{
	//�����ֳ���Ϣ
	CPU.Sceneprotection(L.process[ProID]);
	//�޸Ľ���״̬
	L.process[ProID].ProState = 0;
	//������ض��� 
	block.EnQueue(L.process[ProID]);
	return 0;
}

//���̵Ļ��� 
int Process::procall(PCB_Table &L,LinkQueue &ready,LinkQueue &block)
{
	//�����������Ƶ���������
	Process e;
	block.DeQueue(e);
	ready.EnQueue(e); 
	//�޸Ľ���״̬ 
	L.process[ProID].ProState = 1;
	return 0;
}

//------------------------------------------PCB�ຯ��ʵ��ģ��--------------------------------------------------------------------------
//���캯�� 
PCB_Table::PCB_Table()
{
	process = new Process[20];   //�����ʼ�����̱�ռ� 
	length = 0;   // PCB�н��̸�����ʼ��Ϊ0
}

//��PCB��ĵ�i������Ԫ��֮ǰ����һ������x (PCB���0λ�ճ�)
void PCB_Table::InsertPCB(Process x,int i)
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
			process[k+1] = process[k];
		}	
		process[k+1] = x;
	}
} 

//���ݽ���ID����PCB����ɾ����Ӧ���� 
void PCB_Table::DeleteProID(int id)
{
	int i;
	int flag=0;
	for(i = 1;i < length + 1;i++)
	{
		if(process[i].ProID == id)
		{
			int k = i;
			for(k=k+1;k < length + 1;k++)
			{
				process[k-1] = process[k];
			}
			length--;
			flag = 1;
			break;
		}
	}
	if(flag == 0)
	{
		cout<<"PCB����û�и�ID�Ľ���"<<endl;
	}
} 

//���ݽ���ID������PCB���е�ĳ������ ,���ظ�ͬ��Ĳ��� 
void PCB_Table::SearchProID(int id,Process &temp)
{
	int i;
	int flag = 0;
	for(i = 1;i < length + 1;i++)
	{
		if(process[i].ProID == id)
		{
			temp = process[i];
			flag = 1;
			break;
		}
	}
	if(flag == 0)
	{
		cout<<"PCB����û�и�ID�Ľ���"<<endl;
	}
} 

//��ӡPCB�� 
void PCB_Table::PrintTable(char *a)
{
	ofstream file;
	file.open(a,ios::trunc); //ios::trunc��ʾ�ڴ��ļ�ǰ���ļ����
	//int ProID;//���̱�� 
	//	int JobId;//���̶�Ӧ��ҵID 
	//	int Priority;//���������� 
	//	int InTimes;//���̴���ʱ�� 
	//	int ProState;//����״̬ 
	//	int RunTimes;//��������ʱ�� 
	//	int InstrucNum;//���̰�����ָ����Ŀ 
	//	Instruc *instruc_arry;//���̰�����ָ������ָ�� 
	//	int alltime;//������תʱ�� 
	file<<"ProID"<<setw(15)<<"JobId"<<setw(15)<<"InTimes"<<setw(15)<<"ProState"<<setw(15)<<"RunTime"<<setw(15)<<"InstrucNum"
			<<setw(15)<<"PSW"<<setw(15)<<"Instruc_ID"<<setw(20)<<"Instruc_State"<<setw(20)<<"Instruc_Times(ms)"<<setw(20)<<"Instruc_Addr";
	for(int i = 1;i <= length;i++)
	{
		file<<"\r\n"<<process[i].ProID<<setw(15)<<process[i].JobId<<setw(15)<<process[i].InTimes<<setw(15)<<
			process[i].ProState<<setw(15)<<process[i].RunTimes<<setw(15)<<process[i].InstrucNum<<setw(18)<<process[i].PSW<<setw(15);
		//���ָ�����Ϣ
		for(int j=0;j<process[i].InstrucNum;j++)
		{
			file<<process[i].instruc_arry[j].ID<<setw(15)<<process[i].instruc_arry[j].State<<setw(15)
				<<process[i].instruc_arry[j].time<<setw(25)<<process[i].instruc_arry[j].addr;
			if(j<process[i].InstrucNum-1)//���ǰ��Ŀո�
			{
				file<<"\r\n"<<setw(109);
			}	
		}
	}
	file.close();
} 

//-----------------------------------------��������ʵ�ֺ���-------------------------------------------------------------------------
LinkQueue::LinkQueue()
{
	rear = new QNode;  //Ϊ���е�ͷβ������ռ�
	front = rear;
	if (!front )  return ;
	front->next = NULL;
}

void LinkQueue::EnQueue(Process e)  //�����
{
	QueuePtr p;
	p = new QNode;
	if (!p) return;
	p->data = e; 
	p->next = NULL;
	rear ->next = p;
	rear = p;
}

void LinkQueue::DeQueue(Process &e) //������
{
	if(front == rear)
	{
		cout<<"�ö���Ϊ�ն���chu"<<endl;
	}
	QueuePtr p;
	p = front ->next ;
	e = p->data ;
	//printf("��������ɣ���ɾ����Ԫ��Ϊ%d\n",e);
	front ->next = p->next ;
	if (rear == p ) rear = front ; //���������ԭ��ֻ��һ��Ԫ�أ�Ҫͬʱ�޸Ķ�βָ��
	delete p;
}

void LinkQueue::GetTop(Process &e)  //ȡ����ͷԪ��
{
	if (rear ==  front ) 
	{
		cout<<"�ö���Ϊ�ն���"<<endl;
	}
	QueuePtr p;
	p = front ->next ;
	e = p->data;
}

void LinkQueue::PrintQueue() //��������
{
	if (front == rear)
	{
		cout<<"�ö���Ϊ�ն���"<<endl;
		return;
	}
	
	QueuePtr p;
	p = front->next ;
	cout<<"�ö����е�ProID���£�";
	while(p)
	{
		cout<<p->data.ProID<<",";
		p = p->next ;
	}
	cout<<endl;
}

int LinkQueue::LenQueue()  //ȡ���г���
{
	if (front == rear )
	{
		//cout<<"�ö���Ϊ��"<<endl;
		return 0; 
	}
	int i=0;
	QueuePtr p;
	p = front ->next ;
	while(p) 
	{
		i++;
		p = p->next ;
	}
	return i;
}

void LinkQueue::MinPriority(Process &e)
{
	QueuePtr p,min,f;
	if(front == rear)
	{
		cout<<"����Ϊ��!"<<"\n";
	}
	else
	{
		min = front->next;//��һ���ڵ� 
		f = front;//p�ڵ��ǰһ���ڵ� 
		p = min;//�ڶ����ڵ� 
		while(p->next)
		{
			f = p;	
			p = p->next;
			if(p->data.Priority < min->data.Priority)	
			{
				min = p;
			}		
		}
		f->next = min->next;
		e=min->data;
		delete min;
	}	
	
}

void LinkQueue::sort()
{
	
	QueuePtr p,r;

	p =front ;r=p->next->next;
	if(front==rear)
		cout<<"����Ϊ��!"<<"\n";
	for(int i=0;i<6;i++)
	while(p->next->next)
	{
		if(p->next->data.Priority>r->data.Priority)
		{
			QueuePtr q=new QNode;
			q->data=r->data;
			//p->next=q->next;
			p->next=q;	
			q->next=p->next;
			q->next->next=r->next;
			delete r;
		}
		p=p->next;
		r=p->next->next;
	}
	
	
}

/*void LinkQueue::DeleteQueue(int v)
{
	QueuePtr p,q;
	p=front;
	if(front == rear)
		cout<<"����Ϊ�գ�"<<"\n";
	while(p->next)
	{
		if(p->next->data.ProID==v)
		{
			q=p->next;
			p->next=q->next;
			delete q;
			break;
		}
		else
			p=p->next;
	}

	
}*/




