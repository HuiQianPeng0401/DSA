#pragma once
#include "Vector.hpp"
#include <iostream>


template <typename T>
class Stack : public Vector<T>{
    public:
        void push(T const &e) //做向量的末元素插入
        { this->insert(e); }
        T pop() //删除向量的末元素
        { return this->remove(this->size() - 1); }
        T &top() //返回向量的末元素
        { return (*this)[this->size() - 1]; }
};

// void convert(Stack<char> &S, int n, int base){
//     static char digit[] = {'0', '1', '2', '3', '4', '5', 
//                             '6', '7', '8', '9',
//                            'A', 'B', 'C', 'D', 'E', 'F'};
//     if(0<n){
//         convert(S, n / base, base);
//         S.push(digit[n % base]);
//         std::cout << S.pop();
//     }
// }
// void trim(const char exp[], int &lo, int &hi){//删除表达式exp[lo,hi]不含括号的最长前后缀
//     //找出第一个和最后一个括号
//     while((lo<=hi)&&(exp[lo]!='(')&&(exp[lo]!=')'))
//         lo++;
//     while((lo<=hi)&&(exp[hi]!='(')&&(exp[hi]!=')'))
//         hi--;
// }
// int divide(const char exp[], int lo, int hi){
//     //切分表达式exp[lo,hi]，使exp匹配仅当他们匹配
//     int mi = lo;
//     int crc = 1;//cra为[lo，mi]范围内左右括号数目之差
//     while((0<crc)&&(++mi<hi)){//逐个检查各字符，直到左右括号数目相等或者越界
//         if(exp[mi]==')')
//             crc--;
//         if(exp[mi]=='(')
//             crc++;
//     }
//     return mi;
// }
// bool paren(const char exp[], int lo, int hi){
//     Stack<char> s;
//     for (int i = 0; exp[i];i++){
//         switch(exp[i]){
//             case '(':
//             case '[':
//             case '{':
//                 s.push(exp[i]);
//                 break;
//             case ')':
//             if((s.empty())||('('!=s.pop()))
//                 return false;
//                 break;
//             case ']':
//             if((s.empty())||('['!=s.pop()))
//                 return false;
//                 break;
//             case '}':
//             if((s.empty())||('{'!=s.pop()))
//                 return false;
//                 break;
//         }
//     }
//     return s.empty();
// }
// struct Queen{
//     int x, y;
//     Queen(int xx = 0, int yy = 0) : x(xx), y(yy){}
//     bool operator==(Queen const &q) const{
//         return (x == q.x) || (y == q.y) || (x + y == q.x + q.y) || (x - y == q.x - q.y);
//         //行冲突、列冲突、正对角线冲突、反对角线冲突
//     }
//     bool operator!=(Queen const &q) const{
//         return !(*this == q);//利用重载的==来实现!=
//     }
// };
// void placeQueens(int N){
//     Stack<Queen> solu;//存放解的栈
//     Queen q(0, 0);
//     int nSolu = 0, nCheck = 0;
//     do{//N皇后算法：采用试探/回溯的策略
//         if(N<=solu.size()||N<=q.y){//若已经出界，则
//             q = solu.pop();//回溯到上一个皇后
//             q.y++;//试探下一列
//         }
//         else{//否则试探下一行
//             while(q.y<N&&0<=solu.find(q))//与栈中皇后对比
//                 q.y++;//不断试探
//             if(N>q.y){//存在合适位置
//                 solu.push(q);//入栈
//                 if(N<=solu.size())
//                     nCheck++;
//                 q.x++;//在试探下一行
//                 q.y = 0;//从首列开始试探
//             }
//         }
//     } while ((0 < q.x) || (q.y < N));//所有分支均已经穷举或剪枝后结束
// }
// typedef enum
// {
//     AVAILABLE,//原始可用的
//     ROUTE,//在当前路径上的
//     BACKTRACKED,//所有方向均尝试失败后回溯过的
//     WALL//墙壁
// } Status;//迷宫单元状态
// typedef enum
// {
//     UNKNOWN,//未定
//     EAST,//东
//     SOUTH,//西
//     WEST,//南
//     NORTH,//北
//     NO_WAY//无路可通
// } ESWN;//单元的邻接方向
// inline ESWN nextESWN(ESWN eswn){ 
//     return ESWN(eswn + 1); 
// }
// struct Cell{
//     int x, y;//坐标
//     Status status;//单元格的状态
//     ESWN incoming, outgoing;//进入和走出方向
// };
// #define LABT_MAX 24
// Cell laby[LABT_MAX][LABT_MAX];
// inline Cell *neighbor(Cell *cell){//查询当前位置的相邻隔点
//     switch(cell->outgoing){
//         case EAST://向东
//             return cell + LABT_MAX;
//         case SOUTH://向南
//             return cell + 1;
//         case WEST://向西
//             return cell - LABT_MAX;
//         case NORTH://向北
//             return cell - 1;
//         default:
//             exit(-1);
//     }
// }
// inline Cell *advance(Cell *cell){//从当前位置转入相邻隔点
//     Cell *next;
//     switch(cell->outgoing){
//         case EAST://向东
//             next = cell + LABT_MAX;
//             next->incoming = WEST;
//             break;
//         case SOUTH://向南
//             next = cell + 1;
//             next->incoming = NORTH;
//             break;
//         case WEST://向西
//             next = cell - LABT_MAX;
//             next->incoming = EAST;
//             break;
//         case NORTH://向北
//             next = cell - 1;
//             next->incoming = SOUTH;
//             break;
//     }
//     return next;
// }
// bool labyrinth(Cell laby[LABT_MAX][LABT_MAX], Cell *s, Cell *t){
//     //在s到t之间找一条通路
//     if((AVAILABLE!=s->status)||(AVAILABLE!=t->status))
//         return false;//s或t不可用直接返回
//     Stack<Cell *> path;//用栈记录通路
//     s->incoming = UNKNOWN;
//     s->status = ROUTE;
//     path.push(s);//起点
//     do{//从起点出发不断试探回溯直到抵达终点
//         Cell *c = path.top();//检查当前位置（栈顶）
//         if(c==t)//抵达终点
//             return true;
//         while(NO_WAY>(c->outgoing=nextESWN(c->outgoing)))//逐一检查所有方向
//             if(AVAILABLE==neighbor(c)->status)//找到尚未试探的方向
//                 break;
//         if(NO_WAY<=c->outgoing){//所有方向均尝试过
//             c->status = BACKTRACKED;//此位置失败 向后回溯
//             c = path.pop();
//         }
//         else{//成功了 向前试探
//             path.push(c = advance(c));//移动到新位置
//             c->outgoing = UNKNOWN;//新位置走出方向不确定
//             c->status = ROUTE;//状态更新
//         }
//     } while (!path.empty());
//     return false;
// }
