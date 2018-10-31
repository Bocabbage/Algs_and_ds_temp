#include"stringFind.h"
const int DHash=5;

/* BF算法*/
int BF(const string& P,const string& T)
{
	int pl= P.length();
	int tl= T.length();
	int i,j;
	for(i=0;i<=tl-pl;i++)
	{	
		for(j=0;j<pl && P[j]==T[i+j];j++);
		if(j==pl)return i;
	}
	return -1;
}

/********/

/* KMP算法*/
int KMP(const string& P,const string& T)
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
			return i-pl+1;

	}
	delete[] preP;
	return -1;
}

void ComparePrefix(int preP[], int n,const string &P)
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
/********/


/*BM算法*/
void getBC(int bc[],const string& P)
{
	// Bad-Character抽象为Hash表来查询每个坏字符i匹配时
	// 模式串应向前移动至bc[i]与坏字符对齐
	// 若模式串中不存在坏字符则bc[i]=1
	// Hash函数：默认映射为该字符的ASCII码，可拓展更改
	// 接受参数：BC表指针*bc,BC表长度(取决于字符种类),模式串P

	const int len=P.length();
	int i;
	for(i=0;i<DHash;i++)bc[i]=-1;		// 初始化为-1,表示该字符未在模式串中出现
	for(i=0;i<len;i++)					// 从左向右遍历，记录最右位置覆盖保存
		//bc[P[i]] = i;
		bc[P[i]%DHash]=i;					// 针对ATCG的特化Hash函数

}

void getGS(int gs[],const string& P)
{
	// gs[]数组长度为P.length(),表示在P[i]遇到好后缀时模式串移动的距离
	// 三个主for循环的顺序不可调换
	const int len=P.length();
	int *suffix = new int[len];
	get_suffix(suffix,P);
	int i;
	int Index;
	// 第一个循环:默认将整个模式串移动一个P.length()
	for(i=0;i<len;i++)
		gs[i]=len;
	// 第二个循环：找到与后缀对应的最佳前缀(类似于KMP的子过程)
	// 待优化(!)
	for(i=len-1;i>=0;i--)	// 从后向前遍历,保证“安全移动”
		if(suffix[i]==i+1)	// 存在一个前缀与后缀相同
		{	Index = len-1-i;
			for(int j=0;j<Index;j++)
				if(gs[j]==len)gs[j]=Index;
		}
	// 第三个循环：找到一个串中子串
	for(i=0;i<len-1;i++)
		gs[len-1-suffix[i]]=len-1-i;

	delete[] suffix;
}

void get_suffix(int su[],const string& P)
{
	// 接受参数：后缀记录数组su(长度为P.length())，模式串P
	// su[i]记录的是当前位置作为末元素可以匹配到的最大模式串后缀长度

	const int len=P.length();
	int i;
	int pn;					// 临时记录匹配长度

	su[len-1]=len;
	for(i=len-2;i>=0;i--)
	{
		pn=0;
		while(pn<=i && (P[i-pn]==P[len-1-pn]))pn++;
		su[i]=pn;
	}

}

int max(int a,int b){return (a>b)?a:b;}

int BM(const string& P,const string& T)	
{
	const int pl=P.length();
	const int tl=T.length();
	int *bc = new int[DHash];
	int *gs = new int[pl];
	getBC(bc,P);
	getGS(gs,P);
	int p=0;			// p指明P和T串当前匹配的第一个元素位置
	int q;				// q指明当前比较的位置
	while(tl>=(pl+p))
	{
		for(q=pl-1;q>=0 && P[q]==T[p+q];q--);
		if(q==-1)break;
		else
			p+=max(gs[q],q-bc[T[p+q]%DHash]);		// 针对'ATCG'字符集的特化
	}
	delete[] bc;
	delete[] gs;
	return (p+pl <= tl)?p:-1;
}
