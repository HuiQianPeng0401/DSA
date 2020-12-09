#pragma once
#include <iostream>
using namespace std;
#include <algorithm>
//#include "comparator.hpp"
#include "Stack.hpp"
#include "Queue.hpp"

#define BinNodePosi(T) BinNode<T>* //用指针来记录节点位置
#define stature(p) ((p) ? (p)->height : -1)//节点高度
//*****************************************************
//BinNode性质判断
#define IsRoot(x) (!((x).parent))//判断是否为根节点
#define IsLChild(x) (!IsRoot(x) && (&(x) == (x).parent->lChild))//x是否为左孩子
#define IsRChild(x) (!IsRoot(x) && (&(x) == (x).parent->rChild))//x是否为右孩子
#define HasParent(x) (!IsRoot(x))//x是否为根节点
#define HasLChild(x) ((x).lChild)//x有无左孩子
#define HasRChild(x) ((x).rChild)//x有无右孩子
#define HasChild(x) (HasLChild(x) || HasRChild(x))//x有无孩子
#define HasBothChild(x) (HasLChild(x) && HasRChild(x))//x有无两孩子
#define IsLeaf(x) (!HasChild(x))//x是否为叶子节点
//*****************************************************

//*****************************************************
//与BinNode有特定关系的节点及指针
#define sibling(p) ( \
    IsLChild(*(p)) ? \
        (p)->parent->rChild : \
        (p)->parent->lChild \
)//返回p的兄弟
#define uncle(p) ( \
    IsLChild(*((x)->parent)) ? \
        (x)->parent->parent->rChild : \
        (x)->parent->parent->lChild \
)//返回p的叔叔
#define FromParentTo(p) ( \
    IsRoot(p) ? this->_root : ( \
    IsLChild(p) ? (p).parent->lChild : (p).parent->rChild \
    ) \
)//返回来自父亲的指针
//*****************************************************

