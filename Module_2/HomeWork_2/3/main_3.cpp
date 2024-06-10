#include <vector>
#include <iostream>
#include <cstdint>
#include <queue>
#include <string>
#include <sstream>

template<class T>
class Comparator {
public:
    bool operator()( const T& l, const T& r ) const { return l < r; }
};

template<class T, class Compare = Comparator<T>>
class BTree {
public:
    BTree( size_t _t, const Compare& _cmp = Compare() ) : root( nullptr ), t( _t ), cmp( _cmp ) {}
    ~BTree() { delete root; }

    BTree( const BTree& ) = delete;
    BTree& operator=( const BTree& ) = delete;

    void Insert( const T& key );
    void PrintByLevels();

private:
    struct Node {
        bool IsLeaf;
        std::vector<T> Keys;
        std::vector<Node*> Children;

        Node( bool isLeaf ) : IsLeaf( isLeaf ) {}
        ~Node() { for ( Node* child : Children ) { delete child; } }
    };

    Node* root;
    size_t t;
    Compare cmp;

    bool isNodeFull( Node* node ) { return node->Keys.size() == 2 * t - 1; }
    void splitChild( Node* node, int pos );
    void insertNonFull( Node* node, const T& key );
};

template<class T, class Compare>
void BTree<T, Compare>::Insert( const T& key ) {
    if ( root == nullptr ) {
        root = new Node( true );
    }

    if ( isNodeFull( root ) ) {
        Node* newRoot = new Node( false );
        newRoot->Children.push_back( root );
        splitChild( newRoot, 0 );
        root = newRoot;
    }

    insertNonFull( root, key );
}

template<class T, class Compare>
void BTree<T, Compare>::insertNonFull( Node* node, const T& key ) {
    if ( node->IsLeaf ) {
        node->Keys.resize( node->Keys.size() + 1 );
        int pos = node->Keys.size() - 2;
        while ( pos >= 0 && cmp( key, node->Keys[pos] ) ) {
            node->Keys[pos + 1] = node->Keys[pos];
            --pos;
        }
        node->Keys[pos + 1] = key;
    } else {
        int pos = node->Keys.size() - 1;
        while ( pos >= 0 && cmp( key, node->Keys[pos] ) ) {
            --pos;
        }
        ++pos;
        if ( isNodeFull( node->Children[pos] ) ) {
            splitChild( node, pos );
            if ( cmp( node->Keys[pos], key ) ) {
                ++pos;
            }
        }
        insertNonFull( node->Children[pos], key );
    }
}

template<class T, class Compare>
void BTree<T, Compare>::splitChild( Node* node, int pos ) {
    Node* child = node->Children[pos];
    Node* newChild = new Node( child->IsLeaf );

    newChild->Keys.assign( child->Keys.begin() + t, child->Keys.end() );
    child->Keys.resize( t - 1 );

    if ( !child->IsLeaf ) {
        newChild->Children.assign( child->Children.begin() + t, child->Children.end() );
        child->Children.resize( t );
    }

    node->Keys.insert( node->Keys.begin() + pos, child->Keys[t - 1] );
    node->Children.insert( node->Children.begin() + pos + 1, newChild );
}

template<class T, class Compare>
void BTree<T, Compare>::PrintByLevels() {
    if ( root == nullptr ) {
        return;
    }

    std::queue<Node*> levelQueue;
    levelQueue.push( root );

    while ( !levelQueue.empty() ) {
        size_t levelSize = levelQueue.size();
        for ( size_t i = 0; i < levelSize; ++i ) {
            Node* node = levelQueue.front();
            levelQueue.pop();
            for ( const auto& key : node->Keys ) {
                std::cout << key << " ";
            }
            if ( !node->IsLeaf ) {
                for ( auto child : node->Children ) {
                    levelQueue.push( child );
                }
            }
        }
        std::cout << std::endl;
    }
}

int main() {
    size_t t;
    std::cin >> t;
    std::cin.ignore();

    BTree<uint32_t> tree( t );

    std::string line;
    std::getline( std::cin, line );
    std::istringstream iss( line ); 

    uint32_t key;
    while ( iss >> key ) {
        tree.Insert( key );
    }

    tree.PrintByLevels();

    return 0;
}
