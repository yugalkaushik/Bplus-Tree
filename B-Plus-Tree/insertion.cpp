#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include "B+Tree.h"

using namespace std;

void BPTree::insert(int key, FILE* filePtr) {
    // Handle empty tree case
    if (root == NULL) {
        root = new Node;
        root->isLeaf = true;
        root->keys.push_back(key);
        new (&root->ptr2TreeOrData.dataPtr) std::vector<FILE*>;
        root->ptr2TreeOrData.dataPtr.push_back(filePtr);

        cout << key << ": I AM ROOT!!" << endl;
        return;
    } 
    
    // Find leaf node for insertion
    Node* cursor = root;
    Node* parent = NULL;
    while (cursor->isLeaf == false) {
        parent = cursor;
        int idx = std::upper_bound(cursor->keys.begin(), cursor->keys.end(), key) - cursor->keys.begin();
        cursor = cursor->ptr2TreeOrData.ptr2Tree[idx];
    }

    // Case 1: Leaf node has space
    if (cursor->keys.size() < maxLeafNodeLimit) {
        int i = std::upper_bound(cursor->keys.begin(), cursor->keys.end(), key) - cursor->keys.begin();
        cursor->keys.push_back(key);
        cursor->ptr2TreeOrData.dataPtr.push_back(filePtr);

        // Shift elements to make space for new key
        if (i != cursor->keys.size() - 1) {
            for (int j = cursor->keys.size() - 1; j > i; j--) {
                cursor->keys[j] = cursor->keys[j - 1];
                cursor->ptr2TreeOrData.dataPtr[j] = cursor->ptr2TreeOrData.dataPtr[j - 1];
            }
            cursor->keys[i] = key;
            cursor->ptr2TreeOrData.dataPtr[i] = filePtr;
        }
        cout << "Inserted successfully: " << key << endl;
    } 
    // Case 2: Leaf node needs splitting
    else {
        // Create virtual nodes for splitting
        vector<int> virtualNode(cursor->keys);
        vector<FILE*> virtualDataNode(cursor->ptr2TreeOrData.dataPtr);

        int i = std::upper_bound(cursor->keys.begin(), cursor->keys.end(), key) - cursor->keys.begin();
        virtualNode.push_back(key);
        virtualDataNode.push_back(filePtr);

        // Shift and insert in virtual nodes
        if (i != virtualNode.size() - 1) {
            for (int j = virtualNode.size() - 1; j > i; j--) {
                virtualNode[j] = virtualNode[j - 1];
                virtualDataNode[j] = virtualDataNode[j - 1];
            }
            virtualNode[i] = key;
            virtualDataNode[i] = filePtr;
        }

        // Create new leaf node
        Node* newLeaf = new Node;
        newLeaf->isLeaf = true;
        new (&newLeaf->ptr2TreeOrData.dataPtr) std::vector<FILE*>;

        // Update leaf node links
        Node* temp = cursor->ptr2next;
        cursor->ptr2next = newLeaf;
        newLeaf->ptr2next = temp;

        // Distribute elements between old and new leaf
        cursor->keys.resize((maxLeafNodeLimit) / 2 + 1);
        cursor->ptr2TreeOrData.dataPtr.resize((maxLeafNodeLimit) / 2 + 1);
        for (int i = 0; i <= (maxLeafNodeLimit) / 2; i++) {
            cursor->keys[i] = virtualNode[i];
            cursor->ptr2TreeOrData.dataPtr[i] = virtualDataNode[i];
        }

        for (int i = (maxLeafNodeLimit) / 2 + 1; i < virtualNode.size(); i++) {
            newLeaf->keys.push_back(virtualNode[i]);
            newLeaf->ptr2TreeOrData.dataPtr.push_back(virtualDataNode[i]);
        }

        // Handle root split case
        if (cursor == root) {
            Node* newRoot = new Node;
            newRoot->keys.push_back(newLeaf->keys[0]);
            new (&newRoot->ptr2TreeOrData.ptr2Tree) std::vector<Node*>;
            newRoot->ptr2TreeOrData.ptr2Tree.push_back(cursor);
            newRoot->ptr2TreeOrData.ptr2Tree.push_back(newLeaf);
            root = newRoot;
            cout << "Created new Root!" << endl;
        } else {
            insertInternal(newLeaf->keys[0], &parent, &newLeaf);
        }
    }
}

