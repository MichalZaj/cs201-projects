#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <cstdlib>
#include <ctime>
using namespace std;

template <class keytype,class valuetype>

class RBTree{
    public:
    struct Node{
        int color; //1 is red, 0 is black
        int size;
        keytype key;
        valuetype value;
        Node *left;
        Node *right;
        Node *parent;
    };

    Node *root = new Node; //root 
    Node *tnil = new Node; //pointer for t.nil
    int nodeCount;

    RBTree(){
        tnil ->color = 0;
        tnil->left = nullptr;
        tnil->right = nullptr;
        root = tnil;
        
    }
    RBTree(keytype k[], valuetype V[], int s){
		tnil->color = 0;
		tnil->left = nullptr;
		tnil->right = nullptr;
		root = tnil;

        for(int i = 0; i < s; i++){
            insert(k[i], V[i]);
        }
        nodeCount = s;
    }
    ~RBTree(){
        clear(root);
        free(tnil);
        free(root);
    }
    RBTree(const RBTree& obj){
        nodeCount = obj.nodeCount;
        root = copy(obj.root);
    }

    RBTree& operator = (const RBTree& obj){
        nodeCount = obj.nodeCount;
        root = copy(obj.root);
        return *this;
    }

    valuetype* search(keytype k){
        Node* temp = new Node;
        temp = root;
        valuetype* a;
        while(temp != tnil){
            if(temp->key == k){
                a = &(temp->value);
                return a;
            }
            else if(temp->key < k){
                temp = temp->right;
            }
            else{
                temp = temp->left;
            }
        }
        return nullptr;
    }
    void insert(keytype k, valuetype v){
        inserter(k,v);

        nodeCount++;

    }

    int remove(keytype k){
        if(search(k) == nullptr){
            return 0;
        }
        remover(this->root, k);
        return 1;
    }
    int rank(keytype k){
        if(search(k) == nullptr){
            return 0;
        }
        int rank = getRank(this->root, k);
        return rank;
    }
    keytype select(int pos){
        keytype temp = kthSmallest(root, pos);
        return temp;
    }
    keytype *successor(keytype k){
        keytype* b;
        int succ = rank(k);
        if(succ == nodeCount){
            b = &k;
            return b;
        }
        keytype a = (select(succ + 1));

        b = &a;
        return b;
    }
    keytype *predecessor(keytype k){
        keytype* b;
        int pred = rank(k);
        if(pred == 1){
            b = &k;
            return b;
        }
        keytype a = (select(pred - 1));
        b = &a;
        return b;
    }
    int size(){
        return nodeCount;
    }
    void preorder(){
        pre(this->root);
        cout << endl;
    }
    void inorder(){
        in(this->root);
        cout << endl;
    }
    void postorder(){
        post(this->root);
        cout << endl;
    }
    void printk(int k){
        for(int i = 1; i <= k; i++){
            cout << kthSmallest(this->root, i) << " ";
        }
        cout << endl;
    }

    private:
    int treeSize(Node* temp){
        if (temp == nullptr){
            return 0;
        }
        return temp->size;
    }

    void leftRotate(Node* x) {
		Node *y = x->right;
		x->right = y->left;
		if (y->left != tnil) {
			y->left->parent = x;
		}
		y->parent = x->parent;
		if (x->parent == nullptr) {
			this->root = y;
		} else if (x == x->parent->left) {
			x->parent->left = y;
		} else {
			x->parent->right = y;
		}
		y->left = x;
		x->parent = y;
        //x->size = treeSize(x->left) + treeSize(x->right) + 1;
        //Maintain Subtree sizes
        y->size = treeSize(x);
        x->size = treeSize(x->left) + treeSize(x->right) + 1;
	}

