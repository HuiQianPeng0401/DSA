#include <iostream>
using namespace std;
#include <stack>

#define BinNodePosi(T) BinNode<T>*
#define stature(p) ((p) ? (p)->height : -1)//节点高度
typedef enum
{
    RB_RED,
    RB_BLACK
} RBcolor;//节点颜色
#define IsRoot(x) (!((x).parent))
#define IsLChild(x) (!IsRoot(x)&&(&(x)==(x).parent->lChild))
#define IsRChild(x) (!IsRoot(x)&&(&(x)==(x).parent->rChild))
#define HasParent(x) (!IsRoot(x))
#define HasLChild(x) ((x).lChild)
#define HasRChild(x) ((x).rChild)
#define HasChild(x) (HasLChild(x) || HasRChild(x))
#define HasBothChild(x) (HasLChild(x) && HasRChild(x))
#define IsLeaf(x) (!HasChild(x))
#define sibling(p) ( \
    IsLChild(*(p)) ? \
        (p)->parent->rChild : \
        (p)->parent->lChild \
)
#define uncle(p) ( \
    IsLChild(*((x)->parent)) ? \
        (x)->parent->parent->rChild : \
        (x)->parent->parent->lChild \
)
#define FromParentTo(x) ( \
    IsRoot(x) ? _root : ( \
    IsLChild(x) ? (x).parent->lChild : (x).parent->rChild \
    ) \
)

#define Balanced(X) (stature((x).lChild) == stature((x).rChild))
#define BalFac(X) (stature(x), lChild - stature(x).rChild)
#define AvlBalanced(x) ((-2 < BalFac(x)) && (BalFac(x) < 2))

