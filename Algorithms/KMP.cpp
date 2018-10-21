/*
	项目：Knuth-Morris-Pratt 字串匹配算法
	描述：利用KMP算法进行模式串的搜索
	时间渐进复杂度：θ(N)(其中辅助函数π的时间复杂度：θ（M))
	【N为被搜索串长度，M为模式串长度】
	更新时间：2018\10\21(已验证)
*/

#include<iostream>
#include<string>
using std::string;
using std::cout;
using std::endl;

void ComparePrefix(int preP[], int n, string &P);		// sub-method ：前缀函数
void KMP(string &T, string &P);							// KMP-Algorithm

void KMP(string &T, string &P)
{
	/* 参数：被搜索串T，模式串P */

	int pl = P.length();
	int tl = T.length();
	
	int *preP = new int[pl];		// preP:作为Prefix函数的数据接口，得到前缀表
	ComparePrefix(preP, pl, P);
	int q = -1;						// 用于记录当前比对到模式串的第q位，-1表示没有匹配
	for (int i = 0; i<tl; ++i)
	{
		while (q >= 0 && P[q + 1] != T[i])		// 检测到q+1位不匹配，则比对位置移动到模式串最长前缀
			q = preP[q];
		if (P[q + 1] == T[i])					// 检测到q+1位匹配，则将q+1纳入已匹配
			q += 1;
		if (q == pl - 1)
		{
			cout << "从第" << i - pl + 1 << "个字符到第" << i << "个字符匹配" << endl;
			q = preP[q];						// 寻找下一个匹配段
		}

	}
	delete[] preP;
}

void ComparePrefix(int preP[], int n, string &P)
{	
	/*参数：前缀记录数组，数组长度，模式串*/

	preP[0] = -1;				// 记录“已匹配”的模式串子串最大下标
	int k = -1;
	for (int q = 1; q<n; ++q)
	{
		// 经摊还分析可得在for循环中while循环最多
		// 迭代总和为m-1次(详见<算法导论>)
		while (k >= 0 && P[k + 1] != P[q])
			k = preP[k];
		if (P[k + 1] == P[q])
			k += 1;
		preP[q] = k;
	}
}

