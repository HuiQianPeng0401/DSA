#pragma once
#include "BinTree.hpp"
#include <iostream>

template <typename T>
class BST : public BinTree<T>{
    protected:
        BinNodePosi(T) _hot;//BST::search()最后访问的非空的节点的位置
        BinNodePosi(T) connect34(BinNodePosi(T), BinNodePosi(T), BinNodePosi(T),
                 BinNodePosi(T), BinNodePosi(T), BinNodePosi(T), BinNodePosi(T));
        BinNodePosi(T) rotateAt(BinNodePosi(T) x);//对x及其父亲、祖父做旋转调整
    public:
        virtual BinNodePosi(T) & search(const T &e);//查找
        virtual BinNodePosi(T) insert(const T &e);//插入
        virtual bool remove(const T &e);//删除
};
template <typename T>
static BinNodePosi(T) & searchIn(BinNodePosi(T) & v, const T &e, BinNodePosi(T) & hot){
    if(!v||e==v->data)//递归基：在v处命中或是假想的通配节点处命中
        return v;
    hot = v;//先记下当前节点然后
    return searchIn(((e < v->data) ? v->lChild : v->rChild), e, hot);//递归查找
}//返回时，返回值指向命中节点(或假想的统配节点)，hot则指向其父亲(退化时初始值为NULL)

template <typename T>
BinNodePosi(T)& BST<T>::search(const T &e){
    return searchIn(this->_root, e, _hot = NULL); //返回目标节点位置的引用以便后续插入等操作
}
template <typename T>
BinNodePosi(T) BST<T>::insert(const T &e){//将关键码e插入树中
    BinNodePosi(T) &x = search(e);//寻找e的位置 更新hot位置
    if(x)//存在返回即可
        return x;
    x = new BinNode<T>(e, _hot);//以hot为父创建新节点
    this->_size++;//更新规模
    this->updateHeightAbove(x);//更新高度
    return x;//新插入的节点必定为叶子
}//无论e是否存在原树中，返回时总有x->data==e
template <typename T>
static BinNodePosi(T) removeAt(BinNodePosi(T) & x, BinNodePosi(T) & hot){
    BinNodePosi(T) w = x;//实际被删除的节点
    BinNodePosi(T) succ = NULL;//实际被删除节点的接替者
    if(!HasLChild(*x))//若x左子树为空 则可
        succ = x = x->rChild;//将x替换为右子树
    else if(!HasRChild(*x))//若x右子树为空
        succ = x = x->lChild;//将x替换为左子树
    else{//两树都存在
        w = w->succ();//找到x中序遍历的直接后继w
        BinNodePosi(T) u = w->parent;//找到w的父节点方便连接
        swap(x->data, w->data);//交换数值
        //将直接后继与删除节点的值互换后
        if(u==x){//若直接后继与删除节点是相邻两层
        //记下直接后继的父节点u（和被删除节点相等）
        //p的右子树替换为直接后继的右子树
        //（因为直接后继在右子树中且直接后继没有左子树）
            u->rChild = succ = w->rChild;
        }
        else{//不属于相邻两层
        //记下直接后继的父节点p（和被删除节点不相等）
        //p的左子树替换为直接后继的右子树
        //（因为直接后继在右子树的左子树中且直接后继没有左子树）
            u->lChild = succ = w->lChild;
        }
    }
    hot = w->parent;//记录实际删除节点的父亲
    if(succ)//将被删除节点的接替者与hot连接
        succ->parent = hot;
    return succ;
}
template <typename T>
bool BST<T>::remove(const T &e){
    BinNodePosi(T) &x = search(e);
    if(!x)
        return false;
    removeAt(x, _hot);
    this->_size--;
    this->updateHeightAbove(_hot);
    return true;
}
template <typename T>
BinNodePosi(T) BST<T>::connect34(BinNodePosi(T) a, BinNodePosi(T) b, BinNodePosi(T) c,
                BinNodePosi(T) T0, BinNodePosi(T) T1, BinNodePosi(T) T2, BinNodePosi(T) T3){
    a->lChild = T0;
    if(T0)
        T0->parent = a;
    a->rChild = T1;
    if(T1)
        T1->parent = a;
    this->updateHeight(a);
    c->lChild = T2;
    if(T2)
        T2->parent = c;
    c->rChild = T3;
    if(T3)
        T3->parent = c;
    this->updateHeight(c);
    b->lChild = a;
    a->parent = b;
    b->rChild = c;
    c->parent = b;
    this->updateHeight(b);
    return b;
}
template <typename T>
BinNodePosi(T) BST<T>::rotateAt(BinNodePosi(T) v){
    BinNodePosi(T) p = v->parent;
    BinNodePosi(T) g = p->parent;
    if(IsLChild(*p)){
        if(IsLChild(*v)){
            p->parent = g->parent;
            return connect34(v, p, g, v->lChild, v->rChild, p->rChild, g->rChild);
        }
        else{
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