typedef enum
{
    RB_RED,//红色
    RB_BLACK//黑色
} RBColor;//节点颜色
template <typename T>
struct BinNode{//节点模板类
    T data;//数值
    BinNodePosi(T) parent;
    BinNodePosi(T) lChild;
    BinNodePosi(T) rChild;
    int height;//通用高度
    int npl;//Null Path Length （左式堆）
    RBColor color;//颜色 （红黑树）
//构造函数
    BinNode() : parent(NULL), lChild(NULL), rChild(NULL), height(0), npl(1), color(RB_RED){}
    BinNode(T e, BinNodePosi(T) p = NULL, BinNodePosi(T) lc = NULL, BinNodePosi(T) rc = NULL,
            int h = 0, int l = 1, RBColor c = RB_RED)
        : data(e), parent(p), lChild(lc), rChild(rc), height(h), npl(l), color(c){}
//操作接口
    int size();//统计当前节点后代的总数，即以此节点为根的子树的规模
    BinNodePosi(T) insertAsLC(T const &);//作当前节点左孩子插入新节点
    BinNodePosi(T) insertAsRC(T const &);//作当前节点右孩子插入新节点
    BinNodePosi(T) succ();//当前节点的直接后继
    template <typename VST>
    void travLevel(VST &);//子树层序遍历
    template <typename VST>
    void travPre(VST &);//子树先序遍历
    template <typename VST>
    void travIn(VST &);//子树中序遍历
    template <typename VST>
    void travPost(VST &);//子树后序遍历
    bool operator<(BinNode const &bn){
        return data < bn.data;
    }
    bool operator==(BinNode const &bn){
        return data == bn.data;
    }
};
template <typename T>
int BinNode<T>::size(){
    int l = 0, r = 0;
    if(lChild)
        l = lChild->size();
    if(rChild)
        r = rChild->size();
    return l + r + 1;
}
template <typename T>
BinNodePosi(T) BinNode<T>::succ(){
    BinNodePosi(T) s = this;
    /*有右孩子，则其直接后继必然存在，且属于其右子树
    此时只需转入右子树，再沿该子树的最左侧通路朝左下方深入
    直到抵达子树中最靠左的节点*/
    if(rChild){//有右孩子，则其后继必在右子树中
        s = rChild;
        while(HasLChild(*s))
            s = s->lChild;//右子树中最靠左的节点
    }
    /*若当前节点没有右子树，则若其直接后继存在，必为该节点的某一祖先
    且是将当前节点纳入其左子树的最低祖先 于是首先沿右侧通路朝左上方上升
    不能继续前进时，再朝右上方移动一步即可*/
    else{//否则若存在后继，应是”将当前节点包含于其左子树中的最低祖先“
        while(IsRChild(*s))//若节点是右孩子，则其必在当前节点之前访问过了
            s = s->parent;//朝左上移动直到找到左节点
        s = s->parent;//此左节点的父亲就是要找的后继
    }
    return s;
}
template <typename T>
BinNodePosi(T) BinNode<T>::insertAsLC(T const &e){
    return lChild = new BinNode(e, this);
}
template <typename T>
BinNodePosi(T) BinNode<T>::insertAsRC(T const &e){
    return rChild = new BinNode(e, this);
}
template <typename T>
template <typename VST>
void BinNode<T>::travPre(VST &visit){
    switch(rand()%2){
        case 1:
            travPre_I(this, visit);
            break;
        default:
            travPre_R(this, visit);
            break;
    }
}
template <typename T>
template <typename VST>
void BinNode<T>::travPost(VST &visit){
    switch(rand()%2){
        case 1:
            travPost_I(this, visit);
            break;
        default:
            travPost_R(this, visit);
            break;
    }
}
template <typename T>
template <typename VST>
void BinNode<T>::travIn(VST &visit){
    switch(rand()%2){
        case 1:
            travIn_I(this, visit);
            break;
        default:
            travIn_R(this, visit);
            break;
    }
}
template <typename T>
template <typename VST>
void BinNode<T>::travLevel(VST &visit){
    Queue<BinNodePosi(T)> Q;
    Q.push(this);
    while(!Q.empty()){
        BinNodePosi(T) x = Q.pop();
        visit(x->data);
        if(HasLChild(*x))
            Q.push(x->lChild);
        if(HasRChild(*x))
            Q.push(x->rChild);
    }
}
template <typename T>
class BinTree{
    protected:
        int _size;//规模大小
        BinNodePosi(T) _root;//根节点
        virtual int updateHeight(BinNodePosi(T) x);//更新x的高度
        void updateHeightAbove(BinNodePosi(T) x);//更新x及其祖先的高度
    public:
        BinTree() : _size(0), _root(NULL){}
        ~BinTree(){
            if(0<_size)
                remove(_root);
        }
        int size() const { return _size; }//返回规模大小
        bool empty() const { return !_root; }//判空
        BinNodePosi(T) root() { return _root; }//返回树根
        BinNodePosi(T) insertAsRoot(T const &e);//e作为根节点插入
        BinNodePosi(T) insertAsLC(BinNodePosi(T) x, T const &e);//e作为x的左孩子插入
        BinNodePosi(T) insertAsRC(BinNodePosi(T) x, T const &e);//e做为x的右孩子插入
        BinNodePosi(T) attachAsLC(BinNodePosi(T) x, BinTree<T> *&S);//T作为x左子树插入
        BinNodePosi(T) attachAsRC(BinNodePosi(T) x, BinTree<T> *&S);//T作为x右子树插入
        int remove(BinNodePosi(T) x);//删除以位置x处节点为根的子树 返回该子树原先的规模
        BinTree<T> *secede(BinNodePosi(T) x);//将子树x从树中删除 将其转为一颗独立子树
        //以VST为操作器
        template <typename VST>
        void travLevel(VST &visit){
            if(_root)
                _root->travLevel(visit);
        }
        template <typename VST>
        void travPre(VST &visit){
            if(_root)
                _root->travPre(visit);
        }
        template <typename VST>
        void travIn(VST &visit){
            if(_root)
                _root->travIn(visit);
        }
        template <typename VST>
        void travPost(VST &visit){
            if(_root)
                _root->travPost(visit);
        }
        // bool operator<(BinTree<T> const &t){
        //     return _root && t._root && lt(_root, t._root);
        // }
        // bool operator==(BinTree<T> const &t){
        //     return _root && t._root && (_root == t._root);
        // }
};
template <typename T>
int BinTree<T>::updateHeight(BinNodePosi(T) x){//更新节点x的高度
    //选左右子树较高者
    return x->height = 1 + max(stature(x->lChild), stature(x->rChild));
}
template <typename T>
void BinTree<T>::updateHeightAbove(BinNodePosi(T) x){//更新x及其祖先的高度
    while(x){//不断延伸
        updateHeight(x);
        x = x->parent;
    }
}
template <typename T>
BinNodePosi(T) BinTree<T>::insertAsRoot(T const &e){
    _size = 1;
    return _root = new BinNode<T>(e);//将e当做根节点插入
}
template <typename T>
BinNodePosi(T) BinTree<T>::insertAsLC(BinNodePosi(T) x, T const &e){
    _size++;//规模加一
    x->insertAsLC(e);//e做x左孩子插入
    updateHeightAbove(x);//更新高度
    return x->lChild;//返回
}
template <typename T>
BinNodePosi(T) BinTree<T>::insertAsRC(BinNodePosi(T) x, T const &e){
    _size++;//规模加一
    x->insertAsRC(e);//e做x右孩子插入
    updateHeightAbove(x);//更新高度
    return x->rChild;//返回
}
template <typename T>//S做x左子树接入 S本身置空
BinNodePosi(T) BinTree<T>::attachAsLC(BinNodePosi(T) x, BinTree<T> *&S){
    if(x->lChild=S->_root)//若S不为空将其接入
        x->lChild->parent = x;//更新父节点
    _size += S->_size;//更新规模
    updateHeightAbove(x);//更新高度
    S->_root = NULL;//清除根节点
    S->_size = 0;//更新S的规模
    release(S);//释放S
    S = NULL;//S置空
    return x;//返回
}
template <typename T>//S做x右子树接入 S本身置空
BinNodePosi(T) BinTree<T>::attachAsRC(BinNodePosi(T) x, BinTree<T> *&S){
    if(x->rChild=S->_root)//若S不为空将其接入
        x->rChild->parent = x;//更新父节点
    _size += S->_size;//规模
    updateHeightAbove(x);//高度
    S->_root = NULL;
    S->_size = 0;
    release(S);
    S = NULL;
    return x;
}
template <typename T>
static int removeAt(BinNodePosi(T) x){
    if(!x)//递归基：空树为0
        return 0;
    int n = 1 + removeAt(x->lChild) + removeAt(x->rChild);//递归的释放左右子树
    //release(x->data);//释放
    //release(x);//释放被摘除节点
    return n;//返回删除总数
}
template <typename T>
int BinTree<T>::remove(BinNodePosi(T) x){
    FromParentTo(*x) = NULL;//切断来自x父亲的指针
    updateHeightAbove(x->parent);//更新x父亲的高度
    int n = removeAt(x);//删除子树x
    _size -= n;//更新规模
    return n;//返回删除节点的总数
}
template <typename T>//将子树x从当前树中摘除 并将其封装为一颗独立子树
BinTree<T> *BinTree<T>::secede(BinNodePosi(T) x){
    FromParentTo(*x) = NULL;//切断来自父亲的指针
    updateHeightAbove(x->parent);//更新其父亲的高度
    BinTree<T> *S = new BinTree<T>;//创建新树
    S->_root = x;//S以x为根
    x->parent = NULL;//父节点为空
    S->_size = x->size();//更新S规模
    _size -= S->_size;//更新原树规模
    return S;
}
template <typename T, typename VST>
void travPre_R(BinNodePosi(T) x, VST &visit){
    if(!x)
        return;
    visit(x->data);
    travPre_R(x->lChild, visit);
    travPre_R(x->rChild, visit);
}
template <typename T, typename VST>
void travPost_R(BinNodePosi(T) x, VST &visit){
    if(!x)
        return;
    travPost_R(x->lChild, visit);
    travPost_R(x->rChild, visit);
    visit(x->data);
}
template <typename T, typename VST>
void travIn_R(BinNodePosi(T) x, VST &visit){
    if(!x)
        return;
    travIn_R(x->lChild, visit);
    visit(x->data);
    travIn_R(x->rChild, visit);
}
template <typename T, typename VST>
void travPre_I(BinNodePosi(T) x, VST &visit){
    Stack<BinNodePosi(T)> S;
    while(true){
        while(x){//从当前节点出发 逐批访问
            visit(x->data);//先序遍历先访问根节点
            S.push(x->rChild);//右孩子先入栈
            x = x->lChild;//向左延伸
        }
        if(S.empty())//栈空退出
            break;
        x = S.pop();//弹出下一批的起点
    }
}
template <typename T, typename VST>
void travIn_I(BinNodePosi(T) x, VST &visit){
    Stack<BinNodePosi(T)> S;
    while(true){
        while(x){//从当前节点出发沿左分支不断深入
            S.push(x);
            x = x->lChild;//迭代直到无左孩子
        }
        if(S.empty())//节点处理完毕
            break;
        x = S.pop();//弹出节点
        visit(x->data);//访问
        x = x->rChild;//转向右子树
    }
}
template <typename T>
void gotoHLVFL(Stack<BinNodePosi(T)> &S){
    while(BinNodePosi(T) x=S.top()){
        if(HasLChild(*x)){
            if(HasRChild(*x))
                S.push(x->rChild);
            S.push(x->lChild);
        }
        else
            S.push(x->rChild);
    }
    S.pop();
}
template <typename T, typename VST>
void travPost_I(BinNodePosi(T) x, VST &visit){
    Stack<BinNodePosi(T)> S;
    if(x)
        S.push(x);//根节点入栈
    while(!S.empty()){
        if(S.top()!=x->parent){//若栈顶非当前节点之父(则必为其右兄弟)此时应在其以右兄为根的子树中找到最先访问的叶节点(等效访问其右树)
            while(BinNodePosi(T) t=S.top()){//不断访问节点
                if(HasLChild(*t)){//尽可能向左
                    if(HasRChild(*t))//让右孩子先入栈
                        S.push(t->rChild);
                    S.push(t->lChild);//左孩子后入栈
                }
                else{//无左孩子就让右孩子入栈
                    S.push(t->rChild);
                }
            }
            S.pop();//弹出空节点
        }
        x = S.pop();//弹出栈顶的节点
        visit(x->data);//访问
    }
}
// template <typename T, typename VST>
// void travPre_I1(BinNodePosi(T) x, VST &visit){
//     Stack<BinNodePosi(T)> S;
//     if(x)
//         S.push(x);
//     while(!S.empty()){
//         x = S.pop();
//         visit(x->data);
//         if(HasRChild(*x))
//             S.push(x->rChild);
//         if(HasLChild(*x))
//             S.push(x->lChild);
//     }
// }
