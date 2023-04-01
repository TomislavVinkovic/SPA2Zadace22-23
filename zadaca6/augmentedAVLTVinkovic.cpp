#include <iostream>
#include <vector>

using namespace std;

struct Node
{
    Node(
        int key, 
        int height = 0,
        int size = 0,
        Node* left = nullptr,
        Node* right = nullptr,
        Node* parent = nullptr
    ) : key{key}, height{height}, size{size}, left{left},
        right{right}, parent{parent} 
    {}


	int key;
    int height;
    int size;
	Node *left, *right, *parent;
};

class AVL
{
private:
    
    Node *root; 

    Node* search(int key, Node *node) {
        if (node == nullptr || key == node->key)
            return node;
        if (key > node->key)
            return search(key, node->right);
        return search(key, node->left);
    }

    Node* OS_SELECT (Node* node, int i) {
        int r = 1;

        if(node == nullptr) return node;

        if(node->left != nullptr) r += node->left->size;

        if(r == i) return node;
        else if(i < r) return OS_SELECT(node->left, i);
        else return OS_SELECT(node->right, i - r);

    }

    int OS_RANK(Node* x) {
        int r = (x->left == nullptr ? 0 : x->left->size) + 1;
        Node* y = x;
        while(y != root) {
            if(y == y->parent->right) {
                r += (y->parent->left == nullptr ? 0 : y->parent->left->size) + 1;
            }
            y = y->parent;
        }
        return r;
    }

	int height(Node *node)
	{
		if (node == nullptr)
			return 0;
		
        return node->height;
	}

    Node* rightRotate(Node *y)
    {
        Node *x = y->left;
        Node *temp = x->right;

        x->size = y->size;
        y->size = 
            (y->right == nullptr ? 0 : y->right->size) 
            +(x->right == nullptr ? 0 : x->right->size) + 1;

        x->right = y;
        y->left = temp;

        y->height = max(height(y->left), height(y->right)) + 1;
        x->height = max(height(x->left), height(x->right)) + 1;

        if(temp != nullptr)
            temp->parent = y;

        x->parent = y->parent;
        y->parent = x;

        return x;
    }

    Node* leftRotate(Node *x)
    {
        Node *y = x->right;
        Node *temp = y->left;

        y->size = x->size;
        x->size = 
            (x->left == nullptr ? 0 : x->left->size) 
            +(y->left == nullptr ? 0 : y->left->size) + 1;

        y->left = x;
        x->right = temp;

        x->height = max(height(x->left), height(x->right)) + 1;
        y->height = max(height(y->left), height(y->right)) + 1;

        if(temp != nullptr)
            temp->parent = x;

        y->parent = x->parent;
        x->parent = y;

        return y;
    }

    int balanceOfNode(Node *node)
    {
        if(node == nullptr)
            return 0;
        
        return height(node->left) - height(node->right);
    }

    Node* insert(Node *node, int key)
    {
        if(node == nullptr)
            node = new Node {key, 0, 1, nullptr, nullptr, nullptr};
        else if(key < node->key)
        {
            node->left = insert(node->left, key);
            node->left->parent = node;
            node->size++;
        }
        else if(key > node->key) {
            node->right = insert(node->right, key);
            node->right->parent = node;
            node->size++;
        }
    
        node->height = 1 + max(height(node->left), height(node->right));

        int balance = balanceOfNode(node);

        if (balance > 1 && key < node->left->key)
            return rightRotate(node);

        if (balance < -1 && key > node->right->key)
            return leftRotate(node);
        
        if (balance > 1 && key > node->left->key)
        {
            node->left = leftRotate(node->left);
            return rightRotate(node);
        }

        if (balance < -1 && key < node->right->key)
        {
            node->right = rightRotate(node->right);
            return leftRotate(node);
        }
    
        return node;
    }

    void inorder(Node *node)
    {
        if(node == nullptr)
            return;
        
        inorder(node->left);
        cout << "[K: " << node->key << ", H: " << node->height << ", B: " << balanceOfNode(node) << ", S: " << node->size <<  "] ";
        //Ako te zanima ispis koji je laksi za procitati
        //cout << "(" << node->key << ", " << node->size << ") ";
        inorder(node->right);
    }

    void preorder(Node *node)
    {
        if(node == nullptr)
            return;
        
        cout << "[K: " << node->key << ", H: " << node->height << ", B: " << balanceOfNode(node) << ", S: " << node->size <<  "] ";
        inorder(node->left);
        inorder(node->right);
    }

public:
    int getRootKey() {return root->key;}
    AVL() : root(nullptr) {}

	void insert(int key)
	{
        root = insert(root, key);
	}

    void printInorder()
    {
        inorder(root);
    }

    void printPreorder()
    {
        preorder(root);
    }

    void OS_SELECT(int i) {
        Node* x = OS_SELECT(root, i);
        if(x == nullptr) {
            cout << "A node with rank " << i << " does not exist in the tree" << endl;
        }
        else cout << "[K: " << x->key << 
            ", H: " << x->height << ", B: " 
            << balanceOfNode(x) << ", S: " << x->size <<  "]" << endl;
    }

    void ithSuccessor(int key, int i) {
        Node* x = search(key, root);
        if(x == nullptr) {
            cout << "Node sa trazenim kljucem ne psotoji u stablu" << endl;
            return;
        }
        
        int r = OS_RANK(x);
        Node* y = OS_SELECT(root, i + r);

        if(y == nullptr) {
            cout << "Trazeni sljedbenik ne postoji" << endl;
            cout << y->key << endl;
            return;
        }
        cout << y->key;
    }

    friend int OS_KEY_RANK(AVL& T, int k);
};

int OS_KEY_RANK(AVL& T, int k) {
    Node* x = T.search(k, T.root);
    if(x == nullptr) return -1;
    
    return T.OS_RANK(x);
}

int main() {
	AVL avl;
    vector<int> keys = {29, 6, 56, 3, 7, 37, 61, 4, 22, 48, 59, 69};

    for(auto& k : keys) avl.insert(k);
    avl.printInorder();
    cout << endl << endl;

    cout << "Drugi najmanji element u stablu:" << endl;
    avl.OS_SELECT(2); cout << endl;

    cout << "Cetvrti sljedbenik od 6:";
    avl.ithSuccessor(6, 4); cout << endl;

    cout << "Rang nodea s kljucem 59: " << OS_KEY_RANK(avl, 59) << endl;
}