// 1. Хеш-таблица

// Реализуйте структуру данных типа “множество строк” 
// на основе динамической хеш-таблицы с открытой адресацией. 
// Хранимые строки непустые и состоят из строчных латинских букв.
// Хеш-функция строки должна быть реализована с помощью 
// вычисления значения многочлена методом Горнера. 
// Начальный размер таблицы должен быть равным 8-ми. 
// Перехеширование выполняйте при добавлении элементов в случае, 
// когда коэффициент заполнения таблицы достигает 3/4.
// Структура данных должна поддерживать операции добавления строки в множество, 
// удаления строки из множества и проверки принадлежности данной строки множеству.

// Вариант 2. Для разрешения коллизий используйте двойное хеширование.

// Формат ввода
// Каждая строка входных данных задает одну операцию над множеством. 
// Запись операции состоит из типа операции и следующей за ним через пробел строки, 
// над которой проводится операция. 
// Тип операции – один из трех символов: 
//         + означает добавление данной строки в множество; 
//         - означает удаление строки из множества; 
//         ? означает проверку принадлежности данной строки множеству. 
// При добавлении элемента в множество НЕ ГАРАНТИРУЕТСЯ, что он отсутствует в этом множестве. 
// При удалении элемента из множества НЕ ГАРАНТИРУЕТСЯ, что он присутствует в этом множестве.

// Формат вывода
// Программа должна вывести для каждой операции одну из двух строк OK или FAIL.
// Для операции '?': OK, если элемент присутствует во множестве. FAIL иначе.
// Для операции '+': FAIL, если добавляемый элемент уже присутствует во множестве и потому не может быть добавлен. OK иначе.
// Для операции '-': OK, если элемент присутствовал во множестве и успешно удален. FAIL иначе.


#include <vector>
#include <string>
#include <cassert>
#include <iostream>

#define FILLING_COEFF 0.75

using namespace std;

template<class T, class H1, class H2>
class HashTable {
public:
    // Все ячейки в состоянии Empty
	HashTable( int initialSize, const H1& hasherFirst, const H2& hasherSecond );

    // Empty: return false
	// Key: key == Key ? return true : пробируемся дальше
	// Deleted: пробируемся дальше
	bool Has( const T& key );

    // Empty: firstDeleted == -1 ?
	//  (записываем ключ в текущую ячейку, помечаем ячейку Key и return true) :
	//  (записываем ключ в firstDeleted, помечаем ячейку Key и return true)
	// Key: key == Key ? return false : пробируемся дальше
	// Deleted: встречали ли Deleted раньше ? пробируемся дальше : запоминаем индекс в firstDeleted
	bool Add( const T& key );

    // Empty: return false
	// Key: key == Key ? помечаем ячейку Deleted и return true : пробируемся дальше
	// Deleted: пробируемся дальше
	bool Delete( const T& key );

private:
	H1 hasherFirstFunc;
    H2 hasherSecondFunc;
	enum CellState { Empty, Key, Deleted }; 
	struct HashTableCell {
		T Key;
		unsigned int Hash;
		CellState State;
		HashTableCell() : Hash( 0 ), State( Empty ) {}
	};
	std::vector<HashTableCell> table;

	unsigned int keysCount;

	void growTable();
};

template<class T, class H1, class H2>
HashTable<T, H1, H2>::HashTable( int initialSize, const H1& _hasherFirst, const H2& _hasherSecond ) :
	hasherFirstFunc( _hasherFirst ),
    hasherSecondFunc( _hasherSecond ),
	table( initialSize ),
	keysCount( 0 )
{
}

template<class T, class H1, class H2>
bool HashTable<T, H1, H2>::Has( const T& key )
{
	unsigned int hasherFirst = hasherFirstFunc( key );
    unsigned int hasherSecond = hasherSecondFunc( key );
    unsigned int hasher;
    for ( int i = 0; i < table.size(); ++i ) {
        hasher = ( hasherFirst + i * hasherSecond ) % table.size();
        if ( table[hasher].State == Empty ) {
            return false; 
        } else if ( table[hasher].State == Key && table[hasher].Key == key ) {
            return true;
        }
    }
    return false;
}

