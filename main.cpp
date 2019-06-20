//���ͷ�ļ����� 
#include "utils.h"
#include "windows.h"  //ʹ��ϵͳ���߳�
#include "process.h"
#include "hardware.h" 
#include "job.h"


//�����̲߳����Ľṹ�� 
typedef struct node{
	Memory memory;//�����ڴ� 
	JCB_Table jcb_table;//������ҵ��
	Pool_Queue pool_queue;//�����󱸶��� 
	Job_Scheduler job_scheduler;//������ҵ������ 
	PCB_Table pcb_table;//����PCB��
	Page_Table page_table[11];//����10��ҳ�� 
	LinkQueue ready;//��������
	LinkQueue run;//���ж��� 
	LinkQueue block;//��������
	LinkQueue finish;//��ɶ��� 
	ofstream fmemory;//�ڴ��ļ� 
	ofstream fjob;//��ҵ�ļ� 
	ofstream frun;//������������ļ� 
	MMU mmu;
	Time time;//ʱ�ӣ���������ɣ� 
	int k; 
}DATA; 

//�߳����庯�� 
DWORD WINAPI ThreadFun(LPVOID pM) 
{
	DATA *data = (DATA*)pM;
	int y;
	data->k=1;
	while(1)
	{		
		if(data->k == 1)
		{
			JCB e;
			data->pool_queue.GetTop(e);
			int loop1 = e.JobId;
			while(data->pool_queue.front != data->pool_queue.rear)//�ȴ��ڴ��п�����Դ 
			{
				cout<<"\n"<<"�ڴ���У�һ���µ���ҵ����ϵͳ��"<<"\n"<<endl;
				data->frun<<"\n"<<"�ڴ���У�һ���µ���ҵ����ϵͳ��"<<"\n"<<endl;
				JCB temp;
				if(data->job_scheduler.go(data->pool_queue,temp,data->memory,data->page_table[loop1],data->time,data->pcb_table,data->ready,data->jcb_table) == 0)//��������ҵ�ĵ���
				{
					data->k = 0;
					break;
				}
				cout<<endl;
				loop1++;	
			}
		} 
		
			
			
		if(data->block.front != data->block.rear)
		{
			Sleep(50);
			Process e;
			data->block.DeQueue(e);
			data->ready.EnQueue(e);
			//
			cout<<"\n"<<"һ�������Ľ����Ѿ�������"<<"\n"<<endl;
			data->frun<<"\n"<<"һ�������Ľ����Ѿ�������"<<"\n"<<endl;
		}
	
			//Sleep(50);
	}		
   	return 0; 	 
}

