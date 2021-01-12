#pragma once
#include "List.hpp"

template <typename T>
class Queue : public List<T>{
    public:
        T &front() { return this->first()->data; }
        void push(T const &e) { this->insertAsLast(e); }
        T pop() { return this->remove(this->first()); }
};


// struct Customer{//顾客类
//     int window;//所属窗口（队列）
//     unsigned int time;//服务时长
// };
// int bestWindow(Queue<Customer> windows[], int nWin){
//     int minSize = windows[0].size();
//     int optiWin = 0;
//     for (int i = 1; i < nWin;i++)
//         if(minSize>windows[i].size()){
//             minSize = windows[i].size();
//             optiWin = i;
//         }
//     return optiWin;
// }
// void simulate(int nWin, int servTime){//按指定窗口数、服务总时间模拟银行业务
//     Queue<Customer> *windows = new Queue<Customer>[nWin];//为每一个窗口创建队列
//     for (int now = 0; now < servTime;now++){//在下班之前，每隔一个单位时间
//         if(rand()%(1+nWin)){//新顾客以nWin/(nWin+1)的概率到达
//             Customer c;
//             c.time = 1 + rand() % 98;//新顾客到达，服务时长随机
//             c.window = bestWindow(windows, nWin);//选取人最少的窗口
//             windows[c.window].push(c);//加入对应窗口
//         }
//         for (int i = 0; i < nWin;i++)//分别检查每个窗口
//             if(!windows[i].empty())//窗口有人
//                 if(--windows[i].front().time<=0)//队首顾客的服务时长减少1
//                     windows[i].pop();//若服务结束出列即可
//     }
//     delete[] windows;
// }