void BPTree::insertInternal(int x, Node** cursor, Node** child) {
    // Case 1: Internal node has space
    if ((*cursor)->keys.size() < maxIntChildLimit - 1) {
        int i = std::upper_bound((*cursor)->keys.begin(), (*cursor)->keys.end(), x) - (*cursor)->keys.begin();
        (*cursor)->keys.push_back(x);
        (*cursor)->ptr2TreeOrData.ptr2Tree.push_back(*child);

        // Shift elements if necessary
        if (i != (*cursor)->keys.size() - 1) {
            for (int j = (*cursor)->keys.size() - 1; j > i; j--) {
                (*cursor)->keys[j] = (*cursor)->keys[j - 1];
            }

            for (int j = (*cursor)->ptr2TreeOrData.ptr2Tree.size() - 1; j > (i + 1); j--) {
                (*cursor)->ptr2TreeOrData.ptr2Tree[j] = (*cursor)->ptr2TreeOrData.ptr2Tree[j - 1];
            }

            (*cursor)->keys[i] = x;
            (*cursor)->ptr2TreeOrData.ptr2Tree[i + 1] = *child;
        }
        cout << "Inserted key in the internal node" << endl;
    } 
    // Case 2: Internal node needs splitting
    else {
        // Create virtual nodes for splitting
        vector<int> virtualKeyNode((*cursor)->keys);
        vector<Node*> virtualTreePtrNode((*cursor)->ptr2TreeOrData.ptr2Tree);

        int i = std::upper_bound((*cursor)->keys.begin(), (*cursor)->keys.end(), x) - (*cursor)->keys.begin();
        virtualKeyNode.push_back(x);
        virtualTreePtrNode.push_back(*child);

        // Shift and insert in virtual nodes
        if (i != virtualKeyNode.size() - 1) {
            for (int j = virtualKeyNode.size() - 1; j > i; j--) {
                virtualKeyNode[j] = virtualKeyNode[j - 1];
            }
            for (int j = virtualTreePtrNode.size() - 1; j > (i + 1); j--) {
                virtualTreePtrNode[j] = virtualTreePtrNode[j - 1];
            }
            virtualKeyNode[i] = x;
            virtualTreePtrNode[i + 1] = *child;
        }

        // Find partition point
        int partitionKey = virtualKeyNode[(virtualKeyNode.size() / 2)];
        int partitionIdx = (virtualKeyNode.size() / 2);

        // Distribute elements to old node
        (*cursor)->keys.resize(partitionIdx);
        (*cursor)->ptr2TreeOrData.ptr2Tree.resize(partitionIdx + 1);
        for (int i = 0; i < partitionIdx; i++) {
            (*cursor)->keys[i] = virtualKeyNode[i];
        }
        for (int i = 0; i < partitionIdx + 1; i++) {
            (*cursor)->ptr2TreeOrData.ptr2Tree[i] = virtualTreePtrNode[i];
        }

        // Create and populate new internal node
        Node* newInternalNode = new Node;
        new (&newInternalNode->ptr2TreeOrData.ptr2Tree) std::vector<Node*>;
        
        for (int i = partitionIdx + 1; i < virtualKeyNode.size(); i++) {
            newInternalNode->keys.push_back(virtualKeyNode[i]);
        }
        for (int i = partitionIdx + 1; i < virtualTreePtrNode.size(); i++) {
            newInternalNode->ptr2TreeOrData.ptr2Tree.push_back(virtualTreePtrNode[i]);
        }

        // Handle root split case
        if ((*cursor) == root) {
            Node* newRoot = new Node;
            newRoot->keys.push_back(partitionKey);
            new (&newRoot->ptr2TreeOrData.ptr2Tree) std::vector<Node*>;
            newRoot->ptr2TreeOrData.ptr2Tree.push_back(*cursor);
            newRoot->ptr2TreeOrData.ptr2Tree.push_back(newInternalNode);
            root = newRoot;
            cout << "Created new ROOT!" << endl;
        } else {
            insertInternal(partitionKey, findParent(root, *cursor), &newInternalNode);
        }
    }
}