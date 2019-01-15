/*
	Dijkstra Algorithm -- 非负权值单源最短路径查询算法
	实现方式：依托Graph.cpp，构建查找函数ShortestPath()及打印函数
			  printShortestPath()
	更新时间：2018\12\14(已验证)
*/

#define INF 5000					// 定义无连接时情况
#include<queue>
#include<iostream>
using std::queue;
using std::cout;
using std::endl;

class Graph;
void ShortestPath(Graph & g,double dist[], int path[],int v);
void printShortestPath(Graph & g, int v, double dist[], int path[]);

struct Edge
{
	int endpoint;
	double weight;
	Edge* Next;
};

class Graph
{
private:
	enum ScanColor { white, grey, black };			// white:未扫描 grey:已完成入队 black:已扫描
	int v_nums;										// 顶点个数
	Edge** G;										// 邻接表数组
	void release(int k);							// 释放链表
	int FirstNeighbour(int m)const;
	int NextNeighbour(int m, int n)const;
public:
	Graph() :v_nums(0), G(nullptr) {}
	Graph(double *g, int n);					// 接受一张二维邻接矩阵(n*n)
	Graph(Graph& g);
	~Graph() { for (int i = 0; i<v_nums; ++i)release(i); }
	int checkV_nums() { return v_nums; }
	void checkPrint();							// 输出邻接表进行检查
	double checkWeight(int m, int n)const;		// 查看两顶点间权值


};

double Graph::checkWeight(int m, int n)const
{
	if (m>v_nums - 1 || n>v_nums - 1 || m<0 || n<0)exit(0);
	if (m == n) return 0;
	Edge* temp = G[m];
	while (temp != nullptr)
	{
		if (temp->endpoint == n)return temp->weight;
		temp = temp->Next;
	}
	return INF;
}

void Graph::checkPrint()
{
	for (int i = 0; i<v_nums; ++i)
	{
		Edge* temp = G[i];
		cout << i << ':';
		while (temp != nullptr)
		{
			cout << temp->endpoint << ' ';
			temp = temp->Next;
		}
		cout << endl;
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
	// 有序查找与顶点m相连的顶点n，边E(m,n)为权值最小项
	// 适用于带权有向/无向图
	if (G[m] == nullptr)exit(0);
	int MinNeighbour = -1;
	int MinWeight = INF;
	Edge* temp = G[m];
	while (temp != nullptr)
	{
		if ((temp->weight)<MinWeight)
		{
			MinNeighbour = temp->endpoint;
			MinWeight = temp->weight;
		}
		temp = temp->Next;
	}
	return MinNeighbour;
}

int Graph::NextNeighbour(int m, int n)const
{
	// 有序查找与顶点m相连的顶点k，边E(m,k)的权值仅次于E(m,n)
	if (G[m] == nullptr)exit(0);
	if (m == n)return FirstNeighbour(m);		// 退化为FirstNeighbour的情况
	int nWeight = checkWeight(m, n);
	int MinNeighbour = -1;
	int MinWeight = INF;
	Edge* temp = G[m];
	while (temp != nullptr)
	{
		if ((temp->weight)<MinWeight && (temp->weight)>nWeight)
		{
			MinNeighbour = temp->endpoint;
			MinWeight = temp->weight;
		}
		temp = temp->Next;
	}
	return MinNeighbour;
}

Graph::Graph(double *g, int n)
{
	// 构造函数
	// 参数：一维指针*g(元素个数:n*n) 顶点个数n
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
		if (isNull)G[i] = nullptr;
	}
}

Graph::Graph(Graph& g)
{
	// 复制构造函数
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

void ShortestPath(Graph & g, double dist[], int path[], int v = 0)
{
	// Dijkstra Algorithm
	// 针对非负权值单源最短路径
	// 算法时间复杂度：O(V^2)

	int n = g.checkV_nums();
	bool *S = new bool[n];	// 最短路径顶点集
	int i, j, k;
	double w, min;
	for (i = 0; i<n; ++i)
	{
		// 初始化
		dist[i] = g.checkWeight(v, i);
		S[i] = false;
		if (i != v && dist[i] < INF)path[i] = v;
		else path[i] = -1;
	}

	S[v] = true;
	for (i = 0; i<n - 1; i++)
	{
		min = INF;
		int u = v;
		for (j = 0; j<n; j++)
			if (!S[j] && dist[j] < min)
			{
				// 找到dist[j]最小值
				u = j;
				min = dist[j];
			}
		S[u] = true;
		for (k = 0; k<n; k++)
		{
			// 更新S到V-S集合的最短距离
			w = g.checkWeight(u, k);
			if (!S[k] && w<INF && dist[u] + w<dist[k])
			{
				dist[k] = dist[u] + w;
				path[k] = u;
			}
		}
	}
}

void printShortestPath(Graph & g, int v, double dist[], int path[])
{
	cout << "from vertex:" << v << "to other vertices:" << endl;
	int i, j, k;
	int n = g.checkV_nums();
	int *d = new int[n];
	for (i = 0; i<n; ++i)
		if (i != v)
		{
			j = i;
			k = 0;
			while (j != v)
			{
				d[k++] = j;
				j = path[j];
			}
			cout << "to vertex:" << i << ",shortest path is:" << v;
			while (k>0)
				cout << d[--k] << " ";
			cout << "the length of this path:" << dist[i] << endl;
		}
	delete[] d;

}
