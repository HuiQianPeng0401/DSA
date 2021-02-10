#pragma once
#include "List.hpp"
#include "Entry.hpp"
#include <algorithm>

template <typename K, typename V>
struct Dictionary
{
    virtual int size() const = 0;
    virtual bool put(K, V) = 0;
    virtual V *get(K k) = 0;
    virtual bool remove(K k) = 0;
};
#define QlistNodePosi(T) QuadlistNode<T>*

template <typename T>
struct QuadlistNode
{
    T entry;
    QlistNodePosi(T) pred;
    QlistNodePosi(T) succ;
    QlistNodePosi(T) above;
    QlistNodePosi(T) below;
    QlistNodePosi(T) insertAsSuccAbove(const T &e, QlistNodePosi(T) b = nullptr);
    QuadlistNode(T e = T(), QlistNodePosi(T) p = nullptr, QlistNodePosi(T) s = nullptr, QlistNodePosi(T) a = nullptr, QlistNodePosi(T) b = nullptr)
        : entry(e), pred(p), succ(s), above(a), below(b){}
};
template <typename T>
QlistNodePosi(T) QuadlistNode<T>::insertAsSuccAbove(const T &e, QlistNodePosi(T) b)
{
    QlistNodePosi(T) x = new QuadlistNode<T>(e, this, succ, nullptr, b);
    succ->pred = x;
    succ = x;
    if(b)
        b->above = x;
    return x;
}
template <typename T>
class Quadlist
{
    private:
        int _size;
        QlistNodePosi(T) header;
        QlistNodePosi(T) trailer;
    protected:
        void init();
        int clear();
    public:
        Quadlist() { init(); }
        ~Quadlist() { 
            clear();
            delete header;
            delete trailer;
        }
        int size() const { return _size; }
        bool empty() const { return _size == 0; }
        QlistNodePosi(T) first() const { return header->succ; }
        QlistNodePosi(T) last() const { return trailer->pred; }
        bool valid(QlistNodePosi(T) p) { return p && (trailer != p) && (header != p); }
        T remove(QlistNodePosi(T) p);//删除p位置 返回数值
        //将e做p的后继 b的上邻插入
        QlistNodePosi(T) insertAfterAbove(const T &e, QlistNodePosi(T) p, QlistNodePosi(T) b = nullptr);
        template <typename VST>
        void traverse(VST &);
};

template <typename K, typename V>
class Skiplist : public Dictionary<K, V>, public List<Quadlist<Entry<K, V>> *>
{
    protected:
        bool skipSearch(ListNode<Quadlist<Entry<K, V>> *> *&qlist, QuadlistNode<Entry<K, V>> *&p, K &k);
    public:
        int size() const { return List<Quadlist<Entry<K, V>> *>::empty() ? 0 : List<Quadlist<Entry<K, V>> *>::last()->data->size(); }
        int level() { return List<Quadlist<Entry<K, V>> *>::size(); }
        bool put(K, V);
        V *get(K k);
        bool remove(K k);
};
template <typename T>
void Quadlist<T>::init()
{
    header = new QuadlistNode<T>;
    trailer = new QuadlistNode<T>;
    header->succ = trailer;
    header->pred = nullptr;
    trailer->pred = header;
    trailer->succ = nullptr;
    header->above = trailer->above = nullptr;
    header->below = trailer->below = nullptr;
    _size = 0;
}
template <typename K, typename V>
V *Skiplist<K, V>::get(K k)
{
    if(List<Quadlist<Entry<K, V>> *>::empty())
        return nullptr;
    ListNode<Quadlist<Entry<K, V>> *> *qlist = List<Quadlist<Entry<K, V>> *>::first();
    QuadlistNode<Entry<K, V>> *p = qlist->data->first();
    return skipSearch(qlist, p, k) ? &(p->entry.value) : nullptr;
}
template <typename K, typename V>
bool Skiplist<K, V>::skipSearch(ListNode<Quadlist<Entry<K, V>> *> *&qlist, QuadlistNode<Entry<K, V>> *&p, K &k)
{
    while(1){
        while (p->succ && (p->entry.key <= k))
            p = p->succ;
        p = p->pred;
        if (p->pred && (k == p->entry.key))
            return true;
        qlist = qlist->succ;
        if (!qlist->succ)
            return false;
        p = (p->pred) ? p->below : qlist->data->first();
    }
}
template <typename K, typename V>
bool Skiplist<K, V>::put(K k, V v)
{
    Entry<K, V> e = Entry<K, V>(k, v);
    if(List<Quadlist<Entry<K, V>> *>::empty())
        List<Quadlist<Entry<K, V>> *>::insertAsFirst(new Quadlist<Entry<K, V>>);
    ListNode<Quadlist<Entry<K, V>> *> *qlist = List<Quadlist<Entry<K, V>> *>::first();
    QuadlistNode<Entry<K, V>> *p = qlist->data->first();
    if (skipSearch(qlist, p, k)){
        while(p->below)
            p = p->below;
    }
    qlist = List<Quadlist<Entry<K, V>> *>::last();
    QuadlistNode<Entry<K, V>> *b = qlist->data->insertAfterAbove(e, p);
    while (rand() & 1){
        while (qlist->data->valid(p) && !p->above)
            p = p->pred;
        if (!qlist->data->valid(p)){
            if (qlist == List<Quadlist<Entry<K, V>> *>::first())
                List<Quadlist<Entry<K, V>> *>::insertAsFirst(new Quadlist<Entry<K, V>>);
            p = qlist->pred->data->first()->pred;
        }
        else
            p = p->above;
        qlist = qlist->pred;
        b = qlist->data->insertAfterAbove(e, p, b);
    }
    return true;
}
template <typename T>
QlistNodePosi(T) Quadlist<T>::insertAfterAbove(const T &e, QlistNodePosi(T) p, QlistNodePosi(T) b)
{
    _size++;
    return p->insertAsSuccAbove(e, b);
}
template <typename K, typename V>
bool Skiplist<K, V>::remove(K k)
{
    if(List<Quadlist<Entry<K, V>> *>::empty())
        return false;
    ListNode<Quadlist<Entry<K, V>> *> *qlist = List<Quadlist<Entry<K, V>> *>::first();
    QuadlistNode<Entry<K, V>> *p = qlist->data->first();
    if (!skipSearch(qlist, p, k))
        return false;
    do{
        QuadlistNode<Entry<K, V>> *lower = p->below;
        qlist->data->remove(p);
        p = lower;
        qlist = qlist->succ;
    } while (qlist->succ);
    while (!List<Quadlist<Entry<K, V>> *>::empty() && List<Quadlist<Entry<K, V>> *>::first()->data->empty())
        List<Quadlist<Entry<K, V>> *>::remove(List<Quadlist<Entry<K, V>> *>::first());
    return true;
}
template <typename T>
T Quadlist<T>::remove(QlistNodePosi(T) p)
{
    p->pred->succ = p->succ;
    p->succ->pred = p->pred;
    _size--;
    T e = p->entry;
    delete p;
    return e;
}
template <typename T>
int Quadlist<T>::clear()
{
    int oldSize = _size;
    while (0 < _size)
        remove(header->succ);
    return oldSize;
}
