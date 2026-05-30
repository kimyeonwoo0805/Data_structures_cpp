#include<stdio.h>
#include<iostream>
using namespace std;
class node {
private:
	friend class BSTMap;
	int key;
	string value;
	node* parent;
	node* left;
	node* right;
public:
	explicit node(int key, string value, node* parent)
		:key(key), value(value), parent(parent), left(nullptr), right(nullptr) {
	}
};
class BSTMap {
private:
	int n;
	node* root_node;

	node* find_entry(int key);
	node* get_successor(node* Node);
	bool erase(node* Node);
	void clear(node* Node);
	void inorder(node* Node);
	int getheight(node* Node);
public:
	BSTMap();
	~BSTMap();
	int size();
	int getheight();
	void print();
	bool empty();
	bool erase(int key);
	bool insert(int key, string value);
	string find(int key);
};
BSTMap::BSTMap()
	:root_node(nullptr), n(0) {
}
BSTMap::~BSTMap() {
	if (!empty()) {
		clear(root_node);
	}
}
int BSTMap::getheight() {
	if (empty()) {
		return -1;
	}
	return getheight(root_node);
}
int BSTMap::getheight(node*Node) {
	int max = 0;
	if (Node == nullptr) {
		return -1;
	}
	int lefth=getheight(Node->left);
	int righth=getheight(Node->right);
		if(max<lefth){
			max = lefth;
		}
		if (max<righth) {
			max = righth;
		}
	return max + 1;

}
void BSTMap::print() {
	if (empty()) {
		return;
	}
	inorder(root_node);
}
void BSTMap::inorder(node* Node) {

	if (Node->left != nullptr) { inorder(Node->left); }
	cout << Node->key << " ";
	if (Node->right != nullptr) { inorder(Node->right); }
}
node* BSTMap::find_entry(int key) {
	node* trv = root_node;
	while (trv != nullptr) {
		if (trv->key == key) {
			break;
		}
		if (key < trv->key) {
			trv = trv->left;
		}
		else {
			trv = trv->right;
		}
	}
	return trv;
}
node* BSTMap::get_successor(node* Node) {
	if (Node->right) {
		node* trv = Node->right;
		while (trv->left) {
			trv = trv->left;
		}
		return trv;
	}
	else {
		/*내가 오른쪽 자식인경우 타고 올라가다 처음으로 왼쪽자식이되는경우의 부모 반환 그게 successor*/
		node* trv = Node;
		node* Par = Node->parent;
		while (Par && Par->right == trv) {
			Par = Par->parent;
			trv = trv->parent;
		}
		return Par;
	}
}
bool BSTMap::erase(node* Node) {
	if (Node == nullptr) {
		return false;
	}
	if (Node->left != nullptr && Node->right != nullptr) {
		node* successor = get_successor(Node);
		Node->key = successor->key;
		Node->value = successor->value;
		Node = successor;
	}
	node* child;
	if (Node->left != nullptr) { child = Node->left; }
	else { child = Node->right; }
	if (Node == root_node) {
		root_node = child;
		if (child != nullptr) {
			child->parent = nullptr;
		}
	}
	else {
		node* Par = Node->parent;
		if (child != nullptr) { child->parent = Par; }
		if (Node == Par->left) { Par->left = child; }
		else { Par->right = child; }
	}
	--n;
	delete Node;
	return true;

}
void BSTMap::clear(node* Node) {
	if (Node == nullptr) {
		return;
	}
	if (Node->left != nullptr) { clear(Node->left); }
	if (Node->right != nullptr) { clear(Node->right); }
	delete Node;
}
int BSTMap::size() {
	return n;
}
bool BSTMap::empty() {
	return size() == 0;
}
bool BSTMap::erase(int key) {
	return erase(find_entry(key));
}
bool BSTMap::insert(int key, string value) {
	if (empty()) {
		root_node = new node(key, value, nullptr);
		++n;
		return true;
	}
	else {
		node* trv = root_node;
		node* Par = nullptr;
		while (trv != nullptr) {
			if (trv->key == key) {
				return false;
			}
			Par = trv;
			if (key < trv->key) {
				trv = trv->left;
			}
			else {
				trv = trv->right;
			}
		}
		node* Newnode = new node(key, value, Par);
		if (key < Par->key) {
			Par->left = Newnode;
		}
		else {
			Par->right = Newnode;
		}
		++n;
		return true;
	}
}
string BSTMap::find(int key) {
	node* target = find_entry(key);
	if (target == nullptr) {
		return "";
	}
	return target->value;
}
int main() {
	BSTMap myMap;

	// 1. 비어있는 상태 테스트
	cout << "--- Test 1: Empty State ---" << endl;
	cout << "Size: " << myMap.size() << " (Expected: 0)" << endl;
	cout << "Is Empty: " << (myMap.empty() ? "Yes" : "No") << endl << endl;

	// 2. 삽입 테스트 (루트 및 일반 노드)
	cout << "--- Test 2: Insertion ---" << endl;
	myMap.insert(50, "Root Node");
	myMap.insert(30, "Left Child");
	myMap.insert(70, "Right Child"); // 30과 70은 형제 관계가 됨
	myMap.insert(20, "Leaf 1");
	myMap.insert(40, "Leaf 2");
	myMap.insert(60, "Leaf 3");
	myMap.insert(80, "Leaf 4");
	cout<<myMap.getheight();

	return 0;
}