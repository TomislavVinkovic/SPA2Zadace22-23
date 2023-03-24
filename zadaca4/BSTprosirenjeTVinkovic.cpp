#include <iostream>
#include <string>
#include <vector>
#include <cmath>
using namespace std;

struct Node
{
    Node() = default;
    Node(int key) : key{key} {}
	int key;
	Node *left, *right, *parent;
};

class BST
{

	//zad1
	friend bool structurallyEqual(const BST& T1, const BST& T2);

    //zad2
    friend BST nthPowerBST(BST& T, int n);

    //zad3
    friend BST BSTadd(BST& T1, BST& T2, bool optimize);
    friend BST BSTbuilder(const vector<int>& nodes);

	Node *root;
	Node* insert(int key, Node *node)
	{
		if (node == NULL) 
		{
			node = new Node;
			node->key = key;
			node->left = node->right = node->parent = NULL;
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

	void inorder(Node *node)
	{
		if (node == NULL)
			return;
		inorder(node->left);
		cout << node->key << ' ';
		inorder(node->right);
	}

	Node* search(int key, Node *node)
	{
		if (node == NULL || key == node->key)
			return node;
		if (key > node->key)
			return search(key, node->right);
		return search(key, node->left);
	}

	Node* find_min(Node *node)
	{
		if (node == NULL)
			return NULL;
		if (node->left == NULL)
			return node;
		return find_min(node->left);
	}

	Node* find_max(Node *node)
	{
		if (node == NULL)
			return NULL;
		if (node->right == NULL)
			return node;
		return find_min(node->right);
	}

	Node* successor(Node *node)
	{
		if (node->right != NULL)
			return find_min(node->right);

		Node *parent = node->parent;
		while (parent != NULL && node == parent->right) {
			node = parent;
			parent = node->parent;
		}

		return parent;
	}

	Node* predecessor(Node *node)
	{
		if (node->left != NULL)
			return find_max(node->left);

		Node *parent = node->parent;
		while (parent != NULL && node == parent->left) {
			node = parent;
			parent = node->parent;
		}

		return parent;
	}

public:
	BST() : root(NULL) {}

    //pomocna metoda za treci zadatak
    const Node* getRoot() {
        return root;
    }

	void print()
	{
		inorder(root);
	}

	void insert(int key)
	{
		root = insert(key, root);
	}

    void insert(const vector<int>& keys) {
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
		if (node != NULL)
			return node->key;
		return -1;
	}

	int find_max()
	{
		Node *node = find_max(root);
		if (node != NULL)
			return node->key;
		return -1;
	}

	int successor(int key)
	{
		Node *node = search(key, root);
		if (node == NULL)
			return -1;
		node = successor(node);
		if (node == NULL)
			return -1;
		return node->key;
	}

	int predecessor(int key)
	{
		Node *node = search(key, root);
		if (node == NULL)
			return -1;
		node = predecessor(node);
		if (node == NULL)
			return -1;
		return node->key;
	}
};


//zad 1
bool nodesEqual(const Node *node1, const Node *node2) {
	if(node1 == nullptr && node2 == nullptr) {
        return true;
    }

	else if((node1 == nullptr && node2 != nullptr) || (node2 == nullptr && node1 != nullptr)) {
		return false;
	}

	return 
		nodesEqual(node1->left, node2->left) &&
		nodesEqual(node1->right, node2->right);
}

bool structurallyEqual(const BST& T1, const BST& T2) {
	return nodesEqual(T1.root, T2.root);
}


//zad 2
void nthPowerNode(Node *& src, Node*& dest, int n) {
    if(src == nullptr) return;
    dest = new Node{static_cast<int>(pow(src->key, n))};
    nthPowerNode(src->left, dest->left, n);
    nthPowerNode(src->right, dest->right, n);
}


BST nthPowerBST(BST& T, int n) {
    BST b;
    nthPowerNode(T.root, b.root, n);
    return b;
}

//zad 3

void SortedArrayToBST(const vector<int>& nodes, Node*& node, int start, int end) {
    if (start > end) return;

    int mid = (start + end) / 2;
    node = new Node(nodes[mid]);


    SortedArrayToBST(nodes, node->left, start, mid - 1);
    SortedArrayToBST(nodes, node->right, mid + 1, end);
}

BST BSTbuilder(const vector<int>& nodes) {
    BST b;
    SortedArrayToBST(nodes, b.root, 0, nodes.size() - 1);

    return b;
}

void addNodes(Node *& src1, Node*& src2, Node*& dest) {
    //ako su isto strukturirani, dovoljno je orivjeriti jel 1 nullptr
    //jer su onda sigurno oba nullptr
    if(src1 == nullptr) return;

    dest = new Node{(src1->key + src2->key)};
    addNodes(src1->left, src2->left, dest->left);
    addNodes(src1->right, src2->right, dest->right);
}

void addNodesOptimized(Node* src1, Node* src2, vector<int>& nodes) {
    //ako su isto strukturirani, dovoljno je provjeriti jel 1 nullptr
    //jer su onda sigurno oba nullptr
    if(src1 == nullptr) return;

    nodes.push_back((src1->key + src2->key));
    addNodesOptimized(src1->left, src2->left, nodes);
    addNodesOptimized(src1->right, src2->right, nodes);
}

//zad3
BST BSTadd(BST& T1, BST& T2, bool optimize = false) {
    if(!structurallyEqual(T1, T2)) {
        //samo kopiraj T1 u novi BST i returnaj ga
        //ovo je najlaksi nacin jer koristi gotovu metodu
        return nthPowerBST(T1, 1);
    };

    if(optimize) {
        vector<int> nodes{};
        addNodesOptimized(T1.root, T2.root, nodes);
        return BSTbuilder(nodes);
    }

    else {
        BST b;
        addNodes(T1.root, T2.root, b.root);
        return b;
    }
}

int main() {
    //zad1
    cout << "Zadatak 1:" << endl;
	BST bst1{}, bst2{};

    vector<int> keys1{50, 25, 10, 30, 75, 100};
    vector<int> keys2{6, 3, 1, 4, 8, 10};

    bst1.insert(keys1);
    bst2.insert(keys2);

	string result1 = structurallyEqual(bst1, bst2)
		? "Stablo 1 i 2 su strukturalno jednaki"
		: "Stablo 1 i 2 nisu strukturalno jednaki";

	cout << result1 << endl;

    vector<int> keys3{50, 25, 10, 75, 100};
    vector<int> keys4{6, 3, 1, 8};

    BST bst3, bst4;
    bst3.insert(keys3);
    bst3.insert(keys4);

    string result2 = structurallyEqual(bst3, bst4)
                     ? "Stablo 3 i 4 su strukturalno jednaki"
                     : "Stablo 3 i 4 nisu strukturalno jednaki";

    cout << result2 << endl;

    //zad2
    cout << "Zadatak 2:" << endl;
    BST bst5;
    vector<int> keys5 ={5, 3, 1, 4, 7, 9};
    bst5.insert(keys5);

    BST pow2BST = nthPowerBST(bst5, 2);
    pow2BST.print();
    cout << endl;

    //zadatak 3:

    //način 1: standardan način, bez optimizacije "binarnosti" stabla
    cout << "Zadatak 3:" << endl;

    cout << "Način 1: standardan način, bez optimizacije 'binarnosti' stabla. Primjer iz pdf-a." << endl;
    BST bst6, bst7;
    vector<int> keys6 ={5, 3, 1, 4, 7, 9};
    vector<int> keys7 ={10, 5, 2, 8, 14, 16};

    bst6.insert(keys6);
    bst7.insert(keys7);

    BST addBST = BSTadd(bst6, bst7);
    addBST.print();
    cout << endl;

    //način 2: s optimizaicijom binarnosti stabla
    cout << "Način 2: s optimizaicijom binarnosti stabla. Ekstremno desno-težak primjer." << endl;

    vector<int> keys8 ={17, 18, 19, 20, 50, 15};
    vector<int> keys9 ={19, 50, 70, 80, 90, 5};

    BST bst8, bst9;
    bst8.insert(keys8);
    bst9.insert(keys9);

    cout << "Bez optimizacije:" << endl;
    BST addBSTNonOptimized = BSTadd(bst8, bst9, false);
    addBSTNonOptimized.print();
    cout << ", s rootom " << (addBSTNonOptimized.getRoot() != nullptr ? addBSTNonOptimized.getRoot()->key : -1e9);
    cout << endl;

    cout << "S optimizacijom:" << endl;
    BST addBSTOptimized = BSTadd(bst8, bst9, true);
    addBSTOptimized.print();
    cout << ", s rootom " << (addBSTOptimized.getRoot() != nullptr ? addBSTOptimized.getRoot()->key : -1e9);
    cout << endl;

    return 0;
}