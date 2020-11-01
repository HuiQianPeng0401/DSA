#include <iostream>
using namespace std;
#include "myVector.hpp"

#define BTNodePosi(T) BTNode<T>*

template <typename T>
struct BTNode{
    Vector<T> key;
    Vector<BTNodePosi(T)> child;
    BTNodePosi(T) parent;
    BTNode() { parent = NULL, child.insert(0, NULL); }
    BTNode(T e, BTNodePosi(T) lc = NULL, BTNodePosi(T) rc = NULL){
        parent = NULL;
        key.insert(0, e);
        child.insert(0, lc);
        child.insert(1, rc);
        if(lc)
            lc->parent = this;
        if(rc)
            rc->parent = this;
    }
};
template <typename T>
class BTree{
    public:
        int _size;//关键码总数
        int _order;//阶次
        BTNodePosi(T) _root;
        BTNodePosi(T) _hot;//BTree::search()最后访问的非空节点位置
        void solveOverflow(BTNodePosi(T));
        void solveUnderflow(BTNodePosi(T));
        BTree(int order=3):_order(order),_size(0)
        {
            _root = new BTNode<T>();
        }
        ~BTree(){if(_root)
                delete _root;
        }
        int const order() { return _order; }
        int const size() { return _size; }
        BTNodePosi(T) & root() { return _root; }
        bool empty() const { return !_root; }
        BTNodePosi(T) search(const T &e);
        bool insert(const T &e);
        bool remove(const T &e);
};//BTree

template <typename T>
BTNodePosi(T) BTree<T>::search(const T &e){
    BTNodePosi(T) v = _root;
    _hot = NULL;
    while(v){
        Rank r = v->key.search(e);
        if((0<=r)&&(e==v->key[r]))
            return v;
        _hot = v;
        v = v->child[r + 1];
    }
    return NULL;
}
template <typename T>
bool BTree<T>::insert(const T &e){
    BTNodePosi(T) v = search(e);
    if(v)
        return false;
    Rank r = _hot->key.search(e);
    _hot->key.insert(r + 1, e);
    _hot->child.insert(r + 2, NULL);
    _size++;
    solveOverflow(_hot);
    return true;
}
template <typename T>
void BTree<T>::solveOverflow(BTNodePosi(T) v){
    if(_order>=v->child.size())
        return;
    BTNodePosi(T) u = new BTNode<T>();
    Rank s = _order / 2;
    for (Rank j = 0; j < _order - s - 1;j++){
        u->child.insert(j, v->child.remove(s + 1));
        u->key.insert(j, v->key.remove(s + 1));
    }
    u->child[_order - s - 1] = v->child.remove(s + 1);
    if(u->child[0]){
        for (Rank j = 0; j < _order - s;j++)
            u->child[j]->parent = u;
    }
    BTNodePosi(T) p = v->parent;
    if(!p){
        p = new BTNode<T>();
        p->child[0] = v;
        v->parent = p;
    }
    Rank r = 1 + p->key.search(v->key[0]);
    p->key.insert(r, v->key.remove(s));
    p->child.insert(r + 1, u);
    u->parent = p;
    solveOverflow(p);
}
template <typename T>
bool BTree<T>::remove(const T &e){
    BTNodePosi(T) v = search(e);
    if(!v)
        return false;
    Rank r = v->key.search(e);
    if(v->child[0]){
        BTNodePosi(T) u = v->child[r + 1];
        while(u->child[0])
            u = u->child[0];
        v->key[r] = u->key[0];
        v = u;
        r = 0;
    }
    v->key.remove(r);
    v->child.remove(r + 1);
    _size--;
    solveUnderflow(v);
    return true;
}
template <typename T>
void BTree<T>::solveUnderflow(BTNodePosi(T) v){
    if((_order+1)/2<=v->child.size())
        return;
    BTNodePosi(T) p = v->parent;
    if(!p){//已到根节点，么有孩子的下限
        if(!v->key.size()&&v->child[0]){
            //但若是作为树根的v已不含关键点，却有唯一的非空孩子，
            //则这个节点可以被跳过，并因为没用而被销毁
            _root = v->child[0];
            _root->parent = NULL;
            v->child[0] = NULL;
            delete v;
        }
        return;
    }
    Rank r = 0;
    while(p->child[r]!=v)
        r++;
    //确定v是p的第r个孩子———此时v可能不含关键码，故不能通过关键码查找
    //另外在实现了孩子指针的判等器之后也可调用Vector::find()

    //情况一：向左兄弟借关键码
    if(0<r){//若v不是p的第一个孩子，则左兄弟必存在
        BTNodePosi(T) ls = p->child[r - 1];//左兄弟
        if((_order+1)/2<ls->child.size()){
            //若左兄弟足够胖，则p借出一个关键码给v作为最小关键码
            //同时ls的最大关键码转入p
            v->key.insert(0, p->key[r - 1]);
            p->key[r - 1] = ls->key.remove(ls->key.size() - 1);
            v->child.insert(0, ls->child.remove(ls->child.size() - 1));
            if(v->child[0])
                v->child[0]->parent = v;
            return;
        }
    }
    //情况2：向右兄弟借关键码
    if(p->child.size()-1>r){//若v不是p的最后一个孩子，则
        BTNodePosi(T) rs = p->child[r + 1];//右兄弟
        if((_order+1)/2<rs->child.size()){//若右兄弟足够胖
            v->key.insert(v->key.size(), p->key[r]);
            p->key[r] = rs->key.remove(0);
            v->child.insert(v->child.size(), rs->child.remove(0));
        }
        if(v->child[v->child.size()-1])
            v->child[v->child.size() - 1]->parent = v;
        return;
    }
    //情况3：左右兄弟要么为空 要么太廋
    if(0<r){//与左兄弟合并
        BTNodePosi(T) ls = p->child[r - 1];//左兄弟必存在
        ls->key.insert(ls->key.size(), p->key.remove(r - 1));
        p->child.remove(r);
        //p的第r-1个关键码转入ls，v不再是p的第r个孩子
        //这里先过继可能是因为child.size()=key.size()+1
        ls->child.insert(ls->child.size(), v->child.remove(0));
        if(ls->child[ls->child.size()-1])
            ls->child[ls->child.size() - 1]->parent = ls;
        while(!v->key.empty()){
            //v剩余的关键码和孩子依次转入ls
            ls->key.insert(ls->key.size(), v->key.remove(0));
            ls->child.insert(ls->child.size(), v->child.remove(0));
            if(ls->child[ls->child.size()-1])
                ls->child[ls->child.size() - 1]->parent = ls;
        }
        delete v;
    }
    else{//与右兄弟合并
        BTNodePosi(T) rs = p->child[r + 1];
        rs->key.insert(0, p->key.remove(r));
        p->child.remove(r);
        //p的第r个关键码转入rs v不再是p的第r个孩子
        //v的最右侧孩子过继给rs做最左侧孩子
        //这里先过继可能是因为child.size()=key.size()+1
        rs->child.insert(0, v->child.remove(v->child.size() - 1));
        if(rs->child[0])
            rs->child[0]->parent = rs;
        while(!v->key.empty()){
            rs->key.insert(0, v->key.remove(v->key.size() - 1));
            rs->child.insert(0, v->child.remove(v->child.size() - 1));
            if(rs->child[0])
                rs->child[0]->parent = rs;
        }
        delete v;
    }
    solveUnderflow(p);
    return;
}
