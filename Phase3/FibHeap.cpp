#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
using namespace std;
template <typename keytype>
class FibHeapNode{
    public:

    int rank;
    bool mark;
    FibHeapNode<keytype> *child;
    FibHeapNode<keytype> *left;
    FibHeapNode<keytype> *right;
    FibHeapNode<keytype> *sibling;
    //FibHeapNode<keytype> *parent;

    keytype key;
    FibHeapNode(keytype k){
        key = k;

    }
};
template <class keytype>

class FibHeap {
    private:

    void consolidate(){
        int max = 0;
        FibHeapNode<keytype> *arr[1000];
        for(int i = 0; i < 50; i++){
            arr[i] = nullptr;
        }
        FibHeapNode<keytype> *root = head;
        while(root != nullptr){
            int numRank = root->rank;
            while(arr[numRank] != nullptr){
                FibHeapNode<keytype> *temp = arr[numRank];
                if(root->key <= temp->key){
                    root = link(root, temp);
                }
                else{
                    root = link(temp, root);
                }
                arr[numRank] = nullptr;
                numRank++;
            }
            if(numRank > max){
                max = numRank;
            }
            arr[numRank] = root;
            root = root->right;
        }
        head = nullptr;
        tail = nullptr;
        for(int i = 0; i <= max; i++){
            if(head == nullptr && arr[i] != nullptr){
                FibHeapNode<keytype> *temp = arr[i];
                temp->left = nullptr;
                temp->right = nullptr;
                head = temp;
                tail = temp;
                minimum = temp;
            }
            else if(arr[i] != nullptr){
                FibHeapNode<keytype> *temp = arr[i];
                FibHeapNode<keytype> *last = tail;
                last->right = temp;
                temp->left = last;
                temp->right = nullptr;
                if(minimum->key > temp->key){
                    minimum = temp;
                }
                tail = temp;
            }
        }
    }

    void exInsert(FibHeapNode<keytype> *root){
        FibHeapNode<keytype> *temp = tail;
        root->left = nullptr;
        root->sibling = nullptr;
        root->right = nullptr;
        temp->right = root;
        root->left = temp;
        if(minimum == nullptr){
            minimum = root;
        }
        else if(minimum->key > root->key){
            minimum = root;
        }
        tail = root;
    }

    FibHeapNode<keytype> *link(FibHeapNode<keytype> *root, FibHeapNode<keytype> *temp){
        FibHeapNode<keytype> *lTemp = temp->left;
        FibHeapNode<keytype> *rTemp = temp->right;
        if(lTemp == nullptr){
            rTemp->left = lTemp;
        }
        else if(rTemp == nullptr){
            lTemp->right = rTemp;
        }
        else{
            rTemp->left = lTemp;
            lTemp->right = rTemp;

        }
        temp->right = nullptr;
        temp->left = nullptr;
        temp->sibling = root->child;
        root->child = temp;
        root->rank = root->rank + 1;
        root->mark = false;
        return root;
    }

    void preorder(FibHeapNode<keytype> *temp){
        if(temp == nullptr){
            return;
        }

        cout << temp->key << " ";
        preorder(temp->child);
        preorder(temp->sibling);
    }

    void deleteTree(FibHeapNode<keytype> *root){
        if (root == NULL) return;

		deleteTree(root->child);
		deleteTree(root->sibling);
		free(root);
		return;
    }

    public:
    FibHeapNode<keytype> *head;
    FibHeapNode<keytype> *tail;
    FibHeapNode<keytype> *minimum;
    int numNodes;

    FibHeap() {
        head = nullptr;
        tail = nullptr;
        numNodes = 0;
    }

    ~FibHeap(){
        FibHeapNode<keytype> *temp = head;
        while(temp != nullptr){
            FibHeapNode<keytype> *next = temp->right;
            deleteTree(temp);
            temp = next;
        }
    }

    FibHeap(keytype k[], int s, CircularDynamicArray<FibHeapNode<keytype> *> &handle){
        head = nullptr;
        tail = nullptr;
        for(int i = 0; i < s; i++){
            FibHeapNode<keytype> *loc = new FibHeapNode<keytype>(k[i]);
            insert(k[i]);
            handle.addEnd(loc);
        }
        //consolidate();
        numNodes = s;
    }

    keytype peekKey(){
        return minimum->key;
    }
 

    FibHeapNode<keytype>* insert(keytype k){
        FibHeapNode<keytype> *temp = new FibHeapNode<keytype>(k);
        temp->key = k;
        temp->mark = false;
        temp->rank = 0;
        temp->sibling = nullptr;
        temp->child = nullptr;
        //temp->parent = nullptr;
        if(head == nullptr){
            head = temp;
            tail = temp;
            minimum = temp;
            //return head;
        }
        else{
            FibHeapNode<keytype> *tailTemp = tail;
            tailTemp->right = temp;
            temp->left = tailTemp;
            tail = temp;
            if(minimum->key > temp->key){
                minimum = temp;
            }
            //return tail;
        }
        numNodes++;
        return temp;
    }
    bool decreaseKey(FibHeapNode<keytype> *h, keytype k){
        if(k > h->key){
            return false;
        }
        h->key = k;

        return true;
    }
    void cut(FibHeapNode<keytype> *x, FibHeapNode<keytype> *y){
        /*remove x from the child list of y, decrementing y:degree
        add x to the root list of H
        x:p D NIL
        x:mark D FALSE*/
        return;
    }

    void cascadingCut(FibHeapNode<keytype> *y){
        /*
        1 z =y.p
        2 if z != NIL
        3 if y.mark = = FALSE
        4 y:mark = TRUE
        5 else cut(y, z)
        6 cascadingCut(z)
        */
        return;
    }
    void merge(FibHeap<keytype>&H2){
        FibHeapNode<keytype> *mainTemp = tail;
        FibHeapNode<keytype> *mergeTemp = H2.head;

        mainTemp->right = mergeTemp;
        mergeTemp->left = mainTemp;
        tail = H2.tail;
        if(H2.minimum->key < minimum->key){
            minimum = H2.minimum;
        }
    }

    keytype extractMin(){
        keytype extraction = minimum->key;
        FibHeapNode<keytype> *min = minimum;
        FibHeapNode<keytype> *root = min->child;
        minimum = root;
        while(root != nullptr){ 
            FibHeapNode<keytype> *temp = root->sibling;
            exInsert(root);
            root = temp;
        }
        if(min->left == nullptr && min->right == nullptr){
            head = nullptr;
            tail = nullptr;
            min = nullptr;
        }
        else if(min->left == nullptr){
            FibHeapNode<keytype> *temp = min->right;
            temp->left = nullptr;
            head = temp;
        }
        else if(min->right == nullptr){
            FibHeapNode<keytype> *temp = min->left;
            temp->right = nullptr;
            tail = temp;
        }
        else{
            FibHeapNode<keytype> *lTemp = min->left;
            FibHeapNode<keytype> *rTemp = min->right;
            rTemp->left = lTemp;
            lTemp->right = rTemp;
        }
        consolidate();
        numNodes--;
        return extraction;
    }

    void printKey(){
        FibHeapNode<keytype> *temp = head;
        while(temp != nullptr){
            cout << "Rank " << temp->rank << endl;
            preorder(temp);
            cout << endl << endl;
            temp = temp->right;
        }
    }
};