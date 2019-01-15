#include"MST.h"

void Heap::swim(int k)
{
	if (IsMax)
		while (k>1 && root[k].ConnectWeight>root[k / 2].ConnectWeight)
		{
			exchange(root + k, root + k / 2);
			k /= 2;
		}
	else
		while (k>1 && root[k].ConnectWeight<root[k / 2].ConnectWeight)
		{
			exchange(root + k, root + k / 2);
			k /= 2;
		}
}

void Heap::sink(int k)
{
	if (IsMax)
		while (2 * k <= N)
		{
			int j = 2 * k;
			if (j<N && root[j].ConnectWeight<root[j + 1].ConnectWeight)j++;
			if (root[k].ConnectWeight >= root[j].ConnectWeight)break;
			exchange(root + k, root + j);
			k = j;
		}
	else
		while (2 * k <= N)
		{
			int j = 2 * k;
			if (j<N && root[j].ConnectWeight>root[j + 1].ConnectWeight)j++;
			if (root[k].ConnectWeight <= root[j].ConnectWeight)break;
			exchange(root + k, root + j);
			k = j;
		}
}

bool Heap::insert(VEV v)
{
	if (N == msize)return false;
	root[++N] = v;
	swim(N);
	return true;
}

VEV Heap::Deltop()
{
	if (isEmpty())exit(1);
	VEV Top = top();
	exchange(root + 1, root + N);
	N--;
	sink(1);
	return Top;
}