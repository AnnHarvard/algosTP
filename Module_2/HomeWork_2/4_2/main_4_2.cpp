//Задача 4. Использование АВЛ-дерева (5 баллов)
// Требование для всех вариантов Задачи 4
// Решение должно поддерживать передачу функции сравнения снаружи.

// 4_2. Порядковые статистики. Дано число N и N строк. Каждая строка содержит команду добавления 
// или удаления натуральных чисел, а также запрос на получение k-ой порядковой статистики.
// Команда добавления числа A задается положительным числом A, 
// команда удаления числа A задается отрицательным числом “-A”. 
// Запрос на получение k-ой порядковой статистики задается числом k. 
// Требования: скорость выполнения запроса - O(log n).

#include<stack>
#include<iostream>
#include<cassert>

template<class T>
class Comparator {
public:
	bool operator()( const T& l, const T& r ) const { return l < r; }
};

template<class T, class Compare = Comparator<T>>
class AVLTree {
public:
	AVLTree( const Compare& _cmp = Compare() ) : root( nullptr ), cmp( _cmp ) {}
	~AVLTree();

    AVLTree( AVLTree& ) = delete;
    AVLTree& operator=( const AVLTree& ) = delete;

	void Add( const T& key );
	bool Has( const T& key ) const;
    void RemoveNode( const T& key );

    T FindKStat(const int k) const;

	void InOrder( void (*visit)(const T& key) ) const;

private:
	struct Node {
		T Key;
		Node* Left;
		Node* Right;
        int Height;
        int Size;

		Node( const T& key ) : Key( key ), Left( nullptr ), Right( nullptr ), Height( 1 ), Size( 1 ) {}
	};
	Node* root;
	Compare cmp;

	void destroyTree( Node* node );
	void add( Node*& node, const T& key );
	bool has( Node* node, const T& key ) const;
    void removeNode( Node*& node, const T& key );
    T findKStat( Node* node, const int k ) const;
	void inOrder( Node* node, void (*visit)(const T& key) ) const;
    int getHeight( Node* node ) const;
    void fixHeightAndSize( Node* node );
    void rightRotate( Node*& node );
    void leftRotate (Node*& node );
    void balance( Node*& node );
    int balanceFactor( Node* node ) const;

    Node* findAndRemoveMinNode( Node* node, Node* & minNode );

};

template<class T, class Compare>
AVLTree<T, Compare>::~AVLTree()
{
	destroyTree( root );
}

template<class T, class Compare>
void AVLTree<T, Compare>::destroyTree( Node* node ) {
    if( node == nullptr ) {
		return;
	}

	destroyTree( node->Left );
	destroyTree( node->Right );
	delete node;
}

template<class T, class Compare>
void AVLTree<T, Compare>::Add( const T& key )
{
	add( root, key );
}

template<class T, class Compare>
void AVLTree<T, Compare>::add( Node*& node, const T& key) {
    if( node == nullptr ) {
		node = new Node( key );
		return;
	}

	if( cmp( key, node->Key ) ) {
		add( node->Left, key );
	} else {
		add( node->Right, key );
	}

	balance( node );
}

template<class T, class Compare>
bool AVLTree<T, Compare>::Has( const T& key ) const
{
	return has( root, key );
}

template<class T, class Compare>
bool AVLTree<T, Compare>::has(Node* node, const T& key) const {
    if( node == nullptr ) {
		return false;
	}

	if( node->Key == key ) {
		return true;
	}

	if( cmp( key, node->Key ) ) {
		return has( node->Left, key );
	}
	return has( node->Right, key );
}

template<class T, class Compare>
void AVLTree<T, Compare>::RemoveNode(const T& key) {
    removeNode(root, key);
}

template<class T, class Compare>
typename AVLTree<T, Compare>::Node* AVLTree<T, Compare>::findAndRemoveMinNode( Node* node, Node* & minNode )
{
    if ( node->Left == nullptr ) {
        minNode = node;

        return node->Right;
    }

    node->Left = findAndRemoveMinNode( node->Left, minNode );

    balance( node );

    return node;
}

