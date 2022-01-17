/*

9 11
1
2
3
4
5
6
7
8
9
1 2 6
1 3 4
1 4 5
2 5 1
3 5 1
4 6 2
5 7 9
5 8 7
6 8 4
7 9 2
8 9 4

*/
#include<stdio.h>
#include<stdlib.h>
#define MAXVEX 100
#define OK 0
#define ERROR 1
int* etv, * ltv;
int* stack2, * stack3;

typedef struct EdgeNode
{
	int adjvex;//�ڽӵ��򣬴洢��Ӧ�±�
	int weight;//�洢Ȩֵ
	struct EdgeNode* next;//ָ����һ���
}EdgeNode;

typedef struct VertexNode//�������
{
	int in = 0;//�������
	int data;//�����򣬴洢������Ϣ
	EdgeNode* firstedge;//�߱�ͷָ��
}VertexNode,AdjList[MAXVEX];

typedef struct
{
	AdjList adjList;
	int numVertexes, numEdges;//��ǰ�������ͱ���
}GraphAdjList;

int top2 = 0;//����stack2��ָ��
//��������
int Tppological(GraphAdjList GL)
{
	EdgeNode* e;
	int i, k, gettop, flag = 0;
	int top = 0;//ջָ���±�
	int count = 0;//ͳ�����붥�����
	int* stack;//��ջ�洢���Ϊ0�Ķ���
	stack = (int*)malloc((GL.numVertexes + 5) * sizeof(int));
	for (i = 1; i <= GL.numVertexes; i++)
	{
		if (GL.adjList[i].in == 0)
			stack[++top] = i;//���Ϊ0�Ķ�����ջ 
	}
	etv = (int*)malloc((GL.numVertexes+5)*sizeof(int));//�¼����緢��ʱ��
	for (i = 1; i <= GL.numVertexes; i++)
		etv[i] = 0;//��ʼ��Ϊ0
	stack2 = (int*)malloc((GL.numVertexes + 5) * sizeof(int));//��ʼ��
	//printf("��������Ϊ:\n");
	while (top != 0)
	{
		gettop = stack[top--];//��ջ
		count++; //ͳ�����������
		stack2[++top2] = gettop;//�����Ķ������ѹ����������ջ
		for (e = GL.adjList[gettop].firstedge; e; e = e->next)
		{//���㻡�����
			k = e->adjvex;//ʹ��һ��*****************************************************************
			if (!(--GL.adjList[k].in))
				stack[++top] = k;//���ڽӶ�������Ϊ0����ջ
			if ((etv[gettop] + e->weight) > etv[k])//��������¼����緢��ʱ��ֵ
				etv[k] = etv[gettop] + e->weight;
		}
	}
	if (count < GL.numVertexes)
		return ERROR;
	else
		return OK;
}
//�ڽӱ�
void CreatALGraph(GraphAdjList& GL)
{
	int i, j, k, w;
	EdgeNode* e;
	printf("�����붥�����ͱ���:\n");
	scanf_s("%d %d",&GL.numVertexes,&GL.numEdges);//���붥�����ͱ���
	for (i = 1; i <= GL.numVertexes; i++)//���붥����Ϣ�����������
	{
		printf("�����붥����Ϣ:");
		scanf_s("%d",&GL.adjList[i].data);//���붥����Ϣ
		GL.adjList[i].firstedge = NULL;
	}
	for (k = 0; k < GL.numEdges; k++)
	{
		printf("������ϵĶ�����ź�Ȩֵ<vi,vj,w>:\n");
		scanf_s("%d %d %d",&i,&j,&w);
		GL.adjList[j].in++;
		e = (EdgeNode*)malloc(sizeof(EdgeNode));
		e->adjvex = j;
		e->weight = w;
		e->next = GL.adjList[i].firstedge;//eָ��ָ��ǰ����ָ����
		GL.adjList[i].firstedge = e;//��ǰ����ָ��e
	}
}

int aim;//�յ�
int top3 = 0;
void dfs(GraphAdjList GL, int u)//�����������
{
	//��ֹ����
	if (GL.adjList[u].data == GL.adjList[aim].data) {
		printf("1");
		for (int i = 1; i <= top3; i++) {
			printf("->%d", stack3[i]);
		}
		printf("\n");
		return;
	}
	EdgeNode* e = GL.adjList[u].firstedge;
	for (; e; e = e->next)
	{
		int k = e->adjvex;
		if (etv[u] == ltv[k] - (e->weight)) {//
			stack3[++top3] = k;
			dfs(GL, k);
			stack3[top3--];
		}
	}
}

//�ؼ�·��
void CriticalPath(GraphAdjList GL)
{
	EdgeNode* e;
	int i, gettop, k = 0, j;
	int ete, lte;//��������������� ʱ��
	Tppological(GL);//��������etv��stack2��ֵ
	ltv = (int*)malloc((GL.numVertexes + 5) * sizeof(int));//ʱ��������ʱ��
	for (i = 1; i <= GL.numVertexes; i++)
		ltv[i] = etv[GL.numVertexes];//��ʼ��ltv
	aim = stack2[top2];
	while (top2 != 0)//����ltv
	{
		gettop = stack2[top2--];//�������г�ջ������ȳ�
		for (e = GL.adjList[gettop].firstedge; e; e = e->next)
		{
			//��������¼���������ʱ��ltv
			k = e->adjvex;//�ڶ���****************************************************
			if (ltv[k] - e->weight < ltv[gettop])//��������¼���������ʱ��ltv
				ltv[gettop] = ltv[k] - e->weight;
		}
	}
	printf("���ٻ���ʱΪ��%d\n", etv[aim]);
	printf("\n�ؼ��Ϊ��\n");
	for (j = 1; j <= GL.numVertexes; j++)
	{
		for (e = GL.adjList[j].firstedge; e; e = e->next)
		{
			k = e->adjvex;//*********************************�����Ρ����ʳ�ͻ����ԭ��j
			ete = etv[j];//���緢��ʱ��
			lte = ltv[k] - e->weight;//��ٷ���ʱ��
			if (ete == lte)
				printf("<v%d,v%d> length:  %d\n", GL.adjList[j].data, GL.adjList[k].data, e->weight);
		}
	}
	stack3 = (int*)malloc((GL.numVertexes + 5) * sizeof(int));
	printf("\n�ؼ�·��Ϊ��\n");
	dfs(GL, 1);
}

int main()
{
	GraphAdjList GL;
	CreatALGraph(GL);
	CriticalPath(GL);
	return 0;
}