//Owner: Mark

#pragma once

#include <iostream>
#include <fstream>
#include <type_traits>
#include "word.hpp"

//implementation derived from GeeksForGeeks
template <typename T>
class AVL {
    struct Node {
        T data = nullptr;
        Node* left = nullptr;
        Node* right = nullptr;
        int height = 1;
        explicit Node(T& d):data(d) {}
        ~Node() {
            delete left;
            delete right;
        }
    };
    Node* head = nullptr;
    T* curr = nullptr;
    int totalNodes = 0;

    int getHeight(Node* node);
    Node* insert(Node* node, T& data);
    Node* rightRotate(Node* x);
    Node* leftRotate(Node* x);
    void inOrder(Node* node);
    T* search(Node* root, const T& data);
    void copyNodes(Node* node);
    void nodeToFile(Node* node, int depth, std::ofstream& out);

public:
    //Add lvalue and rvalue support
    AVL() = default;
    AVL(const AVL<T>& tree);
    ~AVL();

    AVL& operator=(const AVL& tree);

    void insert(T data);
    T* getRecent();
    int getTotalNodes();
    void inOrder();
    T* search(const T& data);
    void treeToFile(std::ofstream& file);
    void treeFromfile(std::ifstream& filename);
    void clear();

    template<typename Func>
    std::vector<T*> getCount(int count, Func f);
};

template<typename T>
AVL<T>::AVL(const AVL<T>& tree) {
    *this = tree;
}

template<typename T>
AVL<T>::~AVL() {
    //recursive delete of tree
    delete head;
    head = nullptr;
    curr = nullptr;
    totalNodes = 0;
}

template<typename T>
AVL<T>& AVL<T>::operator=(const AVL<T>& tree) {
    if(this != &tree) return *this;

    copyNodes(tree.head);
    totalNodes = tree.totalNodes;

    return *this;
}

template<typename T>
int AVL<T>::getHeight(Node* node) {
    if(node == nullptr) return 0;
    else return node->height;
}

template<typename T>
void AVL<T>::insert(T data) {
    head = insert(head, data);
}

template<typename T>
typename AVL<T>::Node* AVL<T>::insert(Node* node, T& data) {
    if(node == nullptr) {
        node = new Node(data);
        curr = &node->data;
        totalNodes++;
        return node;
    }
    else if(data == node->data) {
        curr = &node->data;
        return node;
    }
    else if(data < node->data) {
        node->left = insert(node->left, data);
    }
    else if(data > node->data) {
        node->right = insert(node->right, data);
    }

    int balance = getHeight(node->left) - getHeight(node->right);

    //Left Left Case
    if (balance > 1 && data < node->left->data) {
        node = rightRotate(node);
    }

    //Right Right Case
    else if (balance < -1 && data > node->right->data) {
        node = leftRotate(node);
    }

    //Left Right Case
    else if (balance > 1 && data > node->left->data)
    {
        node->left = leftRotate(node->left);
        node = rightRotate(node);
    }

    //Right Left Case
    else if (balance < -1 && data < node->right->data)
    {
        node->right = rightRotate(node->right);
        node = leftRotate(node);
    }

    //Update balance
    node->height = 1 + std::max(getHeight(node->left), getHeight(node->right));
    return node;
}

template<typename T>
typename AVL<T>::Node* AVL<T>::rightRotate(Node* x) {
    Node* y = x->left;

    x->left = y->right;
    y->right = x;

    // Update heights
    y->height = std::max(getHeight(y->left), getHeight(y->right)) + 1;
    x->height = std::max(getHeight(x->left), getHeight(x->right)) + 1;

    // Return new root
    return y;
}

template<typename T>
typename AVL<T>::Node* AVL<T>::leftRotate(Node* x) {
    Node* y = x->right;

    x->right = y->left;
    y->left = x;

    // Update heights
    y->height = std::max(getHeight(y->left), getHeight(y->right)) + 1;
    x->height = std::max(getHeight(x->left), getHeight(x->right)) + 1;

    // Return new root
    return y;
}

template<typename T>
void AVL<T>::inOrder() {
    inOrder(head);
    std::cout << std::endl;
}

template<typename T>
void AVL<T>::inOrder(Node* node) {
    if(node == nullptr) return;
    inOrder(node->left);
    std::cout << node->data << " ";
    inOrder(node->right);
}

template<typename T>
T* AVL<T>::search(const T& data) {
    return search(head, data);
}

template<typename T>
T* AVL<T>::search(Node* root, const T& data) {
    if(root == nullptr) return nullptr;
    if(root->data == data) return &root->data;
    if(root->data < data) return search(root->right, data);
    if(root->data > data) return search(root->left, data);
    //should never get here
    return nullptr;
}

template<typename T>
T* AVL<T>::getRecent() {
    return curr;
}

template<typename T>
int AVL<T>::getTotalNodes() {
    return totalNodes;
}

template<typename T>
void AVL<T>::copyNodes(AVL::Node* node) {
    if(node == nullptr) return;
    insert(node->data);
    copyNodes(node->left);
    copyNodes(node->right);
}

template<typename T>
void AVL<T>::treeToFile(std::ofstream& file) {
    //Uses a level order traversal
    for(int x = 1; x <= head->height; x++) {
        nodeToFile(head, x, file);
    }
}

template<typename T>
void AVL<T>::nodeToFile(AVL::Node* node, int depth, std::ofstream& out) {
    if(node == nullptr) return;
    if(depth == 1) {
        out << node->data << "\n";
    }
    else {
        nodeToFile(node->left, depth - 1, out);
        nodeToFile(node->right, depth - 1, out);
    }
}

template<typename T>
void AVL<T>::treeFromfile(std::ifstream& file) {
    while(!file.eof()) {
        T temp;
        file >> temp;
        head = insert(head, temp);
    }
}

//Gets the highest value nodes in the tree with value defined in a lambda
template<typename T>
template<typename Func>
std::vector<T*> AVL<T>::getCount(int count, Func f) {
    std::vector<T*> top;

    //Generic recursive lambda to test each node
    auto countFromTree = [&count, &f, &top](AVL::Node* node, const auto& recurse) -> void {
        if(node == nullptr) return;

        //If the list of top nodes has not yet been filled add the current node
        if(top.size() < count) {
            top.push_back(&node->data);
        }
        //Else replace the lowest value node with the current node if the current node is greater
        else {
            auto iter = std::min_element(top.begin(), top.end(), [&f](T* lhs, T* rhs) {return f(*lhs) < f(*rhs);});
            if(f(**iter) < f(node->data)) {
                *iter = &node->data;
            }
        }

        //Recursively iterate through the tree
        recurse(node->left, recurse);
        recurse(node->right, recurse);
    };

    //Actually calling the created function
    countFromTree(head, countFromTree);

    return top;
}

template<typename T>
void AVL<T>::clear() {
    delete head;
    head = nullptr;
    curr = nullptr;
    totalNodes = 0;
}
