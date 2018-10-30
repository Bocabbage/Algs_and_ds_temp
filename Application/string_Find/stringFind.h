#include<iostream>
#include<string>
using namespace std;


/* BF Algorithm */
int BF(const string& P,const string& T);			// BF Algorithm


/* KMP Algorithm */
void ComparePrefix(int preP[], int n,const string &P);		// sub-method ：前缀函数
int KMP(const string& P,const string& T);					// KMP Algorithm

/* BM Algorithm */
void getBC(int bc[],const string& P,int bc_n);				// 产生Bad-Character移动表的子方法
void getGS(int gs[],const string& P);						// 产生Good-Suffix移动表的子方法
void get_suffix(int su[],const string& P);					// getGS()调用的子方法，产生后缀表
int BM(const string& P,const string& T,int bc_n);			// Boyer-Moore Algorithm