#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <string>
using namespace std;

#ifndef NODE_H
#define NODE_H

class Node {
   public:
    bool isLeaf;
    vector<int> keys;
    Node* ptr2next;  // Links leaf nodes for sequential access
    
    // Memory-efficient union for different node types
    union ptr {
        vector<Node*> ptr2Tree;  // Internal node: pointers to children
        vector<FILE*> dataPtr;   // Leaf node: pointers to data

        ptr();
        ~ptr();
    } ptr2TreeOrData;

    friend class BPTree;
   
   public:
    Node();
};

class BPTree {
   private:
    // Tree configuration
    int maxIntChildLimit;  // Maximum children for internal nodes
    int maxLeafNodeLimit;  // Maximum keys for leaf nodes
    Node* root;           // Root node pointer

    // Internal operations
    void insertInternal(int x, Node** cursor, Node** child);
    Node** findParent(Node* cursor, Node* child);
    Node* firstLeftNode(Node* cursor);

   public:
    // Constructors
    BPTree();
    BPTree(int degreeInternal, int degreeLeaf);

    // Accessors
    Node* getRoot();
    int getMaxIntChildLimit();
    int getMaxLeafNodeLimit();
    void setRoot(Node *);

    // Tree operations
    void display(Node* cursor);     // Level-order display
    void seqDisplay(Node* cursor);  // Sequential leaf display
    void search(int key);
    void insert(int key, FILE* filePtr);
    void removeKey(int key);
    void removeInternal(int x, Node* cursor, Node* child);
};

#endif