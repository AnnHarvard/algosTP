#include <iostream>
#include <string>
#include <vector>

struct Node {
    int key;
    int height;
    Node* left;
    Node* right;

    Node(int k) : key(k), height(1), left(nullptr), right(nullptr) {}
};

int height(Node* node) {
    return node ? node->height : 0;
}

int balanceFactor(Node* node) {
    return height(node->right) - height(node->left);
}

void fixHeight(Node* node) {
    int leftHeight = height(node->left);
    int rightHeight = height(node->right);
    node->height = (leftHeight > rightHeight ? leftHeight : rightHeight) + 1;
}

Node* rotateRight(Node* node) {
    Node* pivot = node->left;
    node->left = pivot->right;
    pivot->right = node;
    fixHeight(node);
    fixHeight(pivot);
    return pivot;
}

Node* rotateLeft(Node* node) {
    Node* pivot = node->right;
    node->right = pivot->left;
    pivot->left = node;
    fixHeight(node);
    fixHeight(pivot);
    return pivot;
}

Node* balance(Node* node) {
    fixHeight(node);
    if (balanceFactor(node) == 2) {
        if (balanceFactor(node->right) < 0)
            node->right = rotateRight(node->right);
        return rotateLeft(node);
    }
    if (balanceFactor(node) == -2) {
        if (balanceFactor(node->left) > 0)
            node->left = rotateLeft(node->left);
        return rotateRight(node);
    }
    return node;
}

Node* insert(Node* node, int key) {
    if (!node)
        return new Node(key);
    if (key < node->key)
        node->left = insert(node->left, key);
    else if (key > node->key)
        node->right = insert(node->right, key);
    return balance(node);
}

Node* findMin(Node* node) {
    return node->left ? findMin(node->left) : node;
}

Node* removeMin(Node* node) {
    if (!node->left)
        return node->right;
    node->left = removeMin(node->left);
    return balance(node);
}

Node* remove(Node* node, int key) {
    if (!node)
        return nullptr;
    if (key < node->key)
        node->left = remove(node->left, key);
    else if (key > node->key)
        node->right = remove(node->right, key);
    else {
        Node* leftChild = node->left;
        Node* rightChild = node->right;
        delete node;
        if (!rightChild)
            return leftChild;
        Node* minNode = findMin(rightChild);
        minNode->right = removeMin(rightChild);
        minNode->left = leftChild;
        return balance(minNode);
    }
    return balance(node);
}

bool exists(Node* node, int key) {
    if (!node)
        return false;
    if (key == node->key)
        return true;
    if (key < node->key)
        return exists(node->left, key);
    else
        return exists(node->right, key);
}

int findNext(Node* node, int key) {
    int next = -1;
    while (node) {
        if (node->key > key) {
            next = node->key;
            node = node->left;
        } else
            node = node->right;
    }
    return next;
}

int findPrev(Node* node, int key) {
    int prev = -1;
    while (node) {
        if (node->key < key) {
            prev = node->key;
            node = node->right;
        } else
            node = node->left;
    }
    return prev;
}

int main() {
    Node* root = nullptr;
    std::vector<std::string> operations;
    std::vector<int> keys;
    std::string line;
    while (true) {
        std::getline(std::cin, line);
        if (line.empty())
            break;
        std::stringstream ss(line);
        std::string operation;
        int key;
        ss >> operation >> key;
        operations.push_back(operation);
        keys.push_back(key);
    }
    for (int i = 0; i < operations.size(); ++i) {
        std::string operation = operations[i];
        int key = keys[i];
        if (operation == "insert")
            root = insert(root, key);
        else if (operation == "delete")
            root = remove(root, key);
        else if (operation == "exists")
            std::cout << (exists(root, key) ? "true" : "false") << std::endl;
        else if (operation == "next")
            std::cout << (findNext(root, key) == -1 ? "none" : std::to_string(findNext(root, key))) << std::endl;
        else if (operation == "prev")
            std::cout << (findPrev(root, key) == -1 ? "none" : std::to_string(findPrev(root, key))) << std::endl;
    }
    return 0;
}