int main()
{
	                                                //�����ʼ��
	cout<<"============================================================================"<<endl; 
	cout<<"=                        ����ҳʽ�洢����ϵͳ                              ="<<endl;
	cout<<"=                                                  �ƿ�142   ��ӱ          ="<<endl;  
	cout<<"============================================================================"<<endl; 
	cout<<"�����ʼ��.."<<endl;
	Sleep(1000);
	srand(time(0));//ʱ������	
	DATA *Adata = new DATA;//�����̵߳Ĳ��� 
	Cpu CPU;
	Adata->k = 0;

	if(Adata == 0)
	{
		cout<<"�����ʼ��ʧ�ܣ�"<<endl;
		return 1;
	}
	Adata->fmemory.open("�ڴ�.txt",ios::trunc); //ios::trunc��ʾ�ڴ��ļ�ǰ���ļ����
	Adata->fjob.open("��ҵ��.txt",ios::trunc); //ios::trunc��ʾ�ڴ��ļ�ǰ���ļ����
	Adata->frun.open("�������.txt",ios::trunc); //ios::trunc��ʾ�ڴ��ļ�ǰ���ļ����
	Adata->fmemory<<"��ҵ��δ�����ڴ�ʱ�ڴ����"<<"\r\n";
	Adata->fmemory<<"-----------------------------------------------------------------"<<"\r\n"; 
	Adata->memory.Print(Adata->fmemory); //��ӡ�ڴ�״��
	cout<<"�����ʼ���ɹ���"<<endl;
	//Adata->frun<<"�����ʼ���ɹ���"<<endl;
	
	
	                                           //��������ҵ����֯�����
	/*
	1����������ҵ�Ľ�����Ϊÿ����ҵ������Ӧ����ҵ���ƿ飬��������JCB��֯����ҵ�� ,
		�������벢���õ�JCB��ҵ����󱸶��У��ȴ���ҵ���� 
	*/ 
	for(int i = 1;i <= 10;i++)
	{
		JCB jcb;
		jcb.Init(i,(i+1)*50,Adata->time.RandNum(15,25));//Ϊÿ����ҵ������Ӧ����ҵ���ƿ� 
		Adata->jcb_table.InsertJCB(jcb,i);//��JCB��֯����ҵ�� 
		Adata->pool_queue.EnQueue(jcb);//�������벢���õ�JCB����󱸶��� 
		cout<<endl; 
		Sleep(200);
	}
	Adata->fjob<<"��ҵ�ȴ�����"<<endl;//��ӡĿǰ��ҵ�� 
	Adata->fjob<<"----------------------------------------------------"<<endl;
	Adata->jcb_table.PrintTable(Adata->fjob);
	cout<<"Ŀǰ��ҵ����Ϣ�Ѵ��롰��ҵ��.txt����"<<endl;
	cout<<"------------------------------------------------------------------"<<endl;
	cout<<endl;
	
	/*
	2����������ҵ�ĵ��ȣ���ҵ���ȳ����������������
		a.ѡ����ҵ�������ȷ����㷨
		b.������Դ����ҵ���ȳ�����洢����������ͨѶ��Ϊ��ҵ�����������Դ��
		c.�������̡�ÿ��ѡ����ҵ�ҽ���װ���ڴ�ʱ��ϵͳ��Ϊ����ҵ����Ӧ�ý��̣�����PCB����Ӧ����ʵ�壬��Щ���̽��ڵͼ����ȿ�����ռ�ô��������С�����ͬʱװ������ҵ���������У�
		d.����������ҵ���������������ֹʱ����ҵ���ȳ���Ҫ������ҵ��������ƹ��������ӡ�����Ϣ�����ո�����Դ������JCB�ȣ�ͬʱ��������ҵ���ȳ���ѡ������ҵ�����ڴ棻
	*/
	cout<<"��ʼ��ҵ����.."<<endl;
	cout<<"�����κζ������س�������"<<endl; 
	cout<<"***********************************************************"<<endl;
	char get = getchar();
	int loop1 = 1;
	while(Adata->pool_queue.front != Adata->pool_queue.rear)
	{
		JCB temp;
		if(Adata->job_scheduler.go(Adata->pool_queue,temp,Adata->memory,Adata->page_table[loop1],Adata->time,Adata->pcb_table,Adata->ready,Adata->jcb_table) == 0)//��������ҵ�ĵ���
		{
			break;
		}
		cout<<endl;
		loop1++; 
		Sleep(200);
	}
	//��ӡ�����Ϣ
	Adata->fjob<<"��ҵ���ε��Ⱥ�"<<endl;//��ӡĿǰ��ҵ�� 
	Adata->fjob<<"----------------------------------------------------"<<endl;
	Adata->jcb_table.PrintTable(Adata->fjob);
	cout<<"Ŀǰ��ҵ����Ϣ�Ѵ��롰��ҵ��.txt����"<<endl;
	
	Adata->pcb_table.PrintTable("���̱�.txt");//��ӡ���̱�
	cout<<"Ŀǰ���̱���Ϣ�Ѵ��롰���̱�.txt����"<<endl;	
	
	char *Npage[] = {"ҳ��1.txt","ҳ��2.txt","ҳ��3.txt","ҳ��4.txt","ҳ��5.txt","ҳ��6.txt","ҳ��7.txt","ҳ��8.txt","ҳ��9.txt","ҳ��10.txt",};
	for(int i = 1;i <= 10;i++)//��ӡҳ��
	{	
		Adata->page_table[i].PrintTable(Npage[i-1]);
		cout<<"����"<<i<<"��Ӧҳ����Ϣ�Ѵ��롰ҳ��"<<i<<".txt����"<<endl;
	}
	Adata->fmemory<<"��ҵ���ε��Ⱥ���ڴ����"<<"\r\n";
	Adata->fmemory<<"-----------------------------------------------------------------"<<"\r\n"; 
	Adata->memory.Print(Adata->fmemory); //��ӡ�ڴ�״��
	cout<<"Ŀǰ�ڴ�״̬��Ϣ�Ѵ��롰�ڴ�.txt����"<<endl;	
	cout<<"------------------------------------------------------------"<<endl;
	
	
	                                                      /*���̵��ȣ�ʱ��Ƭ��ת����*/	
	cout<<"\r\n"<<"��ʼ���̵��ȣ�ʱ��Ƭ��ת����.."<<endl;
	cout<<"�����κζ������س�������"<<endl; 
	cout<<"***********************************************************"<<endl;
	getchar();
	get = getchar();

	HANDLE handle = CreateThread(NULL,0,ThreadFun,Adata,0,NULL);//�߳��������� 
	
	//ʱ��Ƭ��ת�� 
	while(Adata->ready.front != Adata->ready.rear) 
	{
		Sleep(100);
		int alltime = 0;
		int capacity = 60;//��һ����ʱ��Ƭ 
		Process e;
		Adata->ready.DeQueue(e);//�������е�һ��Ԫ�س��ӣ��������ж��� 
		Adata->run.EnQueue(e);
					 
		//���иý��̵�һ��ָ�� 
		Adata->pcb_table.process[e.ProID].ProState = 2;
		CPU.Scenerevover(Adata->pcb_table.process[e.ProID]);//CPU�ֳ��ظ����� 
					
		cout<<"����"<<e.ProID<<"ҳ���ַװ��ҳ���ַ�Ĵ�����"<<endl; 
		Adata->frun<<"����"<<e.ProID<<"ҳ���ַװ��ҳ���ַ�Ĵ�����"<<endl; 
		cout<<"�������е�"<<e.ProID<<"������"<<"����"<<CPU.IR<<"��ָ��,"; 
		Adata->frun<<"�������е�"<<e.ProID<<"������"<<"����"<<CPU.IR<<"��ָ��,";
					
		if(Adata->pcb_table.process[e.ProID].instruc_arry[CPU.IR-1].State == 1)//��ָ��Ϊ�û�̬���� 
		{
			int addr = Adata->pcb_table.process[e.ProID].instruc_arry[CPU.IR-1].addr;
			int pageid;int pianyi;int paddr;
			cout<<"��ָ��Ϊ�û�̬�������"<<",�߼���ַΪ"<<addr<<endl; 
			Adata->frun<<"��ָ��Ϊ�û�̬�������"<<",�߼���ַΪ"<<addr<<endl; 
			Adata->mmu.go(Adata->frun,Adata->page_table[e.ProID],addr,pageid,pianyi,paddr);//MMU���� 
			cout<<endl;
			Adata->frun<<endl; 
			Sleep(Adata->pcb_table.process[e.ProID].instruc_arry[CPU.IR-1].time); 
			Adata->pcb_table.process[e.ProID].RunTimes += Adata->pcb_table.process[e.ProID].instruc_arry[CPU.IR-1].time;
			alltime += Adata->pcb_table.process[e.ProID].instruc_arry[CPU.IR-1].time;
			CPU.IR++;
			CPU.Sceneprotection(Adata->pcb_table.process[e.ProID]);
					 	
			Adata->run.DeQueue(e);
			if(CPU.IR > Adata->pcb_table.process[e.ProID].InstrucNum)//�ж��Ƿ�ִ����� 
			{
				Adata->pcb_table.process[e.ProID].alltime = alltime;//���̵���תʱ�� 
				Adata->finish.EnQueue(e);
				cout<<"****����"<<e.ProID<<"��������ɣ�****"<<endl; 
				Adata->frun<<"****����"<<e.ProID<<"��������ɣ�****"<<endl; 
				//�����ڴ� 
				JCB jcb;
				Adata->jcb_table.SearchJobId(e.JobId,jcb);
				Adata->memory.RecycleSpace(jcb.MemorySize-5,jcb.JobId);
				cout<<"****��ҵ"<<e.ProID<<"��������ɣ�****"<<endl; 
				Adata->frun<<"****��ҵ"<<e.ProID<<"��������ɣ�****"<<endl;
				cout<<"****�ѻ���"<<"��ҵ"<<e.ProID<<"����Դ��****"<<endl; 
				Adata->frun<<"****�ѻ���"<<"��ҵ"<<e.ProID<<"����Դ��****"<<endl;
				
				
				Adata->fmemory<<"��ҵ"<<e.ProID<<"�������ʱ�ڴ�״��"<<"\r\n";
				Adata->fmemory<<"-----------------------------------------------------------------"<<"\r\n"; 
				Adata->memory.Print(Adata->fmemory); //��ӡ�ڴ�״��
				
				Adata->k = 1; 
				Sleep(500);
			}
			else
			{
				Adata->ready.EnQueue(e);
			}
							
		}
		else//��ָ��ΪI/O���� 
		{
			int addr = Adata->pcb_table.process[e.ProID].instruc_arry[CPU.IR-1].addr;
			int pageid;int pianyi;int paddr;
			cout<<"��ָ��ΪI/O����"<<",�߼���ַΪ"<<addr<<endl; 
			Adata->frun<<"��ָ��ΪI/O����"<<",�߼���ַΪ"<<addr<<endl; 
			Adata->mmu.go(Adata->frun,Adata->page_table[e.ProID],addr,pageid,pianyi,paddr);//MMU���� 
			cout<<endl;
			Adata->frun<<endl; 
			Sleep(Adata->pcb_table.process[e.ProID].instruc_arry[CPU.IR-1].time); 
			Adata->pcb_table.process[e.ProID].RunTimes += Adata->pcb_table.process[e.ProID].instruc_arry[CPU.IR-1].time;
			//alltime += data1->L.process[e.ProID].instruc_arry[IR-1].time;
			CPU.IR++;
			CPU.Sceneprotection(Adata->pcb_table.process[e.ProID]);
					 	
			Adata->run.DeQueue(e);
			if(CPU.IR > Adata->pcb_table.process[e.ProID].InstrucNum)//�ж��Ƿ�ִ����� 
			{
				Adata->pcb_table.process[e.ProID].alltime = alltime;
				Adata->finish.EnQueue(e);
				cout<<"****����"<<e.ProID<<"��������ɣ�****"<<endl; 
				Adata->frun<<"****����"<<e.ProID<<"��������ɣ�****"<<endl; 
				//data1->L.DeleteProID(e.ProID);
				//�����ڴ� 
				JCB jcb;
				Adata->jcb_table.SearchJobId(e.JobId,jcb);
				Adata->memory.RecycleSpace(jcb.MemorySize-5,jcb.JobId);
				cout<<"****��ҵ"<<e.ProID<<"��������ɣ�****"<<endl; 
				Adata->frun<<"****��ҵ"<<e.ProID<<"��������ɣ�****"<<endl;
				cout<<"****�ѻ���"<<"��ҵ"<<e.ProID<<"����Դ��****"<<endl; 
				Adata->frun<<"****�ѻ���"<<"��ҵ"<<e.ProID<<"����Դ��****"<<endl; 
				 
				
				Adata->fmemory<<"��ҵ"<<e.ProID<<"�������ʱ�ڴ�״��"<<"\r\n";
				Adata->fmemory<<"-----------------------------------------------------------------"<<"\r\n"; 
				Adata->memory.Print(Adata->fmemory); //��ӡ�ڴ�״��
				
				Adata->k = 1;
				Sleep(500);
			}
			else
			{
				Adata->block.EnQueue(e);
			}
		}
		Sleep(100);						 	 				  
	}
	cout<<"\r\n"<<"�������й����Ѵ��롰�������.txt����"<<endl;	
	cout<<"�������й��̵��ڴ�״̬�仯����Ѵ��롰�ڴ�.txt����"<<endl;
	cout<<"������ҳ����Ϣ��ͬ��������ļ���"<<endl;
	Adata->pcb_table.PrintTable("���̱�(���).txt");//��ӡ���̱�
	cout<<"Ŀǰ���̱���Ϣ�Ѵ��롰���̱�(���).txt����"<<endl;	
	Adata->fjob<<"��ҵȫ��������ɺ�"<<endl;//��ӡĿǰ��ҵ�� 
	Adata->fjob<<"----------------------------------------------------"<<endl;
	Adata->jcb_table.PrintTable(Adata->fjob);
	cout<<"Ŀǰ��ҵ����Ϣ�Ѵ��롰��ҵ��.txt����"<<endl;	
	for(int i = 1;i <= 10;i++)//��ӡҳ��
	{	
		Adata->page_table[i].PrintTable(Npage[i-1]);
		//cout<<"����"<<i<<"1��Ӧҳ����Ϣ�Ѵ��롰ҳ��"<<i<<".txt����"<<endl;
	}
	
	//�ر������ļ� 
	Adata->fmemory.close();
	Adata->fjob.close();
	Adata->frun.close();
	
	char stop = 'a';
	while(stop != 'e')
	{
		cout<<"�������ַ�e�˳�"<<endl;
		stop = getchar();
	}
	cout<<"������ɣ�"; 
	return 0;
}


