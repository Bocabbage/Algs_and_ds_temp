#pragma once
#include<iostream>
#define INF 5000
using namespace std;
struct VEV
{	
	// 有权无向图三元组：顶点-边-顶点
	int Endpoint1;
	double ConnectWeight;
	int Endpoint2;
	void operator=(VEV& b)
	{
		Endpoint1 = b.Endpoint1;
		Endpoint2 = b.Endpoint2;
		ConnectWeight = b.ConnectWeight;
	}
};

void exchange(VEV* a, VEV* b);	// 交换两个三元组的值


class Heap
{
	// 二叉堆：动态数组实现
private:
	VEV * root;
	int N;
	int msize;
	bool IsMax;			// IsMax==true则为最大堆，否则为最小堆
	void swim(int k);	// 由下至上的堆有序化
	void sink(int k);	// 由上至下的堆有序化
public:
	Heap() { msize = 50; root = new VEV[msize + 1]; N = 0; IsMax = true; }
	Heap(int n, bool ismax = true) { msize = n; root = new VEV[msize + 1]; N = 0; IsMax = ismax; }
	~Heap() { delete[] root; }
	VEV top() { if (N != 0)return root[1]; }
	bool isEmpty() { return N == 0; }
	int size() { return N; }
	bool insert(VEV v);
	VEV Deltop();
};

class Graph
{

	friend void Kruskal(Graph g, VEV* MST);
	friend void Prim(Graph g, VEV* MST);
private:
	int n;
	double **G;
public:
	Graph(int N = 10)
	{
		n = N;
		G = new double*[n];
		for (int i = 0; i<n; ++i)
			G[i] = new double[n];

	}
	Graph(Graph& g)
	{
		n = g.n;
		G = new double*[n];
		for (int i = 0; i<n; ++i)
			G[i] = new double[n];
		for (int i = 0; i<n; ++i)
			for (int j = 0; j<n; ++j)
				G[i][j] = g.G[i][j];
	}
	~Graph()
	{
		for (int i = 0; i<n; ++i)
			delete[] G[i];
		delete[] G;
	}
	void readData(double *data, int N);
	void print();
	int FindFirstNeighbour(int M);
	int FindNextNeighbour(int M, int v);
};

class UnionFind
{
private:
	int* id;
	int* sz;	// 每个连通分量的元素数量
	int count;	// 连通分量的数量
	int Root(int p)
	{
		// 寻找所在分量的标识符
		int tempP = p;
		while (id[p] != p)p = id[p];
		id[tempP] = p;
		return p;
	}
public:
	UnionFind()
	{
		id = new int[10];
		sz = new int[10];
		count = 10;
		for (int i = 0; i < 10; i++)id[i] = i, sz[i] = 1;
	}
	UnionFind(int n)
	{
		id = new int[n];
		sz = new int[n];
		count = n;
		for (int i = 0; i < n; i++)id[i] = i, sz[i] = 1;
	}
	~UnionFind() { delete[]id; delete[]sz; }

	// Union(p,q)在p,q之间添加一个连接
	// IsConnect(p,q)判断p,q是否存在于同一连通分量中
	bool Union(int, int);
	bool IsConnect(int, int);
	int Count() { return count; }
	int checksize(int p) { return sz[p]; }

};

void Kruskal(Graph g, VEV *MST);
void Prim(Graph g, VEV *MST);