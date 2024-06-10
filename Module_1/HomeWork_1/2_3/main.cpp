// 2_3. Даны два массива неубывающих целых чисел, упорядоченные по возрастанию. 
// A[0..n-1] и B[0..m-1]. n >> m. Найдите их пересечение.
// Требования: Время работы: O(m * log k), где k - позиция элемента B[m-1] в массиве A.. 
// В процессе поиска очередного элемента B[i] в массиве A пользуйтесь результатом поиска элемента B[i-1].
// Внимание! В этой задаче для каждого B[i] сначала нужно определить диапазон для бинарного поиска размером порядка k 
//                                          с помощью экспоненциального поиска, а потом уже в нем делать бинарный поиск.

// n, k ≤ 10000.


#include<iostream>
#include<cassert>

bool exponentialSearch( int* A, int& leftBorder, int& rightBorder, int target, int sizeA ) {
    while ( rightBorder < ( sizeA - 1 ) && A[rightBorder] < target ) {
        rightBorder *= 2;
    }
    if ( rightBorder >= ( sizeA - 1 ) ) {
        rightBorder = sizeA - 1;
        if ( A[rightBorder] < target ) {  //случай, когда последний элемент B > последнего элемента A
            return false;                 // чтобы, лишний раз не проходится бинарным поиском
        }
    }
    leftBorder = rightBorder / 2;
    return true;
}

int binarySearch( int* A, int first, int last, int target ) {
    while( first < last ) {
        int mid = ( first + last ) / 2;
        if( A[mid] < target ) {
            first = mid + 1;
        } else {
            last = mid;
        }
    }
    return ( A[first] != target ) ? -1 : A[first];
}

void run( std::istream& input, std::ostream& output ) {
    int n, m; //размеры массива A и B соответственно
    input >> n >> m;

    int* A = new int[n];
    int* B = new int[m];
    
    for( int i = 0; i < n; ++i ) {
        input >> A[i];
    }
    for( int i = 0; i < m; ++i ) {
        input >> B[i];
    }

    int leftBorder = 0;
    int rightBorder = 1;
    for( int i = 0; i < m; ++i ) {
        bool numberIntervalFound = false;
        numberIntervalFound = exponentialSearch( A, leftBorder, rightBorder, B[i], n );
        if ( numberIntervalFound ) {
            int result = binarySearch( A, leftBorder, rightBorder, B[i] );
            if ( result != -1 ) {
                output << result << " ";
            }
        }
    }

    delete[] A;
    delete[] B;
}
 
int main() {
    run( std::cin, std::cout );
    return 0;
}