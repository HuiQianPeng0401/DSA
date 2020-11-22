#pragma once

#include "Entry.hpp"
#define QlistNodePosi(T)  QuadlistNode<T>* //跳转表节点位置

template <typename T> struct QuadlistNode { //QuadlistNode模板类
   T entry; //所存词条
   QlistNodePosi(T) pred;  
   QlistNodePosi(T) succ; //前驱、后继
   QlistNodePosi(T) above; //上邻
   QlistNodePosi(T) below; //下邻
   QuadlistNode //构造器
   ( T e = T(), QlistNodePosi(T) p = NULL, QlistNodePosi(T) s = NULL,
     QlistNodePosi(T) a = NULL, QlistNodePosi(T) b = NULL )
      : entry ( e ), pred ( p ), succ ( s ), above ( a ), below ( b ) {}
   QlistNodePosi(T) insertAsSuccAbove //插入新节点，以当前节点为前驱，以节点b为下邻
   ( T const& e, QlistNodePosi(T) b = NULL );
};

template <typename T>
QlistNodePosi(T) QuadlistNode<T>::insertAsSuccAbove(T const &e, QlistNodePosi(T) b = NULL){
   QlistNodePosi(T) x = new QuadlistNode<T>(e, this, succ, NULL, b);//创建新节点
   succ->pred = x;
   succ = x;//水平逆向链接
   if(b)
      b->above = x;//垂直逆向链接
   return x;//返回新节点位置
}
