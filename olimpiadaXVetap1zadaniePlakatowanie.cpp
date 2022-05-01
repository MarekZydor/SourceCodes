#include <iostream>

class UnionFind
{
private:
    int * parents;
    int * rangs;
public:
    UnionFind(int n);
    int compress_find(int index);
    void union_by_rangs(int i, int j);
    ~UnionFind();
};

UnionFind::UnionFind(int n) {
    parents = new int[n];
    rangs = new int[n]{0};
    for(int i = 0; i<n; i++)
    {
        parents[i] = i;
    }
}

void UnionFind::union_by_rangs(int i, int j) {
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

int UnionFind::compress_find(int index) {
    if(index == parents[index])
        return index;

    parents[index] = compress_find(parents[index]);
    return parents[index];
}

UnionFind::~UnionFind() {
    delete [] parents;
    delete [] rangs;
}

struct DataBuilding
{
    int height;
    int idBuilding;
    DataBuilding() {height = 0; idBuilding = 0;};
    DataBuilding(int height, int idBuilding) : height(height), idBuilding(idBuilding) {};
};

int dataBuildingCMP(DataBuilding a, DataBuilding b)
{
    int diff = a.height - b.height;
    if(diff != 0)
        return diff;
    return a.idBuilding - b.idBuilding;
}
template<class T>
class HeapSort{

private:
    static void heapify_down(T*array, int parentIndex, int size, int(*cmp)(T, T))
    {
        int left = (2 * parentIndex) + 1;
        int right = left+1;

        int largest = parentIndex;

        if (left < size && cmp(array[left], array[parentIndex]) < 0) {
            largest = left;
        }

        if (right < size && cmp(array[right], array[largest]) < 0) {
            largest = right;
        }

        if (largest != parentIndex)
        {
            T daneR = array[parentIndex];
            array[parentIndex] = array[largest];
            array[largest] = daneR;
            heapify_down(array, largest, size, cmp);
        }
    }
    static void heapify_up(T*array, int index, int(*cmp)(T, T))
    {
        if(index == 0)
            return;
        int parentIndex = int((index - 1) / 2);
        if(cmp(array[index], array[parentIndex]) < 0)
        {
            T temp = array[parentIndex];
            array[parentIndex] = array[index];
            array[index] = temp;
            heapify_up(array, parentIndex, cmp);
        }
        else
            return;
    }
public:
    static void sort(T * array, int size, int(*cmp)(T,T))
    {
        for(int index = 0; index < size; index++)
        {
            heapify_up(array, index, cmp);
        }

        int size_temp = size;
        for(int i = 0; i<size-1; i++)
        {
            size_temp--;
            T temp = array[size_temp];
            array[size_temp] = array[0];
            array[0] = temp;

            heapify_down(array, 0, size_temp, cmp);
        }
    }
};





int main() {
    int maxHeight = 0;
    int numberOfBuilding;
    std::ios_base::sync_with_stdio(false);

    std::cin>>numberOfBuilding;

    DataBuilding db[numberOfBuilding];
    for(int i = 0; i<numberOfBuilding; i++)
    {
        int j,height;
        std::cin>>j;
        std::cin>>height;
        if(height > maxHeight)
            maxHeight = height;
        db[i] = {height, i};
    }

    HeapSort<DataBuilding>::sort(db, numberOfBuilding, dataBuildingCMP);

    UnionFind ob_uf(numberOfBuilding+1);

    int result = 0;
    for(int i = 0; i<numberOfBuilding;)
    {
        int i0 = i;

        while (i0 < numberOfBuilding && db[i0].height == db[i].height)
        {
            int root_first = ob_uf.compress_find(db[i0].idBuilding);
            int root_second = ob_uf.compress_find(db[i0].idBuilding + 1);
            if (root_first != root_second)
                ob_uf.union_by_rangs(root_first, root_second);
            i0++;
        }
        int prev = -1;
        i0 = i;
        while (i0 < numberOfBuilding && db[i0].height == db[i].height)
        {
            int root = ob_uf.compress_find(db[i0].idBuilding);
            if (root != prev)
            {
                prev = root;
                result++;
            }
            i0++;
        }
        i=i0;
    }

    std::cout<<result;

    return 0;
}
