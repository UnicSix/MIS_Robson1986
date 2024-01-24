#ifndef MYGRAPH_H
#define MYGRAPH_H
#include <cstddef>
#include <vector>
#include <bitset>
#include <list>
#include <ctime>
#include <iostream>

using namespace std;

class myGraph{
  public:
    myGraph(int); //set graph size
    myGraph(int, int); // set graph and init all edge 0
    myGraph operator-(myGraph &other);
    myGraph operator-(int v);
    myGraph operator-(vector<int>);
    myGraph operator+(myGraph &other);
    myGraph& operator=(const myGraph &other);
    myGraph VertexSetSubG(int);
    //create a induced subgragph with neighbours of vertex v
    myGraph NeighbourSubG(int v);  //N
    myGraph NeighbourSubG2(int v); //N2
    myGraph NbarSubG(int v);
    vector<int> N(int v);
    vector<int> N2(int v);
    vector<int> Nbar(int v);
    void RandG(int);     //return randomized Graph
    void printGraph();
    void printVtxDeg();
    bool edge(int, int); //is vertex m, n connected to each other
    bool dominate(myGraph);
    bool isConnected();
    bool cmpEdge(int s1, int s2);
    int getOrder(){return order;}
    int ms(myGraph);
    int ms(myGraph, vector<int>);
    int ms2(myGraph, vector<int>);
    int MinimalDegreeVertex();
    int MaxDegreeVtx();
    int GreatestNeighbourVtx(int);
    int getGrhDeg(){return degree;}
    int getVtxDeg(int v){return vertex_degree[v];}
    vector<int> LeastCntG();
    vector<int> sortVtxSet(vector<int>);
    vector<int>N_Intersec(int, int);
  
    static bool dominate(vector<int>, vector<int>);

  private:
    /*Variables*/
    vector<long long> adj;
    vector<int> vertex_degree;
    int degree;
    size_t order;

    /*Functions*/
    void countVtxDeg();                         
    void countGrhDeg();
    void DFS(int, int*);
};

#endif 