template<class T, class Compare>
void AVLTree<T, Compare>::removeNode( Node*& node, const T& key ) {
    if ( node == nullptr ) {
        return;
    }

    if ( cmp( key, node->Key ) ) {
        removeNode( node->Left, key );
    } else if ( cmp( node->Key, key ) ) {
        removeNode( node->Right, key );
    } else {
        Node* left = node->Left;
        Node* right = node->Right;
        delete node;

        if ( right == nullptr ) {
            node = left;
            return;
        }

        Node * minNode = nullptr;
        right = findAndRemoveMinNode( right, minNode );

        minNode->Right = right;
        minNode->Left = left;
        
        balance( minNode );
        node = minNode;
    }

    if (node != nullptr) {
        balance(node);
    }
}

template<class T, class Compare>
T AVLTree<T, Compare>::FindKStat(const int k) const {
    return findKStat(root, k);
}

template<class T, class Compare>
T AVLTree<T, Compare>::findKStat(Node* node, int k) const {
    if (node == nullptr || k < 0 || k >= root->Size) {
        throw std::out_of_range("Invalid k");
    }

    int leftSize = (node->Left != nullptr) ? node->Left->Size : 0;

    if (k == leftSize ) {
        return node->Key;
    } else if (k < leftSize ) {
        return findKStat(node->Left, k);
    } else {
        return findKStat(node->Right, k - leftSize - 1);
    }
}


template<class T, class Compare>
void AVLTree<T, Compare>::InOrder( void (*visit)(const T& key) ) const
{
	inOrder( root, visit );
}

template<class T, class Compare>
void AVLTree<T, Compare>::inOrder( Node* node, void (*visit)(const T& key) ) const {
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

template<class T, class Compare>
int AVLTree<T, Compare>::getHeight( Node* node ) const {
    return node ? node->Height : 0;
}

template<class T, class Compare>
void AVLTree<T, Compare>::fixHeightAndSize( Node* node ) {
    int leftHeight = getHeight( node->Left );
    int rightHeight = getHeight( node->Right );
    node->Height = ( leftHeight > rightHeight ? leftHeight : rightHeight ) + 1;

    int leftSize = ( node->Left != nullptr ) ? node->Left->Size : 0;
    int rightSize = ( node->Right != nullptr ) ? node->Right->Size : 0;
    node->Size = leftSize + rightSize + 1;
} 

template<class T, class Compare>
void AVLTree<T, Compare>::balance( Node*& node ) {
    fixHeightAndSize( node );

    if ( balanceFactor( node ) == 2 ) {
        if ( balanceFactor( node->Right ) < 0 ) {
            rightRotate( node->Right );
        }
        leftRotate( node );
    } else if ( balanceFactor( node ) == -2 ) {
        if ( balanceFactor( node->Left ) > 0 ) {
            leftRotate( node->Left );
        }
        rightRotate( node );
    }
}

template<class T, class Compare>
void AVLTree<T, Compare>::rightRotate( Node*& node ) {
    Node* leftChild = node->Left;
    node->Left = leftChild->Right;
    leftChild->Right = node;
    fixHeightAndSize( node );
    fixHeightAndSize( leftChild );
    node = leftChild;
}

template<class T, class Compare>
void AVLTree<T, Compare>::leftRotate( Node*& node ) {
    Node* rightChild = node->Right;
    node->Right = rightChild->Left;
    rightChild->Left = node;
    fixHeightAndSize( node );
    fixHeightAndSize( rightChild );
    node = rightChild;
}

template<class T, class Compare>
int AVLTree<T, Compare>::balanceFactor( Node* node ) const {
    return getHeight( node->Right ) - getHeight( node->Left );
}


int main()
{
	AVLTree<int> tree;

	int N, a, k;
    std::cin >> N;

    for (int i = 0; i < N; ++i) {
        std::cin >> a >> k;
        a >= 0 ? tree.Add( a ) : tree.RemoveNode( abs( a ) );
        std::cout << tree.FindKStat( k ) << '\n'; 
    }

	return 0;
}

