#pragma once
#include "Vector.hpp"
#include "Queue.hpp"
#include "Stack.hpp"
#include <iostream>

typedef enum
{
    UNDISCOVERED,
    DISCOVERED,
    VISTED
} VStatus;//顶点状态
typedef enum
{
    UNDETERMINED,
    TREE,
    CROSS,
    FORWARD,
    BACKWARD
} EType;//边在遍历树中所属的类型
template <typename Tv>
struct Vertex{
    Tv data;
    int inDegree, outDegree;
    VStatus status;
    int dTime, fTime;
    int parent;
    Vertex(Tv const &d = (Tv) 0) : 
        data(d), inDegree(0), outDegree(0), status(UNDISCOVERED),
        dTime(-1), fTime(-1), parent(-1){}
};
template  <typename Te>
struct Edge{
    Te data;
    int weight;
    EType type;
    Edge(Te const &d, int w) : data(d), weight(w), type(UNDETERMINED){}
};
template <typename Tv, typename Te>
class GraphMatrix{
    private:
        Vector<Vertex<Tv>> V;
        Vector<Vector<Edge<Te> *>> E;
    public:
        int n, e;
        GraphMatrix() { 
            n = e = 0; 
        }
        ~GraphMatrix(){
            for (int j = 0; j < n; j++){
                for (int k = 0; k < n; k++)
                    delete E[j][k];
            }
        }
        Tv &vertex(int i) { return V[i].data; }
        int inDegree(int i) { return V[i].inDegree; }
        int outDegree(int i) { return V[i].outDegree; }
        int firstNbr(int i) { return nextNbr(i, n); }
        int nextNbr(int i, int j){
            while ((-1 < j) && (!exists(i, --j)));
            return j;
        }
        VStatus &status(int i) { return V[i].status; }
        int &dTime(int i) { return V[i].dTime; }
        int &fTime(int i) { return V[i].fTime; }
        int &parent(int i) { return V[i].parent; }
        int insert(Tv const &vertex){
            for (int j = 0; j < n; j++)
                E[j].insert(NULL);
            n++;
            E.insert(Vector<Edge<Te> *>(n, n, (Edge<Te> *)NULL));
            return V.insert(Vertex<Tv>(vertex));
        }
        Tv remove(int i){
            for (int j = 0; j < n; j++)
                if (exists(i, j)){
                    delete E[i][j];
                    V[j].inDegree--;
                    e--;
                }
            E.remove(i);
            n--;
            Tv vBak = vertex(i);
            V.remove(i);
            for (int j = 0; j < n; j++)
                if (Edge<Te> *x = E[j].remove(i)){
                    delete x;
                    V[j].outDegree--;
                    e--;
                }
            return vBak;
        }
        bool exists(int i, int j){
            return (0 <= i) && (i < n) && (0 <= j) && (j < n) && E[i][j] != NULL;
        }
        EType &type(int i, int j) { return E[i][j]->type; }
        Te &edge(int i, int j) { return E[i][j]->data; }
        int &weight(int i, int j) { return E[i][j]->weight; }
        void insert(Te const &edge, int w, int i, int j){
            if (exists(i, j))
                return;
            E[i][j] = new Edge<Te>(edge, w);
            e++;
            V[i].outDegree++;
            V[j].inDegree++;
        }
        Te remove(int i, int j){
            Te eBak = edge(i, j);
            delete E[i][j];
            E[i][j] = NULL;
            e--;
            V[i].outDegree--;
            V[j].inDegree--;
            return eBak;
        }
        void BFS(int, int &);
        void DFS(int, int &);
        void BCC(int, int &, Stack<int> &);
        bool TSort(int, int &, Stack<Tv> *);
        void bfs(int);
        void dfs(int);
        void bcc(int);
        Stack<Tv> *tSort(int);
        int prim(int);
        void dijkstra(int);
};
template <typename Tv, typename Te>
void GraphMatrix<Tv, Te>::bfs(int s){
    int clock = 0;
    int v = s;
    do
        if (UNDETERMINED == status(v))
            BFS(v, clock);
    while (s != (v = (++v % n)));
}
template <typename Tv, typename Te>
void GraphMatrix<Tv, Te>::BFS(int v, int &clock){
    Queue<int> Q;
    status(v) = DISCOVERED;
    Q.push(v);
    while (!Q.empty()){
        int v = Q.pop();
        dTime(v) = ++clock;
        for (int u = firstNbr(v); - 1 < u; u = nextNbr(v, u)){
            if (UNDISCOVERED == status(u)){
                status(u) = DISCOVERED;
                Q.push(u);
                type(v, u) = TREE;
                parent(u) = v;
            }
            else
                type(v, u) = CROSS;
        }
        status(v) = VISTED;
    }
}
template <typename Tv, typename Te>
void GraphMatrix<Tv, Te>::dfs(int s){
    int clock = 0;
    int v = s;
    do
        if (UNDISCOVERED == status(v))
            DFS(v, clock);
    while (s != (v = (++v % n)));
}
template <typename Tv, typename Te>
void GraphMatrix<Tv, Te>::DFS(int v, int &clock){
    dTime(v) = ++clock;
    status(v) = DISCOVERED;
    for (int u = firstNbr(v); - 1 < u; u = nextNbr(v, u)){
        switch (status(u)){
            case UNDISCOVERED:
                type(v, u) = TREE;
                parent(u) = v;
                DFS(u, clock);
                break;
            case DISCOVERED:
                type(v, u) = BACKWARD;
                break;
            default:
                type(v, u) = (dTime(v) < dTime(u)) ? FORWARD : CROSS;
                break;
        }
    }
    status(v) = VISTED;
    fTime(v) = ++clock;
}
