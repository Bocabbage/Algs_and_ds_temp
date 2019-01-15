
#include"MST.h"
void exchange(VEV* a, VEV* b)
{
	VEV temp;
	temp.Endpoint1 = a->Endpoint1;
	temp.ConnectWeight = a->ConnectWeight;
	temp.Endpoint2 = a->Endpoint2;

	a->Endpoint1 = b->Endpoint1;
	a->ConnectWeight = b->ConnectWeight;
	a->Endpoint2 = b->Endpoint2;

	b->Endpoint1 = temp.Endpoint1;
	b->ConnectWeight = temp.ConnectWeight;
	b->Endpoint2 = temp.Endpoint2;
}
void Graph::readData(double *data, int N)
{
	for (int i = 0; i<N; ++i)
		for (int j = 0; j<N; ++j)
			G[i][j] = data[i*N + j];
}

void Graph::print()
{
	for (int i = 0; i<n; ++i)
	{
		for (int j = 0; j<n; ++j)
			cout << G[i][j] << ' ';
		cout << endl;
	}
}

int Graph::FindFirstNeighbour(int M)
{
	// 返回与M相连的权值最小顶点
	double Min = INF;
	int MinIndex = 0;
	for (int i = 0; i<n; ++i)
		if (G[M][i]<Min && i!=M)
		{
			Min = G[M][i];
			MinIndex = i;
		}
	return MinIndex;
}

int Graph::FindNextNeighbour(int M, int v)
{
	// 返回与M相连的仅次于v顶点的相邻权值最小者
	double vVal = G[M][v];
	double Min = INF;
	int MinIndex = v;
	for (int i = 0; i<n; ++i)
		if (G[M][i]>vVal && G[M][i]<Min && i!=M)
		{
			Min = G[M][i];
			MinIndex = i;
		}
	// 若v已为与M相连的顶点中边权值最大者，则返回-1
	if (MinIndex == v)return -1;
	else return MinIndex;
}