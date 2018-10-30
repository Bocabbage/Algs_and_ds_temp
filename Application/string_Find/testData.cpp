#include<iostream>
#include<fstream>
#include<string>
#include<time.h>
using std::string;
using namespace std;
const int PairsNums = 20000;		// 设置文件中T-P串对数量
const int Tlen = 2000;				// 每组数据中被查找串长度
const int Plen = 10;				// 每组数据中模式串长度
int main()
{
	srand((unsigned)time(NULL));
	ofstream ofile("test.txt");
	int i, j;
	ofile<<PairsNums<<'\n';
	for (i = 0; i<PairsNums; ++i)
	{
		string T;
		string P;
		for (j = 0; j<Tlen; ++j)
		{
			int temp = rand() % 4;
			switch (temp)
			{
			case 0:T += 'A'; break;
			case 1:T += 'T'; break;
			case 2:T += 'C'; break;
			case 3:T += 'G';
			}
		}

		for (j = 0; j<Plen; ++j)
		{
			int temp = rand() % 4;
			switch (temp)
			{
			case 0:P+='A'; break;
			case 1:P += 'T'; break;
			case 2:P += 'C'; break;
			case 3:P += 'G';
			}
		}
		ofile << T << '\n';
		ofile << P << '\n';
	}

	ofile.close();


}