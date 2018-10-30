#include<fstream>
#include<time.h>
#include"stringFind.h"
/*
	项目：模式串匹配性能比较
	描述：比较朴素模式串查找、KMP算法和BM算法的性能(针对DNA序列)
		  另有testData.cpp生成随机测试数据
	更新时间：2018\10\30(已验证)
*/

int main()
{
   ifstream ifile("test.txt");
   int PairsNums;
   ifile>>PairsNums;
   string T;
   string P;
   getline(ifile,T);		// 将输入流的中的换行符读出

   clock_t start,finish;
   double totaltime;
   start=clock();
   
   for(int i=0;i<PairsNums;++i)
   {
   		getline(ifile,T);
   		getline(ifile,P);
   		//T="atcgatcggtatcatcgatcggtatcatcgatcggtatcatcgatcggtatcatcgatcggtatcatcgatcgatcgat";
   		//P="atcgatcgat";
   		cout<<BM(P,T,4)<<endl;
   }
   finish=clock();
   totaltime=(double)(finish-start)/CLOCKS_PER_SEC;
   cout<<endl<<"runtime:"<<totaltime<<'s'<<endl;

}