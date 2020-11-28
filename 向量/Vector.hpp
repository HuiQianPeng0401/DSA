#pragma once
#include "comparator.hpp"
#include <iostream>
using namespace std;
#include <algorithm>

typedef int Rank;//循秩访问
#define DEFAULT_CAPACITY 3//默认初始容量
/**
 * @彭慧乾
 * Date 2020/11.27 11.28
*/

template <typename T>
class Vector{//向量模板类
    protected:
        Rank _size;//规模（大小）
        int _capacity;//容量
        T *_elem;//元素
        void copyFrom(T const *A, Rank lo, Rank hi);//区间复制
        void expand();//扩容
        void shrink();//缩容
        bool bubble(Rank lo, Rank hi);//扫描交换
        void bubbleSort(Rank lo, Rank hi);//冒泡排序
        Rank max(Rank lo, Rank hi);//选取最大元素
        void selectionSort(Rank lo, Rank hi);//选择排序
        void merge(Rank lo, Rank mid, Rank hi);//归并
        void mergeSort(Rank lo, Rank hi);//归并排序
        Rank partition(Rank lo, Rank hi);//轴点构造算法
        void quickSort(Rank lo, Rank hi);//快速排序
        void heapSort(Rank lo, Rank hi);//堆排序
    public:
        Vector(int c = DEFAULT_CAPACITY, int s = 0, T v = 0){
            //容量为c，规模为s，所有元素初始化为v s<=c
            _elem = new T[_capacity = c];
            for (_size = 0; _size < s;_size++)
                _elem[_size] = v;
        }
        Vector(T const *A, Rank lo, Rank hi){
            copyFrom(A, lo, hi);
        }//数组区间复制
        Vector(T const *A, Rank n){
            copyFrom(A, 0, n);
        }//数组整体复制
        Vector(Vector<T> const &V, Rank lo, Rank hi){
            copyFrom(V._elem, lo, hi);
        }//向量区间复制
        Vector(Vector<T> const &V){
            copyFrom(V._elem, 0, V._size);
        }//向量整体复制
        ~Vector() {delete[] _elem;}//释放空间
    //只写访问接口
        Rank size() const//大小
            {return _size;}
        bool empty() const //判空
            {return (_size == 0);}
        int disordered() const;//判断是否有序
        Rank find(T const &e) const//无序向量整体查找
            {return find(e, 0, _size);}
        Rank find(T const &e, Rank lo, Rank hi) const;//无序向量区间查找
        Rank search(T const &e) const{//有序向量整体查找
            return (0 >= _size) ? -1 : search(e, 0, _size);
        }
        Rank search(T const &e, Rank lo, Rank hi) const;//有序向量区间查找
    //可写访问接口
        T &operator[](Rank r) const;//重载下标符，以便访问
        Vector<T> &operator=(Vector<T> const &);//重载赋值运算
        T remove(Rank r);//删除秩为r的元素
        int remove(Rank lo, Rank hi);//区间删除
        Rank insert(Rank r, T const &e);//在r处插入
        Rank insert(T const &e){
            return insert(_size, e);
        }//默认作末元素插入
        void sort(Rank lo, Rank hi);//区间排序
        void sort(){
            sort(0, _size);
        }
        void unsort(Rank lo, Rank hi);//区间置乱
        void unsort()//整体置乱
            {unsort(0, _size);}
        int deduplicate();//无序去重
        int uniquify();//有序去重
    //遍历
        void traverse(void (*)(T &));//用函数指针遍历
        template <typename VST>
        void traverse(VST &);//使用函数对象遍历
};
template <typename T>
void Vector<T>::copyFrom(T const *A, Rank lo, Rank hi){
    _elem = new T[_capacity = 2 * (hi - lo)];//分配空间
    _size = 0;//规模清零
    while (lo<hi)
    {
        _elem[_size++] = A[lo++];//逐个赋值
    }
}
template <typename T>
Vector<T> &Vector<T>::operator=(Vector<T> const &V){
    if(_elem)
        delete[] _elem;//释放原有内容
    copyFrom(V._elem, 0, V.size());//整体复制
    return *this;//返回当前对象的引用，以便链式赋值
}
template <typename T>
void Vector<T>::expand(){
    if(_size<_capacity)//未满员不扩容
        return;
    if(_capacity<DEFAULT_CAPACITY)
        _capacity = DEFAULT_CAPACITY;//不低于最小容量
    T *oldElem = _elem;
    _elem = new T[_capacity <<= 1];//加倍扩容
    for (int i = 0; i < _size;i++)//复制原内容
        _elem[i] = oldElem[i];
    delete[] oldElem;//释放原空间
}
template <typename T>
void Vector<T>::shrink(){//装填因子过小时压缩空间
    if(_capacity<DEFAULT_CAPACITY << 1)//不致收缩到DEFAULT_CAPACITY以下
        return;
    if(_size << 2> _capacity)//以25%为界
        return;
    T *oldElem = _elem;
    _elem = new T[_capacity >>= 1];//容量减半
    for (int i = 0; i < _size;i++)
        _elem[i] = oldElem[i];
    delete[] oldElem;//释放原空间
}
template <typename T>
T &Vector<T>::operator[](Rank r) const{
    return _elem[r];
}
template <typename T>
void permute(Vector<T> &V){//随机置乱向量
    for (int i = V.size(); i > 0;i--)//自后向前
        swap(V[i - 1], V[rand() % i]);//V[i-1]与V[0,i)中某一元素互换
}
template <typename T>
void Vector<T>::unsort(Rank lo, Rank hi){//随机置乱向量区间
    T *V = _elem + lo;//利用指针操作
    for (Rank i = hi - lo; i > 0;i--)
        swap(V[i - 1], V[rand() % i]);//V[i-1]与V[0,i)中某一元素互换
}
template <typename T>
Rank Vector<T>::find(T const &e, Rank lo, Rank hi) const {//无序向量的顺序查找：返回最后一个e的位置；失败就返回lo-1
    while(lo<hi--){//从后往前
        if(e==_elem[hi])
            break;
    }
    return hi;//若hi<lo则失败 否则返回秩
}
template <typename T>
Rank Vector<T>::insert(Rank r, T const &e){
    expand();//若有必要则扩容
    for (int i = _size; i > r;i--)
        _elem[i] = _elem[i - 1];//依次后移
    _elem[r] = e;//放入e
    _size++;//大小加一
    return r;//返回秩
}
template <typename T>
int Vector<T>::remove(Rank lo, Rank hi){//删除区间[lo,hi)
    if(lo==hi)
        return 0;
    while(hi<_size)
        _elem[lo++] = _elem[hi++];//[hi,_size)依次前移hi-lo个单元
    _size = lo;//更新大小 丢弃[lo,_size=hi)区间
    shrink();//若有必要则缩容
    return hi - lo;//返回删除个数
}
template <typename T>
T Vector<T>::remove(Rank r){
    T e = _elem[r];//备份被删除元素
    remove(r, r + 1);//调用区间删除算法删除[r,r+1)  >笑脸<避免重复造轮子
    return e;
}
template <typename T>
int Vector<T>::deduplicate(){
    int oldSize = _size;
    Rank i = 1;//从_elem[1]开始 从0开始无意义
    while(i<_size){//自前向后逐一考察各元素
        (find(_elem[i], 0, i) < 0) ? i++ : remove(i);//在i前面找与之雷同 若无则继续考察 有就删除
    }
    return oldSize - _size;//返回被删除元素的个数
}
template <typename T>
void Vector<T>::traverse(void (*visit)(T &)){//利用函数指针机制
    for (int i = 0; i < _size;i++)
        visit(_elem[i]);
}
template <typename T>//元素类型
template <typename VST>//操作器
void Vector<T>::traverse(VST &visit){//利用函数对象机制的遍历
    for (int i = 0; i < _size;i++)
        visit(_elem[i]);
}
//以下 利用函数对象将作为参数的引用对象累加
template <typename T>
struct Increase{//函数对象：递增一个T类对象
    virtual void operator()(T &e){//假设T可直接递增或已重载++
        e++;
    }
};
template <typename T>
void increase(Vector<T> &V){
    V.traverse(Increase<T>());//已Increase<T>()为函数对象进行遍历
}
template <typename T>
int Vector<T>::disordered() const{//返回向量中逆序相邻元素对的总数
    int n = 0;//计数器
    for (int i = 1; i < _size;i++)
        if(_elem[i]<_elem[i-1])
            n++;
    return n;
}
/**
 * template <typename T>
 * int Vector<T>::uniquify(){ 有序向量重复元素剔除 低效O(n^2)
 *     int oldSize=_size;
 *     Rank i=1;
 *     while(i<_size){
 *         (_elem[i]==_elem[i-1])?remove(i):i++;
 *     }
 *     return oldSize-_size;
 * }
*/
template <typename T>
int Vector<T>::uniquify(){//有序向量重复元素剔除算法 高效O(n)
    Rank i = 0, j = 0;//各对互异“相邻”元素的秩
    while(++j<_size){
        if(_elem[i]!=_elem[j])//跳过雷同者
            _elem[++i] = _elem[j];//发现不同元素 向前移至紧邻于前者右侧
    }
    _size = ++i;//更新规模
    shrink();//缩容
    return j - i;//返回剔除的个数
}
template <typename T>//证明见P56页
static Rank binSearch(T *A, T const &e, Rank lo, Rank hi){//查找不大于e的最大元素
    while(lo<hi){//每步迭代仅做两个判断
        Rank mid = (lo + hi) >> 1;
        (e < A[mid]) ? hi = mid : lo = mid + 1;//[lo,mid)或(mid,hi)
    }//不能提前终止 必须等lo==hi
    return lo - 1;//循环结束时lo为大于e的最小秩 lo-1为不大于e的最大秩
}//有多个元素命中 返回秩最大者 查找失败时返回失败的位置
// template <typename T>
// static Rank fibSearch(T *A, T const &e, Rank lo, Rank hi){

