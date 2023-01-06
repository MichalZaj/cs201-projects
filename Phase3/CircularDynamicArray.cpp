#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
using namespace std;

template <class elmtype>

class CircularDynamicArray {
    public:
    elmtype *arr;
    int size;
    int cap;
    int front;
    int back;
    elmtype init_val;
    int *from;
    int *to;
    int top;
    elmtype errval;
    bool construct = false;
    int bsmid;

    CircularDynamicArray() { //Default Constructor
        cap = 2;
        size = 0;
        arr = new elmtype[cap];
        front = -1;
        back = -1;
    }
    CircularDynamicArray(int s){ //Constructor that makes capacity and size s
        cap = s;
        size = s;
        arr = new elmtype[s];
        front = 0;
        back = s - 1;
    }
    ~CircularDynamicArray(){  //Destructor 
        delete [] arr;
        arr = NULL;
    }

    CircularDynamicArray(const CircularDynamicArray& obj){ //Copy Constructor
        cap = obj.cap;
        size = obj.size;
        arr = new elmtype[cap];
        
        for(int i = 0; i < size; i++){
            arr[i] = obj.arr[(i + obj.front) % cap];
        }
        front = 0;
        back = size - 1;
    }

    CircularDynamicArray& operator = (const CircularDynamicArray& obj){ //Copy Assignment Operator
        if(this != &obj){
            cap = obj.cap;
            size = obj.size;
            arr = new elmtype[cap];

            for(int i = 0; i < size; i++){
                arr[i] = obj.arr[(i + obj.front) % cap];
            }
            front = 0;
            back = size - 1;
        }
        return *this;
    }

    elmtype& operator[](int i){ //Traditional [] Operator
        if(i > cap || i < 0){
            cout << "Index out of bounds";
            errval = 0;
            return errval;
        }

        if(construct){
            if (from[i] < top && to[from[i]] == i)
                return arr[i];
            else
            {
                from[i] = top;
                to[top] = i;
                arr[i] = init_val;
                top++;

                return arr[i];
            }
        }
        else{
            if(front == -1 && back == -1){
                front = 0;
                back = 0;
            }
            int loc = (front + i) % cap;
            return arr[loc];
        }
    }

    void addEnd(elmtype v){ //add to end or double and add
        if(front == -1 && back == -1){
            front = 0;
        }
        if(size < cap){
            size++;
            if(back < (cap)){ //if the back of the array is less than the length
                back++; //then make the back one further
                arr[back] = v; //and then make that back the new element
            }
            else if (back == (cap)){ //if the back is equal to the last element
                back = 0; // make the back the "front" of the array
                arr[back] = v; //add the element to the new back
            }
        }
        else{ //otherwise if the size is greater than the capacity
            elmtype *arrNew = new elmtype[cap * 2]; //make a new array that is double the cap
            for(int i = 0; i < size; i++){ 
                arrNew[i] = arr[(i + front) % cap]; //inserting elements
            }

            cap *= 2; //double the capacity
            
            delete [] arr;
            arr = arrNew;
            
        
            front = 0;
            back = size;
            size++;
            arr[back] = v;
        }
    }

    void addFront(elmtype v){ //adds to front or doubles and adds

        if(front == -1 && back == -1){
            back = 0;
            front = 1;
        }
        if(size < cap){
            size++;
            if(front > 0){
                front--;
                arr[front] = v;
            }
            else{
                front = cap - 1;
                arr[front] = v;
            }
        }
        else{
            elmtype *arrNew = new elmtype[cap * 2];
            arrNew[0] = v;
            for(int i = 0; i < size; i++){
                arrNew[i + 1] = arr[(i + front) % cap];
            }

            delete [] arr;
            arr = arrNew;
            size++;
            cap *= 2;
            front = 0;
            back = size - 1;
        }
    }

    void delEnd(){
        int filled = cap / 4;

        if(filled < size){ //checks for the 25% capacity
            if(back == 0){
                back = cap - 1;
                size --;
            }
            else{
                back--;
                size --;
            }
        }
        else if ((cap / 2) >= 4){
            elmtype *arrNew = new elmtype[cap / 2];
            for(int i = 0; i < size; i++){
                arrNew[i] = arr[(i + front) % cap];
            }
            
            delete [] arr;
            arr = arrNew;

            cap = cap / 2;
            size --;
            back = size - 1;
            front = 0;
        }
    }

