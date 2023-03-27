#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <algorithm>
using namespace std;

struct Node
{
    Node() = default;
    Node(int key) : key{key} {}
	int key;
    int h{0};
    int balanceFactor{0};
	Node *left, *right, *parent;
};

class BST {
    protected:
        Node *root;
        virtual Node* insert(int key, Node *node)
        {
            if (node == nullptr) 
            {
                node = new Node;
                node->key = key;
                node->left = node->right = node->parent = nullptr;
            } else if (key > node->key) 
            {
                node->right = insert(key, node->right);
                node->right->parent = node;
            } else 
            {
                node->left = insert(key, node->left);
                node->left->parent = node;
            }
            return node;
        }

        virtual void inorder(Node *node)
        {
            if (node == nullptr)
                return;
            inorder(node->left);
            cout << node->key << ' ';
            inorder(node->right);
        }

        Node* search(int key, Node *node)
        {
            if (node == nullptr || key == node->key)
                return node;
            if (key > node->key)
                return search(key, node->right);
            return search(key, node->left);
        }

        Node* find_min(Node *node)
        {
            if (node == nullptr)
                return nullptr;
            if (node->left == nullptr)
                return node;
            return find_min(node->left);
        }

        Node* find_max(Node *node)
        {
            if (node == nullptr)
                return nullptr;
            if (node->right == nullptr)
                return node;
            return find_min(node->right);
        }

        Node* successor(Node *node)
        {
            if (node->right != nullptr)
                return find_min(node->right);

            Node *parent = node->parent;
            while (parent != nullptr && node == parent->right) {
                node = parent;
                parent = node->parent;
            }

            return parent;
        }

        Node* predecessor(Node *node)
        {
            if (node->left != nullptr)
                return find_max(node->left);

            Node *parent = node->parent;
            while (parent != nullptr && node == parent->left) {
                node = parent;
                parent = node->parent;
            }

            return parent;
        }

public:
	BST() : root(nullptr) {}

    //pomocna metoda za treci zadatak
    const Node* getRoot() {
        return root;
    }

	void print()
	{
		inorder(root);
	}

	virtual void insert(int key)
	{
		root = insert(key, root);
	}

    virtual void insert(const vector<int>& keys) {
        for(auto& key : keys) {
            root = insert(key, root);
        }
    }

	bool search(int key)
	{
		return search(key, root);
	}

	int find_min()
	{
		Node *node = find_min(root);
		if (node != nullptr)
			return node->key;
		return -1;
	}

	int find_max()
	{
		Node *node = find_max(root);
		if (node != nullptr)
			return node->key;
		return -1;
	}

	int successor(int key)
	{
		Node *node = search(key, root);
		if (node == nullptr)
			return -1;
		node = successor(node);
		if (node == nullptr)
			return -1;
		return node->key;
	}

	int predecessor(int key)
	{
		Node *node = search(key, root);
		if (node == nullptr)
			return -1;
		node = predecessor(node);
		if (node == nullptr)
			return -1;
		return node->key;
	}
};

class AVL : public BST {
    protected:
        Node* insert(int key, Node *node) override {
            if(node == nullptr) {
                // if(key > 17) cout << "Null" << endl;
                node = new Node;
                node->key = key;
                node->left = node->right = node->parent = nullptr;
            }
            
            else if (key > node->key) {
                // if(key > 17) cout << "Right" << endl;
                node->right = insert(key, node->right);
                node->right->parent = node;
            }
            else {
                // if(key > 17) cout << "Left" << endl;
                node->left = insert(key, node->left);
                node->left->parent = node;
            }
            update(node);
            // if(key > 17) print();
            // if(key > 17) cout << endl;
            return balance(node);
        }

        void update(Node* node) {
            int lh = -1;
            int rh = -1;

            if(node->left != nullptr) {
                lh = node->left->h;
            }
            if(node->right != nullptr) {
                rh = node->right->h;
            }

            node->h = 1 + max(lh, rh);
            node->balanceFactor = rh - lh;
        }

        Node* leftRotate(Node* node) {
            // cout << "Left rotate" << endl;
            if(node->right == nullptr) return node;

            Node* y = node->right;
            node->right = y->left;

            if(y->left != nullptr) {
                y->left->parent = node;
            }

            if(node->parent == nullptr) {
                root = y;
            }

            else if(node == node->parent->left) {
                node->parent->left = y;
            }
            else node->parent->right = y;

            y->left = node;
            y->parent = node->parent;
            node->parent = y;
            
            update(node);
            update(y);

            return y;
        }

        Node* rightRotate(Node* node) {
            // cout << "Right rotate" << endl;
            if(node->left == nullptr) return node;

            Node* y = node->left;
            node->left = y->right;

            if(y->right != nullptr) {
                y->right->parent = node;
            }

            if(node->parent == nullptr) {
                root = y;
            }

            else if(node == node->parent->left) {
                node->parent->left = y;
            }
            else node->parent->right = y;

            y->right = node;
            y->parent = node->parent;
            node->parent = y;

            update(node);
            update(y);

            return y;
        }

        Node* leftLeftCase(Node* node) {
            return rightRotate(node);
        }

        Node* leftRightCase(Node* node) {
            node->left = leftRotate(node->left);
            return leftLeftCase(node);
        }

        Node* rightRightCase(Node* node) {
            return leftRotate(node);
        }

        Node* rightLeftCase(Node* node) {
            node->right = rightRotate(node->right);
            return rightRightCase(node);
        }

        Node* balance(Node* node) {
            // if(node->key == 19) {
            //     cout << "Balancing node 19" << endl;
            // }
            // cout << "Balance(" << node->key << ")" << endl;
            if(node->balanceFactor == -2) {
                if(node->left->balanceFactor <= 0) {
                    // cout << "Left left" << endl;
                    return leftLeftCase(node);
                }
                else {
                    // cout << "Left right" << endl;
                    return leftRightCase(node);
                }
            }

            else if(node->balanceFactor == 2) {
                if(node->right->balanceFactor >= 0) {
                    // cout << "Right right" << endl;
                    return rightRightCase(node);
                }
                else {
                    // cout << "Right left" << endl;
                    return rightLeftCase(node);
                }
            }

            return node;
        }

        virtual void inorder(Node *node) override {
            if (node == nullptr)
                return;
            inorder(node->left);
            cout << node->key << "(" << node->balanceFactor <<   ") ";
            inorder(node->right);
        }

    public:
        void insert(int key) override {
            //cout << "Inserting key " << key << endl;
            root = insert(key, root);
            cout << root->key << endl;
        }

        void insert(const vector<int>& keys) override {
            for(auto& key : keys) {
                root = insert(key, root);
            }
        }
};

int main() {
    AVL avl{};
    avl.insert({10,20,30,40,50,25});

    avl.print();
    cout << endl;
}