template <typename T>
struct BinNode
{
    T data;
    BinNodePosi(T) parent, lChild, rChild;
    int height;
    int npl;
    RBcolor color;
//构造函数
    BinNode() : parent(nullptr), lChild(nullptr), rChild(nullptr), height(0),
                npl(1), color(RB_RED){}
    BinNode(T e, BinNodePosi(T) p = nullptr, BinNodePosi(T) lc = nullptr, BinNodePosi(T) rc = nullptr, int h = 0, int l = 1, RBcolor c = RB_RED)
        : data(e), parent(p), lChild(lc), rChild(rc), height(h), npl(l), color(c){}
//操作接口
    int size();//统计当前节点后代总数
    BinNodePosi(T) insertAsLC(T const &e);
    BinNodePosi(T) insertAsRC(T const &e);
    BinNodePosi(T) succ();//取中序遍历节点的直接后继
    template <typename VST>
    void travLevel(VST &visit);
    template <typename VST>
    void travPre(VST &visit);
    template <typename VST>
    void travIn(VST &visit);
    template <typename VST>
    void travPost(VST &visit);
    bool operator<(BinNode const &bn)
    {
        return data < bn.data;
    }
    bool operator==(BinNode const &bn)
    {
        return data == bn.data;
    }
};
template <typename T>
BinNodePosi(T) BinNode<T>::insertAsLC(T const &e)
{
    return lChild = new BinNode(e, this);
}
template <typename T>
BinNodePosi(T) BinNode<T>::insertAsRC(T const &e)
{
    return rChild = new BinNode(e, this);
}
template <typename T>
class BinTree
{
    public:
        int _size;           //规模
        BinNodePosi(T) _root;//树根
        virtual int updateHeight(BinNodePosi(T) x);//更新节点x的高度
        void updateHeightAbove(BinNodePosi(T) x);//更新节点x及其祖先的高度
        BinTree() : _size(0), _root(nullptr){}
        ~BinTree()
        {
            if(0<_size)
                remove(_root);
        }
        int size() const
        {
            return _size;
        }
        bool empty() const
        {
            return !_root;
        }
        BinNodePosi(T) root() const
        {
            return _root;
        }
        BinNodePosi(T) insertAsRoot(T const &e);
        BinNodePosi(T) insertAsLC(BinNodePosi(T) x, T const &e);
        BinNodePosi(T) insertAsRC(BinNodePosi(T) x, T const &e);
        BinNodePosi(T) attachAsLC(BinNodePosi(T) x, BinTree<T> *&T);
        BinNodePosi(T) attachAsRC(BinNodePosi(T) x, BinTree<T> *&T);
        int remove(BinNodePosi(T) x);
        BinTree<T> *secede(BinNodePosi(T) x);
        template <typename VST>
        void travLevel(VST &visit)
        {
            if(_root)
                _root->travLevel(visit);
        }
        template <typename VST>
        void travPre(VST &visit)
        {
            if(_root)
                _root->travPre(visit);
        }
        template <typename VST>
        void travIn(VST &visit)
        {
            if(_root)
                _root->travIn(visit);
        }
        template <typename VST>
        void travPost(VST &visit)
        {
            if(_root)
                _root->travPost(visit);
        }
        bool operator<(BinTree<T> const &t)
        {
            return _root && t._root && lt(_root, t._root);
        }
        bool operator==(BinTree<T> const &t)
        {
            return _root && t._root && (_root == t._root);
        }
};
template <typename T>
int BinTree<T>::updateHeight(BinNodePosi(T) x)
{
    return x->height = 1 + max(stature(x->lChild), stature(x->rChild));
}
template <typename T>
void BinTree<T>::updateHeightAbove(BinNodePosi(T) x)
{
    while(x){
        updateHeight(x);
        x = x->parent;
    }
}
template <typename T>
BinNodePosi(T) BinTree<T>::insertAsRoot(T const &e)
{
    _size = 1;
    return _root = new BinNode<T>(e);
}
template <typename T>
BinNodePosi(T) BinTree<T>::insertAsLC(BinNodePosi(T) x, T const &e)
{
    _size++;
    x->insertAsLC(e);
    updateHeightAbove(x);
    return x->lChild;
}
template <typename T>
BinNodePosi(T) BinTree<T>::insertAsRC(BinNodePosi(T) x, T const &e)
{
    _size++;
    x->insertAsRC(e);
    updateHeightAbove(x);
    return x->rChild;
}
template <typename T>
BinNodePosi(T) BinTree<T>::attachAsLC(BinNodePosi(T) x, BinTree<T> *&S)
{
    //将S当做节点x的左子树接入 S本身置空
    if(x->lChild=S->_root)
        x->lChild->parent = x;
    _size += S->_size;
    updateHeightAbove(x);
    S->_root = nullptr;
    S->_size = 0;
    s = nullptr;
    return x;
}
template <typename T>
BinNodePosi(T) BinTree<T>::attachAsRC(BinNodePosi(T) x, BinTree<T> *&S)
{
    if(x->rChild=S->root)
        x->rChild->parent = x;
    _size += S->_size;
    updateHeightAbove(x);
    S->_root = nullptr;
    S->_size = 0;
    S = nullptr;
    return x;
}
template <typename T>
int BinTree<T>::remove(BinNodePosi(T) x)
{
    //删除二叉树位置x处的节点及其后代
    FromParentTo(*x) = nullptr;
    updateHeightAbove(x->parent);
    int n = removeAt(x);
    _size -= n;
    return n;
}
template <typename T>
static int removeAt(BinNodePosi(T) x)
{
    if(!x)
        return 0;
    int n = 1 + removeAt(x->lChild) + removeAt(x->rChild);
    return n;
}
template <typename T>
BinTree<T> *BinTree<T>::secede(BinNodePosi(T) x)
{
    //将子树x从当前树中删除 并将其封装为一颗独立子树返回
    FromParentTo(*x) = nullptr;
    updateHeightAbove(x->parent);
    BinTree<T> *S = new BinTree<T>;
    S->_root = x;
    x->parent = nullptr;
    S->_size = x->size();
    _size -= S->size();
    return S;
}
template <typename T, typename VST>
static void travPre(BinNodePosi(T) x, VST &visit)
{
    stack<BinNodePosi(T)> s;
    while(true){
        while(x){
            visit(x->data);
            s.push(x->rChild);
            x = x->lChild;
        }
        if(s.empty())
            break;
        x = s.top();
        s.pop();
    }
}
template <typename T, typename VST>
static void travIn(BinNodePosi(T) x, VST &visit)
{
    stack<BinNodePosi(T)> s;
    while(true){
        while(true){
            s.push(x);
            x = x->lChild;
        }
        if(s.empty())
            break;
        x = s.top();
        s.pop();
        visit(x->data);
        x = x->rChild;
    }
}
template <typename T, typename VST>
static void travPost(BinNodePosi(T) x, VST &visit)
{
    stack<BinNodePosi(T)> s;
    BinNodePosi(T) cur = x, pre = nullptr;
    while(cur){
        s.push(cur);
        cur = cur->lChild;
    }
    while(!s.empty()){
        cur = s.top();
        s.pop();
        if(cur->rChild==nullptr||cur->rChild==pre){
            visit(cur->data);
            pre = cur;
        }
        else{
            s.push(cur);
            cur = cur->rChild;
            while(cur){
                s.push(cur);
                cur = cur->lChild;
            }
        }
    }
}
template <typename T>
BinNodePosi(T) BinNode<T>::succ()
{
    BinNodePosi(T) s = this;//记录后继的临时变量
    if(rChild){//若有右孩子 则直接后继必在右子树中 
        s = rChild;
        while(HasLChild(*s))
            s = s->lChild;
    }
    else{//否则直接后继应是“将当前节点包含于其左子树中的最低祖先”
        while(IsRChild(*s))
            s = s->parent;
        s = s->parent;
    }
    return s;
}
