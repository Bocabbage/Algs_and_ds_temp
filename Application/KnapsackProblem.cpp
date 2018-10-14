/*

	题设：背包问题
	设有一背包可放入的物品的重量为s，现有n件物品，重量分别为w[1],...w[n],
	问能否从这n件物品中选择若干件放入背包中，使得放入的物品重量之和为s。
	如果存在一种符合上述要求的选择，则称背包问题有解，否则无解。

	实现方式：递归
	最后维护时间：2018\10\14(为按照放入背包内顺序输出，使用压栈再弹出代替直接输出)

*/
#include<iostream>
#include<algorithm>
#include<stack>

using std::cout;
using std::endl;
using std::sort;
using std::stack;

int max_under(int a[], int n, int ts, int s)
{
	// 在降序序列中查找a[i]+ts<=s的最大a[i]
	for (int i = 0; i<n; ++i)
		if (a[i] + ts <= s)return i;
	return -1;
}

bool PushIn(int ordered_w[], int n, int ts, int &s,stack<int>& st)
{
	if (ts == s)return true;
	else if (!n)return false;
	int index = max_under(ordered_w,n, ts, s);
	if (index == -1)return false;
	while (true)
	{
		if (PushIn(ordered_w + index + 1, n - index - 1, ts + ordered_w[index], s, st))
		{
			//cout << "重量为" << ordered_w[index] << "的物品入包" << endl;
			st.push(ordered_w[index]);
			return true;
		}
		else
		{
			int temp_index = max_under(ordered_w + index + 1, n - index - 1, ts, s);
			if (temp_index == -1)break;
			else index += (temp_index+1);
		}

	}
	return false;

}


bool compare(int a, int b) { return a>b; }
int main()
{
	int w[] = { 0,1,3,6,11,14,17,23,31 };
	sort(w, w + 9, compare);
	stack<int> st;
	int s = 150;
	if (!PushIn(w, 8, 0, s,st))cout << "该背包问题无解！" << endl;
	else
	{
		while(!st.empty())
		{
			cout << "重量为" << st.top() << "的物品入包" << endl;
			st.pop();
		}
	}
}