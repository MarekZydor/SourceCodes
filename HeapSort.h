#ifndef HEAPSORT_H
#define HEAPSORT_H
template<class T>
class HeapSort{

private:
    static void heapify_down(T*array, int parentIndex, int size, bool(*cmp)(T, T, T), T startpoint)
    {
        int left = (2 * parentIndex) + 1;
        int right = left+1;

        int largest = parentIndex;

        if (left < size && cmp(startpoint, array[left], array[parentIndex])) {
            largest = left;
        }

        if (right < size && cmp(startpoint, array[right], array[largest])) {
            largest = right;
        }

        if (largest != parentIndex)
        {
            T daneR = array[parentIndex];
            array[parentIndex] = array[largest];
            array[largest] = daneR;
            heapify_down(array, largest, size, cmp, startpoint);
        }
    }
    static void heapify_up(T*array, int index, bool(*cmp)(T, T, T), T startpoint)
    {
        if(index == 0)
            return;
        int parentIndex = int((index - 1) / 2);
        if(cmp(startpoint, array[index], array[parentIndex]))
        {
            T temp = array[parentIndex];
            array[parentIndex] = array[index];
            array[index] = temp;
            heapify_up(array, parentIndex, cmp, startpoint);
        }
        else
            return;
    }
public:
    static void sort(T * array, int size, bool(*cmp)(T,T,T), T startpoint)
    {
        for(int index = 0; index < size; index++)
        {
            heapify_up(array, index, cmp, startpoint);
        }

        int size_temp = size;
        for(int i = 0; i<size-1; i++)
        {
            size_temp--;
            T temp = array[size_temp];
            array[size_temp] = array[0];
            array[0] = temp;

            heapify_down(array, 0, size_temp, cmp, startpoint);
        }
    }
};
#endif //HEAPSORT_H