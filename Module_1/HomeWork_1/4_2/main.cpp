// 4.2. Топ K пользователей из лога

// Имеется лог-файл, в котором хранятся пары для N пользователей (Идентификатор пользователя, посещаемость сайта).
// Напишите программу, которая выбирает K пользователей, которые чаще других заходили на сайт, и выводит их в порядке возрастания посещаемости.
// Количество заходов и идентификаторы пользователей не повторяются.

// Требования:

// Время работы O(N * logK)
// Куча должна быть реализована в виде шаблонного класса.
// Решение должно поддерживать передачу функции сравнения снаружи.
// Куча должна быть динамической.

// Формат ввода
// Сначала вводятся N и K, затем пары (Идентификатор пользователя, посещаемость сайта).

// Формат вывода
// Идентификаторы пользователей в порядке возрастания посещаемости.

#include<iostream>
#include<cassert>

template<class T>
class IsBiggerDefault {
public:
	bool operator()( const T& l, const T& r ) { return l > r; }
};

template<class T, class IsBigger = IsBiggerDefault<T>>
class Heap {
public:
	Heap( IsBigger isBigger = IsBigger()) : isBigger( isBigger ), buffer( nullptr ), bufferSize( 0 ), size( 0 ) { };
	Heap( T* arr, int arrSize, IsBigger isBigger = IsBigger()) : isBigger( isBigger ), bufferSize ( arrSize ), size( arrSize ) {
        buffer = new T[arrSize];
        for( int i = 0; i < arrSize; ++i ) {
            buffer[i] = arr[i];
        }
        buildHeap();
    };
	~Heap() {
        delete[] buffer;
    };

	void Insert( const T& element );
	T ExtractMin();
	const T& PeekMin() const;
	int Size() const;

private:
	IsBigger isBigger;
	T* buffer;
	int bufferSize;
	int size;

	void buildHeap();
	void siftDown( int i );
	void siftUp( int i );
	void grow();
    bool isEmpty();
};

template<class T, class IsBigger>
void Heap<T, IsBigger>::siftDown( int i ) {
    int smallest = i;
    while ( true ) {
        int left = 2 * i + 1;
        int right = 2 * i + 2;

        if (left < size && isBigger( buffer[smallest], buffer[left] ))
            smallest = left;

        if (right < size && isBigger( buffer[smallest], buffer[right] ))
            smallest = right;

        if (smallest != i) {
            std::swap( buffer[i], buffer[smallest] );
            i = smallest;
        } else {
            break;
        }
    }
}

template<class T, class IsBigger>
void Heap<T, IsBigger>::siftUp( int index ) {
    while ( index > 0 ) {
        int parent = ( index - 1 ) / 2;
        if( isBigger( buffer[index], buffer[parent]) )  
            return;
        std::swap( buffer[index], buffer[parent] );
        index = parent;
    }  
}

template<class T, class IsBigger>
void Heap<T, IsBigger>::buildHeap() {
    if( Size() == 0 || Size() == 1 ) {
        return;
    }

    for( int i = Size() / 2 - 1; i >= 0; --i ) {
        siftDown( i );
    }
}

template<class T, class IsBigger>
void Heap<T, IsBigger>::grow() {
    int newBufferSize = ( bufferSize == 0 ) ? 1 : 2 * bufferSize;

    T* newBuffer = new T[newBufferSize]; 

    for ( int i = 0; i < bufferSize; ++i ) {
        newBuffer[i] = buffer[i];
    }

    delete[] buffer; 
    buffer = newBuffer; 
    bufferSize = newBufferSize; 
}

template<class T, class IsBigger>
bool Heap<T, IsBigger>::isEmpty() {
    return size == 0;
}

template<class T, class IsBigger>
void Heap<T, IsBigger>::Insert( const T& element ) {
    ++size;
    if( size >= bufferSize ) {
        grow();
    }
    buffer[size - 1] = element;
    siftUp ( size - 1 );
   
}

template<class T, class IsBigger>
T Heap<T, IsBigger>::ExtractMin( ) {
    assert( !isEmpty() );
    --size;
    T result = buffer[0];
    buffer[0] = buffer[size];
    if( !isEmpty() ) {
        siftDown( 0 );
    }
    --bufferSize;
    return result;
}

template<class T, class IsBigger>
const T& Heap<T, IsBigger>::PeekMin() const { 
    return buffer[0];
}

template<class T, class IsBigger>
int Heap<T, IsBigger>::Size() const {
    return size;
}

struct User {
    int id;
    int attendance;

    bool operator<( const User& anotherUser ) const {
        return attendance < anotherUser.attendance;
    }
    bool operator>( const User& anotherUser ) const {
        return attendance > anotherUser.attendance;
    }
};

int main() {
    int n, k;
    std::cin >> n >> k;
    assert( ( n > 0 ) && ( k <= n ) );

    User* users = new User[n];
    
    for ( int i = 0; i < n; ++i ) {
        std::cin >> users[i].id >> users[i].attendance;
    }

    Heap<User> userHeap(users, k);

    for (int i = k; i < n; ++i) {
        if ( users[i].attendance > userHeap.PeekMin().attendance ) {
            userHeap.ExtractMin();
            userHeap.Insert(users[i]);
        }
    }
    
    for (int i = 0; i < k; i++){
        std::cout << userHeap.PeekMin().id << " ";
        userHeap.ExtractMin();
    }

    delete[] users;
    return 0;
}