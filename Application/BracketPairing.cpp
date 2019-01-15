/*
	练习：括号匹配
	更新时间：2019\1\15(增加栈空检查：排除左括号冗余错误)
*/
#include<iostream>
#include<stack>
using namespace std;

bool BracketPairing(string& str)
{
	stack<char> Leftp;
	for(int i=0;i<str.length();++i)
	{
		if(str[i]=='('||str[i]=='['||str[i]=='{')
			Leftp.push(str[i]);
		else if(str[i]==')')
		{
			if(Leftp.top()!='('||Leftp.empty())return false;
			Leftp.pop();
		}
		else if(str[i]==']'||Leftp.empty())
		{
			if(Leftp.top()!='[')return false;
			Leftp.pop();
		}
		else if(str[i]=='}'||Leftp.empty())
		{
			if(Leftp.top()!='{')return false;
			Leftp.pop();
		}
	}
	if(!Leftp.empty())return false;
	return true;
}