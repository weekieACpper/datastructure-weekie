/*
 * @Author: weekie
 * @Date: 2022-03-04 21:02:16
 * @LastEditTime: 2022-03-04 21:05:33
 * @LastEditors: Please set LastEditors
 * @Description: 实现堆排序
 * @FilePath: /datastructure/basic_datastructre/binary_heap/heapSort.cpp
 */
#include<maxHeap.hpp>
//堆排序
void heapSort(int* array, int size)
{
    maxHeap heap(array, size);//建堆
    for (int i = size - 1; i >= 0; i--)
    {
        array[i] = heap.extractMax();
    }
}