template<class T, class H1, class H2>
bool HashTable<T, H1, H2>::Add( const T& key )
{
	if( keysCount / table.size() > FILLING_COEFF )
	{
		growTable();
	}

    unsigned int hasherFirst = hasherFirstFunc( key );
	unsigned int hasherSecond = hasherSecondFunc( key );
	int firstDeleted = -1;
	unsigned int hasher;

	for ( int i = 0; i < table.size(); ++i ) {
        hasher = ( hasherFirst + i * hasherSecond ) % table.size();
		if ( table[hasher].State == Empty ) {
			if ( firstDeleted == -1 ) {
				table[hasher].Key = key;
				table[hasher].State = Key;
			} else {
				table[firstDeleted].Key = key;
				table[firstDeleted].State = Key;
			}
            table[hasher].Hash = hasher;
			++keysCount;
			return true;
		} else if ( table[hasher].State == Key && table[hasher].Key == key ) {
			return false;
		} else if ( table[hasher].State == Deleted && firstDeleted == -1 ) {
			firstDeleted = hasher;
		}
	}

	if ( firstDeleted != -1 ) {
		table[firstDeleted].Key = key;
        table[firstDeleted].Hash = hasher;
		table[firstDeleted].State = Key;
		++keysCount;
		return true;
	}
    
	return false;
}

template<class T, class H1, class H2>
bool HashTable<T, H1, H2>::Delete( const T& key )
{
    unsigned int hasherFirst = hasherFirstFunc( key );
	unsigned int hasherSecond = hasherSecondFunc( key );
	unsigned int hasher;
	for ( int i = 0; i < table.size(); ++i ) {
        hasher = ( hasherFirst + i * hasherSecond ) % table.size();

		if ( table[hasher].State == Empty ) {
			return false; 
		} else if ( table[hasher].State == Key && table[hasher].Key == key ) {
			table[hasher].State = Deleted; 
			--keysCount;
			return true;
		}
	}

	return false;
}

template<class T, class H1, class H2>
void HashTable<T, H1, H2>::growTable()
{
    std::vector<HashTableCell> newTable( table.size() * 2 ); 

    for ( int i = 0; i < table.size(); ++i ) {
        if ( table[i].State == Key ) {
            T key = table[i].Key;
            unsigned int hasherFirst = hasherFirstFunc( key );
            unsigned int hasherSecond = hasherSecondFunc ( key );
            unsigned int hasher; 
            for ( int j = 0; j < newTable.size(); ++j ) {
                hasher = ( hasherFirst + j * hasherSecond ) % newTable.size();
                if ( newTable[hasher].State == Empty ) {
                    newTable[hasher].Key = key;
                    newTable[hasher].State = Key;
                    newTable[hasher].Hash = hasher;
                    break;
                }
            }
        }
    }
	table = move( newTable );
}   

struct StringHasherFirst {
	unsigned int operator()( const string& key ) const
	{
		unsigned int hash = 0;
		for ( unsigned int i = 0; i < key.size(); ++i )
		{
			hash = hash * 11 + key[i];
		}
		return hash;
	}
};

struct StringHasherSecond {
	unsigned int operator()( const string& key ) const
	{
		unsigned int hash = 0;
		for ( unsigned int i = 0; i < key.size(); ++i )
		{
			hash = hash * 13 + key[i];
		}
		return hash * 2 + 1;
	}
};

int main()
{
	StringHasherFirst hasherFirst;
	StringHasherSecond hasherSecond;
    
	HashTable<std::string, StringHasherFirst, StringHasherSecond> hashTable( 8, hasherFirst, hasherSecond );
	char operation = 0;
	std::string word;
	while( std::cin >> operation >> word ) {
		switch( operation ) {
			case '+':
				std::cout << ( hashTable.Add( word ) ? "OK" : "FAIL" ) << std::endl;
				break;
			case '-':
				std::cout << (hashTable.Delete( word ) ? "OK" : "FAIL") << std::endl;
				break;
			case '?':
				std::cout << (hashTable.Has( word ) ? "OK" : "FAIL") << std::endl;
				break;
		}
	}

	return 0;
}