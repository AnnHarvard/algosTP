//6.1 Порядковая статистика и параметры множества
// Дано множество целых чисел из [0..10^9] размера n.
// Используя алгоритм поиска k-ой порядковой статистики, требуется найти следующие параметры множества:
// 10% перцентиль
// медиана
// 90% перцентиль

// Требования:

// К дополнительной памяти: O(n).
// Среднее время работы: O(n)
// Должна быть отдельно выделенная функция partition.
// Рекурсия запрещена.
// Решение должно поддерживать передачу функции сравнения снаружи.

#include<iostream>
#include<cassert>

template<class T>
class IsLessDefault {
public:
	bool operator()( const T& l, const T& r ) { return l < r; }
};

template<class T, class IsLess = std::less<T>>
int FindPivotIndex( T* arr, int left, int right, IsLess isLess = IsLess() ) {
    int mid = left + ( right - left ) / 2;

    if (isLess(arr[mid], arr[left])) std::swap(arr[left], arr[mid]);
    if (isLess(arr[right], arr[left])) std::swap(arr[left], arr[right]);
    if (isLess(arr[right], arr[mid])) std::swap(arr[mid], arr[right]);

    return mid;
}


template<class T, class IsLess = IsLessDefault<T>>
int Partition( T* arr, int left, int right, IsLess isLess = IsLess() ) {
	if( right <= 1 ) {
		return 0;
	}

    int pivotIndex = FindPivotIndex( arr, left, right );
    T pivot = arr[pivotIndex];
    
    std::swap( arr[pivotIndex], arr[right] );
    int i = left;
    // int j = left;
    for( ; arr[i] < pivot; i++ ) {}
    int j = i + 1;

    while (j < right) {
        if (isLess(arr[j], pivot)) {
            std::swap(arr[i], arr[j]);
            i++;
        }
        j++;
    }
    std::swap(arr[i], arr[right]); // Помещаем опорный элемент на свое место
    return i;
	
}

template<class T, class IsLess = IsLessDefault<T>>
T FindKStat( T* arr, int n, int k, IsLess isLess = IsLess() )
{
    int left = 0;
    int right = n - 1;
    while ( left < right ) {
        int pivotIndex = Partition( arr, left, right );
        if( pivotIndex == k ) {
            return arr[k];
        } else if ( isLess( pivotIndex, k ) ) {
            left = pivotIndex + 1;
        } else {
            right = pivotIndex;
        }
    }
    return arr[left];
}

int main() {
    int n;
    std::cin >> n;
    int* array = new int[n];
    for( int i = 0; i < n; ++i ) {
        std::cin >> array[i];
    }

    int k10 = n / 10;
    int k90 = n * 9 / 10;
    int kMedian = n / 2;

    std::cout << FindKStat<int>( array, n, k10 ) << std::endl << FindKStat<int>( array, n, kMedian ) << std::endl << FindKStat<int>( array, n, k90);
    delete[] array;

    return 0;
}