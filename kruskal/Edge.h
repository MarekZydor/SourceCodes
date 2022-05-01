//
// Created by QwerTY on 31.12.2021.
//

#ifndef KRUSKAL_EDGE_H
#define KRUSKAL_EDGE_H


class Edge {
private:
    int id_node_first;
    int id_node_second;
    float weight;
public:
    Edge(int idNodeFirst, int idNodeSecond, float weight);
    int getIdNodeFirst() const;
    int getIdNodeSecond() const;
    float getWeight() const;
};

Edge::Edge(int idNodeFirst, int idNodeSecond, float weight) : id_node_first(idNodeFirst), id_node_second(idNodeSecond), weight(weight) {}

int Edge::getIdNodeFirst() const {
    return id_node_first;
}

int Edge::getIdNodeSecond() const {
    return id_node_second;
}

float Edge::getWeight() const {
    return weight;
}

#endif //KRUSKAL_EDGE_H
