#include <iostream>

template <typename T>
class Heapnode {
private:
    T val;
    int keyval;
    Heapnode* leftnode;
    Heapnode* rightnode;
public:
    T value() {return val;}
    int key() {return keyval;}
    Heapnode* left() {return leftnode;}
    Heapnode* right() {return rightnode;}
    std::pair<Heapnode*, Heapnode*> children() {return {leftnode, rightnode};}
    void setleft(Heapnode* node) {this.leftnode = node;}
    void setright(Heapnode* node) {this.rightnode = node;}
    Heapnode(T value, int key) {
        leftnode = nullptr;
        rightnode = nullptr;
        this->val = value;
        this->keyval = key;
    }
};

template <typename T>
class Minheap {
private:
    Heapnode<T> root;
public:
    Heapnode<T> gettop() {return this->root;}
    Heapnode<T> extracttop();
    void removetop();
    void add(Heapnode<T> node);
    void add(T value, int key) {add(Heapnode<T>(value, key))}
    void heapify(int A);
};

template <typename T>
void Minheap<T>::heapify(int A) {
    Heapnode<T>* current = root;
    Heapnode<T>* prev;
    for (int i = 0; i < A; fin)
}