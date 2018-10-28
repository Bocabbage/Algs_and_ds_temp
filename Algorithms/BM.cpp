/*
	项目：Boyer-Moore 字符串匹配算法
	描述：利用BM算法进行模式串查找，BM算法使用两个启发式子算法：
		 Bad-Character Shift(坏字符移动)
		 Good-Suffix Shift	(好后缀移动)
	更新时间：2018\10\28(已验证)
*/

#include<iostream>
#include<string>
using std::string;
using std::cout;
using std::endl;

void getBC(int bc[],const string& P,int bc_n);	// 产生Bad-Character移动表的子方法
void getGS(int gs[],const string& P);				// 产生Good-Suffix移动表的子方法
void get_suffix(int su[],const string& P);			// getGS()调用的子方法，产生后缀表
int BM(const string& P,const string& T);			// Boyer-Moore Algorithm


void getBC(int bc[],const string& P,int bc_n=256)
{
	// Bad-Character抽象为Hash表来查询每个坏字符i匹配时
	// 模式串应向前移动至bc[i]与坏字符对齐
	// 若模式串中不存在坏字符则bc[i]=1
	// Hash函数：默认映射为该字符的ASCII码，可拓展更改
	// 接受参数：BC表指针*bc,BC表长度(取决于字符种类),模式串P

	const int len=P.length();
	int i;
	for(i=0;i<bc_n;i++)bc[i]=-1;		// 初始化为-1,表示该字符未在模式串中出现
	for(i=0;i<len;i++)					// 从左向右遍历，记录最右位置覆盖保存
		bc[P[i]] = i;

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
	const int plen=P.length();
	const int tlen=T.length();
	int *bc = new int[256];
	int *gs = new int[plen];
	getBC(bc,P);
	getGS(gs,P);
	int p=0;			// p指明P和T串当前匹配的第一个元素位置
	int q;				// q指明当前比较的位置
	while(tlen>=(plen+p))
	{
		for(q=plen-1;q>=0 && P[q]==T[p+q];q--);
		if(q==-1)break;
		else
			p+=max(gs[q],q-bc[T[p+q]]);
	}
	delete[] bc;
	delete[] gs;
	return (p+plen <= tlen)?p:-1;
}