/*
	Graph类(有向赋权图)
	实现思路：邻接链表实现，有向/无向取决于使用方式
	提供深度/广度遍历接口，未提供泛用接口
	若需提升泛用性，使用时添加Vertex结构来记录顶点属性，
	再扩展泛用接口
	更新时间：2018\12\11

*/
#define INF 5000					// 定义无连接时情况
#include<queue>
#include<iostream>
using std::queue;
using std::cout;
using std::endl;

struct Edge
{
	int endpoint;
	double weight;
	Edge* Next;
};

class Graph
{
private:
	int v_nums;									// 顶点个数
	Edge** G;									// 邻接表数组
	void release(int k);						// 释放链表
	double checkWeight(int m,int n)const;		// 查看两顶点间权值
	int FirstNeighbour(int m)const;
	int NextNeighbour(int m, int n)const;
public:
	Graph() :v_nums(0), G(nullptr) {}
	Graph(double *g, int n);			// 接受一张二维邻接矩阵(n*n)
	Graph(Graph& g);
	~Graph() { for (int i = 0; i<v_nums; ++i)release(i); }
	void BFS(int n);				// 广度优先搜索接口：不提供泛用接口
	void DFS(int n);				// 深度优先搜索接口：不提供泛用接口
	void checkPrint();				// 输出邻接表进行检查

};

double Graph::checkWeight(int m,int n)const
{
	if(m>v_nums-1 || n>v_nums-1 || m<0 || n<0)exit(0);
	Edge* temp = G[m];
	while(temp!=nullptr)
	{
		if(temp->endpoint==n)return temp->weight;
		temp=temp->Next;
	}
	return INF;
}

void Graph::checkPrint()
{
	for(int i=0;i<v_nums;++i)
	{
		Edge* temp= G[i];
		cout<<i<<':';
		while(temp!=nullptr)
		{
			cout<<temp->endpoint<<' ';
			temp=temp->Next;
		}
		cout<<endl;
	}
}

void Graph::release(int k)
{
	Edge* temp;
	while (G[k] != nullptr)
	{
		temp = G[k];
		G[k] = temp->Next;
		delete temp;
	}
}

int Graph::FirstNeighbour(int m)const
{
	if (G[m] == nullptr)exit(0);
	int MinNeighbour = -1;
	int MinWeight = INF;
	Edge* temp = G[m];
	while(temp!=nullptr)
	{
		if((temp->weight)<MinWeight)
		{
			MinNeighbour = temp->endpoint;
			MinWeight = temp->weight;
		}
		temp=temp->Next;
	}
	return MinNeighbour;
}

int Graph::NextNeighbour(int m, int n)const
{
	if (G[m] == nullptr)exit(0);
	int nWeight = checkWeight(m,n);
	int MinNeighbour = -1;
	int MinWeight = INF;
	Edge* temp = G[m];
	while (temp != nullptr)
	{
		if((temp->weight)<MinWeight && (temp->weight)>nWeight)
		{
			MinNeighbour = temp->endpoint;
			MinWeight = temp->weight;
		}
		temp=temp->Next;
	}
	return MinNeighbour;
}

Graph::Graph(double *g, int n)
{
	v_nums = n;
	G = new Edge*[n];
	Edge* temp1;
	Edge* temp2;
	bool isNull;
	for (int i = 0; i<n; ++i)
	{
		temp1 = nullptr;
		temp2 = nullptr;
		isNull = true;
		for (int j = 0; j<n; ++j)
			if (g[i*n + j] != INF && g[i*n + j] != 0)
			{
				temp1 = temp2;
				temp2 = new Edge;
				temp2->endpoint = j;
				temp2->Next = nullptr;
				temp2->weight = g[i*n + j];
				if (isNull)
				{
					G[i] = temp2;
					isNull = false;
					continue;
				}
				temp1->Next = temp2;
			}
	}
}

Graph::Graph(Graph& g)
{
	v_nums = g.v_nums;
	G = new Edge*[v_nums];
	Edge* temp1;
	Edge* temp2;
	Edge* tempg;
	bool isNull;
	for (int i = 0; i<v_nums; ++i)
	{
		tempg = g.G[i];
		temp1 = nullptr;
		temp2 = nullptr;
		isNull = true;
		while (tempg != nullptr)
		{
			temp1 = temp2;
			temp2 = new Edge;
			temp2->endpoint = tempg->endpoint;
			temp2->weight = tempg->weight;
			tempg = tempg->Next;
			if (isNull)
			{
				isNull = false;
				G[i] = temp2;
				continue;
			}
			temp1->Next = temp2;
		}
	}
}

void Graph::BFS(int n=0)
{
	if(n>v_nums-1 || n<0)exit(0);
	int i,deque;
	int scanTemp;
	int counts=0;
	queue<int> Q;
	enum ScanColor{white,grey,black};
	ScanColor *Color = new ScanColor[v_nums];
	for(i=0;i<v_nums;i++)Color[i]=white;

	Q.push(n);
	Color[n]=grey;
	while(!Q.empty() && counts<v_nums)
	{
		deque = Q.front();
		Q.pop();
		cout << deque << ' ';
		counts++;
		Color[deque]=black;
		scanTemp = FirstNeighbour(deque);
		while(scanTemp!=-1)
		{	
			if(Color[scanTemp]==white)
			{	
				Q.push(scanTemp);
				Color[scanTemp]=grey;
				scanTemp=NextNeighbour(deque,scanTemp);
			}
		}
	}

	delete[] Color;
}

void Graph::DFS(int n=0)
{

}