/*
	项目：稀疏矩阵
	描述：用三元组的方式实现稀疏矩阵数据的存储
		 同时维护基本运算，如矩阵加法、乘法与转置
	更新时间：2018\10\22(未验证)
*/

#include<iostream>
using std::cout;
using std::endl;
const int DefaultSize = 100;

template<typename T>
struct Trituple
{
	T value;	// 储存非零元素的值
	int row;	// 非零元素的行坐标
	int col;	// 非零元素的纵坐标

	Trituple<T>& operator=(Trituple<T>& x)	// 结点赋值
	{row=x.row,col=x.col,value=x.value;}

};

template<typename T>
class SparseMatrix
{
	friend ostream& operator<<(ostream& out,SparseMatrix<T>& M);
	friend ostream& operator>>(istream& in,SparseMatrix<T>& M);

private:
	int Rows;				// 行数
	int Cols;				// 列数
	int Terms;				// 非零元素个数
	Trituple<T> *smArray;	// 三元组数组

	// Notes:该三元组数组维护为按行顺序排列
public:
	SparseMatrix(rs=10,cs=10,Terms=0)
	{
		Rows=rs,Cols=cs;
		Terms=Terms;
		smArray=new Trituple<T>[DefaultSize];
	}
	SparseMatrix(SparseMatrix<T>& x)			// 复制构造函数
	{
		Rows=x.Rows;
		Cols=x.Cols;
		Terms=x.Terms;
		smArray = new Trituple<T>[Terms];
		for(int i=0;i<Terms;i++)
			smArray[i] = x.smArray[i];
	}
	~SparseMatrix(){delete[] smArray;}
	SparseMatrix<T> Trans();					// 矩阵转置
	SparseMatrix<T> Add(SparseMatrix<T>& b);	// 矩阵相加
	SparseMatrix<T> Mult(SparseMatrix<T>& b);	// 矩阵相乘


};

template<typename T>
SparseMatrix<T> SparseMatrix<T>::Trans()
{
	/*
		稀疏矩阵的快速转置
		时间复杂度：O(Cols+Terms)=O(Rows*Cols)
		思路：引进两个辅助数组，事先统计好转置后各行非零元素
			 在转置矩阵中的三元组表应该存放的位置

		相较于先按列排序(O(Terms*log(Terms)))再三元组行列号而言时间复杂度更低
	*/
	int *rowSize = new int[Cols];	// 辅助数组1，统计原矩阵各列非零元素个数(b各行非零元素个数)
	int *rowStart = new int[Cols];	// 辅助数组2，预计转置结果b三元组中各行元素开始存放位置
	SparseMatrix<T> b(Cols,Rows,Terms);	// 结果矩阵
	if(Terms>0)
	{
		int i,j;
		// 初始化
		for(i=0;i<Cols;i++)rowSize[i]=0;	
		for(i=0;i<Teams;i++)rowSize[smArray[i].col]++;
		rowStart[0]=0;
		for(i=1;i<Cols;i++)rowStart[i]=rowStart[i-1]+rowSize[i-1];

		for(i=0;i<Teams;i++)
		{
			j=rowStart[smArray[i].col];
			b.smArray[j].row=smArray[i].col;
			b.smArray[j].col=smArray[i].row;
			b.smArray[j].value=smArray[i].value;
			rowStart[smArray[i].col]++;		// 此行已填入一个元素，填入下一个此行元素需到新位置
		}

	}
	delete[] rowSize;
	delete[] rowStart;
	return b;
}

template<typename T>
SparseMatrix<T> SparseMatrix<T>::Add(SparseMatrix<T> &b)
{
	/*
		稀疏矩阵的加法
		时间复杂度：O(Terms+b.Terms)
		思路：类似于有序列的二路归并
	*/
	SparseMatrix<T> result(Rows,Cols,0);
	if(Rows!=b.Rows || Cols!=b.Cols)	// 返回空矩阵
	{
		cout<<"矩阵行列不匹配！"<<endl;
		return result;
	}
	int i=0;
	int j=0;
	int index_a,index_b;
	while(i<Terms && j<b.Terms)
	{	
		// 两个index参数用于比较并确定放入顺序
		index_a = Cols*smArray[i].row + smArray[i].col;
		index_b = Cols*smArray[j].row + b.smArray[j].col;
		if(index_a<index_b)
		{
			result.smArray[result.Terms++]=smArray[i];
			i++;
		}
		else if(index_a>index_b)
		{
			result.smArray[result.Terms++]=b.smArray[j];
			j++;
		}
		else
		{
			result.smArray[result.Terms]=smArray[i];
			result.smArray[result.Terms].value += b.smArray[j].value;
			result.Terms++;
			i++;j++;
		}
	}

	// 复制剩下的元素
	for(;i<Terms;i++)
		result.smArray[result.Terms++]=smArray[i];
	for(;j<b.Terms;j++)
		result.smArray[result.Terms++]=b.smArray[j];

	return result;
}

template<typename T>
SparseMatrix<T> SparseMatrix<T>::Mult(SparseMatrix<T> &b)
{
	/*
		稀疏矩阵的乘法
		时间复杂度：O(max(Rows*b.Cols,Terms*b.Cols))
		思路：建立两个辅助数组，一个用于存放矩阵b各行的非零元素个数
			 另一个用于存放矩阵b各行非零元素在三元组表中的起始位置
	*/
	SparseMatrix<T> result(Rows,b.Cols,0);
	if(Cols!=b.Rows)
	{
		cout<<"矩阵行列不匹配！"<<endl;
		return result;
	}

	int *rowSize = new int[b.Rows];		// b各行非零元素个数
	int *rowStart = new int[b.Rows+1];	// b各行三元组开始位置
	T *temp = new T[b.Cols];			// 暂存每一行的计算结果
	int i;
	int Current;
	int LastInResult;
	int RowA;
	int ColA;
	int ColB;

	for(i=0;i<b.Rows;i++)rowSize[i]=0;
	for(i=0;i<b.Terms;i++)rowSize[b.smArray[i].row]++;
	rowStart[0]=0;
	for(i=1;i<b.Rows+1;i++)rowStart[i]=rowStart[i-1]+rowSize[i-1];
	Current=0; LastInResult = -1;		// a扫描指针 result存指针
	while(Current < Terms)
	{
		RowA=smArray[Current].row;
		for(i=0;i<b.Cols;i++)temp[i]=0;
		while(Current<Terms&& smArray[Current].row==RowA)
		{
			ColA=smArray[Current].col;
			for(i=rowStart[ColA];i<rowStart[ColA+1];i++)
			{
				ColB=b.smArray[i].col;
				temp[ColB] += smArray[Current].value * b.smArray[i].value;
			}
			Current++;
		}
		for(i=0;i<b.Cols;i++)
		if(temp[i]!=0)
		{
			LastInResult++;
			result.smArray[LastInResult].row = RowA;
			result.smArray[LastInResult].col = i;
			result.smArray[LastInResult].value = temp[i];
		}
	}
	result.Terms = LastInResult+1;
	delete[] rowStart;
	delete[] rowSize;
	delete[] temp;
	return result;


}