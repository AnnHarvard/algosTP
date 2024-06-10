// Задача 2. Порядок обхода (4 балла)
// Обязательная задача
// Дано число N < 106 и последовательность целых чисел из [-231..231] длиной N.
// Требуется построить бинарное дерево, заданное наивным порядком вставки.
// Т.е., при добавлении очередного числа K в дерево с корнем root, если root→Key ≤ K, 
// то узел K добавляется в правое поддерево root; иначе в левое поддерево root.
// Требования: Рекурсия запрещена. Решение должно поддерживать передачу функции сравнения снаружи.

// 2_1. Выведите элементы в порядке in-order (слева направо).

#include<stack>
#include<cassert>
#include<iostream>

template<class T>
class Comparator {
public:
	bool operator()( const T& l, const T& r ) { return l < r; }
};

template<class T, class Compare = Comparator<T>>
class BinaryTree {
public:
	BinaryTree( const Compare& _cmp = Compare() ) : root( nullptr ), cmp( _cmp ) {}
	~BinaryTree();

    BinaryTree( BinaryTree& ) = delete;
    BinaryTree& operator=( const BinaryTree& ) = delete;

	void Add( const T& key );
	bool Has( const T& key ) const;

	void InOrder( void (*visit)(const T& key) ) const;

private:
	struct Node {
		T Key;
		Node* Left;
		Node* Right;

		Node( const T& key ) : Key( key ), Left( nullptr ), Right( nullptr ) {}
	};
	Node* root;
	Compare cmp;

	void destroyNode( Node* node );
	void add( Node*& node, const T& key );
	bool has( Node* node, const T& key ) const;
	void inOrder( Node* node, void (*visit)(const T& key) ) const;
};

template<class T, class Compare>
BinaryTree<T, Compare>::~BinaryTree()
{
	destroyNode( root );
}

template<class T, class Compare>
void BinaryTree<T, Compare>::destroyNode( Node* node ) {
    if ( node == nullptr ) {
        return;
    }

    std::stack<Node*> stack;
    stack.push( node );

    while ( !stack.empty() ) {
        Node* currentNode = stack.top();
        stack.pop();
        if ( currentNode->Left != nullptr) stack.push( currentNode->Left );
        if ( currentNode->Right != nullptr) stack.push( currentNode->Right );
        delete currentNode;
    }
}

template<class T, class Compare>
void BinaryTree<T, Compare>::Add( const T& key )
{
	add( root, key );
}

template<class T, class Compare>
void BinaryTree<T, Compare>::add( Node*& node, const T& key) {
    Node* newNode = new Node( key );

    if ( node == nullptr) {
        node = newNode;
        return;
    }

    Node* currentNode = node;
    Node* parentNode = nullptr;

    while ( currentNode != nullptr) {
        parentNode = currentNode;
        if ( cmp( key, currentNode->Key ) ) {
            currentNode = currentNode->Left;
        } else {
            currentNode = currentNode->Right;
        }
    }

    if ( cmp( key, parentNode->Key ) ) {
        parentNode->Left = newNode;
    } else {
        parentNode->Right = newNode;
    }
}

template<class T, class Compare>
bool BinaryTree<T, Compare>::Has( const T& key ) const
{
	return has( root, key );
}

template<class T, class Compare>
bool BinaryTree<T, Compare>::has(Node* node, const T& key) const {
    Node* currentNode = node;

    if ( currentNode == nullptr ) {
        return false;
    }

    if ( currentNode->Key == key ) {
        return true;
    }

    while ( currentNode != nullptr ) {
        if ( cmp( key, currentNode->Key ) ) {
            currentNode = currentNode->Left;
        } else {
            currentNode = currentNode->Right;
        }
    }

    return false;
}


template<class T, class Compare>
void BinaryTree<T, Compare>::InOrder( void (*visit)(const T& key) ) const
{
	inOrder( root, visit );
}

template<class T, class Compare>
void BinaryTree<T, Compare>::inOrder( Node* node, void (*visit)(const T& key) ) const {
    Node* currentNode = node;
    std::stack<Node*> stack;

    if( currentNode == nullptr ) {
		return;
	}

    while ( currentNode != nullptr || !stack.empty() ) {
        while ( currentNode != nullptr ) {
            stack.push( currentNode );
            currentNode = currentNode->Left;
        }

        currentNode = stack.top();
        stack.pop();
        visit( currentNode->Key );
        currentNode = currentNode->Right;
    }
}


int main()
{
	BinaryTree<int> tree;

	int N;
    std::cin >> N;

    for (int i = 0; i < N; ++i) {
        int key;
        std::cin >> key;
        tree.Add(key);
    }

	tree.InOrder( []( const int& key ) { std::cout << key << " "; } );
	return 0;
}