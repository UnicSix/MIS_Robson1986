#include "myGraph.h"
#include <bitset>
#include <cstddef>
#include <iostream>
#include <stdexcept>
#include <algorithm>
#include <vector>
#include <math.h>


myGraph::myGraph(int m){
  order = m;
  degree = 0;
  long long unit=1;
  for(int i=0; i<m; i++){
    long long temp_v=0;
    temp_v = 1LL << i;
    adj.push_back(temp_v);
    vertex_degree.push_back(0);
  }
  countGrhDeg();
  countVtxDeg();
  return;
}

myGraph::myGraph(int m, int zero){
  if(zero!=0){
    throw runtime_error("invalid argument");
  }
  order = m;
  degree = 0;
  for(size_t i=0; i<order; i++){
    adj.push_back(0);
  }
  countGrhDeg();
  countVtxDeg();

  return;
}

bool myGraph::edge(int v1, int v2){
  long long temp = (adj[v1]>>v2)%2&1;
  return temp;
}

void myGraph::printGraph(){
  cout << endl;
  for(size_t i=0; i<adj.size(); i++){
    cout << bitset<64>(adj[i]) << endl;
  }
  return;
}

void myGraph::RandG(int cnt_rate){
  srand(time(NULL));
  for(size_t i=0; i<adj.size(); i++){
    for(size_t j=i; j<adj.size(); j++){
      int cnt = rand()%100;
      if(cnt < cnt_rate && i!=j){
        adj[i] |= 1LL<<j;
        adj[j] |= 1LL<<i;
      }
    }
  }
  /*Set degree properties*/
  countGrhDeg();
  countVtxDeg();
  return;
}

void myGraph::printVtxDeg(){
    cout << "size: " << vertex_degree.size() << endl;
    for(int i:vertex_degree) cout << i << " ";
}

void myGraph::countVtxDeg(){
  int d=0;
  long long temp;
  for(size_t i=0; i<adj.size(); i++){
      temp=adj[i];
      while(temp!=0){
        if((temp%2)&1) d++;
        temp>>=1;
      }
    d--;
    vertex_degree[i]=d;
    d=0;
  }
}

void myGraph::countGrhDeg(){
  int dg=0;
  long long cur_v;
  for(size_t i=0; i<adj.size(); i++){
    cur_v = adj[i];
    for(size_t j=i; j>0; j--){
      dg+=cur_v%2&1;
      cur_v>>=1;
    }
  }
  degree = dg;
}

myGraph myGraph::NeighbourSubG2(int v){
  myGraph sub2Graph(adj.size());
  sub2Graph = *this;
  for(size_t i=0; i<adj.size(); i++){
    if(edge(v,i))
      sub2Graph=sub2Graph-i;
  }
  return sub2Graph;
}

myGraph myGraph::NeighbourSubG(int v){
  myGraph subGraph(adj.size());
  subGraph.adj = adj;
  subGraph.order=0;
  for(size_t i=0; i<adj.size(); i++){
    //vertex v is connected with vertex i
    if(!edge(v, i) || v==int(i)){ 
      subGraph.adj[i]=0;      
      for(size_t j=0; j<adj.size(); j++){
        subGraph.adj[j] &= ~(1LL<<i);
      }
    }
    else if(edge(v, i)){
      subGraph.adj[i] &= ~(1LL<<v);
      subGraph.order++;
    }
  }
  subGraph.countGrhDeg();
  subGraph.countVtxDeg();
  return subGraph;
}

myGraph& myGraph::operator=(const myGraph &other){
  vertex_degree = other.vertex_degree;
  order = other.order;
  degree = other.degree;
  adj = other.adj;
  return *this;
}
myGraph myGraph::operator+(myGraph &other){
  myGraph addition(order);
  for(size_t i=0; i<adj.size(); i++){
    
  }
  return addition;
}

myGraph myGraph::operator-(vector<int> v){
  //remove vertex from graph
  myGraph new_graph = *this;
  for(size_t i=0; i<adj.size(); i++){
    for(int j:v){
      new_graph.adj[i]&=~(1LL<<j);
      new_graph.adj[j]=0;
    }
  order--;
  }
  new_graph.countGrhDeg();
  new_graph.countVtxDeg();
  return new_graph;
}

myGraph myGraph::operator-(int v){
  //remove vertex from graph
  myGraph new_graph = *this;
  for(size_t i=0; i<adj.size(); i++){
    new_graph.adj[i]&=~(1LL<<v);
  }
  order--;
  new_graph.adj[v]=0;
  new_graph.countGrhDeg();
  new_graph.countVtxDeg();
  return new_graph;
}

myGraph myGraph::operator-(myGraph &other){
  myGraph submission(adj.size());
  vector<int> exc_vtx;
  for(size_t i=0; i<adj.size(); i++){
    if(other.adj[i]!=0){
      submission.adj[i] = 0;
      exc_vtx.push_back(i);
    }
    else{
      submission.adj[i] = adj[i];
    }
  }
  for(int i:exc_vtx){
      for(size_t k=0; k<adj.size(); k++){
        submission.adj[k]&=~(1LL<<i);
        submission.order--;
      }   
  }
  submission.countGrhDeg();
  submission.countVtxDeg();
  return submission;
}

