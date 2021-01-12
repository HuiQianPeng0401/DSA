#pragma once
#include <iostream>
#include "Vector.hpp"

#define BTNodePosi(T) BTNode<T>* //B-树节点位置

template <typename T>
struct BTNode{//B-树节点模板类
    BTNodePosi(T) parent;//父节点
    Vector<T> key;//关键码向量
    Vector<BTNodePosi(T)> child;//孩子位置向量
    //构造函数(注意：BTNode只能作为根节点创建 而且初始时有0个关键码和1个空孩子)
    BTNode() { parent = NULL; child.insert(0,NULL); }
    BTNode(T e, BTNodePosi(T) lc = NULL, BTNodePosi(T) rc = NULL){
        parent = NULL;//作为根节点
        key.insert(0, e);//只有一个关键码
        child.insert(0, lc);
        child.insert(1, rc);//两个孩子
        if(lc)
            lc->parent = this;
        if(rc)
            rc->parent = this;
    }
};
template <typename T>
class BTree{
    protected:
        int _size;//存放的关键码总数
        int _order;//B-树的阶次 至少为3————创建时指定 不可修改
        BTNodePosi(T) _root;//根节点
        BTNodePosi(T) _hot;//BTree::search()最后访问的非空节点
        void solveOverflow(BTNodePosi(T));//因插入而上溢后的分裂处理
        void solveUnderflow(BTNodePosi(T));//因删除而下溢后的删除处理
    public:
        BTree(int order = 3) : _order(order), _size(0) { _root = new BTNode<T>(); }
        // ~BTree(){
        //     if(_root)
        //         release(_root);
        // }
        int const order() { return _order; }//阶次
        int const size() { return _size; }//规模
        BTNodePosi(T) & root() { return _root; }//根节点
        bool empty() const { return !_root; }//判空
        BTNodePosi(T) search(const T &e);//查找
        bool insert(const T &e);//插入
        bool remove(const T &e);//删除
};
template <typename T>
BTNodePosi(T) BTree<T>::search(const T &e){
    BTNodePosi(T) v = _root;//从根节点开始
    _hot = NULL;
    while(v){//逐层查找
        Rank r = v->key.search(e);//在当前节点中找到不大于e的最大关键码
        if((0<=r)&&(e==v->key[r]))//成功命中
            return v;
        _hot = v;//更新hot
        v = v->child[r + 1];//否则转入对应子树(hot对应其父)——需做I/O 费时间
    }//这里在向量内是二分查找 但对通常的_order可直接顺序查找
    return NULL;//失败：抵达外部节点
}
template <typename T>
bool BTree<T>::insert(const T &e){
    BTNodePosi(T) v = search(e);
    if(v)//确认目标节点不存在
        return false;
    Rank r = _hot->key.search(e);//在节点_hot的有序关键码向量中查找合适的插入位置
    _hot->key.insert(r + 1, e);//插入
    _hot->child.insert(r + 2, NULL);//插入一个空子树指针
    solveOverflow(_hot);//如有必要则做上溢处理
    return true;
}
template <typename T>//插入后的上溢处理
void BTree<T>::solveOverflow(BTNodePosi(T) v){
    if(_order>=v->child.size())
        return;//递归基：当前节点并未上溢
    Rank s = _order / 2;//中心轴点(此时有_order=key.size()=child.size()-1)
    BTNodePosi(T) u = new BTNode<T>();//创建有一个空孩子的新节点
    for (Rank j = 0; j < _order - s - 1;j++){//v右侧的_order-s-1个孩子及关键码分裂为右侧节点u
        u->key.insert(j, v->key.remove(s + 1));
        u->child.insert(j, v->child.remove(s + 1));
    }
    u->child[_order - s - 1] = v->child.remove(s + 1);//此时还剩下一个最靠右的孩子
    if(u->child[0])//统一父节点
        for (Rank j = 0; j < _order - s;j++)
            u->child[j]->parent = u;
    BTNodePosi(T) p = v->parent;//v当前的父节点
    if(!p){//若为空则创建根节点
        _root = new BTNode<T>();
        p->child[0] = v;
        v->parent = p;
    }
    Rank r = 1 + p->key.search(v->key[0]);//p中指向u的指针的秩
    p->key.insert(r, v->key.remove(s));//将轴点关键码上升至合适位置
    p->child.insert(r + 1, u);//将u插入
    solveOverflow(p);
}
template <typename T>
bool BTree<T>::remove(const T &e){
    BTNodePosi(T) v = search(e);//找到e所在的节点位置
    if(!v)//不存在返回即可
        return false;
    Rank r = v->key.search(e);//确定e在v中的秩
    if(v->child[0]){//若v非叶子节点
        BTNodePosi(T) u = v->child[r + 1];//确定v的右子树
        while(u->child[0])//利用二叉搜索树的性质 不断向左寻找
            u = u->child[0];//找到叶节点停止
        v->key[r] = u->key[0];//u->key[0]必定是v->key[r]的直接后继
        v = u;//交换位置
        r = 0;
    }//此时 v位于最底层，且其中第r个关键码是待删除者
    v->key.remove(r);//移除关键码
    v->child.remove(r + 1);//移除两个外部节点之一
    solveUnderflow(v);//处理
    return true;
}
template <typename T>//节点下溢处理
void BTree<T>::solveUnderflow(BTNodePosi(T) v){
    if((_order+1)/2<=v->child.size())
        return;//递归基：当前节点并未下溢
    BTNodePosi(T) p = v->parent;
    if(!p){//递归基：已到根节点，没有孩子的下限
        if(!v->key.size()&&v->child[0]){
            //倘若作为树根的v已不含关键码 却有唯一的非空孩子 则
            _root = v->child[0];//成为根节点
            _root->parent = NULL;
            v->child[0] = NULL;
            //release(v);
        }//树高下降1
        return;
    }
    Rank r = 0;
    //确定v是p的第r个孩子————此时v发生下溢 有可能不含关键码 故不能通过关键码查找
    //情况1：向左兄弟借关键码
    while(p->child[r]!=v)
        r++;
    if(0<r){//若v不是p的第一个孩子
        BTNodePosi(T) ls = p->child[r - 1];//记录左兄弟
        if((_order+1)/2<=ls->child.size()){//左兄弟足够胖
            v->key.insert(0, p->key[r - 1]);//p借出关键码给v
            p->key[r - 1] = ls->key.remove(ls->key.size() - 1);//ls的最大关键码转入p
            v->child.insert(0, ls->child.remove(ls->child.size() - 1));//ls最右侧孩子过继给v
            if(v->child[0])
                v->child[0]->parent = v;//作v的最左侧孩子
            return;
        }
    }//至此 左兄弟要么为空 要么太廋
    if(p->child.size()-1>r){//若v不是p的最后一个孩子
        BTNodePosi(T) rs = p->child[r + 1];//记录右兄弟
        if((_order+1)/2<=rs->child.size()){//右兄弟足够胖
            v->key.insert(v->key.size(), p->key[r]);//p借出关键码给v
            p->key[r] = rs->key.remove(0);//rs的最小关键码转入p
            v->child.insert(v->child.size(), rs->child.remove(0));//rs最左侧孩子过继给v
            if(v->child[v->child.size()-1])
                v->child[v->child.size() - 1]->parent = v;//作v最右侧孩子
        }
    }
    //情况3：左右兄弟要么太廋 要么为空
    if(0<r){//与左兄弟合并
        BTNodePosi(T) ls = p->child[r - 1];//记录左兄弟
        ls->key.insert(ls->key.size(), p->key.remove(r - 1));//p的第r-1个关键码转入ls
        p->child.remove(r);//v不再是p的第r个孩子
        ls->child.insert(ls->child.size(), v->child.remove(0));//v的最左侧孩子过继给ls做最右侧孩子
        if(ls->child[ls->child.size()-1])
            ls->child[ls->child.size() - 1]->parent = ls;
        while(!v->key.empty()){//此时剩余的child和key数目相等 都转入ls
            ls->key.insert(ls->key.size(), v->key.remove(0));
            ls->child.insert(ls->child.size(), v->child.remove(0));
            if(ls->child[ls->child.size()-1])
                ls->child[ls->child.size() - 1]->parent = ls;
        }
        //release(v);
    }
    else{//与右兄弟合并
        BTNodePosi(T) rs = p->child[r + 1];//记录右兄弟
        rs->key.insert(0, p->key.remove(r));//p的第r个关键码转入rs
        p->child.remove(r);//v不再是p的第r个孩子
        rs->child.insert(0, v->child.remove(v->child.size() - 1));//v最右侧孩子过继给rs做最左侧孩子
        if(rs->child[0])
            rs->child[0]->parent = rs;
        while(!v->key.empty()){//此时剩余的child和key数目相等 都转入rs
            rs->key.insert(0, v->key.remove(v->key.size() - 1));
            rs->child.insert(0, v->child.remove(v->child.size() - 1));
            if(rs->child[0])
                rs->child[0]->parent = rs;
        }
        //release(v);
    }
    solveUnderflow(p);//上升一层 如有必要继续分裂 至多递归O(logn)层
    return;
}