	// rotate right at node x
	void rightRotate(Node* x) {
		Node *y = x->left;
		x->left = y->right;
		if (y->right != tnil) {
			y->right->parent = x;
		}
		y->parent = x->parent;
		if (x->parent == nullptr) {
			this->root = y;
		} else if (x == x->parent->right) {
			x->parent->right = y;
		} else {
			x->parent->left = y;
		}
		y->right = x;
		x->parent = y;
        //y->size = treeSize(y->left) + treeSize(y->right) + 1;
        //Maintain Subtree sizes
        y->size = treeSize(x);
        x->size = treeSize(x->left) + treeSize(x->right) + 1;
	}
    Node* copy(Node* root){
        if(root == nullptr){
            return nullptr;
        }
        Node* temp = new Node;
        temp->color = root->color;
        temp->parent = root->parent;
        temp->key = root->key;
        temp->value = root->value;
        temp->size = root->size;
        temp->left = copy(root->left);
        temp->right = copy(root->right);
        return temp;
    }
    void inserter(keytype k, valuetype v){
        //first create the node
        Node *temp = new Node;
		temp->parent = nullptr;
        temp->left = tnil;
        temp->right = tnil;
        temp->color = 1;
		temp->key = k;
        temp->value = v;
        temp->size = 1;

		Node *y = nullptr;
		Node *x = this->root;

		while (x != tnil) {
			y = x;
			if (temp->key < x->key) {
                x->size = treeSize(x) + 1;
				x = x->left;
			} 
            else {
                x->size = treeSize(x) + 1;
				x = x->right;
			}
             //for maintaining subtree sizes
		}
        temp->parent = y;
        if (y == nullptr) {
			root = temp;
		} 
        else if (temp->key < y->key) {
			y->left = temp;
		} 
        else {
			y->right = temp;
		}

        if (temp->parent == nullptr){ //if the new node is a root
			temp->color = 0;
			return;
		}
        if (temp->parent->parent == nullptr) { //grandparent null
			return;
		} 
        

        insertFixup(temp);
    }
    void insertFixup(Node* z){
        Node *y = new Node;
		while (z->parent->color == 1) {
			if (z->parent == z->parent->parent->right) {
				y = z->parent->parent->left; 
				if (y->color == 1) {
					y->color = 0;
					z->parent->color = 0;
					z->parent->parent->color = 1;
					z = z->parent->parent;
				} 
                else {
					if (z == z->parent->left) {
						z = z->parent;
						rightRotate(z);
					}
					z->parent->color = 0;
					z->parent->parent->color = 1;
					leftRotate(z->parent->parent);
				}
			} 
            else {
				y = z->parent->parent->right; 
				if (y->color == 1) {
					y->color = 0;
					z->parent->color = 0;
					z->parent->parent->color = 1;
					z = z->parent->parent;	
				} 
                else {
					if (z == z->parent->right) {
						z = z->parent;
						leftRotate(z);
					}
					z->parent->color = 0;
					z->parent->parent->color = 1;
					rightRotate(z->parent->parent);
				}
			}
			if (z == root) {
				break;
			}
		}
        //root->size = treeSize(root->left) + treeSize(root->right) + 1;
		root->color = 0;
    }
    void remover(Node* node, keytype k){
        Node* z = tnil;
		Node* x;
        Node* y;
		while (node != tnil){ //this finds the node with the key k
			if (node->key == k) {
				z = node;
			}

			if (node->key < k) {
                node->size = treeSize(node) - 1;
				node = node->right;
			}
            else {
                node->size = treeSize(node) - 1;
				node = node->left;
			}
		}
        //cout << "After updating size: " << endl;
        //post(root);

		if (z == tnil) { //if the key doesnt key
			return;
		} 
        //textbook
		y = z;
		int yOGc = y->color;
		if (z->left == tnil) {
			x = z->right;
			rbTransplant(z, z->right);
		} 
        else if (z->right == tnil) {
			x = z->left;
			rbTransplant(z, z->left);
		} 
        else {
            //maximum(z->left) and switch directions
			y = maximum(z->left);
			yOGc = y->color;
			x = y->left;
			if (y->parent == z) {
				x->parent = y;
			} 
            else {
				rbTransplant(y, y->left);
				y->left = z->left;
				y->left->parent = y;
			}

			rbTransplant(z, y);
			y->right = z->right;
			y->right->parent = y;
			y->color = z->color;
             //takes the old size of z
            y->size = treeSize(z);
		}
		delete z; //necessary to delete z?
		if (yOGc == 0){
			fixDelete(x);
		}
        nodeCount--;
    }
    Node* maximum(Node* node) {
		while (node->right != tnil) {
			node = node->right;
		}
		return node;
	}
    void fixDelete(Node* x) {
		Node* w;
		while (x != root && x->color == 0) {
			if (x == x->parent->left) {
				w = x->parent->right;
				if (w->color == 1) {
					w->color = 0;
					x->parent->color = 1;
					leftRotate(x->parent);
					w = x->parent->right;
				}

				if (w->left->color == 0 && w->right->color == 0) {
					w->color = 1;
					x = x->parent;
				} 
                else {
					if (w->right->color == 0) {
						w->left->color = 0;
						w->color = 1;
						rightRotate(w);
						w = x->parent->right;
					} 
					w->color = x->parent->color;
					x->parent->color = 0;
					w->right->color = 0;
					leftRotate(x->parent);
					x = root;
				}
			} 
            else {
				w = x->parent->left;
				if (w->color == 1) {
					w->color = 0;
					x->parent->color = 1;
					rightRotate(x->parent);
					w = x->parent->left;
				}

				if (w->right->color == 0 && w->right->color == 0) { //should it be like this?
					w->color = 1;
					x = x->parent;
				} 
                else {
					if (w->left->color == 0) {
						w->right->color = 0;
						w->color = 1;
						leftRotate(w);
						w = x->parent->left;
					} 

					w->color = x->parent->color;
					x->parent->color = 0;
					w->left->color = 0;
					rightRotate(x->parent);
					x = root;
				}
			} 
		}
        //root->size = treeSize(root->left) + treeSize(root->right) + 1;
		x->color = 0;
	}
    void rbTransplant(Node* u, Node* v){ //textbook, but use nullptr instead of tnil
		if (u->parent == nullptr) {
			root = v;
		} 
        else if (u == u->parent->left){
			u->parent->left = v;
		} 
        else {
			u->parent->right = v;
		}
		v->parent = u->parent;
	}
    
