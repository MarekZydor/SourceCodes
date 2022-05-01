//
// Created by QwerTY on 31.12.2021.
//

#ifndef KRUSKAL_GRAPH_H
#define KRUSKAL_GRAPH_H
#include "Edge.h"
#include <vector>
#include "gnuplot-iostream.h"

template<typename T> std::string toStr(const T& t)
{
    std::ostringstream os;
    os << t;
    return os.str();
}

class Graph {
private:
    std::vector<Edge*> * total_list_edge;
    Node ** array_node;
    Edge ** array_edge;
    int edge_count;
    int node_count;
public:
    Graph();
    void add(Edge * element);
    std::string showEdges();
    unsigned long long int getEdgeCount();
    float sumWeightOfEdges();
    ~Graph();
    void showGraph();
    std::string showNodes();
    void plug_array_edge(Edge ** array_edge_arg);
    void plugArrayNode(Node **array_node_new);

    void setAllEdgeCount(int i);

    void setNodeCount(int node_count);

};

Graph::Graph() {
    total_list_edge = new std::vector<Edge*>();
}

Graph::~Graph() {
    delete total_list_edge;
}

float Graph::sumWeightOfEdges() {
    float sum = 0;
    for(int i = 0; i < total_list_edge->size(); i++)
    {
        sum += (*total_list_edge)[i]->getWeight();
    }
    return sum;
}

void Graph::add(Edge *element) {
    total_list_edge->push_back(element);
}
void Graph::plugArrayNode(Node ** array_node_new) {
    array_node=array_node_new;
}

unsigned long long Graph::getEdgeCount() {
    return total_list_edge->size();
}

std::string Graph::showEdges() {
    std::string result = "";
    for(int i = 0; i < total_list_edge->size(); i++)
    {
        result += toStr((*total_list_edge)[i]->getIdNodeFirst())+
                  " "+toStr((*total_list_edge)[i]->getIdNodeSecond())+" "
                  +toStr((*total_list_edge)[i]->getWeight())+"\n";
    }
    return result;
}

void Graph::showGraph() {
    Gnuplot gp(R"("C:\Program Files\gnuplot\bin\gnuplot.exe" -persist)");
    gp << "set xrange [-0.1:1.1]\n";
    gp << "set yrange [-0.1:1.1]\n";
    gp << "$alledge << EOD\n";
    for(int i = 0; i<edge_count; i++) {
        gp << toStr(array_node[array_edge[i]->getIdNodeFirst()]->getX()) << " "
           << toStr(array_node[array_edge[i]->getIdNodeFirst()]->getY()) << " "
           << toStr(array_node[array_edge[i]->getIdNodeSecond()]->getX()) << " "
           << toStr(array_node[array_edge[i]->getIdNodeSecond()]->getY()) << "\n";
    }gp << "EOD\n";
    gp << "plot '$alledge' using 1:2:($3-$1):($4-$2) with vectors lw 0.5 lc 'gray' nohead notitle\n";
        gp << "$edge << EOD\n";
    for(int i = 0; i < total_list_edge->size(); i++) {
        gp << toStr(array_node[(*total_list_edge)[i]->getIdNodeFirst()]->getX()) << " "
           << toStr(array_node[(*total_list_edge)[i]->getIdNodeFirst()]->getY()) << " "
           << toStr(array_node[(*total_list_edge)[i]->getIdNodeSecond()]->getX()) << " "
           << toStr(array_node[(*total_list_edge)[i]->getIdNodeSecond()]->getY()) << "\n";
    }gp << "EOD\n";
    gp << "replot '$edge' using 1:2:($3-$1):($4-$2) with vectors lw 2 nohead notitle\n";

    gp << "$data << EOD\n";
    for(int i = 0; i < node_count; i++)
        gp << toStr(array_node[i]->getX()) << " " << toStr(array_node[i]->getY()) << " " << toStr(i) << "\n";
    gp << "EOD\n";
    gp << "replot '$data' using 1:2:3 with labels point pt 7 offset char 0.6,0.6 title 'ID punktow'\n";
}
std::string Graph::showNodes() {
    std::string result = "";
    for(int i = 0; i < node_count; i++)
    {
        result += toStr(array_node[i]->getX())+" "+
                  toStr(array_node[i]->getY())+"\n";
    }
    return result;
}

void Graph::plug_array_edge(Edge ** array_edge_arg) {
    array_edge = array_edge_arg;
}

void Graph::setAllEdgeCount(int i) {
    edge_count = i;
}

void Graph::setNodeCount(int node_count) {
    this->node_count = node_count;
}


#endif //KRUSKAL_GRAPH_H
