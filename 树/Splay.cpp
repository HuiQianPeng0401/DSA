template <typename T>
class Splay : public BST<T>{
    protected:
        BinNodePosi(T) splay(BinNodePosi(T) v);
    public:
        BinNodePosi(T) & search(const T &e);
        BinNodePosi(T) insert(const T &e);
        bool remove(const T &e);
};
template <typename NodePosi>
inline void attachAsLChild(NodePosi p, NodePosi lc){
    //在节点*p与*lc之间建立父子关系
    p->lChild = lc;
    if(lc)
        lc->parent = p;
}
template <typename NodePosi>
inline void attachAsRChild(NodePosi p, NodePosi rc){
    p->rChild = rc;
    if(rc)
        rc->parent = p;
}
template <typename T>
BinNodePosi(T) Splay<T>::splay(BinNodePosi(T) v){
    //从节点v逐层扩展
    if(!v)
        return nullptr;
    BinNodePosi(T) p, g;//父亲与祖父
    while((p=v->parent)&&(g=p->parent)){
        BinNodePosi(T) gg = g->parent;
        if(IsLChild(*v)){
            if(IsLChild(*p)){
                attachAsLChild(g, p->rChild);
                attachAsLChild(p, v->rChild);
                attachAsRChild(p, g);
                attachAsRChild(v, p);
            }
            else{
                attachAsLChild(p, v->rChild);
                attachAsRChild(g, v->lChild);
                attachAsLChild(v, g);
                attachAsRChild(v, p);
            }
        }
        else if(IsRChild(*p)){
            attachAsRChild(g, p->lChild);
            attachAsRChild(p, v->lChild);
            attachAsLChild(p, g);
            attachAsLChild(v, p);
        }
        else{
            attachAsRChild(p, v->lChild);
            attachAsLChild(g, v->rChild);
            attachAsRChild(v, g);
            attachAsLChild(v, p);
        }
        if(!gg)
            v->parent = nullptr;
        else
            (g == gg->lChild) ? attachAsLChild(gg, v) : attachAsRChild(gg, v);
        updateHeight(g);
        updateHeight(v);
        updateHeight(p);
    }
    //循环结束时g==null但未必p==null
    //此时再做一次单旋
    if(p=v->parent){
        if(IsLChild(*v)){
            attachAsLChild(p, v->rChild);
            attachAsRChild(v, p);
        }
        else{
            attachAsRChild(p, v->lChild);
            attachAsLChild(v, p);
        }
    }
    else
        v->parent = nullptr;
    return v;
}
template <typename T>
BinNodePosi(T) & Splay<T>::search(const T &e){
    //在伸展树中查找关键码e
    BinNodePosi(T) p = searchIn(_root,e, _hot = nullptr);
    _root = splay(p ? p : _hot);
    return _root;
}

template <typename T>
BinNodePosi(T) Splay<T>::insert(const T &e){
    if(!_root){
        _size++;
        return _root = new BinNode<T>(e);
    }
    if(e==search(e)->data)
        return _root;
    _size++;
    BinNodePosi(T) t = _root;
    if(_root->data<e){
        t->parent = _root = new BinNode<T>(e, nullptr, t, t->rChild);
        if(HasRChild(*t)){
            t->rChild->parent = _root;
            t->rChild = nullptr;
        }
    }
    else{
        t->parent = _root = new BinNode<T>(e, nullptr, t->lChild, t);
        if(HasLChild(*t)){
            t->lChild->parent = _root;
            t->lChild = nullptr;
        }
    }
    updateHeightAbove(t);
    return _root;
}
template <typename T>
bool Splay<T>::remove(const T &e){
    if(!_root||(e!=search(e)->data))
        return false;
    BinNodePosi(T) w = _root;//e经过search已经到树根
    if(!HasLChild(*_root)){
        _root = _root->rChild;
        if(_root)
            _root->parent = nullptr;
    }
    else if(!HasRChild(*_root)){
        _root = _root->lChild;
        if(_root)
            _root->parent = nullptr;
    }
    else{
        BinNodePosi(T) lTree = _root->lChild;
        lTree->parent = nullptr;
        _root->lChild = nullptr;//将左子树删除
        _root = _root->rChild;
        _root->parent = nullptr;//只保留右子树
        search(w->data);
        //以原树根为目标，做一次必定失败的查找
        //至此右子树中最小节点必伸展至树根，且其左子树必定为空
        //于是将原左子树接入即可
        _root->lChild = lTree;
        lTree->parent = _root;
    }
    _size--;
    if(_root)
        updateHeight(_root);
    return true;
}
