#pragma once
#include <iostream>
#include "BST.hpp"

template <typename T>
class Splay : public BST<T>{
    protected:
        BinNodePosi(T) splay(BinNodePosi(T) v);//将节点v伸展至树根
    public:
        BinNodePosi(T) & search(const T &e);//重写查找
        BinNodePosi(T) insert(const T &e);//重写插入
        bool remove(const T &e);//重写删除
};
template <typename NodePosi>//在节点*p与*lc(可能为空)之间建立父子关系
inline void attachAsLChild(NodePosi p, NodePosi lc){
    p->lChild = lc;
    if(lc)
        lc->parent = p;
}
template <typename NodePosi>//在节点*p与*lc(可能为空)之间建立父子关系
inline void attachAsRChild(NodePosi p, NodePosi rc){
    p->rChild = rc;
    if(rc)
        rc->parent = p;
}
template <typename T>//Splay树伸展算法：将节点v伸展到树根
BinNodePosi(T) Splay<T>::splay(BinNodePosi(T) v){
    if(!v)
        return NULL;
    BinNodePosi(T) p;//v的父亲
    BinNodePosi(T) g;//v的祖父
    while((p=v->parent)&&(g=p->parent)){//对v做双层伸展
        BinNodePosi(T) gg = g->parent;//记录曾祖父的位置
        if(IsLChild(*v)){
            if(IsLChild(*p)){
                attachAsLChild(g, p->rChild);
                attachAsLChild(p, v->rChild);
                attachAsRChild(p, g);
                attachAsRChild(v, p);
            }
            else{
                attachAsLChild(p, v->rChild);
                attachAsRChild(g, v->lChild);
                attachAsLChild(v, g);
                attachAsRChild(v, p);
            }
        }
        else if(IsRChild(*p)){
            attachAsRChild(g, p->lChild);
            attachAsRChild(p, v->lChild);
            attachAsLChild(p, g);
            attachAsLChild(v, p);
        }
        else{
            attachAsRChild(p, v->lChild);
            attachAsLChild(g, v->rChild);
            attachAsRChild(v, g);
            attachAsLChild(v, p);
        }
        if(!gg)//若v原先的曾祖父gg不存在 则v现在是树根
            v->parent = NULL;
        else//否则 gg应为左孩子或右孩子
            (g == gg->lChild) ? attachAsLChild(gg, v) : attachAsRChild(gg, v);
        this->updateHeight(g);
        this->updateHeight(p);
        this->updateHeight(v);
    }
    /*
    若v最初的深度为奇数，则经过若干次
    双层调整至最后一次调整时，v的父亲p即是树根r。
    */
    if(v->parent){//在做一次单旋
        p = v->parent;
        if(IsLChild(*v)){
            attachAsLChild(p, v->rChild);
            attachAsRChild(v, p);
        }
        else{
            attachAsRChild(p, v->lChild);
            attachAsLChild(v, p);
        }
        this->updateHeight(p);
        this->updateHeight(v);
    }
    v->parent = NULL;
    return v;
}//调整之后新树根应为被伸展的节点，故返回该节点的位置以便上层函数更新树根
template <typename T>
BinNodePosi(T) & Splay<T>::search(const T &e){
    BinNodePosi(T) p = searchIn(this->_root, e, this->_hot);
    this->_root = splay((p ? p : this->_hot));//将最后一个被访问的节点伸展至树根
    return this->_root;
}
template <typename T>
BinNodePosi(T) Splay<T>::insert(const T &e){
    if(!this->_root){//处理原树退化为空的情况
        this->_size++;
        return this->_root = new BinNode<T>(e);
    }
    if(e==search(e)->data)//执行search
        return this->_root;
    this->_size++;
    BinNodePosi(T) t = this->_root;//记录原树根
    if(this->_root->data<e){
        t->parent = this->_root = new BinNode<T>(e, NULL, t, t->rChild);
        if(HasRChild(*t)){
            t->rChild->parent = this->_root;
            t->rChild = NULL;
        }
    }
    else{
        t->parent = this->_root = new BinNode<T>(e, NULL, t->lChild, t);
        if(HasLChild(*t)){
            t->lChild->parent = this->_root;
            t->lChild = NULL;
        }
    }
    this->updateHeightAbove(t);//更新t及其祖先的高度
    return this->_root;//新节点即是树根
}//返回时有this->_root==e
template <typename T>
bool Splay<T>::remove(const T &e){
    if(!this->_root||(e!=search(e)->data))//判树空和执行search
        return false;
    //经search后e已经被伸展至树根
    BinNodePosi(T) w = this->_root;//记录原根节点
    if(!HasLChild(*this->_root)){//无左子树直接删除
        this->_root = this->_root->rChild;
        if(this->_root)
            this->_root->parent = NULL;
    }
    else if(!HasRChild(*this->_root)){//无右子树直接删除
        this->_root = this->_root->lChild;
        if(this->_root)
            this->_root->parent = NULL;
    }
    else{//拥有两颗子树
        BinNodePosi(T) lTree = this->_root->lChild;//记录左子树
        lTree->parent = NULL;//切断
        this->_root->lChild = NULL;//切断
        this->_root = this->_root->rChild;//移动根节点
        this->_root->parent = NULL;//切断
        search(w->data);//利用必然失败的search来寻找右子树中的最小节点
        //此时此最小节点伸展至树根 并且其左子树必定为空
        this->_root->lChild = lTree;//接入
        lTree->parent = this->_root;//接入
    }
    //release(w->data);
    //release(w);
    this->_size--;
    if(this->_root)
        this->updateHeight(this->_root);//更新高度
    return true;
}
