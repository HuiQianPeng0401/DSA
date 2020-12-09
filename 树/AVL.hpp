#pragma once
#include <iostream>
#include "BST.hpp"

#define Balanced(x) (stature((x).lChild) == stature((x).rChild))//理想平衡
#define BalFac(x) (stature((x).lChild) - stature((x).rChild))//平衡因子
#define AvlBalanced(x) ((-2 < BalFac(x)) && (BalFac(x) < 2))//平衡条件
//在左右孩子中取更高者 若等高，则与父亲p同侧者优先
#define tallerChild(x) ( \
    stature((x)->lChild) > stature((x)->rChild) ? (x)->lChild : ( \
    stature((x)->rChild) > stature((x)->lChild) ? (x)->rChild : ( \
    IsLChild(*(x)) ? (x)->lChild : (x)->rChild \
    ) \
    ) \
)

template <typename T>
class AVL : public BST<T>{
    public:
        BinNodePosi(T) insert(const T &e);//重写插入
        bool remove(const T &e);//重写删除
};

template <typename T>
BinNodePosi(T) AVL<T>::insert(const T &e){
    BinNodePosi(T) &x = this->search(e);//先查询x的位置 同时更新hot位置
    if(x)//存在返回即可
        return x;
    x = new BinNode<T>(e, this->_hot);//以hot为父创建新节点(此时hot可能增高，祖父可能失衡)
    for (BinNodePosi(T) g = this->_hot; g; g = g->parent){//从x之父出发 寻找最深失衡节点g
        if(!AvlBalanced(*g)){
            BinNodePosi(T) &tmp = FromParentTo(*g);//标记来自g父亲的指针
            tmp = this->rotateAt(tallerChild(tallerChild(g)));//连接起来
            break;//g复衡后 局部子树必定复原，其祖先亦必定复原，即可停止
        }
        else
            this->updateHeight(g);//即使g未失衡 高度也可能增加
    }
    return x;//返回新节点
}//无论e是否存在原树中 返回时总有e==x->data

template <typename T>
bool AVL<T>::remove(const T &e){
    BinNodePosi(T) &x = this->search(e);//查询x的位置顺便更新hot的位置
    removeAt(x, this->_hot);//按BST规则删除再调整
    if(!x)//不存在返回即可
        return false;
    for (BinNodePosi(T) g = this->_hot; g; g = g->parent){//从x之父出发 找到失衡节点g
        if(!AvlBalanced(*g)){
            BinNodePosi(T) &tmp = FromParentTo(*g);//标记g父亲的指针
            g = tmp = this->rotateAt(tallerChild(tallerChild(g)));//调整后g的高度会减小1，故祖先可能会失衡
        }
        this->updateHeight(g);//更新高度
    }
    return true;
}
