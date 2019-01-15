/*
	项目：无向连通图-最小生成树(Minimum Spanning Tree)
	实现方法：Kruskal(并查集与最小堆实现) / Prim(最小堆实现)
	更新日期：2018\12\8(已验证)
*/

#include"MST.h"

void Kruskal(Graph g, VEV *MST)
{
	/*
		Kruskal-Algorithm:扩边法
		接受参数：图(Graph)，返回的MST(点-边-点三元组数组)
		构建最小生成树：
			扫描二维矩阵：O(n^2)
			建堆时间复杂度(insert)：O(eloge)
			O(e)次出堆(Deltop)：O(eloge)，
			O(e)次并查集IsConnect()：O(elogn)
			n-1次Union(): O(n)
		总时间复杂度: O(n^2+n+eloge+elogn)
	*/
	int N = g.n;
	UnionFind uf(N);                 // 辅助并查集
	Heap h(N*(N + 1) / 2, false);    // 辅助最小堆
	// 辅助临时变量
	int i, j;
	VEV temp;

	for (i = 0; i<N; ++i)
		for (j = i+1; j<N; ++j)
		{	
			// 扫描矩阵，边入堆
			if (g.G[i][j] == INF)continue;
			temp.Endpoint1 = i;
			temp.Endpoint2 = j;
			temp.ConnectWeight = g.G[i][j];
			h.insert(temp);
		}
	
	int index = 0;
	while (/*index<N-1*/uf.Count()>1)
	{
		// 出堆，生成MST
		temp = h.top();
		h.Deltop();
		if (!uf.IsConnect(temp.Endpoint1, temp.Endpoint2))
		{
			MST[index++] = temp;
			uf.Union(temp.Endpoint1, temp.Endpoint2);
		}
	}
}

void Prim(Graph g, VEV* MST)
{
	/*
		Prim-Algorithm：扩点法
		接受参数：图(Graph)，返回的MST(点-边-点三元组数组)
		构建最小生成树：
			总迭代次数：O(n)，每次迭代：
				平均堆插入2e/n次：O(eloge/n)
				e条边从堆中删除：O(eloge)
		总时间复杂度：O(neloge)
	*/
	int N = g.n;
	Heap h(N*(N + 1) / 2, false);	// 辅助最小堆
	bool *VinMST = new bool[N];		// 记录顶点是否在MST中的bool数组
	// 辅助变量
	int i;
	int u, v;
	VEV temp;
	for (i = 0; i<N; ++i)
		VinMST[i] = false;
	u = 0;
	VinMST[u] = true;
	int count = 0;	// 已纳入MST的顶点数

	do
	{
		v = g.FindFirstNeighbour(u);
		while (v != -1)
		{
			if (!VinMST[v])
			{
				// 注意此处赋值的顶点顺序
				temp.Endpoint2 = u;
				temp.Endpoint1 = v;
				temp.ConnectWeight = g.G[u][v];
				h.insert(temp);
			}
			v = g.FindNextNeighbour(u, v);
		}

		while ((!h.isEmpty()) && count < N-1)	// 注意此处为n-1
		{
			temp = h.top();
			h.Deltop();
			if (!VinMST[temp.Endpoint1])
			{	
				// 若点不在MST内则纳入
				MST[count++] = temp;
				u = temp.Endpoint1;
				VinMST[u] = true;
				break;
			}
		}

	} while (count<N-1);

}

int main()
{
	double G[49] = { 0, 28, INF,INF,INF,10, INF,
					28, 0,  16, INF,INF,INF,14,
					INF,16, 0,  12, INF,INF,INF,
					INF,INF,12, 0,  22, INF,18,
					INF,INF,INF,22, 0,  25, 24,
					10, INF,INF,INF,25, 0,  INF,
					INF,14, INF,18, 24, INF,0 };
	VEV mst[6];
	Graph g(7);
	g.readData(G, 7);
	//Kruskal(g, mst);
	Prim(g, mst);
	for (int i = 0; i < 6; ++i)
	{
		cout << "EndPoint1:" << mst[i].Endpoint1 << '\t';
		cout << "EndPoint2:" << mst[i].Endpoint2 << '\t';
		cout << "ConnectWeight:" << mst[i].ConnectWeight << '\t';
		cout << endl;
	}
}