
/*
	Vector模板类

	2018\9\22 维护至无序向量接口完成(已验证)
	2018\9\23 维护有序向量接口
	
*/


typedef int Rank;
const int DEFAULT_CAPACITY = 50;

template<typename T>
class Vector
{
private:

	// 数据域
	Rank _size;
	int _capacity;
	T * _data;

	// 内部接口
	void expand();
	void shrink();

public:

	// 构造与析构函数
	Vector(int c=DEFAULT_CAPACITY,int s=0,T *d=nullptr)
	{
		size=s,_capacity=c;
		_data = new T[_capacity];
		for(int i=0;i<_size;i++)_data[i]=d[i];
	}
	Vector(Vector<T> const& V)
	{
		_size=V._size,_capacity=V._capacity;
		_data=new T[_capacity];
		for(int i=0;i<_size;i++)_data[i]=V._data[i];
	}
	~Vector(){delete[] _data;}

	// 只读访问入口
	Rank size() const{return _size;}
	bool isempty() const{return _size==0;}
	Rank find(T const& e) const{return find(e,0,_size);}	// 统一到区间查找接口
	Rank find(T const& e,Rank lo,Rank hi) const;		// T const& e 确保该引用不会指向其他地址
	bool disordered(bool ud) const;  // 检查是否有序
	//void print()const{for(int i=0;i<_size;i++)cout<<_data[i]<<endl;}

	// 可写访问接口
	T& operator[](Rank r)const;
	Vector<T>& operator=(Vector<T> const&);
	T remove(Rank r);
	int remove(Rank lo, Rank hi);	//删除区间[lo,hi)
	Rank insert(Rank r,T const& e);
	Rank insert(T const& e){return insert(_size,e);}
	//void sort(Rank lo,Rank hi);
	//void sort(){sort(0,_size);}
	int deduplicate(); // 无序去重
	int uniquify();	   // 有序去重

};


/*内部接口实现*/

template<typename T>
void Vector<T>::expand()
{
	//采用策略：被动装填
	//仅insert方法可能导致扩容
	//分摊平均时间：O(1)

	if(_size >= _capacity)
	{
		T *old_data=_data;
		_data = new T[_capacity<<=1];
		for(int i=0;i<_size;i++)
			_data[i]=old_data[i];
		delete[] old_data;
	}
}

template<typename T>
void Vector<T>::shrink()
{
	//在考虑空间利用率的场合用于节省空间
	//采用策略：当装填率不足25%时缩容
	//仅remove方法可能导致缩容

	if(_capacity > DEFAULT_CAPACITY && _capacity > (_size>>2))
	{
		T *old_data=_data;
		_data = new T[_capacity>>=1];
		for(int i=0;i<_size;i++)
			_data[i]=old_data[i];
		delete[] old_data;
	}

}

/*外部只读接口实现*/

template<typename T>
Rank Vector<T>::find(T const& e, Rank lo, Rank hi) const
{
	//无序向量查找：策略为顺序查找,返回rank最小者(从前向后查找)
	//输入敏感型算法
	//最坏情况：O(lo-hi)

	while (lo < hi)
	{
		if (_data[lo] == e)break;
		lo++;
	}
	if(lo==hi)
		return -1;
	else return lo;
}

template<typename T>
bool Vector<T>::disordered(bool ud=0) const
{
	//ud：	0	|	1	
	//	  顺序	| 逆序 	

	//return:	0	|	1	
	//			有序|	无序
	int i;	// 用于计数
	if(ud)
	{
		for(i=0;i<_size-1 && _data[i]>_data[i+1];i++);
		return (i==(_size-1))?0:1;
	}
	else
	{

		for(i=0;i<_size-1 && _data[i]<_data[i+1];i++);
		return (i==(_size-1))?0:1;
	}
}

/*外部可写接口实现*/

template<typename T>
T& Vector<T>::operator[](Rank r) const
{
	if(r<0 || r>=_size)exit(1);	// 异常退出
	return _data[r];
}

template<typename T>
Vector<T>& Vector<T>::operator=(Vector<T> const& V)
{
	T *old_data = _data;
	_size=V._size,_capacity=V._capacity;
	_data=new T[_capacity];
	for(int i=0;i<_size;i++)_data[i]=V._data[i];
	delete[] old_data;
}

template<typename T>
int Vector<T>::remove(Rank lo,Rank hi)
{
	//删除时间由_size-hi+1决定
	//时间复杂度：O(_size-hi+1)


	if(lo==hi)return 0;
	if(hi>_size)exit(1);
	while(hi<_size)
		_data[lo++]=_data[hi++];

	_size = lo;		// !停止时lo刚好抵达更新后序列末端
	shrink();		// 必要时缩容
	return hi-lo;	// 返回被删除元素个数
}

template<typename T>
T Vector<T>::remove(Rank r)
{
	if(r<0 || r>_size)exit(1);
	T r_data=_data[r];
	remove(r,r+1);
	return r_data;
}

template<typename T>
Rank Vector<T>::insert(Rank r,T const& e)
{
	expand();
	for(Rank j=_size-1;j>r;j--)
		_data[j]=_data[j-1];
	_data[r]=e;
	_size++;
	return r;
}

template<typename T>
int Vector<T>::deduplicate()
{

	// 使用find/remove接口遍历向量
	// 时间复杂度：O(n^2)
	// 算法正确性保证：在while循环中，在当前元素的前缀_data[0,i)内
	//				   所有元素彼此互异

	int old_size=_size;
	Rank i=1;
	while(i<_size)
		(find(_data[i],0,i) < 0)?i++:remove(i);
	return old_size - _size;	//返回剔除的元素个数

}

template<typename T>
int Vector<T>::uniquify()
{
	//deduplicate复杂度过高的根源在于
	//对remove接口的各次调用中
	//同一元素可能作为后继元素向前移动多次
	//且每次仅移动一个单元

	//故优化思路为有序化后批量删除（自己设计）
	/*
	if(!disordered())
	{
		int i=1;
		int j;
		int old_size=_size;
		while(i<_size)
		{
			if(find(_data[i],0,i))
				for(j=i+1;_data[j]==data[i];j++);
			remove(i,j);
			i++;
		}
		return old_size - _size;
	}
	else
		exit(1);
	*/

	//出自《数据结构-邓俊辉》第二章
	//优化思路：将不重复元素全部前移,再一次性剔除末端
	//[i,j]确定一个元素重复的序列
	//j最后停留在原_size-1处，i停留在被舍弃段第一个元素处
	//故返回被删除元素个数

	//时间渐进复杂度：O(n) -- 最佳优化(去重必须至少遍历1次向量)
	Rank i=0,j=0;
	while(++j < _size)
		if(_data[i] != _data[j])
			_data[++i]=_data[j];
	_size=++i;
	shrink();
	return j - i;
}

