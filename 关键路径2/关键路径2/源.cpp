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
	int adjvex;//邻接点域，存储对应下标
	int weight;//存储权值
	struct EdgeNode* next;//指向下一结点
}EdgeNode;

typedef struct VertexNode//顶点表结点
{
	int in = 0;//顶点入读
	int data;//顶点域，存储顶点信息
	EdgeNode* firstedge;//边表头指针
}VertexNode,AdjList[MAXVEX];

typedef struct
{
	AdjList adjList;
	int numVertexes, numEdges;//当前顶点数和边数
}GraphAdjList;

int top2 = 0;//用于stack2的指针
//拓扑排序
int Tppological(GraphAdjList GL)
{
	EdgeNode* e;
	int i, k, gettop, flag = 0;
	int top = 0;//栈指针下标
	int count = 0;//统计输入顶点个数
	int* stack;//建栈存储入读为0的顶点
	stack = (int*)malloc((GL.numVertexes + 5) * sizeof(int));
	for (i = 1; i <= GL.numVertexes; i++)
	{
		if (GL.adjList[i].in == 0)
			stack[++top] = i;//入读为0的顶点入栈 
	}
	etv = (int*)malloc((GL.numVertexes+5)*sizeof(int));//事件最早发生时间
	for (i = 1; i <= GL.numVertexes; i++)
		etv[i] = 0;//初始化为0
	stack2 = (int*)malloc((GL.numVertexes + 5) * sizeof(int));//初始化
	//printf("拓扑排序为:\n");
	while (top != 0)
	{
		gettop = stack[top--];//出栈
		count++; //统计输出顶点数
		stack2[++top2] = gettop;//弹出的顶点序号压入拓扑序列栈
		for (e = GL.adjList[gettop].firstedge; e; e = e->next)
		{//顶点弧表遍历
			k = e->adjvex;//使用一次*****************************************************************
			if (!(--GL.adjList[k].in))
				stack[++top] = k;//若邻接顶点的入读为0，入栈
			if ((etv[gettop] + e->weight) > etv[k])//求个顶点事件最早发生时间值
				etv[k] = etv[gettop] + e->weight;
		}
	}
	if (count < GL.numVertexes)
		return ERROR;
	else
		return OK;
}
//邻接表
void CreatALGraph(GraphAdjList& GL)
{
	int i, j, k, w;
	EdgeNode* e;
	printf("请输入顶点数和边数:\n");
	scanf_s("%d %d",&GL.numVertexes,&GL.numEdges);//输入顶点数和边数
	for (i = 1; i <= GL.numVertexes; i++)//读入顶点信息，建立顶点表
	{
		printf("请输入顶点信息:");
		scanf_s("%d",&GL.adjList[i].data);//输入顶点信息
		GL.adjList[i].firstedge = NULL;
	}
	for (k = 0; k < GL.numEdges; k++)
	{
		printf("输入边上的顶点序号和权值<vi,vj,w>:\n");
		scanf_s("%d %d %d",&i,&j,&w);
		GL.adjList[j].in++;
		e = (EdgeNode*)malloc(sizeof(EdgeNode));
		e->adjvex = j;
		e->weight = w;
		e->next = GL.adjList[i].firstedge;//e指针指向当前顶点指向结点
		GL.adjList[i].firstedge = e;//当前顶点指向e
	}
}

int aim;//终点
int top3 = 0;
void dfs(GraphAdjList GL, int u)//深度优先搜索
{
	//终止条件
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

//关键路径
void CriticalPath(GraphAdjList GL)
{
	EdgeNode* e;
	int i, gettop, k = 0, j;
	int ete, lte;//声明活动子最早晚发生 时间
	Tppological(GL);//计算数组etv和stack2的值
	ltv = (int*)malloc((GL.numVertexes + 5) * sizeof(int));//时间最晚发生时间
	for (i = 1; i <= GL.numVertexes; i++)
		ltv[i] = etv[GL.numVertexes];//初始化ltv
	aim = stack2[top2];
	while (top2 != 0)//计算ltv
	{
		gettop = stack2[top2--];//拓扑序列出栈，后进先出
		for (e = GL.adjList[gettop].firstedge; e; e = e->next)
		{
			//求个顶点事件的最晚发生时间ltv
			k = e->adjvex;//第二次****************************************************
			if (ltv[k] - e->weight < ltv[gettop])//求个顶点事件的最晚发生时间ltv
				ltv[gettop] = ltv[k] - e->weight;
		}
	}
	printf("最少花费时为：%d\n", etv[aim]);
	printf("\n关键活动为：\n");
	for (j = 1; j <= GL.numVertexes; j++)
	{
		for (e = GL.adjList[j].firstedge; e; e = e->next)
		{
			k = e->adjvex;//*********************************第三次、访问冲突报错原因j
			ete = etv[j];//最早发生时间
			lte = ltv[k] - e->weight;//最迟发生时间
			if (ete == lte)
				printf("<v%d,v%d> length:  %d\n", GL.adjList[j].data, GL.adjList[k].data, e->weight);
		}
	}
	stack3 = (int*)malloc((GL.numVertexes + 5) * sizeof(int));
	printf("\n关键路径为：\n");
	dfs(GL, 1);
}

int main()
{
	GraphAdjList GL;
	CreatALGraph(GL);
	CriticalPath(GL);
	return 0;
}