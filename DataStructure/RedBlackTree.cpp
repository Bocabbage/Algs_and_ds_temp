/*

    红黑树(Red-Black Tree)
    实现框架：类实现，通过typedef定义ValType
    更新时间：2019\1\25(完成Insert代码，Remove尚未完成)(未验证)

*/

class RBTree_Node;
class RBTree;
typedef int ValType;
typedef RBTree_Node* RBTP;
enum COLOR{red,black};

/***************红黑树节点类******************/
class RBTree_Node
{
	friend class RBTree;
private:
    ValType val;           // 键值
	RBTP parent;           // 父节点
    RBTP leftchild;        // 左孩子
    RBTP rightchild;       // 右孩子
    COLOR color;           // 颜色
public:
    RBTree_Node():
    val(0),parent(nullptr),leftchild(nullptr),
    rightchild(nullptr),color(red){}
    RBTree_Node(ValType V,RBTP P,RBTP LC,RBTP RC,COLOR C);
    ~RBTree_Node(){}

};

RBTree_Node::RBTree_Node(ValType V,RBTP P,RBTP LC,RBTP RC,COLOR C)
{
    val = V;
    parent = P;
    leftchild = LC;
    rightchild = RC;
    color = C;
}
/*****************红黑树类********************/
class RBTree
{
private:
    RBTP root;      // 根节点
    int _size;
    bool release();                 // 释放RB-Tree
    void Transplant(RBTP u,RBTP v); // 交换两节点
    RBTP Min(RBTP z);               // 返回以z为根节点的树最小节点的指针√
    void InsertFixUp(RBTP z);       // 恢复插入操作后被破坏的RB-Tree性质√
    void RemoveFixUp(RBTP z);       // 恢复删除操作后被破坏的RB-Tree性质
    void RotateLeft(RBTP z);        // 左旋操作√
    void RotateRight(RBTP z);       // 右旋操作√
public:
    RBTree(){root = new RBTree_Node;_size=1;root->color=black;}
    RBTree(ValType v);
    ~RBTree(){release();}

    int size(){return _size;}       // 返回节点数√
    bool Insert(RBTP z);            // RB-Tree插入√
    bool Insert(ValType v);         // RB-Tree插入(以键值为参数)√
    void Remove(ValType v);         // RB-Tree节点删除
    RBTP search(ValType v);         // RB-Tree搜索√
};

RBTree::RBTree(ValType v)
{
    root = new RBTree_Node(v,nullptr,nullptr,nullptr,black);
    _size = 1;
}

bool RBTree::Insert(ValType v)
{
    RBTP z = new RBTree_Node(v,nullptr,nullptr,nullptr,red);
    Insert(z);
    // _size++操作在重载参数为RBTP类型中完成
}

bool RBTree::Insert(RBTP z)
{
    if(z==nullptr || search(z->val)!=nullptr)
        return false;
    RBTP x = root;
    RBTP y = nullptr;
    while(x!=nullptr)
    {
        y = x;
        if(x->val > z->val)
            x = x->leftchild;
        else
            x = x->rightchild;
    }
    if(y->val > z->val)
    {   
        y->leftchild = z;
        z->parent = y;
    }
    else
    {
        y->rightchild = z;
        z->parent = y;
    }
    InsertFixUp(z);
    _size++;
    return true;
}

void RBTree::RotateLeft(RBTP z)
{
    RBTP y = z->rightchild;
    RBTP p = z->parent;
    z->rightchild = y->leftchild;
    z->parent = y;
    y->leftchild = z;
    y->parent = p;
    if(p==nullptr) root = y;
    else if(p->leftchild==z)
        p->leftchild = y;
    else p->rightchild = y;

}

void RBTree::RotateRight(RBTree z)
{
    RBTP y = z->leftchild;
    RBTP p = z->parent;
    z->leftchild = y->rightchild;
    z->parent = y;
    y->rightchild = z;
    y->parent = p;
    if(p==nullptr) root = y;
    else if(p->leftchild==z)
        p->leftchild = y; 
    else p->rightchild = y;
}

void RBTree::InsertFixUp(RBTree z)
{   
    if(z==root)
    {
        z->color = black;
        return;
    }
    while(z->parent->color = red)
    {
        if(z->parent = z->parent->parent->left)
        {    
            y = z->parent->parent->rightchild;
            if(y->color == red)
            {
                /*Case1*/
                // 父节点是祖父节点的左孩子
                // 叔父节点为红节点
                z->parent->color = black;
                y->color = black;
                z->parent->parent->color = red;
                z = z->parent->parent;
            }
            else if(z==z->parent->rightchild)
            {
                /*Case2*/
                // 父节点是祖父节点的左孩子
                // 当前节点是父节点的右孩子
                // 且叔父节点为黑节点或空节点
                z = z->parent;
                RotateLeft(z);

                /*Case3*/
                // 承接Case2的处理结果
                // 父节点是祖父节点左孩子
                // 当前节点是父节点的左孩子
                // 且叔父节点为黑节点或空节点
                z->parent->color = black;
                z->parent->parent->color = red;
                RotateRight(z);
            }
        }
        else    // 对称情况
        {
            y = z->parent->parent->leftchild;
            if(y->color == red)
            {
                /*Case4*/
                // 父节点是祖父节点的右孩子
                // 叔父节点为红节点
                z->parent->color = black;
                y->color = black;
                z->parent->parent->color = red;
                z = z->parent->parent;
            }
            else if(z==z->parent->leftchild)
            {
                /*Case5*/
                // 父节点是祖父节点的右孩子
                // 当前节点是父节点的左孩子
                // 且叔父节点为黑节点或空节点
                z = z->parent;
                RotateRight(z);

                /*Case6*/
                // 承接Case2的处理结果
                // 父节点是祖父节点右孩子
                // 当前节点是父节点的右孩子
                // 且叔父节点为黑节点或空节点
                z->parent->color = black;
                z->parent->parent->color = red;
                RotateLeft(z);
            }
        }
    }

    root->color = black;
}

void RBTree::Transplant(RBTP u,RBTP v)
{
    
}

RBTP RBTree::Min(RBTP z)
{
    RBTP p = z;
    if(z==nullptr)return nullptr;
    while(p->leftchild != nullptr)
        p = p->leftchild;
    return p;
}

void RBTree::Remove(ValType v)
{
    /*
    RBTP z = search(v);
    RBTP x,y;
    if(z==nullptr)return;
    y = z;
    color y_origin_color = y->color;
    if(z->left == nullptr)
    {    
        x = z->rightchild;
        Transplant(z,z->rightchild);
    }
    else if(z->right == nullptr)
    {
        x = z->leftchild;
        Transplant(z,z->leftchild);
    }
    else
    {
        y = Min(z->rightchild);
        y_origin_color = y->color;
        x = y->rightchild;
        if(y->parent == z)
            x->parent = y;
        else 
        {
            Transplant(y,y->rightchild);
            y->rightchild = z.rightchild;
            y->rightchild->parent = y;
        }
        Transplant(z,y);
        y->leftchild = z->leftchild;
        y->leftchild->parent = y;
        y->color = z->color;
    }
    if(y_origin_color == black)
        RemoveFixUp(z);
    */
}

RBTP RBTree::search(ValType v)
{
    RBTP p = root;
    while(p!=nullptr)
    {
        if(p->val == v)
            return p;
        else if(p->val < v)
            p = p->rightchild;
        else
            p = p->leftchild;
    }

    return nullptr;
}