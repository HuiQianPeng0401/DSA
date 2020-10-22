#include <iostream>
using namespace std;
/*
@彭慧乾
date 2020/9.13-9.21
*/

typedef int Rank;//秩
#define DEFAULT_CAPACITY 3//默认初始容量
template <typename T> class Vector
{
    public:
        Rank _size;//规模
        int _capacity;//容量
        T *_elem;//数据区
        void copyFrom(T const *A, Rank lo, Rank hi);//赋值数组区间
        void expend();//空间不足时扩容
        void shrink();//装填因子过小时压缩
        bool bubble(Rank lo, Rank hi);//扫描交换
        void bubbleSort(Rank lo, Rank hi);//冒泡排序
        Rank Max(Rank lo, Rank hi);//获得最大值
        void selectionSort(Rank lo, Rank hi);//选择排序算法
        void merge(Rank lo, Rank mid, Rank hi); //归并算法
        void mergeSort(Rank lo, Rank hi);//归并排序
        Rank partition(Rank lo, Rank hi);//轴点构造
        void quickSort(Rank lo, Rank hi);//快速排序
        void heapSort(Rank lo, Rank hi);//堆排序
        //构造函数
        Vector(int c = DEFAULT_CAPACITY, int s = 0, T v = 0)//容量为c，规模为s，所有元素初始为v
        {
            _elem = new T[_capacity = c];
            for (_size = 0; _size < s; _elem[_size++]=v);
        }
        Vector(T const *A, Rank lo, Rank hi)
        {
            copyFrom(A, lo, hi);//向量区间复制
        }
        Vector(T const *A, Rank n)
        {
            copyFrom(A, 0, n);
        }
        Vector(Vector<T> const &V, Rank lo, Rank hi)
        {
            copyFrom(V._elem, lo, hi);//向量区间复制
        }
        Vector(Vector<T> const &V)
        {
            copyFrom(V._elem, 0, V._size);//向量整体复制
        }
        //析构函数
        ~Vector()
        {
            delete[] _elem;//空间释放
        }
        //只读访问接口
        Rank size() const
        {
            return _size;//规模
        }
        bool empty() const
        {
            return !_size;//判断空
        }
        int disordered() const;
        Rank find(T const& e)const
        {
            return find(e, 0, _size);//无序向量整体查找
        }
        Rank find(T const &e, Rank lo, Rank hi) const;//无序向量区间查找
        Rank search(T const &e) const
        {
            return (_size <= 0) ? -1 : search(e, 0, _size);
        }
        Rank search(T const &e, Rank lo, Rank hi) const;
        //可写访问接口
        T &operator[](Rank r) const;//重载下标操作符，可以类似数组引用
        Vector<T> &operator=(Vector<T> const &);//重载赋值操作符,以便直接克隆向量
        T remove(Rank r); //删除秩为r癿元素并返回
        int remove(Rank lo, Rank hi); //删除秩在区间[lo, hi)的元素
        Rank insert(Rank r, T const &e);//插入元素
        Rank insert(T const &e)
        {
            return insert(_size, e);//默认作为末元素插入
        }
        void sort(Rank lo, Rank hi);//对[lo,hi]排序
        void sort()
        {
            sort(0, _size);
        }
        void unsort(Rank lo, Rank hi);//置乱
        void unsort()
        {
            unsort(0, _size);
        }
        int deduplicate();//无序去重
        int uniquify();//有序去重
        //遍历
        void traverse(void (*)(T &));//使用函数指针遍历，只读或局部性修改
        template <typename VST>
        void traverse(VST &);//使用函数对象，可全局性修改
};
template <typename T>
void Vector<T>::copyFrom(T const *A, Rank lo, Rank hi)
{
    _elem = new T[_capacity = (hi - lo) << 1];//分配空间
    _size = 0;//规模清零
    while(lo<hi)
        _elem[_size++] = A[lo++];
}
template <typename T>
Vector<T> &Vector<T>::operator=(Vector<T> const &V)
{
    if(_elem)
        delete[] _elem;//释放原有内容
    copyFrom(V._elem, 0, V.size);//整体复制
    return *this;//返回当前对象的引用，以便链式赋值
}
template <typename T>
void Vector<T>::expend()
{
    if(_size<_capacity)
        return;
    if(_capacity<DEFAULT_CAPACITY)
        _capacity = DEFAULT_CAPACITY;//不低于最小容量
    T *oldElem = _elem;
    _elem = new T[_capacity <<= 1];//容量加倍
    for (int i = 0; i < _size;i++)
        _elem[i] = oldElem[i];
    delete[] oldElem;
}
template <typename T>
void Vector<T>::shrink()//装填因子过小时压缩空间
{
    if (_capacity < DEFAULT_CAPACITY << 1)//控制在DEF以下
        return;
    if (_size << 2 > _capacity)//25%为界
        return;
    T *oldElem = _elem;
    _elem = new T[_capacity >>= 1];
    for (int i = 0; i < _size;i++)
        _elem[i] = oldElem[i];
    delete[] oldElem;
}
template <typename T>
T &Vector<T>::operator[](Rank r) const
{
    return _elem[r];
}
template <typename T>
void permute(Vector<T> &V)//随机置乱
{
    for (int i = V.size(); i > 0;i--)
        swap(V[i - 1], V[rand() % i]);
}
template <typename T>
void Vector<T>::unsort(Rank lo, Rank hi)//区间置乱
{
    T *V = _elem + lo;//将子向量_elem[lo,hi)视作另一向量V[0,hi-lo)
    for (Rank i = hi - lo; i > 0;i--)
        swap(V[i - 1], V[rand() % i]);
}
template <typename T>
Rank Vector<T>::find(T const &e, Rank lo, Rank hi) const
{
    while((lo<hi--)&&(e!=_elem[hi]));
    return hi;//若hi<lo则查找失败 否则返回hi即其位置
}
template <typename T>
Rank Vector<T>::insert(Rank r, T const &e)
{
    expend();
    for (int i = _size; i > r;i--)
        _elem[i] = _elem[i - 1];
    _elem[r] = e;
    return r;
}
template <typename T>
int Vector<T>::remove(Rank lo, Rank hi)//返回删除个数
{
    if(lo==hi)
        return 0;
    while(hi<_size)
        _elem[lo++] = _elem[hi++];//[hi,_size)顺次前移hi-lo单元
    _size = lo;//更新规模
    shrink();
    return hi - lo;//返回删除个数
}
template <typename T>
int Vector<T>::deduplicate()//删除无序向量中的重复元素
{
    int oldSize = _size;//记录园规模
    Rank i = 1;//从_elem[1]开始
    while(i<_size)//逐一检查
        (find(_elem[i], 0, i) < 0) ? i++ : remove(i);
    return oldSize - _size;
}
template <typename T>
void Vector<T>::traverse(void (*visit)(T &))
{
    for (int i = 0; i < _size;i++)
        visit(_elem[i]);
}
template <typename T>
template <typename VST>
void Vector<T>::traverse(VST &visit)
{
    for (int i = 0; i < _size;i++)
        visit(_elem[i]);
}
template <typename T>
int Vector<T>::disordered() const//返回向量中逆序相邻元素对的总数
{
    int n = 0;//计数器
    for (int i = 1; i < _size;i++)//逐一检查_size-1对相邻元素
        if(_elem[i-1]>_elem[i])
            n++;
    return n;
}
template <typename T>
int Vector<T>::uniquify()//有序向量剔除重复元素
{
    Rank i = 0, j = 0;
    while(++j<_size)//逐一扫描到末元素
    {
        if(_elem[j]!=_elem[i])//跳过雷同者
            _elem[++i] = _elem[j];
    }
    _size = ++i;
    shrink();
    return j = i;//返回删除元素总数
}
template <typename T>
static Rank binSearch(T *A, T const &e, Rank lo, Rank hi)
{
    while(lo<hi)
    {
        Rank mid = (lo + hi) >> 1;
        (e < A[mid]) ? hi = mid : lo = mid + 1;
    }
    return --lo;
}
template <typename T>
static Rank fibSearch(T *A, T const &e, Rank lo, Rank hi)
{
    return 0;
}
template <typename T>
Rank Vector<T>::search(T const &e, Rank lo, Rank hi) const
{
    return (rand() % 2) ? binSearch(_elem, e, lo, hi) : fibSearch(_elem, e, lo, hi);
}
template <typename T>
bool Vector<T>::bubble(Rank lo, Rank hi)
{
    bool sorted = true;//整体有序标志
    while(++lo<hi)
    {
        if(_elem[lo-1]>_elem[lo])
        {
            sorted = false;//若逆序则意味着整体未有序
            swap(_elem[lo - 1], _elem[lo]);
        }
    }
    return sorted;
}
template <typename T>
void Vector<T>::bubbleSort(Rank lo, Rank hi)
{
    while(!bubble(lo,hi--));
}
template <typename T>
void Vector<T>::mergeSort(Rank lo, Rank hi)
{
    if(hi-lo<2)//单元素区间自然有序
        return;
    int mid = (lo + hi) >> 1;
    mergeSort(lo, mid);
    mergeSort(mid, hi);
    merge(lo, mid, hi);//分别对前后半段排序 然后归并
}
template <typename T>
void Vector<T>::merge(Rank lo, Rank mid, Rank hi)
{
    T *A = _elem + lo;
    int lb = mid - lo;
    T *B = new T[lb];
    for (Rank i = 0; i < lb;B[i]=A[i++]);
    int lc = hi - mid;
    T *C = _elem + mid;
    for (Rank i = 0, j = 0, k = 0; (j < lb) || (k < lc);)
    {
        if((j<lb)&&(!(k<lc)||(B[j]<=C[k])))
            A[i++] = B[j++];
        if((k<lc)&&(!(j<lb)||(C[k]<B[j])))
            A[i++] = C[k++];
    }
    delete[] B;
}
template <typename T>
void Vector<T>::sort(Rank lo, Rank hi)
{
    switch(rand()%5)
    {
        case 1:
            bubbleSort(lo, hi);
            break;
        case 2:
            selectionSort(lo, hi);
            break;
        case 3:
            mergeSort(lo, hi);
            break;
        case 4:
            heapSort(lo, hi);
            break;
        default:
            quickSort(lo, hi);
            break;
    }
}
