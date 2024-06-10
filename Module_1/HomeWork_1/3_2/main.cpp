// Задача № 3 (4 балла)
// Во всех задачах из следующего списка следует написать структуру данных,
//                                 обрабатывающую команды push* и pop*.
// Формат входных данных.
// В первой строке количество команд n. n ≤ 1000000.
// Каждая команда задаётся как 2 целых числа: a b.
// a = 1 - push front
// a = 2 - pop front
// a = 3 - push back
// a = 4 - pop back
// Команды добавления элемента 1 и 3 заданы с неотрицательным параметром b.
// Для очереди используются команды 2 и 3. Для дека используются все четыре команды.
// Если дана команда pop*, то число b - ожидаемое значение. 
//                                Если команда pop вызвана для пустой структуры данных, то ожидается “-1”. 
// Формат выходных данных.
// Требуется напечатать YES - если все ожидаемые значения совпали. 
//                                Иначе, если хотя бы одно ожидание не оправдалось, то напечатать NO.


// 3_2. Реализовать дек с динамическим зацикленным буфером (на основе динамического массива).
// Требования: Дек должен быть реализован в виде класса.


#include<iostream>
#include<cassert>
#include<sstream>

template <typename T>   
class Deque{
public:
    Deque() : bufferSize( 4 ), leftSide( 0 ), rightSide( 0 ), size( 0 )
    {
        dynamicBuffer = new T[bufferSize];
    }

    ~Deque() {
        if( dynamicBuffer != nullptr ) {
            delete[] dynamicBuffer; 
            size = 0;
            leftSide = 0;
            rightSide = 0; 
        }  
    }

    Deque( Deque& ) = delete;
    Deque& operator=( const Deque& ) = delete;

    void PushFront( T data ) {
        if( size == bufferSize ) {
            ExpandBufferSize();
        }

        if( IsEmpty() ) {
            dynamicBuffer[0] = data;
            // leftSide = rightSide = 0;
        } else {
            leftSide = (leftSide - 1 + bufferSize) % bufferSize; 
            dynamicBuffer[leftSide] = data;         
        }
        ++size;
    }

    void PushBack( T data ) {
       if( size == bufferSize ) {
            ExpandBufferSize();
        }

        if(IsEmpty() ) {
            dynamicBuffer[0] = data;
            leftSide = rightSide = 0;
        } else {
            rightSide = (rightSide + 1) % bufferSize; 
            dynamicBuffer[rightSide] = data;         
        }
        ++size; 
    }

    T PopFront() {
        assert( size > 0 );

        T returnData = dynamicBuffer[leftSide];
        leftSide = (leftSide + 1) % bufferSize;
        --size;
        if( IsEmpty() ) {
            leftSide = rightSide = 0;
        }
        return returnData;
    }

    T PopBack() {
        assert( size > 0 );

        T returnData = dynamicBuffer[rightSide];
        rightSide = (rightSide - 1 + bufferSize) % bufferSize;
        --size;
        if( IsEmpty() ) {
            leftSide = rightSide = 0;
        }
        return returnData;
    }

    int Size() const {
        return size;
    }

    bool IsEmpty() const {
        return size == 0;
    }

    void ExpandBufferSize() {
        T* newDynamicBuffer = new T[bufferSize * 2];
        for( int i = 0; i < size; ++i ) {
            int indexInBuffer = ( leftSide + i ) % bufferSize;
            newDynamicBuffer[i] = dynamicBuffer[indexInBuffer];
        } 
        delete[] dynamicBuffer;
        dynamicBuffer = newDynamicBuffer;
        bufferSize = bufferSize * 2;
        leftSide = 0;
        rightSide = size - 1;
    }

private:
    T* dynamicBuffer;
    int bufferSize;
    int leftSide; //head
    int rightSide; //tail
    int size;
};

void run( std::istream& input, std::ostream& output ) {
    Deque<int> intDeque;
    int n = 0;
    input >> n;
    bool result = true;
    for( int i = 0; i < n; ++i ) {
        int command = 0;
        int data = 0;
        input >> command >> data;
        switch ( command ) {
            case 1:
                intDeque.PushFront( data );
                break;
            case 2:
                if( intDeque.Size() > 0 ) {
                    result = result && intDeque.PopFront() == data;
                } else {
                    result = result && data == -1;
                }
                break;
            case 3:
                intDeque.PushBack( data );
                break;
            case 4:
                if( intDeque.Size() > 0 ) {
                    result = result && intDeque.PopBack() == data;
                } else {
                    result = result && data == -1;
                }
                break;
            default:
                assert( false );
        }
    }
    output << (result ? "YES" : "NO");
}

void testDeque()
{
    {
        std::stringstream input;
        std::stringstream output;
        input << "3 1 44 3 50 2 44";
        run( input, output );
        assert( output.str() == "YES" );
    }
    {
        std::stringstream input;
        std::stringstream output;
        input << "2 2 -1 1 10";
        run( input, output );
        assert( output.str() == "YES" );
    }
    {
        std::stringstream input;
        std::stringstream output;
        input << "2 3 44 4 66";
        run( input, output );
        assert( output.str() == "NO" );
    }
}

int main() {
    run( std::cin, std::cout );
    // testDeque();
    return 0;
}