    void delFront(){
        if(front == cap - 1){
            front = 0;
            size--;
        }
        else{
            front++;
            size--;
        }
        int filled = cap / 4;
        if((filled ) > size){
            if(front == cap - 1){
                front = 0;
            }
            else{
                front++;
            }
            elmtype *arrNew = new elmtype[cap / 2];
            for(int i = 0; i < size; i++){
                arrNew[i] = arr[(i + front) % cap];
            }

            delete [] arr;
            arr = arrNew;

            cap = cap / 2;
            size--;
            back = size - 1;
            front = 0;
        }
    }
    int length(){
        return size;
    }
    int capacity(){
        return cap;
    }
    void clear(){
        //Frees any space currently used and starts over with an 
        //array of capacity 2 and size 0.
        delete [] arr;
        cap = 2;
        size = 0;
        arr = new elmtype[cap];
        front = -1;
        back = -1;
        
    }
    elmtype QuickSelect(int k){
        elmtype *arrNew = new elmtype[cap];
        for(int i = 0; i < size; i++){
            arrNew[i] = arr[(i + front) % cap];
        }
        return quickSelect(arrNew, 0, size - 1, k);
    }
    elmtype WCSelect(int k){
        elmtype *arrNew = new elmtype[cap];
        for(int i = 0; i < size; i++){
            arrNew[i] = arr[(i + front) % cap];
        }
        return wcSelect(arrNew, 0, size - 1, k);
    }
    void stableSort(){
        elmtype *arrNew = new elmtype[cap];
        for(int i = 0; i < size; i++){
            arrNew[i] = arr[(i + front) % cap];
        }

        MergeSort(arrNew, 0, size - 1);

        delete [] arr;
        arr = arrNew;
        back = size - 1;
        front = 0;
    }
    int linearSearch(elmtype e){
        for(int i = 0; i < size; i++){
            if(arr[(i + front) % cap] == e){
                return i;
            }
        }
        return -1;
    }
    int binSearch(elmtype e){
        return BinarySearch(0, size - 1, e);
    }

    private:

    elmtype quickSelect(elmtype arr[], int l, int r, int k){
            if (k > 0 && k <= r - l + 1) {
            int index = Partition(arr, l, r);
    
            if (index - l == k - 1)
                return arr[index];
    
            if (index - l > k - 1)
                return quickSelect(arr, l, index - 1, k);
    
            return quickSelect(arr, index + 1, r, k - index + l - 1);
        }
        return -1;
    }
    int Partition(elmtype arr[], int l, int r)
    {
        int i = l;
        int ranIndex = rand() % size;
        elmtype x = arr[ranIndex];

        for (int j = l; j <= r - 1; j++){
            if (arr[j] <= x){
                elmtype temp = arr[i];
                arr[i] = arr[j];
                arr[j] = temp;
                i++;
            }
        }
        elmtype temp = arr[i];
        arr[i] = arr[r];
        arr[r] = temp;
        return i;
    }
    void MergeSort(elmtype array[], int i, int k)
    {
        if (i >= k){
            return;
        }
        int j;
   
        if (i < k) {

        j = (i + k) / 2;  // Find the midpoint in the partition
      
        // Recursively sort left and right partitions
        MergeSort(array, i, j);
        MergeSort(array, j + 1, k);
      
        // Merge left and right partition in sorted order
        Merge(array, i, j, k);
        }
    }

    void Merge(elmtype array[], int i, int j, int k) {
   int mergedSize;                            // Size of merged partition
   int mergePos;                              // Position to insert merged number
   int leftPos;                               // Position of elements in left partition
   int rightPos;                              // Position of elements in right partition


   mergePos = 0;
   mergedSize = k - i + 1;
   leftPos = i;                               // Initialize left partition position
   rightPos = j + 1;                          // Initialize right partition position
   elmtype *mergedArray = new elmtype[mergedSize];       // Dynamically allocates temporary array
                                              // for merged numbers
   
   // Add smallest element from left or right partition to merged array
   while (leftPos <= j && rightPos <= k) {
      if (array[leftPos] < array[rightPos]) {
         mergedArray[mergePos] = array[leftPos];
         ++leftPos;
      }
      else {
         mergedArray[mergePos] = array[rightPos];
         ++rightPos;
         
      }
      ++mergePos;
   }
   
   // If left partition is not empty, add remaining elements to merged array
   while (leftPos <= j) {
      mergedArray[mergePos] = array[leftPos];
      ++leftPos;
      ++mergePos;
   }
   
   // If right partition is not empty, add remaining elements to merged array
   while (rightPos <= k) {
      mergedArray[mergePos] = array[rightPos];
      ++rightPos;
      ++mergePos;
   }
   
   // Copy merge number back to numbers
   for (mergePos = 0; mergePos < mergedSize; ++mergePos) {
      array[i + mergePos] = mergedArray[mergePos];
   }
   delete[] mergedArray;
}
    int BinarySearch(int l, int r, elmtype a)
    {
        if (r >= l) {
            int mid = l + (r - l) / 2;
            bsmid = mid;

            if (arr[(mid + front) % cap] == a){
                return mid;
            }

            if (arr[(mid + front) % cap] > a){
                return BinarySearch(l, mid - 1, a);
            }

            return BinarySearch(mid + 1, r, a);
        }
        
        if(bsmid >= size - 1){
            return ~(size);
        }
        return -1;
    }
    elmtype wcSelect(elmtype arr[], int l, int r, int k){
            if (k > 0 && k <= r - l + 1) {
            int index = Partition(arr, l, r);
    
            if (index - l == k - 1)
                return arr[index];
    
            if (index - l > k - 1)
                return wcSelect(arr, l, index - 1, k);
    
            return wcSelect(arr, index + 1, r, k - index + l - 1);
        }
        return -1;
    }
    
};