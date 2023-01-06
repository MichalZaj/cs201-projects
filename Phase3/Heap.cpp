#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
using namespace std;

template <class keytype>

class Heap {
    private: 

        CircularDynamicArray<keytype> minHeap;
        
        void heapify(keytype arr[], int size, int loc){
            int min = loc;
            int lChild = 2 * loc + 1;
            int rChild = 2 * loc + 2;

            if(lChild < size && arr[lChild] < arr[min]){
                min = lChild;
            }
            if(rChild < size && arr[rChild] < arr[min]){
                min = rChild;
            }
            if(min != loc){
                keytype temp = arr[loc];
                arr[loc] = arr[min];
                arr[min] = temp;
                heapify(arr, size, min);
            }
        }

        void fixUp(int index){
            int parent = (index -1) / 2;
            if(minHeap[parent] > minHeap[index]){
                keytype temp = minHeap[parent];
                minHeap[parent] = minHeap[index];
                minHeap[index] = temp;
                fixUp(parent);
            }
        }

        void fixDown(int parent){
            int lChild = parent * 2 + 1;
            int rChild = parent * 2 + 2;
            int minChild = lChild;

            if(lChild >= minHeap.length()){
                return;
            }

            if(rChild < minHeap.length() && minHeap[lChild] > minHeap[rChild]){
                minChild = rChild;
            }

            if(minHeap[parent] > minHeap[minChild]){
                keytype temp = minHeap[parent];
                minHeap[parent] = minHeap[minChild];
                minHeap[minChild] = temp;
                fixDown(minChild);
            }
        }

    public: 

    Heap(){
	}

    Heap(keytype k[], int s){
        keytype list[s];
	  	for (int i = 0; i < s; i++) {
			list[i] = k[i];
		}

        for (int i = (s/2) - 1; i >= 0; i--) {
			heapify(list, s, i);
		}

        for(int i = 0; i < s; i++){
            minHeap.addEnd(list[i]);
        }

    }

    ~Heap() {
	}

     Heap(const Heap& obj){
        minHeap = obj.minHeap;
    }

    Heap& operator = (const Heap& obj){
        minHeap = obj.minHeap;
        return *this;
    }

    keytype peekKey(){
        return minHeap[0];
    }

    void insert(keytype k){
        minHeap.addEnd(k);
        fixUp(minHeap.length() - 1);
    }

    keytype extractMin(){
        keytype min = minHeap[0];
        int size = minHeap.length() - 1;
        keytype temp = minHeap[size];
        minHeap[0] = minHeap[size];
        minHeap[size] = temp;
        minHeap.delEnd();
        fixDown(0);
        return min;
    }

    void printKey(){
        for (int i = 0; i < minHeap.length(); i++) {
            cout << minHeap[i] << " ";
        }
        cout << endl;
    }

};