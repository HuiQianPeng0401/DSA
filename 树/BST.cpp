template <typename T>
class BST: public BinTree<T>{
    public:
        BinNodePosi(T) _hot;//命中节点的父亲
        BinNodePosi(T) connect34( //按照“3+4”结构连接三个节点及四颗子树
            BinNodePosi(T), BinNodePosi(T), BinNodePosi(T),
            BinNodePosi(T), BinNodePosi(T), BinNodePosi(T), BinNodePosi(T));
        BinNodePosi(T) rotateAt(BinNodePosi(T) x);//对x及其祖先统一旋转
        //强制要求所有派生类(BST变种)根据各自规则重写
        virtual BinNodePosi(T) & search(const T &e);//查找
        virtual BinNodePosi(T) insert(const T &e);//插入
        virtual bool remove(const T &e);//删除
};

template <typename T>
static BinNodePosi(T) & searchIn(BinNodePosi(T) & v, const T &e, BinNodePosi(T) & hot)
{
    //在以v为根的BST子树查找关键码e
    if(!v||(e==v->data))
        return v;//递归基
    hot = v;//hot为命中节点的父亲
    return searchIn(((e < v->data) ? v->lChild : v->rChild), e, hot);
}
template <typename T>
BinNodePosi(T) & BST<T>::search(const T &e)
{
    return searchIn(_root, e, _hot = NULL);
}
template <typename T>
BinNodePosi(T) BST<T>::insert(const T &e)
{
    BinNodePosi(T) &x = search(e);
    if(x)
        return x;
    x = new BinNode<T>(e, _hot);//e为关键码 _hot为其父
    updateHeightAbove(x);
    return x;
}
template <typename T>
bool BST<T>::remove(const T &e)
{
    BinNodePosi(T) &x = search(e);
    if(!x)
        return false;
    removeAt(x, _hot);
    _size--;
    updateHeightAbove(_hot);
    return true;
}
template <typename T>
static BinNodePosi(T) removeAt(BinNodePosi(T) & x, BinNodePosi(T) & hot)
{
    BinNodePosi(T) w = x;//实际被删除的节点 初值同x
    BinNodePosi(T) succ = nullptr;
    if(!HasLChild(*x))//若*x的左子树为空则直接将*x替换为其右子树
        succ = x = x->rChild;
    else if(!HasRChild(*x))
        succ = x = x->lChild;
    else{
        w = w->succ();//找到直接后继
        swap(x->data, w->data);
        BinNodePosi(T) u = w->parent;
        ((u == x) ? u->rChild : u->lChild) = succ = w->rChild;
    }
    hot = w->parent;//记录被删除节点的父亲
    if(succ)
        succ->parent = hot;//并将被删除节点的接替者与hot关联
    return succ;//返回接替者
}
