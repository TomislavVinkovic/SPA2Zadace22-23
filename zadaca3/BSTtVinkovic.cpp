#include <iostream>
#include <limits>
#include <vector>

using namespace std;

struct Node {
    int key;
    Node *left, *right, *parent;
    ~Node() {
        if(left != nullptr) delete left;
        if(right != nullptr) delete right;
        //sad obrisi ovaj node, delete je implicitan
    }
};

class BST {
    Node *root;

    Node* insert(int key, Node *node) {

        //new node to insert
        Node* n = new Node();
        n->key = key;

        Node* y = nullptr;

        while(node != nullptr) {
            y = node;
            if(n->key > node->key) {
                node = node->right;
            }
            else node = node->left;
        }
        n->parent = y;

        if(y == nullptr) {
            root = n;
            return n;
        }

        if(n->key > y->key) {
            y->right = n;
        }
        else y->left = n;

        return n;
    }

    void inorder(Node *node) {
        if(node == nullptr) return;
        inorder(node->left);
        cout << node->key << " ";
        inorder(node->right);
    }

    Node* find_min(Node *node) {
        Node* y = nullptr;

        while(node != nullptr) {
            y = node;
            node = node->left;
        };

        return y;
    }

    Node* find_max(Node *node) {
        Node* y = nullptr;

        while(node != nullptr) {
            y = node;
            node = node->right;
        };

        return y;
    }

    Node* successor(Node *node) {
        if(node->right != nullptr) {
            return find_min(node->right);
        }

        Node* y = node->parent;
        if(y == nullptr) return nullptr;

        while(y != nullptr && y->right == node) {
            node = y;
            y = y->parent;
        }

        return y;
    }

    Node* predecessor(Node *node) {
        if(node->right != nullptr) {
            return find_max(node->left);
        }

        Node* y = node->parent;
        if(y == nullptr) return nullptr;

        while(y != nullptr && y->left == node) {
            node = y;
            y = y->parent;
        }

        return y;
    }

    Node* search(int key, Node* n) {
        while(n != nullptr) {
            if(n->key == key) return n;
            if(key < n->key) n = n->left;
            else n = n->right;
        }

        return n;
    }

    /// @brief 
    /// @param u node na cije mjesto radim transplant
    /// @param v node koji se transplanta na mjesto u
    void transplant(Node* u, Node* v) {
        if(u->parent == nullptr) {
            root = v;
        }
        else if(u == u->parent->left) {
            u->parent->left = v;
        }
        else {
            u->parent->right = v;
        }
        v->parent = u->parent;
    }

    Node* BSTdelete(int key, Node *node) {
        Node* n = search(key, node);
        if(n == nullptr) return nullptr;
        
        //slucajevi:

        //1. n je list
        if(n->right == nullptr && n->left == nullptr) {
            if(n == n->parent->left) n->parent->left = nullptr;
            else if(n == n->parent->right) n->parent->right = nullptr;

            delete n;

            return nullptr;
        }

        //2. -> n nema desno dijete
        if(n->right == nullptr) {
            Node* m = n->left;
            transplant(n, n->left);
            delete n;
            return m;
            
        }

        //3. -> n nema lijevo dijete
        else if(n->left == nullptr) {
            Node* m = n->right;
            transplant(n, n->right);
            delete n;
            return m;
        }

        // Inace, zamijeni node sa njegovim sljedbenikom
        else {
            Node* y = successor(n);
            if(y->parent != n) {
                transplant(y, y->parent);
                y->right = n->right;
                y->right->parent = y;
            }
            transplant(n, y);
            y->left = n->left;
            y->left->parent = y;

            return y;
        }
    }

    public:
        BST() : root(nullptr) {}

        ~BST() {
            //obrisi uz pomoc Node destruktora
            delete root;
        }

        void print() {
            inorder(root);
        }
        
        bool search(int key) {
            return search(key, root) != nullptr;
        }

        void insert(const vector<int>& keys) {
            for(auto& key : keys) {
                insert(key);
            }
        }

        void insert(int key) {
            //cout << "Inserting key " << key << endl;
            Node* n = search(key, root);
            if(n != nullptr) return;

            insert(key, root);
        }

        int find_min() {
            Node* n = find_min(root);
            if(n == nullptr) {
                //obrnuta min/max logika
                return numeric_limits<int>::max();
            }
            return n->key;
        }

        int find_max() {
            Node* n = find_max(root);
            if(n == nullptr) {
                //obrnuta min/max logika
                return numeric_limits<int>::min();
            }
            return n->key;
        }

        int successor(int key) {
            Node* n = search(key, root);
            if(n == nullptr) {
                return numeric_limits<int>::min();
            }

            n = successor(n);

            if(n == nullptr) {
                return numeric_limits<int>::min(); 
            }
            return n->key;
        }

        int predecessor(int key) {
            Node* n = search(key, root);
            if(n == nullptr) {
                return numeric_limits<int>::max();
            }

            n = predecessor(n);

            if(n == nullptr) {
                return numeric_limits<int>::max(); 
            }
            return n->key;
            
        }

        void BSTdelete(int key) {
            BSTdelete(key, root);
        }
};

int main(int argc, char const *argv[]) {
    
    BST tree;
    tree.insert({5, 10, 4, 8, 9, 1, 11, 15, 3});
    tree.print();

    cout << endl;

    cout << tree.find_min() << endl;
    cout << tree.find_max() << endl;

    cout << tree.successor(8) << endl;
    cout << tree.predecessor(10) << endl;

    tree.BSTdelete(10);

    tree.print();
    cout << endl;
        
    return 0;
}