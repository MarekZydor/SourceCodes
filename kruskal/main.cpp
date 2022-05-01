#include <iostream>
#include <chrono>
#include <queue>
#include "Node.h"
#include "Edge.h"
#include "UnionFind.h"
#include "Graph.h"

struct cmpEdge
{
    bool operator()(const Edge* lhs, const Edge* rhs)
    {
        return lhs->getWeight() > rhs->getWeight();
    }
    bool operator()(const Edge& lhs, const Edge& rhs)
    {
        return lhs.getWeight() > rhs.getWeight();
    }
};

template<class T>
void bucketSort(T * array, int size, double m, float(*float_key)(T))
{
    double range = m / (double)size;
    std::priority_queue<T, std::vector<T>, cmpEdge> * buckets[size];
    for(int i = 0; i < size; i++)
        buckets[i] = nullptr;

    for(int i = 0; i < size; i++)
    {
        float pilka_key = float_key(array[i]);
        int bucketIndex = int(pilka_key / range);
        if(buckets[bucketIndex] == nullptr)
            buckets[bucketIndex] = new std::priority_queue<T, std::vector<T>, cmpEdge>;
        buckets[bucketIndex]->push(array[i]);
    }
    int indeksArr = 0;
    for(int i = 0; i < size; i++)
    {
        if(buckets[i] != nullptr)
        {
            int size_l = buckets[i]->size();
            for(int j = 0; j<size_l; j++)
            {
                array[indeksArr] = buckets[i]->top();
                buckets[i]->pop();
                indeksArr++;

            }
            delete buckets[i];
        }
    }
}

float EdgeKey(Edge a)
{
    return a.getWeight();
}
float EdgeKey(Edge *a)
{
    return a->getWeight();
}



const int SWITCH_FIND_AND_UNION = 1; //1 - compressFind and unionByRangs turn on

int main() {
    std::ifstream file;
    file.open("g2.txt");

    if(!file)
    {
        fprintf(stderr, "Wystapil blad podczas otwarcia pliku");
        file.close();
        exit(1);
    }
    int node_count;
    file>>node_count;
    Graph * total_graph = new Graph();
    Node ** array_node = new Node*[node_count];
    for(int i = 0; i<node_count; i++)
    {
        float x;
        file>>x;
        float y;
        file>>y;
        array_node[i] = new Node(x,y);
    }
    total_graph->plugArrayNode(array_node);
    total_graph->setNodeCount(node_count);

    int edge_count;
    file>>edge_count;
    Edge ** array_edge = new Edge*[edge_count];

    for(int i = 0; i<edge_count; i++)
    {
        int id_node_first;
        file>>id_node_first;
        int id_node_second;
        file>>id_node_second;
        float weight;
        file>>weight;
        array_edge[i] = new Edge(id_node_first, id_node_second, weight);
    }
    total_graph->plug_array_edge(array_edge);
    total_graph->setAllEdgeCount(edge_count);
    auto t3 = std::chrono::high_resolution_clock::now();
    bucketSort(array_edge, edge_count, 1.0, EdgeKey);
    auto t4 = std::chrono::high_resolution_clock::now();
    file.close();

    UnionFind * ob_uf = new UnionFind(node_count);

    auto t1 = std::chrono::high_resolution_clock::now();
    int iloscfind = 0;
    for(int i = 0; i<edge_count; i++)
    {
        iloscfind+=2;
        int root_first = SWITCH_FIND_AND_UNION ? ob_uf->compressFind(array_edge[i]->getIdNodeFirst()) :
                         ob_uf->simpleFind(array_edge[i]->getIdNodeFirst());
        int root_second = SWITCH_FIND_AND_UNION ? ob_uf->compressFind(array_edge[i]->getIdNodeSecond()) :
                          ob_uf->simpleFind(array_edge[i]->getIdNodeSecond());
        if(root_first != root_second)
        {
            if(SWITCH_FIND_AND_UNION) {
                ob_uf->unionByRangs(root_first, root_second);
            }else
                ob_uf->simpleUnion(root_first, root_second);

            total_graph->add(array_edge[i]);
            if(total_graph->getEdgeCount() == node_count-1)
                break;
        }
    }
    auto t2 = std::chrono::high_resolution_clock::now();

    std::cout<<"Liczba krawedzi: "<<total_graph->getEdgeCount()<<"\n";
    std::cout<<std::setprecision(7)<<"Suma wag krawedzi: "<<total_graph->sumWeightOfEdges()<<"\n";
    std::cout<<"Czas glownej petli: "<<std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count()<<"us"<<std::endl;
    std::cout<<"Czas sortowania: "<<std::chrono::duration_cast<std::chrono::microseconds>(t4 - t3).count()<<"us"<<std::endl;
    std::cout<<"Ilosc operacji find: "<<ob_uf->getCounterFind()<<"\n";
    std::cout<<"Ilosc operacji find(internal)/find(external): "<<ob_uf->getCounterFind()/(double)iloscfind<<"\n";
    //ABY POKAZAĆ GRAPH NALEŻY MIEĆ ZAINSTALOWANY PROGRAM GNUPLOT
    //total_graph->showGraph();


    for(int i = 0; i<node_count; i++) {
        delete array_node[i];
    }
    for(int i = 0; i<edge_count; i++) {
        delete array_edge[i];
    }
    delete [] array_edge;
    delete [] array_node;
    delete total_graph;
    delete ob_uf;


    return 0;
}
