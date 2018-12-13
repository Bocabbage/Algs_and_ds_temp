/*
Graph类(有向赋权图)
实现思路：邻接链表实现，有向/无向取决于使用方式
提供深度/广度遍历接口，未提供泛用接口
若需提升泛用性，使用时添加Vertex结构来记录顶点属性，
再扩展泛用接口
更新时间：2018\12\11：构建基本框架，实现BFS
		  2018\12\12：实现DFS，完成注释，完成BFS/DFS算法复杂度分析
		  2018\12\13：修改checkWeight：增加m==n情况

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
	enum ScanColor { white, grey, black };		// white:未扫描 grey:已完成入队 black:已扫描
	int v_nums;									// 顶点个数
	Edge** G;									// 邻接表数组
	void release(int k);						// 释放链表
	int FirstNeighbour(int m)const;
	int NextNeighbour(int m, int n)const;
	void DFS_visit(int u, ScanColor* Color);
public:
	Graph() :v_nums(0), G(nullptr) {}
	Graph(double *g, int n);					// 接受一张二维邻接矩阵(n*n)
	Graph(Graph& g);
	~Graph() { for (int i = 0; i<v_nums; ++i)release(i); }
	void BFS(int n);							// 广度优先搜索接口：不提供泛用接口
	void DFS(int n);							// 深度优先搜索接口：不提供泛用接口
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

void Graph::BFS(int n = 0)
{
	// BFS(Breadth-First Search)：广度优先搜索
	// 可应用于"无权图"最短路径的查询
	// 借助辅助队列实现
	// (邻接表)算法时间复杂度：O(V+E)
	if (n>v_nums - 1 || n<0)exit(0);
	int i, deque;
	int scanTemp;								// 扫描临时变量
	Edge* ScanTemp;								// 扫描临时指针
	int counts = 0;								// 记录已扫描顶点数
	queue<int> Q;								// 辅助队列
	ScanColor *Color = new ScanColor[v_nums];	// 用于记录顶点扫描状态的数组
	for (i = 0; i<v_nums; i++)Color[i] = white;	// 上述数组初始化

	Q.push(n);
	Color[n] = grey;
	while (!Q.empty() && counts<v_nums)
	{
		// 根据循环条件及循环体可知，大循环执行v_nums次

		deque = Q.front();
		Q.pop();
		/* visit()过程，以输出代替 */
		cout << deque << ' ';
		/*************************/
		counts++;
		Color[deque] = black;
		// 2018\12\12修正：BFS构建为普通遍历并服务于无权图最短路搜索
		// 不使用FirstNeighbour扫描元素，下方NextNeighbour更新同
		// scanTemp = FirstNeighbour(deque);
		ScanTemp = G[deque];
		while (ScanTemp != nullptr)
		{
			// 根据聚合分析，该小循环执行总次数与边数E呈线性关系

			scanTemp = ScanTemp->endpoint;
			if (Color[scanTemp] == white)
			{
				Q.push(scanTemp);
				Color[scanTemp] = grey;
			}
			// 2018\12\12修正：
			// scanTemp=NextNeighbour(deque,scanTemp);
			ScanTemp = ScanTemp->Next;
		}
	}

	delete[] Color;
}

void Graph::DFS(int n = 0)
{
	// DFS(Depth-First Search)：深度优先搜索
	// (邻接表)算法时间复杂度：O(V+E)
	if (n>v_nums - 1 || n<0)exit(0);
	int i;
	ScanColor *Color = new ScanColor[v_nums];
	for (i = 0; i<v_nums; ++i)
		Color[i] = white;
	for (i = 0; i<v_nums; ++i)
		if (Color[i] == white)
			DFS_visit(i, Color);

	delete[] Color;
}

void Graph::DFS_visit(int u, ScanColor* Color)
{
	// 整个DFS中，DFS_visit过程执行v_nums次
	// DFS_visit内部while循环总执行次数与边数E呈线性关系

	/*
	// 类中序遍历版本
	Color[u] = grey;
	Edge* ScanTemp = G[u];
	int scanTemp;
	while(ScanTemp!=nullptr)
	{
	scanTemp = ScanTemp->endpoint;
	if(Color[scanTemp]==white)
	DFS_visit(scanTemp,Color);
	ScanTemp = ScanTemp->Next;
	}
	cout << u << ' ';
	Color[u] = black;
	*/

	// 类前序遍历版本
	/* visit()过程，以输出代替 */
	cout << u << ' ';
	/*************************/
	Color[u] = black;
	Edge* ScanTemp = G[u];
	int scanTemp;
	while (ScanTemp != nullptr)
	{
		scanTemp = ScanTemp->endpoint;
		if (Color[scanTemp] == white)
			DFS_visit(scanTemp, Color);
		ScanTemp = ScanTemp->Next;
	}

}