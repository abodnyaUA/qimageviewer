#include "algorithms.h"
int BinSearch(QStringList arr, QString key)
{
    int left = 0;
    int right = arr.size() - 1;
    int mid;

    while (left <= right)
    {
        mid = (left + right) / 2;
        if (arr[mid] == key) return mid;
        if (arr[mid] < key) left = mid + 1;
        if (arr[mid] > key) right = mid - 1;
    }
    return 0;
}
