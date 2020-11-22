#include "myList.hpp"
#include "Entry.hpp"
#include "Quadlist.hpp"
#include "Diciionary.hpp"

template <typename K, typename V>
//符合Dictonary接口的Skiplist模板类（但隐含假设元素间可比较大小
class Skiplist : public Dictionary<K, V>, public List<Quadlist<Entry<K, V>> *>{
    protected:
        bool skipSearch(
            ListNode<Quadlist<Entry<K, V>> *> *&qlist,
            QuadlistNode<Entry<K, V>> *&p, K &k);
    public:
        int size() const { return empty() ? 0 : last()->data->size(); }
        int level() { return List::size(); }
        bool put(K, V);
        V *get(K k);
        bool remove(K k);
};

template <typename K, typename V>
V *Skiplist<K, V>::get(K k){
    //跳转表词条查找算法
    if(empty())
        return NULL;
    //从顶层Quadlist的首节点开始
    ListNode<Quadlist<Entry<K, V>> *> *qlist = first();
    QuadlistNode<Entry<K, V>> *p = qlist->data->first();
    return skipSearch(qlist, p, k) ? (p->entry.value) : NULL;
}
template <typename K, typename V>
bool Skiplist<K, V>::skipSearch(
    ListNode<Quadlist<Entry<K, V>> *> * &qlist,
    QuadlistNode<Entry<K, V>> *&p, K &k){
        /**供内部使用的词条查找算法
         * 入口：qlist为顶层列表，p为qlist的首节点
         * 出口：若成功，p为命中关键码所属塔的顶部节点，qlist为p所属链表
         * 否则，p为所属塔的基座，该塔对应不大于k的最大且最靠右关键码，qlist为空
         * 约定：多个词条命中时，取最靠后者
        */
    //向右向下查找k
    while(true){
        while(p->succ&&(p->entry.key<=k))//从前向后
            p = p->succ;
        p = p->pred;//此时倒回一步
        if(p->pred&&(k==p->entry.key))
            return true;
        qlist = qlist->succ;//否则转入下一层
        if(!qlist->succ)//若已到穿透底层则意味着失败
            return false;
        p = (p->pred) ? p->below : qlist->data->first();//否则转至当前塔的下一节点
    }//通过实验统计验证关于平均查找长度的结论
}

template <typename K, typename V>
bool Skiplist<K,V>::put(K k, V v){
    Entry<K, V> e = Entry<K, V>(k, v);
    if(empty())
        insertAsFirst(new Quadlist<Entry<K, V>>);//插入首个Entry
    ListNode<Quadlist<Entry<K, V>> *> *qlist = first();//顶层
    QuadlistNode<Entry<K, V>> *p = qlist->data->first();//首顶点
    if(skipSearch(qlist,p,k))//查找适当的插入位置
        while(p->below)
            p = p->below;//若已有雷同词条 则强制转换到塔底
    qlist = last();//以下 紧邻p的右侧 建立一座新塔
    QuadlistNode<Entry<K, V>> *b = qlist->data->insertAfterAbove(e, p);
    while(rand()&1){
        while(qlist->data->valid(p)&&!p->above)//找出不低于此高度的最近前驱
            p = p->pred;
        if(!qlist->data->valid(p)){//若该前驱是header
            if(qlist==first())//且是最顶层
                insertAsFirst(new Quadlist<Entry<K, V>>);
            p = qlist->pred->data->first()->pred;//将p转至上一层Skiplist的header
        }
        else
            p = p->above;
        qlist = qlist->pred;
        b = qlist->data->insertAfterAbove(e, p, b);
    }
    return true;
}

template <typename K, typename V>
bool Skiplist<K, V>::remove(K k){
    //跳转表词条删除算法
    if(empty())
        return false;
    ListNode<Quadlist<Entry<K, V>> *> qlist = first();
    QuadlistNode<Entry<K, V>> *p = qlist->data->first();
    if(!skipSearch(qlist,p,k))
        return false;
    do{
        //若词条存在，则拆除与之对应的塔
        QuadlistNode<Entry<K, V>> *lower = p->below;//记住下一层节点
        qlist->data->remove(p);//删除当前层节点
        p = lower;
        qlist = qlist->succ;//删除后再转入下一层
    } while (qlist->succ);//如上不断重复 知道塔基
    while(!empty()&&first()->data->empty())
        List::remove(first());//清除可能不含词条的顶层Quadlist
    return true;
}