    keytype kthSmallest(Node* root, int k)
    {
        int place = treeSize(root->left) + 1;
        if(k == place){
            return root->key;
        }
        else if (k < place){
            return kthSmallest(root->left, k);
        }
        else{
            return kthSmallest(root->right, k - place);
        }
    }
    int getRank(Node* root, keytype k){
        
        if(root == nullptr){
            return 0;
        }
        else if(k > root->key){
            return treeSize(root->left) + 1 + getRank(root->right, k);
        }
        else if(k < root->key){
            return getRank(root->left, k);
        }
        else if(k == root->key){
            return root->size - treeSize(root->right);
        }
        return 0;
        
        /*
        Node* z = tnil;
        Node* node = root;
		while (node != tnil){
			if (node->key == k) {
				z = node;
			}

			if (node->key < k) {
				node = node->right;
			} 
            else {
				node = node->left;
			}
		}

        int r = treeSize(root->left)+ 1;
        Node* y = z;

        while(y != root){
            if (y == y->parent->right)
            {
                r = r + treeSize(y->parent->left) + 1;
            }
            y = y->parent;
        }
        return r; */
    }
    void post(Node *tempNode)
    {
		if (tempNode != tnil) {
			post(tempNode->left);
			post(tempNode->right);
			cout << tempNode->key <<" ";
		} 
    }
    void in(Node *tempNode){
		if (tempNode != tnil) {
			in(tempNode->left);
			cout<<tempNode->key<<" ";
			in(tempNode->right);
		} 
    }
    void pre(Node *tempNode)
    {
        if (tempNode != tnil) {
			cout<< tempNode->key << " " ;
			pre(tempNode->left);
			pre(tempNode->right);
		} 
    }

    void clear(Node* temp){
        if(temp == nullptr){
            return;
        }
        clear(temp->left);
		if (temp->left != tnil){
            free(temp->left);
        }
        clear(temp->right);
		if (temp->right != tnil){
            free(temp->right);
        }
    }
    /*Node* insertNode(Node* root, keytype k, valuetype v){
        //first create the node
        Node *temp = new Node;
		temp->parent = nullptr;
		temp->key = k;
        temp->value = v;
		temp->left = tnil;
		temp->right = tnil;
		temp->color = 1; 

		Node *y = nullptr;
		Node *x = this->root;

		while (x != tnil) {
			y = x;
			if (temp->key < x->key) {
				x = x->left;
			} 
            else {
				x = x->right;
			}
		}
        temp->parent = y;
        if (y == nullptr) {
			root = temp;
		} 
        else if (temp->key < y->key) {
			y->left = temp;
		} 
        else {
			y->right = temp;
		}

        if (temp->parent == nullptr){ //if the new node is a root
			temp->color = 0;
			return root;
		}
        if (temp->parent->parent == nullptr) { //grandparent null
			return root;
		} 

        insertFixup(temp);

        return root;

    }
    */

};