template <typename T>
class AVL : public BST<T>
{
    public:
        BinNodePosi(T) insert(const T &e);
        bool remove(const T &e);
};
template <typename T>
BinNodePosi(T) AVL<T>::insert(const T &e)
{
    BinNodePosi(T) &x = search(e);
    if(x)
        return x;
    BinNodePosi(T) xx = x = new BinNode<T>(e, _hot);
    _size++;
    //x的父亲_hot若增高，则其祖父有可能失衡
    for (BinNodePosi(T) g = _hot; g;g=g->parent){
        //从x之父向上出发 逐层检查各代祖先g
        if(!AvlBalanced(*g)){
            //一旦发现失衡则采用("3+4")算法使之复衡
            //重新接入原树
            FromParentTo(*g) = rotateAt(tallerChild(tallerChild(g)));
            break;
        }
        else
            updateHeight(g);
    }
    return xx;
}//无论e是否存在于原树中 总有AVL::insert(e)->data==e
template <typename T>
bool AVL<T>::remove(const T &e)
{
    BinNodePosi(T) &x = search(e);
    if(!x)
        return false;
    removeAt(x, _hot);
    _size--;
    for (BinNodePosi(T) g = _hot; g;g=g->parent){
        if(!AvlBalanced(*g))
            g = FromParentTo(*g) = rotateAt(tallerChild(tallerChild(g)));
        updateHeight(g);
    }
    return true;
}
/**
 * 按照3+4结构连接3个节点及其四颗子树，返回重组之后
 * 的局部子树根节点位置(即b)
 * 子树根节点与上层节点间的双向连接，均需由上层调用者完成
*/

/**
 * BST节点旋转统一算法(3节点+4子树)，返回调整之后局部子树根节点的位置
 * 注意：尽管子树根会正确的指向上层节点，但反向的连接必须由上层函数完成
*/
template <typename T>
    BinNodePosi(T) BST<T>::connect34(
        BinNodePosi(T) a, BinNodePosi(T) b, BinNodePosi(T) c,
        BinNodePosi(T) T0, BinNodePosi(T) T1, BinNodePosi(T) T2, BinNodePosi(T) T3
)
{
    a->lChild = T0;
    if(T0)
        T0->parent = a;
    a->rChild = T1;
    if(T1)
        T1->parent = a;
    updateHeight(a);
    c->lChild = T2;
    if(T2)
        T2->parent = c;
    c->rChild = T3;
    if(T3)
        T3->parent = c;
    updateHeight(c);
    b->lChild = a;
    a->parent = b;
    b->rChild = c;
    c->parent = b;
    updateHeight(b);
    return b;
}

template <typename T>
BinNodePosi(T) BST<T>::rotateAt(BinNodePosi(T) v)
{
    //v是孙辈节点
    BinNodePosi(T) p = v->parent;
    BinNodePosi(T) g = p->parent;
    if(IsLChild(*p)){//zig
        if(IsLChild(*v)){//zig-zig
            p->parent = g->parent;
            return connect34(v, p, g, v->lChild, v->rChild, p->rChild, g->rChild);
        }
        else{//zig-zag
            v->parent = g->parent;
            return connect34(p, v, g, p->lChild, v->lChild, v->rChild, g->rChild);
        }
    }
    else{
        if(IsRChild(*v)){
            p->parent = g->parent;
            return connect34(g, p, v, g->lChild, p->lChild, v->lChild, v->rChild);
        }
        else{
            v->parent = g->parent;
            return connect34(g, v, p, g->lChild, v->lChild, v->rChild, p->rChild);
        }
    }
}

