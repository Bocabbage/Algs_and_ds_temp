/*
	List(双向)链表模板类
	
	数据结构形式：
	|header|→|data[0]|→|data[1]|→....→|data[n-1]|→|trailer|

	2018\9\29 维护无序双向链表接口(已验证)
	2019\1\13 调整注释缩进格式
	

*/

/******************Node模板结构*********************/
template<typename T>
struct Node
{

	T data;
	Node<T> *pred;
	Node<T> *succ;

	//采用不完整的封装方法(值得商榷)，内存释放依靠List调度
	Node<T>* insertAsPred(T const& e);
	Node<T>* insertAsSucc(T const& e);
};

template<typename T>
Node<T>* Node<T>::insertAsPred(T const& e)
{
	// 插入在常数时间内完成
	Node<T>* E = new Node<T>;
	E->data = e;
	E->pred = this->pred;
	E->succ = this;
	this->pred->succ = E;
	this->pred = E;
	return E;

}

template<typename T>
Node<T>* Node<T>::insertAsSucc(T const& e)
{
	// 插入在常数时间内完成
	Node<T>* E = new Node<T>;
	E->data = e;
	E->succ = this->succ;
	E->pred = this;
	this->succ->pred = E;
	this->succ = E;
	return E;

}

/*********************List模板类**********************/
template<typename T>
class List
{
private:
	int _size;
	Node<T> *header;
	Node<T> *trailer;

	void init();					// 列表创建时的初始化
	int clear();					// 清理所有节点

public:

	// 构造函数
	List(){init();}
	List(List<T> const& L);                     // 复制构造
	List(List<T> const& L, int r,int n);        // 复制构造列表L自第r项起的n项
	List(List<T> const& L, Node<T> *p, int n);  // 复制列表中自位置p起的n项
	~List(){clear();}

	// 只读访问接口
	int size() const {return _size;}
	bool empty() const {return _size<=0;}
	T& operator[](int r) const;
	Node<T>* first() const {return header->succ;}
	Node<T>* last() const {return trailer->pred;}
	bool disordered(bool ud=0) const;                               // 判断链表是否有序
	Node<T>* find(T const& e) const                                 // 全区间无序查找
	 {return find(e,_size,header->succ);}
	Node<T>* find(T const& e, int n, Node<T>* p) const;             // 无序区间查找([p,p+n))
	Node<T>* selectMax(Node<T>* p,int n);                           // 在p及其前n-1个后继中选出最大者
	Node<T>* selectMax()
	{return selectMax(header->succ,_size);}

	//可写访问接口
	Node<T>* insertAsFirst(T const& e);              // 将e作为首节点插入
	Node<T>* insertAsLast(T const& e);               // 将e作为末节点插入
	Node<T>* insertBefore(Node<T>* p, T const& e);
	Node<T>* insertAfter(Node<T>* p, T const& e);
	T remove(Node<T>* p);                            // 删除合法位置p处的节点，返回被删除节点
	int deduplicate();                               // 无序去重
	//void reverse();


};


/*内部接口实现*/

template<typename T>
void List<T>::init()
{
	header=new Node<T>;
	trailer=new Node<T>;

	header->pred = nullptr;
	trailer->succ = nullptr;
	header->succ = trailer;
	trailer->pred = header;

	_size = 0;
}

template<typename T>
int List<T>::clear()
{
	// 时间复杂度：O(n)
	int old_size = _size;
	Node<T>* temp = header->succ;
	while(_size>0)
	{
		temp=temp->succ;
		remove(temp->pred);
	}
	return old_size;
}


/*构造函数实现*/

template<typename T>
List<T>::List(List<T> const& L)
{
	init();
	Node<T>* temp=L.header->succ;
	while(temp!=trailer)
	{
		header->insertAsSucc(temp->data);
		temp=temp->succ;
		_size++;
	}

}

template<typename T>
List<T>::List(List<T> const& L, int r,int n)
{	
	int i;                    // 计数用
	if(r>=L._size)exit(1);
	Node<T>* temp=L.header->succ;
	for(i=0;i<r;++i)
		temp=temp->succ;      // 迭代结束得到L[r]

	init();
	for(i=0;i<n;++i)
	{
		if(temp==L.trailer)exit(1);
		header->insertAsSucc(temp->data);
		temp=temp->succ;
		_size++;
	}

}


/*外部只读接口实现*/

template<typename T>
T& List<T>::operator[](int r) const
{
	// 时间复杂度：O(r)
	Node<T>* temp=first();
	for(int i=0;i<r;i++)
		temp=temp->succ;
	return temp->data;

}

template<typename T>
bool List<T>::disordered(bool ud=0) const
{
	//ud：	0	|	1	
	//	  顺序	| 逆序 	

	//return:	 0   |  1	
	//          有序 | 无序

	Node<T>* temp=first();
	if(!ud)
	{
		while((temp!=trailer->pred) && (temp->data <= temp->succ->data))
			temp=temp->succ;
		return !(temp==trailer);
	}
	else
	{
		while((temp!=trailer->pred) && (temp->data >= temp->succ->data))
			temp=temp->succ;
		return !(temp->succ==trailer);
	}
}

template<typename T>
Node<T>* List<T>::find(T const& e, int n, Node<T>* p) const
{
	// 时间复杂度：O(n)
	int i;
	for(i=0;i<n;i++)
	{
		if(p==nullptr)exit(1);// 查找已过trailer，p+n-1越界
		else if(p->data == e)break;

		p=p->succ;
	}

	if((i==n || p==trailer) && p->pred->data!=e)return nullptr;// 注意！p->pred->data！(开区间搜索)
	else return p;
}

template<typename T>
Node<T>* List<T>::selectMax(Node<T>* p,int n)
{
	if(p==header || p==trailer)exit(1);
	int _max=p->data;
	Node<T>* temp=p;
	p=p->succ;
	for(int i=1;i<n && p!=trailer;i++)
	{
		if(p->data > _max){_max=p->data;temp=p;}
		p=p->succ;
	}
	
	return temp;	

}


/*外部可写接口实现*/
template<typename T>
Node<T>* List<T>::insertAsFirst(T const& e)
{header->insertAsSucc(e);_size++;return first();}

template<typename T>
Node<T>* List<T>::insertAsLast(T const& e)
{trailer->insertAsPred(e);_size++;return last();}

template<typename T>
Node<T>* List<T>::insertBefore(Node<T>* p, T const& e)
{p->insertAsPred(e);_size++;return p->pred;}

template<typename T>
Node<T>* List<T>::insertAfter(Node<T>* p, T const& e)
{p->insertAsSucc(e);_size++;return p->succ;}

template<typename T>
T List<T>::remove(Node<T>* p)
{
	(p->pred)->succ = p->succ;
	(p->succ)->pred = p->pred;

	_size--;
	T temp = p->data;
	delete p;
	return temp;
}

template<typename T>
int List<T>::deduplicate()
{
	// 时间复杂度：O(n^2)
	// 也即List的删除快捷并没有为唯一化带来渐进增长红利

	if(_size<2)return 0;
	int old_size = _size;
	Node<T>* temp=first();
	Node<T>* q;
	int r=1;
	while(trailer != (temp=temp->succ))		
	{
		q = find(temp->data,r,first());		// 除去List[r]的前驱中与之重复的元素
		q? remove(q):r++;
	}

	return old_size - _size;

}