#include<iostream>

int NumTrans(int num,int N)
{
	/*
		进制转换接口：
			接受参数：非负整数num(有检查)，目标进制N(1<N<17)
			功能：接受非负整数num与目标进制N，打印进制转换后的num
	*/

	if(num<0||N<2||N>16)exit(1);
	if(num==0){std::cout<<'0';return 0;}
	if(num==1){std::cout<<'1';return 0;}
	NumTrans(num/N,N);
	char num_r;
	if(num%N>=10)
		switch(num%N)
		{
			case 10:num_r='A';break;
			case 11:num_r='B';break;
			case 12:num_r='C';break;
			case 13:num_r='D';break;
			case 14:num_r='E';break;
			case 15:num_r='F';break;
			default:exit(1);
		}
	else num_r=48+num%N;
	std::cout<<num_r;
	return 0;
}