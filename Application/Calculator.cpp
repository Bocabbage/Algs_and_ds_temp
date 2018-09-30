/*
		
		项目:利用双栈维护一个表达式计算器
		最后维护时间:2018\9\30
		

		进度:维护正整数二元运算(2018\9\30)	(已验证)
*/


#include<iostream>
#include<stack>
#include<cmath>
#include<string>

using namespace std;
const int N_OPTR = 7;
const char pri[N_OPTR][N_OPTR]=
{	
	// 栈顶运算符相对于当前运算符的优先级比较

	/*	|	|----------	当前运算符--------|*/
	/*	|		+	-	*	/	^	(	)	*/
	/*	栈	+*/'>',	'>','<','<','<','<','>',
	/*	顶	-*/'>',	'>','<','<','<','<','>',
	/*	运	**/'>',	'>','>','>','<','<','>',
	/*	算	/*/'>',	'>','>','>','<','<','>',
	/*	符	^*/'>',	'>','>','>','>','<','>',
	/*	|	(*/'<',	'<','<','<','<','<','=',
	/*	|	)*/' ',	' ',' ',' ',' ',' ',' '		// 右括号不存在入栈操作
};

int str2num(string&);				// 读取子串，将char[]转换为int值
char PriJudge(char&,char&);			// 判断栈顶与当前运算符优先级的接口
int cal2(int&,int&,char&);			// 二元操作结果计算

int main()
{
	stack<int> Nums;
	stack<char> Op;
	string Expr;
	size_t i;						// 遍历计数
	size_t k;						// 字符串->数字哨兵

	cout<<"输入表达式:"<<endl;
	while(getline(cin,Expr))
	{
		i=0;
		while(i<Expr.length())
		{
			if(48 <= Expr[i] && Expr[i] <= 57)		// 捕获到数字
			{
				for(k=0;48 <= Expr[i+k] && Expr[i+k] <= 57;k++);
				Nums.push(str2num(Expr.substr(i,k)));	// 数字入栈
				i+=k;
			}
			else					// 捕获到操作符
			{	
				if(Op.empty()){Op.push(Expr[i]);i++;}	// 若栈空，将操作符压入
				else
				{
					switch(PriJudge(Op.top(),Expr[i]))
					{
						case '<': Op.push(Expr[i]);i++;break;	// 栈顶操作符优先级低，则缓冲
						case '=': Op.pop();i++;break;			// 当且仅当右括号与左括号匹配时，弹出左括号
						case '>': 								
						{
							int a=Nums.top();Nums.pop();
							int b=Nums.top();Nums.pop();
							Nums.push(cal2(b,a,Op.top()));		// 注意此处操作数顺序
							Op.pop();
							if(Expr[i]!=')')
								Op.push(Expr[i++]);
							
						}break;

						default:exit(1);
					}
				}

			}
		}
		while(!Op.empty())
		{
			int a=Nums.top();Nums.pop();
			int b=Nums.top();Nums.pop();
			Nums.push(cal2(a,b,Op.top()));
			Op.pop();
		}

		cout<<Nums.top()<<endl;
		Nums.pop();
		cout<<"输入表达式:"<<endl;
	}
}


int str2num(string& S)
{
	size_t _size= S.size();
	int _sum=0;
	int _w=1;

	for(int i=_size-1;i>=0;i--)
	{
		_sum+=(S[i]-48)*_w;
		_w*=10;
	}

	return _sum;
}

char PriJudge(char& a,char& b)
{
	int index_a;	// 栈顶操作符
	int index_b;	// 当前操作符

	switch(a)
	{
		case '+':index_a=0;break;
		case '-':index_a=1;break;
		case '*':index_a=2;break;
		case '/':index_a=3;break;
		case '^':index_a=4;break;
		case '(':index_a=5;break;
		case ')':index_a=6;break;
	}

	switch(b)
	{
		case '+':index_b=0;break;
		case '-':index_b=1;break;
		case '*':index_b=2;break;
		case '/':index_b=3;break;
		case '^':index_b=4;break;
		case '(':index_b=5;break;
		case ')':index_b=6;break;
	}

	return pri[index_a][index_b];
}

int cal2(int& a,int& b,char& c)
{
	int result;
	switch(c)
	{
		case '+': result=a+b;break;
		case '-': result=a-b;break;
		case '*': result=a*b;break;
		case '/': result=a/b;break;
		case '^': result=pow(a,b);break;
	}

	return result;
}