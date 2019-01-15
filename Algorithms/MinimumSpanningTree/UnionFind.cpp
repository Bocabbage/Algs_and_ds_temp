#include"MST.h"

bool UnionFind::Union(int a, int b)
{
	// 合并两连通分量
	// 时间复杂度：O(1)
	int ar = Root(a);
	int br = Root(b);
	if (ar == br)return false;
	if (sz[ar] < sz[br])
	{
		id[ar] = br;
		sz[br] += sz[ar];
	}
	else
	{
		id[br] = ar;
		sz[ar] += sz[br];
	}

	count--;
	return true;

}

bool UnionFind::IsConnect(int a, int b)
{
	// 检测两变量是否在同一连通分量里
	// 时间复杂度：O(logn)
	return (Root(a) == Root(b));
}