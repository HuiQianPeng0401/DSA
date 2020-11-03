#include <iostream>
using namespace std;

#include "BST.hpp"

#define IsBlack(p) (!(p)||(RB_BLACK==(p->color)))//外部节点也视作黑节点
#define IsRed(p) (!IsBlack(p))//非黑即红
#define BlackHeightUpdated(x) ( \
    (stature((x).lChild)==stature((x).rChild)) && \
    ((x).height==(IsRed(&x)?stature((x).lChild) : stature((x).lChild)+1)) \
)//RedBlack高度更新条件

template <typename T>
class RedBlack : public BST<T>{
    public:
        void solveDoubleRed(BinNodePosi(T) x);//双红修正
        void solveDoubleBlack(BinNodePosi(T) x);//双黑修正
        int updateHeight(BinNodePosi(T) x);//更新节点x的高度
        BinNodePosi(T) insert(const T &e);//重写插入算法
        bool remove(const T &e);//重写删除算法
    //沿用BST::search()
};

template <typename T>
int RedBlack<T>::updateHeight(BinNodePosi(T) x){
    //更新红黑树节点高度
    x->height = max(stature(x->lChild), stature(x->rChild));//孩子一般黑高度相等，除非出现双黑
    return IsBlack(x) ? x->height++ : x->height;//若当前节点为黑 则计入黑深度
}//因统一定义stature(NULL)=-1，故height比黑高度少一
/**
 * 解决节点x和其父均为红的问题 
 * RR-1: 2次颜色翻转 2次黑高度更新 1~2次旋转 无递归
 * RR-2：3次颜色翻转 3次黑高度更新 0出旋转 需要递归
*/
template <typename T>
void RedBlack<T>::solveDoubleRed(BinNodePosi(T) x){
    if(IsRoot(*x)){//若已经递归至树根，则将其转黑，整树黑高度也随之递增
        _root->color = RB_BLACK;
        _root->height++;
        return;
    }
    //否则 x的父亲p必存在
    BinNodePosi(T) p = x->parent;
    if(IsBlack(p))
        return;//p为黑直接终止调整
    BinNodePosi(T) g = p->parent;
    BinNodePosi(T) u = uncle(x);//以下按叔父u的颜色来处理
    if(IsBlack(u)){
        if(IsLChild(*x)==IsLChild(*p))
            p->color = RB_BLACK;//p红转黑
        else
            x->color = RB_BLACK;//x红转黑
        g->color = RB_RED;//g必定由黑转红
        BinNodePosi(T) gg = g->parent;
        BinNodePosi(T) r = FromParentTo(*g) = rotateAt(x);//调整后的子树根节点
        r->parent = gg;
    }
    else{//若u为红色
        p->color = RB_BLACK;
        p->height++;//p红转黑
        u->color = RB_BLACK;
        u->height++;//u黑转红
        if(!IsRoot(*g))
            g->color = RB_RED;//若g非根 则转红
        solveDoubleRed(g);
    }
}
template <typename T>
BinNodePosi(T) RedBlack<T>::insert(const T &e){
    BinNodePosi(T) &x = search(e);
    if(x)
        return x;
    x = new BInNode<T>(e, _hot, NULL, NULL, -1);
    _size++;//创建红节点x：以_hot为父，黑高度-1
    solveDoubleRed(x);
    return x;
}

template <typename T>
bool RedBlack<T>::remove(const T &e){
    BinNodePosi(T) &x = search(e);
    if(!x)
        return false;
    BinNodePosi(T) r = removeAt(x, _hot);
    if(0>=--_size)
        return true;
    //_hot某一孩子刚被删除，且被r所指节点(NULL)接替，以下检查是否失衡
    if(!_hot){//若刚被删除的是根节点，则将其置黑并更新黑高度
        _root->color = RB_BLACK;
        updateHeight(_root);
        return true;
    }
    //以下：原x(现r)必非根，_hot必非空
    if(BlackHeightUpdated(*(_hot)))//若所有祖先的黑深度依然平衡，则无需调整
        return true;
    if(IsRed(r)){
        r->color = RB_RED;
        r->height++;
        return true;
    }
    solveDoubleBlack(r);
    return true;
}
/**
 * RedBlack双黑调整算法 解决节点x与被其替代的节点均为黑色的问题
 * 分为三大类四种清空：
 * BB-1，s和其子节点有一个为红：2次颜色翻转，2次黑高度更新，1~2次旋转 无递归
 * BB-2R：2次颜色
*/
template <typename T>
void RedBlack<T>::solveDoubleBlack(BinNodePosi(T) r){
    BinNodePosi(T) p = r ? r->parent : _hot;//r的父亲
    if(!p)
        return;
    BinNodePosi(T) s = (r == p->lChild) ? p->lChild : p->rChild;//r的兄弟
    if(IsBlack(s)){//s为黑
        BinNodePosi(T) t = NULL;//s的红孩子
        if(HasLChild(*s)&&ISRed(s->lChild))
            t = s->lChild;
        else if(HasRChild(*s)&&IsRChild(s->rChild))
            t = s->rChild;
        if(t){//有红孩子 BB-1
            RBcolor oldcolor = p->color;//备份原子树根节点颜色
            BinNodePosi(T) b = FromParentTo(*p) = rotateAt(t);//重平衡，并将新子树的左右孩子染黑
            if(HasLChild(*b))
                b->lChild->color = RB_BLACK;
            updateHeight(b->lChild);
            if(HasRChild(*b))
                b->rChild->color = RB_BLACK;
            updateHeight(b->rChild);
            b->color = oldcolor;
            updateHeight(b);//新子树根节点继承原根节点颜色
        }
        else{//黑s为红孩子
            s->color = RB_RED;
            s->height--;//转红后高度减一
            if(IsRed(p))
                p->color = RB_BLACK;//p转黑但黑高度不变
            else{
                p->height--;
                solveDoubleBlack(p);
            }
        }
    }
    else{//兄弟s为红：BB-3
        s->color = RB_BLACK;
        p->color = RB_RED;
        BinNodePosi(T) t = IsLChild(*s) ? s->lChild : s->rChild;//取t与其父s同侧
        _hot = p;
        FromParentTo(*p) = rotateAt(t);//对t及其父亲、祖父做平衡调整
        solveDoubleBlack(r);
    }
}


