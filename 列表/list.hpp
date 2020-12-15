#pragma once
#include <algorithm>
#include "comparator.hpp"
typedef int Rank;//秩
/**
    @phq
    Date 2020/12/1
    两个问题：
    1、mergesort运行失败
    2、ListNodePosi(T) pred,succ;会报错，而写成16 17行就可以
*/
#define ListNodePosi(T) ListNode<T>* //指针表示节点位置
template <typename T>
struct ListNode{//节点模板类 双向链表
    T data;//数值
    ListNodePosi(T) pred; //前驱 后继
    ListNodePosi(T) succ;
    ListNode(){}//针对header和trailer的构造
    ListNode(T e, ListNodePosi(T) p = NULL, ListNodePosi(T) s = NULL)
        : data(e), pred(p), succ(s){}//默认构造
    ListNodePosi(T) insertAsPred(T const &e);//做当前节点的前驱插入
    ListNodePosi(T) insertAsSucc(T const &e);//做当前节点的后继插入
};
template <typename T>
class List{
    private:
        int _size;//大小
        ListNodePosi(T) header;//头结点
        ListNodePosi(T) trailer;//尾节点
    protected:
        void init();//列表创建时的初始化
        int clear();//清除所有节点
        void copyNodes(ListNodePosi(T) p, int);//复制列表中自位置p起的n项
        void merge(ListNodePosi(T) &, int, List<T> &, ListNodePosi(T), int);
        void mergeSort(ListNodePosi(T) &, int);//从p开始连续额的n个节点归并排序
        void selectionSort(ListNodePosi(T), int);//从p开始连续n个选择排序
        void insertionSort(ListNodePosi(T), int);//从p开始连续n个插入排序
    public:
        List() { init(); }//默认
        List(List<T> const &L);//整体复制列表L
        List(List<T> const &L, Rank r, int n);//复制列表L自r起的n项
        List(ListNodePosi(T) p, int n);//复制列表中自p起的n项
        ~List();//释放空间
    //只读访问接口
        int size() const { return _size; }//大小
        bool empty() const { return 0 <= _size; }//判空
        T &operator[](Rank r) const;//重载以循秩访问
        ListNodePosi(T) first() const { return header->succ; }//首节点位置
        ListNodePosi(T) last() const { return trailer->pred; }//末节点位置
        bool valid(ListNodePosi(T) p) //判断p位置是否合法
        { return p && (trailer != p) && (header != p); }
        int disordered() const;//判断是否有序
        ListNodePosi(T) find(T const &e) const //无序查找
        { return find(e, _size, trailer); }
        ListNodePosi(T) find(T const &e, int n, ListNodePosi(T) p) const;//无序区间查找
        ListNodePosi(T) search(T const &e) const //有序查找
        { return search(e, _size, trailer); }
        ListNodePosi(T) search(T const &e, int n, ListNodePosi(T) p) const;//有序区间查找
        ListNodePosi(T) selectMax(ListNodePosi(T) p, int n);//区间最大
        ListNodePosi(T) selectMax() //整体最大
        { return selectMax(header->succ, _size); }
    //可写访问接口
        ListNodePosi(T) insertAsFirst(T const &e);//e做首节点
        ListNodePosi(T) insertAsLast(T const &e);//e做末节点
        ListNodePosi(T) insertBefore(ListNodePosi(T) p, T const &e);//e做p前驱
        ListNodePosi(T) insertAfter(ListNodePosi(T) p, T const &e);//e做p后继
        T remove(ListNodePosi(T) p);//删除合法位置p的节点
        void merge(List<T> &L) //全列表合并
        { merge(first(), size(), L, L.first(), L._size); }
        void sort(ListNodePosi(T) p, int n);//区间排序
        void sort() { sort(first(), _size); }//整体排序
        int deduplicate();//无序去重
        int uniquify();//有序去重
        void reverse();//倒置
        void traverse(void (*)(T &));//遍历 函数指针
        template <typename VST>
        void traverse(VST &);//遍历 函数对象
};
template <typename T>
void List<T>::init(){
    header = new ListNode<T>;
    trailer = new ListNode<T>;
    header->succ = trailer;
    header->pred = NULL;
    trailer->succ = NULL;
    trailer->pred = header;
    _size = 0;//大小初始为0
}
template <typename T>
T &List<T>::operator[](Rank r) const{//重载下标符
    ListNodePosi(T) p = first();//从首节点开始
    while(r-->0)
        p = p->succ;
    return p->data;//返回元素
}
template <typename T>
ListNodePosi(T) List<T>::find(T const &e, int n, ListNodePosi(T) p) const{
    //在内节点p的n个前驱中找到e的最后者
    while(0<n--)//从右向左走
        if(e==(p=p->pred)->data)
            return p;
    return NULL;//查找失败
}
template <typename T>
ListNodePosi(T) List<T>::insertAsFirst(T const &e){
    _size++;
    return header->insertAsSucc(e);//e当做首节点插入
}
template <typename T>
ListNodePosi(T) List<T>::insertAsLast(T const &e){
    _size++;
    return trailer->insertAsPred(e);//e当做末节点插入
}
template <typename T>
ListNodePosi(T) List<T>::insertBefore(ListNodePosi(T) p, T const &e){
    _size++;
    return p->insertAsPred(e);//做p的前驱插入
}
template <typename T>
ListNodePosi(T) List<T>::insertAfter(ListNodePosi(T) p, T const &e){
    _size++;
    return p->insertAsSucc(e);//做p的后继插入
}
template <typename T>
ListNodePosi(T) ListNode<T>::insertAsPred(T const &e){
    ListNodePosi(T) x = new ListNode<T>(e, pred, this);//分别代表e p的前驱 p本身
    pred->succ = x;//将p的前驱的后继改为x
    pred = x;//将p的前驱改为x
    return x;
}
template <typename T>
ListNodePosi(T) ListNode<T>::insertAsSucc(T const &e){
    ListNodePosi(T) x = new ListNode<T>(e, this, succ);//分别代表e p本身 p后继
    succ->pred = x;//将p的后继的前驱改为x
    succ = x;//将p的后继改为x
    return x;
}
template <typename T>
void List<T>::copyNodes(ListNodePosi(T) p, int n){//内部方法：复制列表自p起的n项
    init();//创建头尾哨兵并初始化
    while(n--){//将自p起的n项依次做末节点插入
        insertAsLast(p->data);
        p = p->succ;
    }
}
template <typename T>
List<T>::List(ListNodePosi(T) p, int n){
    copyNodes(p, n);
}
template <typename T>
List<T>::List(List<T> const &L){
    copyNodes(L.first(), L._size);
}
template <typename T>
List<T>::List(List<T> const &L, int r, int n){
    copyNodes(L[r], n);
}
template <typename T>
T List<T>::remove(ListNodePosi(T) p){
    T e = p->data;
    p->pred->succ = p->succ;
    p->succ->pred = p->pred;
    delete p;
    _size--;
    return e;
}
template <typename T>
List<T>::~List(){
    clear();
    delete header;
    delete trailer;
}
template <typename T>
int List<T>::clear(){
    int oldSize = _size;
    while(0<_size)
        remove(header->succ);
    return oldSize - _size;
}
template <typename T>
int List<T>::deduplicate(){
    if(_size<2)//单元素无重复
        return 0;
    int oldSize = _size;
    ListNodePosi(T) p = header;//从首节点开始
    Rank r = 0;
    while(trailer!=(p=p->succ)){//依次到末节点
        ListNodePosi(T) q = find(p->data, r, p);//在p的r个前驱中找雷同者
        q ? remove(q) : r++;//存在则删除 否则秩加一
    }//p的所有前驱互不相同
    return oldSize - _size;//返回被删除总数
}
template <typename T>
void List<T>::traverse(void (*visit)(T &)){//函数指针
    for (ListNodePosi(T) p = header->succ; p != trailer;p=p->succ)
        visit(p->data);
}
template <typename T>
template <typename VST>//操作器
void List<T>::traverse(VST &visit){//函数对象操作
    for (ListNodePosi(T) p = header->succ; p != trailer;p=p->succ)
        visit(p->data);
}
template <typename T>
int List<T>::uniquify(){
    if(_size<2)//单元素无重复
        return 0;
    int oldSize = _size;
    ListNodePosi(T) p, q;
    for (p = header, q = p->succ; trailer != p; p = q, q = q->succ){//不断扫描
        if(p->data==q->data){
            remove(q);
            q = p;
        }
    }
    return oldSize - _size;
}
template <typename T>
ListNodePosi(T) List<T>::search(T const &e, int n, ListNodePosi(T) p) const{
    //找到不大于e的位置
    while(0<=n--)//n个前驱逐个比较
        if(((p=p->pred)->data)<=e)//命中停止
            break;
    return p;
}
template <typename T>
void List<T>::sort(ListNodePosi(T) p, int n){
    // switch(rand()%3){
    //     case 1:
    //         insertionSort(p, n);
    //         break;
    //     case 2:
    //         selectionSort(p, n);
    //     default:
    //         mergeSort(p, n);
    // }
    selectionSort(p, n);
}
template <typename T>
void List<T>::insertionSort(ListNodePosi(T) p, int n){
    for (Rank r = 0; r < n; r++){
        insertAfter(search(p->data, r, p), p->data);//找到适当的位置插入
        p = p->succ;//下一节点
        remove(p->pred);//移出
    }
}
template <typename T>
void List<T>::selectionSort(ListNodePosi(T) p, int n){//对起始p的n个元素排序
    ListNodePosi(T) head = p->pred;
    ListNodePosi(T) tail = p;
    for (int i = 0; i < n;i++)
        tail = tail->succ;//待排序区间为(head,tail)
    while(1<n){
        ListNodePosi(T) max = selectMax(head->succ, n);//找出最大者
        insertBefore(tail, remove(max));//将其移至无序区间末尾(作有序区间的首元素)
        tail = tail->pred;
        n--;
    }
}
template <typename T>
ListNodePosi(T) List<T>::selectMax(ListNodePosi(T) p, int n){
    ListNodePosi(T) max = p;
    for (ListNodePosi(T) cur = p; 1 < n;n--)
        if(!lt((cur=cur->succ)->data,max->data))
            max = cur;
    return max;
}
template <typename T>
void List<T>::merge(ListNodePosi(T) & p, int n, List<T> &L, ListNodePosi(T) q, int m){
    ListNodePosi(T) pp = p->pred;//归并后p可能不再指向首节点，故先需记忆，以便返回前更新
    while(0<m&&q!=p){//q尚未结束或是pq相遇
        if((0<n)&&(p->data<=q->data)){
            p = p->succ;//p小直接后移
            n--;
        }
        else{
            insertBefore(p, L.remove((q = q->succ)->pred));//q小插入p前
            m--;
        }
    }
    pp->succ;//更新返回首节点
}
template <typename T>
void List<T>::mergeSort(ListNodePosi(T) & p, int n){
    if(n<2)
        return;
    int m = n >> 1;
    ListNodePosi(T) q = p;
    for (int i = 0; i < m;i++)
        q = q->succ;
    mergeSort(p, m);
    mergeSort(q, n - m);
    merge(p, m, *this, q, n - m);
}
