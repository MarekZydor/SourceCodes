
#ifndef KRUSKAL_UNION_FIND_H
#define KRUSKAL_UNION_FIND_H

class UnionFind{
private:
    int * parents;
    int * rangs;
    int counterFind;
public:
    explicit UnionFind(int n);
    int simpleFind(int index);
    void simpleUnion(int i, int j);
    int getCounterFind();

    int compressFind(int index);
    void unionByRangs(int i, int j);
    ~UnionFind();
};

UnionFind::UnionFind(int n) {
    parents = new int[n];
    rangs = new int[n]{0};
    for(int i = 0; i<n; i++)
    {
        parents[i] = i;
    }
    counterFind = 0;
}

int UnionFind::getCounterFind() {
    return counterFind;
}

int UnionFind::simpleFind(int index) {
    counterFind++;
    if(index == parents[index])
        return index;
    return simpleFind(parents[index]);
}

UnionFind::~UnionFind() {
    delete parents;
    delete rangs;
}

void UnionFind::unionByRangs(int i, int j) {
    if(rangs[i] < rangs[j])
        parents[i] = j;
    else if(rangs[i] > rangs[j])
        parents[j] = i;
    else
    {
        parents[i] = j;
        rangs[j]++;
    }
}

int UnionFind::compressFind(int index) {
    counterFind++;
    if(index == parents[index])
        return index;

    int root = compressFind(parents[index]);
    if(root != parents[index])
        parents[index] = root;
    else
        return root;
}

void UnionFind::simpleUnion(int i, int j) {
    parents[j] = i;
}

#endif