vector<int> myGraph::sortVtxSet(vector<int> S){
    vector<int> v = vertex_degree;
    vector<int> s = S;
    sort(s.begin(), s.end(), [&v](int s1, int s2){return v[s1]<v[s2];});
    return s;
}

vector<int> myGraph::N_Intersec(int s1, int s2){
    vector<int> intersec;
    vector<int> Ns1=N(s1);
    vector<int> Ns2=N(s2);
    sort(Ns1.begin(), Ns1.end());
    sort(Ns2.begin(), Ns2.end());
    set_intersection(Ns1.begin(), Ns1.end(), Ns2.begin(), Ns2.end(), intersec.begin());
    return intersec;
}

int myGraph::ms(myGraph G){
    if(!G.isConnected()) {
      vector<int> C_set = G.LeastCntG();
      myGraph C = G-C_set[0];
      if(C.degree<=2) return ms(G-C)+1;
      else            return ms(G-C)+ms(C);
    }
    if(G.degree<=1) return G.degree;
    int A = MinimalDegreeVertex();
    int B = GreatestNeighbourVtx(A);
    if(G.getVtxDeg(A)==1){ return 1+ms(G-Nbar(A)); }
    else if(G.getVtxDeg(A)==2){
        int B2=0;
        for(size_t i=0; i<adj.size(); i++){
          if(edge(i,A)&&i!=B) B2=i;
        }
        if(edge(B,B2)) return 1+ms(G-Nbar(A));
        return max(2+ms(G-Nbar(B)-Nbar(B2)), 1+ms2(G-Nbar(A),N2(A)));
    }
    else if(G.getVtxDeg(A)==3){
        return max(ms2(G-A, N(A)), 1+ms(G-Nbar(A)));
    }
    else if(dominate(N(A), N(B))){
        return ms(G-B);
    }
    return max(ms(G-B), 1+ms(G-Nbar(B)));
    return 0;
}

bool myGraph::dominate(vector<int> NA, vector<int> NB){
    vector<int>::iterator it = NA.begin();
    for(size_t i=0; i<NB.size(); i++){
        it = find(NA.begin(), NA.end(), NB[i]);
        if(it == NA.end()) return false;
    }
    return true;
}


int myGraph::ms(myGraph G, vector<int> S){
    S = G.sortVtxSet(S);
    int s1=S[0], s2=S[1];
    vector<int> intersec = N_Intersec(s1, s2);
    if(G.vertex_degree[s1]<=1) return ms(G);
    else if(G.edge(s1,s2)){
        if(G.getVtxDeg(s1)<=3) return ms(G);
        else return max(ms(G-Nbar(s1)), ms(G-Nbar(s2)))+1;
    }
    // Intersection of Ns1 and Ns2 is not empty
    else if(!intersec.empty()){
        return 1+ms(G-N(s1)-N(s2), S);
    }
    else if(G.getVtxDeg(s2)==2){
        vector<int> Ns1=N(s1);
        int E=Ns1[0], F=Ns1[1];
        if(edge(E, F)) return 1+ms(G-Nbar(s1));
        vector<int> NF=N(F);
        vector<int> collection=N(E);
        for(int i:NF)
          if(find(collection.begin(),collection.end(), i)!=collection.end())
            collection.push_back(i);
        vector<int>::iterator it=find(collection.begin(),collection.end(), s1);
        if(it!=collection.end()) collection.erase(it);
        if(!dominate(N(s2),collection)) return 3+ms(G-Nbar(s1)-Nbar(s2));
        return max(1+ms(G-Nbar(s1)), 3+ms(G-Nbar(E)-Nbar(F)-Nbar(s2)));
    }
  return max(1+ms(G-Nbar(s2)), ms2(G-Nbar(s1)-s2, N(s2)))+1;
}

