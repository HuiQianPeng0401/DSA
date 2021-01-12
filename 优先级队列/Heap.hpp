#pragma once
#include <iostream>
#include <algorithm>
#include "Vector.hpp"
#include "BinTree.hpp"

#define InHeap(n, i) (((-1) < (i)) && ((i) < (n)))//判断PQ[i]是否合法
#define Parent(i) ((i - 1) >> 1)//i的父节点
#define LastInternal(n) Parent(n - 1)//最后一个内部节点 即末节点的父亲
#define LChild(i) (1 + ((i) << 1))//i的左孩子
#define RChild(i) ((1 + (i)) << 1)//i的右孩子
#define ParentValid(i) (0 < i)//i是否有父亲
#define LChildValid(n, i) InHeap(n, LChild(i))//i是否有左孩子
#define RChildValid(n, i) InHeap(n, RChild(i))//i是否有两个孩子
#define Bigger(PQ, i, j) ((PQ[i] < PQ[j]) ? j : i)//取大
//在三者中找最大者
#define ProperParent(PQ, n, i) \
            ( RChildValid(n,i) ? Bigger(PQ, Bigger(PQ, i, LChild(i)), RChild(i) ) : \
            ( LChildValid(n,i) ? Bigger(PQ, i, LChild(i) ) : i \
            ) \
            )


template <typename T>
struct PQ{
    virtual void insert(T e) = 0;
    virtual T getMax() = 0;
    virtual T delMax() = 0;
};
template <typename T>
class Heap : public PQ<T>, public Vector<T>{
    protected:
        Rank percolateDown(Rank n, Rank i);
        Rank percolateUp(Rank i);
        void heapify(Rank n);
    public:
        Heap() { }
        Heap(T *A, Rank n){
            this->copyFrom(A, 0, n);
            heapify(n);
        }
        void insert(T e);
        T getMax();
        T delMax();
};
template <typename T>
void Heap<T>::insert(T e){
    Vector<T>::insert(e);//插入末尾
    percolateUp(this->_size - 1);//上滤调整
}
template <typename T>
Rank Heap<T>::percolateUp(Rank i){
    while(ParentValid(i)){//若还有父亲
        Rank j = Parent(i);//父：j
        if (this->_elem[i] < this->_elem[j])//满足堆序性 停止
            break;
        swap(this->_elem[i], this->_elem[j]);//交换
        i = j;//交换后继续考察上一层
    }
    return i;//返回最终的上滤位置
}
template <typename T>
T Heap<T>::getMax(){
    return this->_elem[0];
}
template <typename T>
T Heap<T>::delMax(){
    T minElem = this->_elem[0];
    this->_elem[0] = this->_elem[--this->_size];
    percolateDown(this->_size, 0);
    return minElem;
}
template <typename T>
Rank Heap<T>::percolateDown(Rank n, Rank i){
    Rank j;
    while (i != (j = ProperParent(this->_elem, n, i))){
        swap(this->_elem[i], this->_elem[j]);
        i = j;
    }
    return i;
}
template <typename T>
void Heap<T>::heapify(Rank n){
    for (Rank i = LastInternal(n); InHeap(n, i); i--){
        percolateDown(n, i);
    }
}

template <typename T>
class LeftHeap : public PQ<T>, public BinTree<T>{
    public:
        LeftHeap(){}
        LeftHeap(T *E, int n){
            for (int i = 0; i < n; i++)
                this->insert(E[i]);
        }
        void insert(T);
        T getMax();
        T delMax();
};
template <typename T>
static BinNodePosi(T) merge(BinNodePosi(T) a, BinNodePosi(T) b){
    if (!a)
        return b;
    if(!b)
        return a;
    if (a->data < b->data)
        swap(a, b);
    a->rChild = merge(a->rChild, b);
    a->rChild->parent = a;
    if (!a->lChild || a->lChild->npl < a->rChild->npl)
        swap(a->lChild, a->rChild);
    a->npl = (a->rChild) ? a->rChild->npl + 1 : 1;
    return a;
}
template <typename T>
T LeftHeap<T>::delMax(){
    BinNodePosi(T) lHeap = this->_root->lChild;
    BinNodePosi(T) rHeap = this->_root->rChild;
    T e = this->_root->data;
    delete this->_root;
    this->_size--;
    this->_root = merge(lHeap, rHeap);
    if(this->_root)
        this->_root->parent = NULL;
    return e;
}
template <typename T>
T LeftHeap<T>::getMax(){
    return this->_root->data;
}
template <typename T>
void LeftHeap<T>::insert(T e){
    BinNodePosi(T) v = new BinNode<T>(e);
    this->_root = merge(this->_root, v);
    this->_root->parent = NULL;
    this->_size++;
}
