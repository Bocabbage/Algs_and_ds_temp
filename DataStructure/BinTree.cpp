/*
	BinTree模板类(三叉链表实现)
	更新时间：2018\11\3(完成迭代遍历代码，未验证)
*/
#include<stack>
#include<queue>
using std::stack;
using std::queue;

template<typename T>
struct BinNode
{
	BinNode<T>* parent;
	BinNode<T>* lChild;
	BinNode<T>* rChild;
	int height;
	T data;

	BinNode():parent(nullptr),lChild(nullptr),rChild(nullptr),height(0){}
	BinNode(T e,BinNode<T>* p=nullptr,BinNode<T>* lc=nullptr,BinNode<T>* rc=nullptr,int h=0):
			parent(p),lChild(lc),rChild(rc),data(e),height(h){}
	/* 应用接口 */
	//int size();						// 统计当前节点后代总数
	BinNode<T>* insertAsLC(T const&);	// 作为当前节点的左孩子插入
	BinNode<T>* insertAsRC(T const&);	// 作为当前节点的右孩子插入
	//BinNode<T>* succ();				// 取当前节点的直接后继

	/* 比较器、判等器 */
	// 根据需要补充
	bool operator<(BinNode<T>* bn){return data<bn.data;}
	bool operator==(BinNode<T>* bn){return data==bn.data;}
};

/* BinNode接口实现 */
template<typename T>
BinNode<T>* BinNode<T>::insertAsLC(T const& e)
{
	return lChild=new BinNode(e,this);
}

template<typename T>
BinNode<T>* BinNode<T>::insertAsRC(T const& e)
{
	return rChild=new BinNode(e,this);
}

/******************/

template<typename T>
class BinTree
{
private:
	int _size;				// 规模
	BinNode<T>* _root;		// 树的根节点
public:
	BinTree():_size(0),_root(nullptr){}
	BinTree(T const& e){_size=1;root = new BinNode(e);}
	~BinTree(){if(_size>0)remove(_root);}

	BinNode<T>* root() const{return _root;}					// 返回树的根节点
	int size() const{return _size;}							// 返回树的规模
	bool empty() const{return !_root;}						// 判空
	BinNode<T>* insertAsLC(BinNode<T>* x, T const& e);		// 元素作为左孩子接入
	BinNode<T>* insertAsRC(BinNode<T>* x, T const& e);		// 元素作为右孩子接入
	BinNode<T>* attachAsLC(BinNode<T>* x, T BinTree<T>* t);	// 作为左子树接入
	BinNode<T>* attachAsRC(BinNode<T>* x, T BinTree<T>* t);	// 作为右子树接入
	int remove(BinNode<T>* x);			// 删除以x处节点为根的子树，返回该子树规模

	// 遍历(函数指针方式迭代实现)
	void travLevel(void (*visit)(BinNode<T>* p));			// 层序遍历
	void travPre(void (*visit)(BinNode<T>* p));				// 前序遍历
	void travIn(void (*visit)(BinNode<T>* p));				// 中序遍历
	void travPost(void (*visit)(BinNode<T>* p));			// 后序遍历

	void gotoHLVFL(stack<BinNode<T>*>& S)					// 后序遍历辅助函数
	{
		// HLVFL(Highest leaf visible from left):最高左侧可见叶结点
		// 该节点既可能是左孩子，也可能是右孩子
		BinNode<T>* temp;
		while(temp=S.top())					// 停止条件为栈顶指针为空
			if(temp->lChild!=nullptr)
			{	
				if(temp->rChild!=nullptr)
					S.push(temp->rChild);
				S.push(temp->lChild);
			}
			else
				S.push(temp->rChild);		// 有可能压进空指针，表示结束
		S.pop();							// 弹出栈顶空指针
	}
};

/* BinTree接口实现 */
template<typename T>
void BinTree<T>::travPre(void (*visit)(BinNode<T>* p))
{
	stack<BinNode<T>*> S;		// 辅助栈
	BinNode<T>* temp=_root;
	while(true)
	{	
		for(temp!=nullptr)
		{
			visit(temp);			// 访问根节点
			S.push(temp->rChild);	// 右孩子入栈
			temp=temp->lChild;		// 深入访问左孩子
		}
		if(S.empty())break;
		temp=S.top();
		S.pop();
	}
}

template<typename T>
void BinTree<T>::travIn(void (*visit)(BinNode<T>* p))
{
	stack<BinNode<T>*> S;		// 辅助栈
	BinNode<T>* temp=_root;

	while(true)
	{	
		for(temp!=nullptr)			
		{
			S.push(temp);
			temp=temp->lChild;
		}
		if(S.empty())break;
		temp=S.top();			
		S.pop();
		visit(temp);
		temp=temp->rChild;	// *若右子树为空，此处被赋值nullptr，即不执行for循环

	}

}

template<typename T>
void BinTree<T>::travPost(void (*visit)(BinNode<T>* p))
{
	stack<BinNode<T>*> S;		// 辅助栈
	BinNode<T>* temp=_root;
	S.push(temp);
	while(!S.empty())
	{
		if(S.top()!=temp->parent)	// 若非父节点，必为右兄弟
			gotoHLVFL(S);			// 在以其右兄弟为根的子树中，找到HLVFL
		temp=S.top();				
		S.pop();					
		visit(temp);				// 弹出栈顶并访问之
	}
}

template<typename T>
void BinTree<T>::travLevel(void (*visit)(BinNode<T>* p))
{
	queue<BinNode<T>*> Q;		// 辅助队列
	BinNode<T>* temp=_root;
	Q.push(temp);				// 根节点入队
	while(Q.empty())
	{
		temp=Q.front();
		Q.pop();
		visit(temp);
		if(temp->lChild!=nullptr)Q.push(temp->lChild);
		if(temp->rChild!=nullptr)Q.push(temp->rChild);
	}
}