int myGraph::ms2(myGraph G, vector<int> S){
    size_t sz = S.size();
    sort(S.begin(), S.end());
    if(sz<=1) return 0;
    else if(sz==2){
        if(edge(S[0], S[1])) return 0;
        else return 2+ms(G-Nbar(S[0])-Nbar(S[1]));
    }
    else if(sz==3){
        if(vertex_degree[S[0]]==0) {
            S.erase(S.begin());
            return ms(G-S[0],S);        
        }
        else if(edge(S[0],S[1])&&edge(S[0],S[2])&&edge(S[1],S[2]))
            return 0;
        else if(edge(S[0],S[1])&&edge(S[0],S[2])&&!edge(S[1],S[2]))
            return 2+ms(G-Nbar(S[1])-Nbar(S[2]));
        else if(edge(S[1],S[0])&&edge(S[1],S[2])&&!edge(S[0],S[2]))
            return 2+ms(G-Nbar(S[0])-Nbar(S[2]));
        else if(edge(S[2],S[0])&&edge(S[2],S[1])&&!edge(S[0],S[1]))
            return 2+ms(G-Nbar(S[0])-Nbar(S[1]));
        else if(edge(S[0],S[1])) {
            return ms(G-Nbar(S[2]),S);
        }
        else if(edge(S[0],S[1])) return 1+ms(G-Nbar(S[2]),S);
        else if(edge(S[0],S[2])) return 1+ms(G-Nbar(S[1]),S);
        else if(edge(S[1],S[2])) return 1+ms(G-Nbar(S[0]),S);
        //else if v belongs to the intersection of Ns1 and Ns2
        else if(vertex_degree[S[0]]==1) {
            S.erase(S.begin());
            return 1+ms(G-Nbar(S[0]),S);
        }
        else {
            int s1=S[0], s2=S[1], s3=S[2];
            S.erase(S.begin());
            return max(1+ms(G-Nbar(s1),S), ms2(G-Nbar(s2)-Nbar(s3)-s1, N(s1)));
        }
    }
    else if(sz==4){
        for(int i:G.vertex_degree){
            if(i<=3) 
                return ms(G);
            else {
                S.erase(S.begin());
                return max(1+ms(G-Nbar(S[0])),ms2(G-S[0],S));
            }
        }
    }
    return ms(G);
}

myGraph myGraph::NbarSubG(int v){
  myGraph subGraph(adj.size());
  subGraph.adj = adj;
  subGraph.order=0;
  for(size_t i=0; i<adj.size(); i++){
    //vertex v is connected with vertex i
    if(!edge(v, i)){ 
      subGraph.adj[i]=0;      
      for(size_t j=0; j<adj.size(); j++){
        subGraph.adj[j] &= ~(1LL<<i);
      }
    }
    //else if(edge(v, i)){
    //  subGraph.adj[i] &= ~(1LL<<v);
    //  subGraph.order++;
    //}
  }
  subGraph.countGrhDeg();
  subGraph.countVtxDeg();
  return subGraph;
}

int myGraph::GreatestNeighbourVtx(int A){
  int greatest=0;
  for(size_t i=0; i<adj.size(); i++){
    if(edge(A,i) && vertex_degree[i]>=greatest){
      greatest=i;
    }
  }
  return greatest;
}

bool myGraph::dominate(myGraph other){
  for(size_t i=0; i<adj.size(); i++){
    if(adj[i]==0 && other.adj[i]!=0){
      return false;
    }
  }
  return true;
}

bool myGraph::isConnected(){
  int visited=0;
  DFS(0, &visited);
  int count=0;
  // count visited vertices
  for(size_t i=0; i<adj.size(); i++){
    if(visited%2==1) count++;
    visited>>=1;
  }
  return count == int(order);
}

int myGraph::MinimalDegreeVertex(){
  int min=0;
  for(size_t i=0; i<order; i++){
    if(min<=vertex_degree[i]){
      min=vertex_degree[i];
    }
  }
  return min;
}

int myGraph::MaxDegreeVtx(){
  int max=0;
  for(size_t i=0; i<order; i++){
    if(max>=vertex_degree[i]){
      max=vertex_degree[i];
    }
  }
  return max;
}

void myGraph::DFS(int src, int* visited){
  *visited|=1LL<<src;
  for(size_t i=0; i<adj.size(); i++){
    // if vertex i is not visited and i is connected to vertex src
    if(!(((*visited>>i)%2)&1) && edge(i,src)){
      DFS(i, visited);
    }
  }
}

vector<int> myGraph::LeastCntG(){
  vector<int> min_set;
  int vtx_set=0, new_set=0;
  int count=0, min_count=1, total=0;
  int src=0; bool chosen=false;
  //while(vtx_set<=pow(2,int(adj.size()))-1){
  while(total < int(adj.size())){
    DFS(src, &new_set);
    vtx_set|=new_set;
    for(size_t i=0; i<adj.size(); i++){
      if((new_set>>i)%2==1)
        count++;
      else if((vtx_set>>i)%2!=1 && !chosen){
        src=i;
        chosen=true;
      }
    }

    if(count<=min_count) {
      min_count = count;
      min_set.push_back(new_set);
    }
    total += count;
    chosen=false;
    new_set=0;
    count=0;
  }
  
  return min_set;
}

vector<int> myGraph::N(int v){
    vector<int> S;
    for(size_t i=0; i<adj.size(); i++){
        if(v!=i && edge(i,v)) S.push_back(i);
    }
    return S;
}
vector<int> myGraph::Nbar(int v){
    vector<int> S;
    for(size_t i=0; i<adj.size(); i++){
        if(edge(i,v)) S.push_back(i);
    }
    return S;
}
vector<int> myGraph::N2(int v){
    vector<int> S;
    for(size_t i=0; i<adj.size(); i++){
        if(v!=i && edge(i,v)) { // i is a neighbour of v
            for(size_t j=0; j<adj.size(); j++){ //collect neighbour of i exclude v
                if(j!=v && edge(j,i) && find(S.begin(),S.end(), j)==S.end()){
                    S.push_back(j);
                }
            }
        }
    }
    return S;
}
