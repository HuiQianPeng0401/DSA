#include <iostream>
using namespace std;
/*
@彭慧乾
date 2020/9.21-9.2
*/
#define Rank int
#define ListNodePosi(T) ListNode<T>*
template <typename T> struct ListNode//双向链表
{
//成员
    T data;//数值
    ListNodePosi(T) pred;//前驱
    ListNodePosi(T) succ;//后继
//构造函数
    ListNode(){};//针对header和trailer的构造
    ListNode(T e, ListNodePosi(T) p = NULL, ListNodePosi(T) s = NULL)
        : data(e), pred(p), succ(s) {} //默认构造器
//操作接口
    ListNodePosi(T) insertAsPred(T const &e);//紧靠当前节点之前插入
    ListNodePosi(T) insertAsSucc(T const &e);//紧靠当前节点之后插入
};
template <typename T> class List
{
    private:
        int _size;//规模
        ListNodePosi(T) header, trailer;//头尾哨兵

    protected://内部函数
        void init();//初始化
        int clear();//清除所有节点
        void copyNodes(ListNodePosi(T), int);//复制列表中自位置p起的n项
        void merge(ListNodePosi(T) &, int, List<T> &, ListNodePosi(T), int);//有序列表区间归并
        void mergeSort(ListNodePosi(T), int);//对从p开始的连续n个节点归并排序
        void selectionSort(ListNodePosi(T), int);//对从p开始的连续n个节点选择排序
        void insertionSort(ListNodePosi(T), int);//对从p开始的连续n个节点插入排序

    public://构造函数 析构函数 只读函数
        List() { init(); }//默认构造
        List(List<T> const &L);//整体复制构造
        List(List<T> const &L, Rank r, int n);//复制区间构造
        List(ListNodePosi(T) p, int n);//复制列表自p起的n项
        ~List();//释放包含头尾哨兵的所有节点
    //只读访问接口
        Rank size() const
        {
            return _size;//规模
        }
        bool empty() const
        {
            return _size <= 0;//判空
        }
        T &operator[](Rank r) const;//重载以支持循秩访问
        ListNodePosi(T) first() const
        {
            return header->succ;//首节点位置
        }
        ListNodePosi(T) last() const
        {
            return trailer->pred;//末节点位置
        }
        bool valid(ListNodePosi(T) p)//判断位置p是否对外合法
        {
            return (p && (trailer != p) && (header != p));//将头尾节点等同于NULL
        }
        int disordered() const;//判断是否有序
        ListNodePosi(T) find(T const &e) const//无序列表查找
        {
            return find(e, _size, trailer);
        }
        ListNodePosi(T) find(T const &e, int n, ListNodePosi(T) p) const;
        ListNodePosi(T) search(T const &e) const//有序列表查找
        {
            return search(e, _size, trailer);
        }
        ListNodePosi(T) search(T const &e, int n, ListNodePosi(T) p) const;
        ListNodePosi(T) selectMax(ListNodePosi(T) p, int n);//在p及其前n-1个后继找最大
        ListNodePosi(T) selectMax()
        {
            return selectMax(header->succ, _size);//整体最大者
        }
    //可写访问接口
        ListNodePosi(T) insertAsFirst(T const &e);//将e作首节点插入
        ListNodePosi(T) insertAsLast(T const &e);//将e作末节点插入
        ListNodePosi(T) insertBefore(ListNodePosi(T) p, T const &e);//将e当做p前驱插入
        ListNodePosi(T) insertAfter(ListNodePosi(T) p, T const &e);//将e当做p的后继插入
        T remove(ListNodePosi(T) p);//删除合法位置p处的节点并返回
        void merge(List<T> &L)
        {
            merge(first(),_size,L,L.first(),L._size;//全列表归并
        }
        void sort(ListNodePosi(T) p, int n);//区间排序
        void sort()
        {
            sort(first(), _size);//整体排序
        }
        int deduplicate();//无序去重
        int uniquify();//有序去重
        void reverse();//前后倒置
    //遍历
        void traverse(void (*)(T &));
        template <typename VST>
        void traverse(VST &);
};
template <typename T>
void List<T>::init()
{
    header = new ListNode<T>;
    trailer = new ListNode<T>;
    header->succ = trailer;
    header->pred = NULL;
    trailer->pred = header;
    trailer->succ = NULL;
    _size = 0;
}
template <typename T>
T &List<T>::operator[](Rank r) const//重载下标操作符，通过秩访问节点
{
    ListNodePosi(T) p = first();
    while(0<r--)
        p = p->succ;
    return p->data;
}
template <typename T>
ListNodePosi(T) List<T>::find(T const &e, int n, ListNodePosi(T) p) const
{
    //在无序列表内节点p的n个前驱中找到等于e的最后者
    while(0<n--)
        if(e==(p->pred)->data)
            return p;
    return NULL;
}
template <typename T>
ListNodePosi(T) List<T>::insertAsFirst(T const &e)
{
    _size++;
    return header->insertAsSucc(e);//e当做首节点插入
}
template <typename T>
ListNodePosi(T) List<T>::insertAsLast(T const &e)
{
    _size++;
    return trailer->insertAsPred(e);//e当做末节点插入
}
template <typename T>
ListNodePosi(T) List<T>::insertBefore(ListNodePosi(T) p, T const &e)
{
    //e当做p的前驱插入
    _size++;
    return p->insertAsPred(e);
}
template <typename T>
ListNodePosi(T) List<T>::insertAfter(ListNodePosi(T) p, T const &e)
{
    //e当做p的后继插入
    _size;
    return p->insertAsSucc(e);
}
template <typename T>
ListNodePosi(T) ListNode<T>::insertAsPred(T const &e)
{
    ListNodePosi(T) x = new ListNode(e, this->pred, this);//创建新节点
    this->pred->succ = x;
    this->pred = x;
}
template <typename T>
ListNodePosi(T) ListNode<T>::insertAsSucc(T const &e)
{
    ListNodePosi(T) x = new ListNode(e, this, this->succ);
    this->succ->pred = x;
    this->succ = x;
}
template <typename T>
void List<T>::copyNodes(ListNodePosi(T) p, int n)//复制列表中自位置p起的n项
{
    init();//创建头尾哨兵并做初始化
    while(n--)
    {
        insertAsLast(p->data);//依次当做末节点插入
        p = p->succ;
    }
}
template <typename T>
List<T>::List(ListNodePosi(T) p,int n)
{
    copyNodes(p, n);
}
template <typename T>
List<T>::List(List<T> const &L)
{
    copyNodes(L.first(), L._size);//整体复制
}
template <typename T>
List<T>::List(List<T> const &L, int r, int n)
{
    copyNodes(L[r], n);//复制L中自第r项起的n项
}
template <typename T>
T List<T>::remove(ListNodePosi(T) p)//删除合法位置p处节点
{
    T e = p->data;
    p->pred->succ = p->succ;
    p->succ->pred = p->pred;
    delete p;
    _size--;
    return e;
}
template <typename T>
List<T>::~List()
{
    int oldsize = _size;
    while(0<_size)
        remove(header->succ);
    return oldsize;
}
template <typename T>
int List<T>::deduplicate()
{
    if(_size<2)
        return 0;
    int oldSize = _size;
    ListNodePosi(T) p = header;
    Rank r = 0;//从首节点开始
    while(trailer!=(p=p->succ))//依次到末节点
    {
        ListNodePosi(T) q = find(p->data, r, p);//在p的r个前驱中查找雷同者
        q ? remove(q) : r++;//若存在则删除，否则个数加一
    }
    return oldSize - _size;//返回删除个数
}
template <typename T>
void List<T>::traverse(void (*visit)(T &))//函数指针
{
    for (ListNodePosi(T) p = header->succ; p != trailer;p=p->succ)
        visit(p->data);
}
template <typename T>//元素类型
template <typename VST>//操作器
void List<T>::traverse(VST &visit)
{
    for (ListNodePosi(T) p = header->succ; p != trailer;p=p->succ)
        visit(p->data);
}
template <typename T>
int List<T>::uniquify()
{
    if(_size<2)
        return 0;
    int oldSize = _size;
    ListNodePosi(T) p;
    ListNodePosi(T) q;//依次指向紧邻的各对节点
    for (p = header, q = p->succ; trailer != q; p = q,q=q->succ)
    {
        if(p->data==q->data)
        {
            remove(q);
            q = p;
        }
    }
    return oldSize - _size;
}
template <typename T>
ListNodePosi(T) List<T>::search(T const &e, int n, ListNodePosi(T) p) const
{
    //在有序列表节点p的n个前驱中找不大于e的最后者
    while(0<=n--)
        if(((p=p->pred)->data)<=e)
            break;
    return p;
}
template <typename T>
void List<T>::sort(ListNodePosi(T) p, int n)//列表区间排序
{
    switch(rand()%3)
    {
        case 1:
            insertionSort(p, n);
            break;
        case 2:
            selectionSort(p, n);
            break;
        default:
            mergeSort(p, n);
            break;
    }
}
template <typename T>
void List<T>::insertionSort(ListNodePosi(T) p, int n)
{
    //对起始位置p的n个元素排序
    for (int r = 0; r < n;r++)
    {
        insertAfter(search(p->data, r, p), p->data);
        p = p->succ;
        remove(p->pred);
    }
}
template <typename T>
void List<T>::selectionSort(ListNodePosi(T) p,int n)
{
    ListNodePosi(T) head = p->pred;
    ListNodePosi(T) tail = p;
    for (int i = 0; i < n;i++)
        tail = tail->succ;//待排序区间为(head,tail)
    while(1<n)//至少还有两个节点
    {
        ListNodePosi(T) maxt = selectMax(head->succ, n);//找出最大者
        insertBefore(tail, remove(maxt));//移至无序区间末尾
        tail = tail->pred;
        n--;
    }
}
template <typename T>
ListNodePosi(T) List<T>::selectMax(ListNodePosi(T) p, int n)
{
    ListNodePosi(T) maxt = p;
    for (ListNodePosi(T) cur = p; 1 < n;n--)
    {
        
    }
}
template <typename T>
void List<T>::merge(ListNodePosi(T) & p, int n, List<T> &L, ListNodePosi(T) q, int m)
{
    //合并链表
    ListNodePosi(T) pp = p->pred;
    while(0<m)//在q尚未移出区间
    {
        if((0<n)&&(p->data<=q->data))
        {
            if(q==(p=p->succ))
                break;
            n--;
        }
        else
        {
            insertBefore(p, L.remove((q = q->succ)->pred));
            m--;
        }
    }
    p = pp->succ;//确定新起点
}