// }
template <typename T>
Rank Vector<T>::search(T const &e, Rank lo, Rank hi) const{
    //return (rand() % 2) ? binSearch(_elem, e, lo, hi) : fibSearch(_elem, e, lo, hi);
    return binSearch(_elem, e, lo, hi);
}
template <typename T>
void Vector<T>::sort(Rank lo, Rank hi){
    // switch(rand()%5){
    //     case 1:
    //         bubbleSort(lo, hi);
    //         break;
    //     case 2:
    //         selectionSort(lo, hi);
    //         break;
    //     case 3:
    //         mergeSort(lo, hi);
    //         break;
    //     case 4:
    //         heapSort(lo, hi);
    //         break;
    //     default:
    //         quickSort(lo, hi);
    //         break;
    // }
    bubbleSort(lo, hi);
}
template <typename T>
void Vector<T>::bubbleSort(Rank lo, Rank hi){
    while(!bubble(lo,hi--));//逐趟做扫描交换直至全序
}
template <typename T>
bool Vector<T>::bubble(Rank lo, Rank hi){
    bool sorted = true;//整体有序标志
    while(++lo<hi)//自左向右检查
        if(_elem[lo-1]>_elem[lo]){
            sorted = false;//若存在逆序则不敢保证整体有序
            swap(_elem[lo - 1], _elem[lo]);
        }
    return sorted;//返回标志
}
template <typename T>
void Vector<T>::merge(Rank lo, Rank mid, Rank hi){//有序向量的归并
    //以mid为界、各自为序的子向量[lo,mid) [mid,hi)
    T *A = _elem + lo;//利用指针操作_elem
    int lb = mid - lo;//长度
    T *B = new T[lb];
    for (Rank i = 0; i < lb;i++)//复制前部分向量
        B[i] = A[i];
    int lc = hi - mid;//长度
    T *C = _elem + mid;//操作后部分
    for (Rank i = 0, j = 0, k = 0; (j < lb) || (k < lc);){//利用循环将B和C小者加入A
        if((j<lb)&&(!(k<lc)||(B[j]<=C[k])))
            A[i++] = B[j++];
        if((k<lc)&&(!(j<lb)||(C[k]<B[j])))
            A[i++] = C[k++];
    }
    delete[] B;
}
template <typename T>
void Vector<T>::mergeSort(Rank lo, Rank hi){//assert:lo>=0 hi<=size
    if(hi-lo<2)
        return;
    Rank mid = (lo + hi) >> 1;
    mergeSort(lo, mid);
    mergeSort(mid, hi);
    merge(lo, mid, hi);
}
