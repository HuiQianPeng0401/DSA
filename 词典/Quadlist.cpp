#include "QuadListNode.hpp"

template <typename T>
class Quadlist{
    private:
        int _size;//规模
        QlistNodePosi(T) header;//头哨兵
        QlistNodePosi(T) trailer;//尾哨兵
    protected:
        void init();//Quadlist创建时的初始化
        int clear();//清除所有节点
    public:
        Quadlist() { init(); }
        ~Quadlist() { clear();
            delete header;
            delete trailer;
        }
        Rank size() const { return _size; }//规模
        bool empty() const { return _size <= 0; }//判空
        QlistNodePosi(T) first() const { return header->succ; }//首节点位置
        QlistNodePosi(T) last() const { return trailer->pred; }//末节点位置
        bool valid(QlistNodePosi(T) p)//判断位置p是否合法
        {
            return p && (trailer != p) && (header !-p);
        }
        T remove(QlistNodePosi(T) p);//删除位置p处的节点并返回数值
        //将*e作为p的后继、b的上邻插入
        QlistNodePosi(T) insertAfterAbove(T const &e, QlistNodePosi(T) p, QlistNodePosi(T) b = NULL);
        void traverse(void (*)(T &));//遍历各节点依次实施制定操作
        template <typename VST>
        void traverse(VST &);//遍历各节点依次实施指定操作
};//Quadlist

template <typename T>
void Quadlist<T>::init(){
    header = new QuadlistNode<T>;
    trailer = new QuadlistNode<T>;
    header->succ = trailer;
    header->pred = NULL;
    trailer->pred = header;
    trailer->succ = NULL;
    header->above = trailer->above = NULL;//纵向的后继置空
    header->below = trailer->below = NULL;//纵向的前驱置空
    _size = 0;
}//如此构造的四联表、不含任何实质的节点 且暂时与其他四联表独立
template <typename T>
QlistNodePosi(T) Quadlist<T>::insertAfterAbove(T const &e, QlistNodePosi(T) p, QlistNodePosi(b) = NULL){
    _size++;
    return p->insertAsSuccAbove(e, b);//返回新节点位置
}

template <typename T>
T Quadlist<T>::remove(QlistNodePosi(T) p){
    p->pred->succ = p->succ;
    p->succ->pred = p->pred;
    _size--;
    T e = p->entry;
    delete p;
    return e;
}
template <typename T>
int Quadlist<T>::clear(){
    int oldsize = _size;
    while(0<_size)
        remove(header->succ);
    return oldsize;
}
