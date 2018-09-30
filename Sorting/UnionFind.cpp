//并查集类
#include<iostream>
using namespace std;

class UnionFind
{
private:
	int* id;
	int* sz;
	int Root(int p)
	{	
		int tempP = p;
		while (id[p] != p)p = id[p];
		id[tempP] = p;	//Â·¾¶Ñ¹Ëõ
		return p;
	}
public:
	UnionFind() { id = new int[10]; sz = new int[10]; for (size_t i = 0; i < 10; i++)id[i] = i,sz[i] = 1; }
	UnionFind(int n) { id = new int[n]; sz = new int[n]; for (size_t i = 0; i < n; i++)id[i] = i, sz[i] = 1; }
	~UnionFind() { delete[]id; delete[]sz; }

	bool Union(int,int);
	bool IsConnect(int,int);

	//¼ì²éº¯Êý
	int checksize(int p) { return sz[p]; }

};

bool UnionFind::Union(int a, int b)
{
	int ar = Root(a);
	int br = Root(b);
	if (ar==br)return false;
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
	
}

bool UnionFind::IsConnect(int a, int b)
{
	return (Root(a) == Root(b));
}

int main(void)
{
	UnionFind Test(10);
	Test.Union(4, 3);
	Test.Union(3, 8);
	Test.Union(6, 5);
	Test.Union(9, 4);
	Test.Union(2, 1);
	Test.Union(8, 9);
	Test.Union(5, 0);
	Test.Union(7, 2);
	Test.Union(6, 1);
	Test.Union(1, 0);
	Test.Union(6, 7);
	cout << Test.checksize(5);
}
