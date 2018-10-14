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

	return true;
}