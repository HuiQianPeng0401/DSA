#pragma once
#include <iostream>
#include <algorithm>
#include "BST.hpp"

#define IsBlack(p) (!(p) || (RB_BLACK == (p)->color))
#define IsRed(p) (!IsBlack(p))
//此处的height已经不再是常规树高 而是红黑树的黑高度
//节点需要更新高度分三种：
//1、左右孩子的黑高度不等
//2、作为红节点，其黑高度不等于其子树的黑高度
//3、作为黑节点，其黑高度不等于其子树的黑高度加一
#define BlackHeightUpdated(x) ( \
    (stature((x).lChild)) == (stature((x).rChild)) && \
    ((x).height == (IsRed(&x) ? stature((x).lChild) : stature((x).lChild) + 1)) \
)

template <typename T>
class RedBlack : public BST<T>{
    protected:
        void solveDoubleRed(BinNodePosi(T) x);//双红处理
        void solveDoubleBlack(BinNodePosi(T) x);//双黑处理
        int updateHeight(BinNodePosi(T) x);//重写高度更新
    public:
        BinNodePosi(T) insert(const T &e);
        bool remove(const T &e);
        //BST::search(const T &e);
};
template <typename T>
int RedBlack<T>::updateHeight(BinNodePosi(T) x){//更新红黑树节点高度
    x->height = max(stature(x->lChild), stature(x->rChild));//孩子一般黑高度相等 除非出现双黑
    return IsBlack(x) ? x->height++ : x->height;//节点若为黑则加一
}
template <typename T>
BinNodePosi(T) RedBlack<T>::insert(const T &e){
    BinNodePosi(T) &x = this->search(e);//按照BST规则寻找e
    if(x)
        return x;
    x = new BinNode<T>(e, this->_hot, NULL, NULL, -1);//以hot为父 黑高度为-1
    this->_size++;
    solveDoubleRed(x);
    return x;
}
template <typename T>
void RedBlack<T>::solveDoubleRed(BinNodePosi(T) x){
    if(IsRoot(*x)){//已递归树根
        this->_root->color = RB_BLACK;//转黑
        this->_root->height++;//增高
        return;
    }
    BinNodePosi(T) p = x->parent;//x之父必定存在
    if(IsBlack(p))//为黑可终止
        return;
    BinNodePosi(T) g = p->parent;//既然p为红 则g必存在
    BinNodePosi(T) u = uncle(x);//以下 视x叔父颜色处理
    if(IsBlack(u)){//u为黑色(含NULL时)
        if(IsLChild(*x)==IsLChild(*p))//x p同侧
            p->color = RB_BLACK;
        else//异侧
            x->color = RB_BLACK;
        g->color = RB_RED;
        BinNodePosi(T) gg = g->parent;
        BinNodePosi(T) &h = FromParentTo(*g);//记录来自父亲的指针
        BinNodePosi(T) r = h = this->rotateAt(x);//r为调整后的子树根节点
        r->parent = gg;
    }
    else{
        p->color = RB_BLACK;
        p->height++;
        u->color = RB_BLACK;
        u->height++;
        if(!IsRoot(*g))
            g->color = RB_RED;
        solveDoubleRed(g);//g染红后可能会再次出现双红
    }
}
template <typename T>
bool RedBlack<T>::remove(const T &e){
    BinNodePosi(T) &x = this->search(e);//寻找关键码e
    if(!x)
        return false;
    BinNodePosi(T) r = removeAt(x, this->_hot);//实施删除
    if(0>=--this->_size)//仅一或零个节点 删除后可返回
        return true;
    //??????????
    if(!this->_hot){//若刚被删除的节点为根节点
        this->_root->color = RB_BLACK;//新根节点置黑
        updateHeight(this->_root);//更新其高度
        return true;
    }//以下 x非根 hot必不为空
    if(BlackHeightUpdated(*(this->_hot)))//满足黑高度平衡 无需调整
        return true;
    if(IsRed(r)){//r为红
        r->color = RB_BLACK;//转黑即可
        r->height++;
        return true;
    }
    solveDoubleBlack(r);//x r均为黑 则需处理
    return true;
}
template <typename T>
void RedBlack<T>::solveDoubleBlack(BinNodePosi(T) r){
    BinNodePosi(T) p = r ? r->parent : this->_hot;//x可能为空
    BinNodePosi(T) s = (r == p->lChild) ? p->rChild : p->lChild;
    if(IsBlack(s)){
        BinNodePosi(T) t = NULL;
        if(HasLChild(*s)&&IsRed(s->lChild))
            t = s->lChild;
        else if(HasRChild(*s)&&IsRed(s->rChild))
            t = s->rChild;
        if(t){
            RBColor oldColor = p->color;
            BinNodePosi(T) &h = FromParentTo(*p);
            BinNodePosi(T) b = h = this->rotateAt(t);
            if(HasLChild(*b))
                b->lChild->color = RB_BLACK;
            updateHeight(b->lChild);
            if(HasRChild(*b))
                b->rChild->color = RB_BLACK;
            updateHeight(b->rChild);
            b->color = oldColor;
            updateHeight(b);
        }
        else{
            s->color = RB_RED;//s转红
            s->height--;
            if(IsRed(p)){//p为红
                p->color = RB_BLACK;
            }
            else{//p为黑
                p->height--;
                solveDoubleBlack(p);//等效B树p所处节点下溢 继续调整
            }
        }
    }
    else{//兄弟s为红色
        s->color = RB_BLACK;//s转黑
        p->color = RB_RED;//p转红
        BinNodePosi(T) t = IsLChild(*s) ? s->lChild : s->rChild;//取同侧
        this->_hot = p;
        BinNodePosi(T) &h = FromParentTo(*p);
        h = this->rotateAt(t);
        solveDoubleBlack(r);//双黑缺陷仍存在 继续调整
    }
}
