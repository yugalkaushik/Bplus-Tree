#include <iostream>
#include <queue>
#ifndef NODE_H
#include "B+Tree.h"

void BPTree::display(Node* cursor) {
    // Level Order (BFS) traversal of the tree
    if (cursor == NULL) return;
    
    queue<Node*> q;
    q.push(cursor);

    while (!q.empty()) {
        int sz = q.size();
        for (int i = 0; i < sz; i++) {
            Node* u = q.front(); 
            q.pop();

            // Print keys at current level
            for (int val : u->keys)
                cout << val << " ";

            cout << "|| ";  // Separator between nodes at same level
            
            // Add children to queue if internal node
            if (u->isLeaf != true) {
                for (Node* v : u->ptr2TreeOrData.ptr2Tree) {
                    q.push(v);
                }
            }
        }
        cout << endl;  // New line for next level
    }
}

void BPTree::seqDisplay(Node* cursor) {
    // Sequential display of leaf nodes using next pointers
    Node* firstLeft = firstLeftNode(cursor);

    if (firstLeft == NULL) {
        cout << "No Data in the Database yet!" << endl;
        return;
    }

    // Traverse and print all leaf nodes
    while (firstLeft != NULL) {
        for (int i = 0; i < firstLeft->keys.size(); i++) {
            cout << firstLeft->keys[i] << " ";
        }
        firstLeft = firstLeft->ptr2next;
    }
    cout << endl;
}

#endif