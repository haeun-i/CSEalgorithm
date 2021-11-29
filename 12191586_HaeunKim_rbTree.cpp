#include <iostream>
#include <algorithm>
using namespace std;

enum Color {
	red, black
};


struct node {
	int A;
	string N;
	int S;
	int P;
	Color color;

	node* left;
	node* right;
	node* parent;

	node(int A, string N, int S, int P) { // ��� ������
		this->A = A;
		this->N = N;
		this->S = S;
		this->P = P;
		this->color = red; // ������ ó������ red node�� �����Ѵ�.

		this->left = nullptr;
		this->right = nullptr;
		this->parent = nullptr;
	}
};



class redBlackTree {
private:
	node* root;
public:
	redBlackTree() {
		this->root = nullptr;
	}

	int findDepth(node* curNode) { // ��͸� ���� depth ���ϱ�
		if (curNode->parent == nullptr) return 0;
		return findDepth(curNode->parent) + 1;
	}

	node* insertNode(int A, string N, int S, int P) {
		node* newNode = new node(A, N, S, P); // ���� �� ��� ����
		node* parentNode = nullptr; // ��尡 ���� �� ��ġ


		if (root == nullptr) {
			newNode->color = black; // root ���� black node�� ����
			root = newNode;
		}
		else {
			node* curNode = root;
			while (curNode != nullptr) { // ��尡 ���� �� ��ġ ã��
				parentNode = curNode;
				if (curNode->A == A) return newNode;
				else if (curNode->A > A) curNode = curNode->left;
				else if (curNode->A < A) curNode = curNode->right;
			}

			// ��� ����
			if (parentNode->A > A) { 
				newNode->parent = parentNode;
				parentNode->left = newNode;
			}
			else {
				newNode->parent = parentNode;
				parentNode->right = newNode;
			}
		}

		node* tempNode = newNode;
		while (tempNode != nullptr && tempNode->parent != nullptr) { 
			// ���� ��忡�� �ö󰡸鼭 �߰������� recoloring, resturcturing�� �ʿ��� ��Ȳ üũ
			if (tempNode->color == red && tempNode->parent->color == red) { // double red�� �߻��ϸ� fix
				fixNode(tempNode);
			}
			tempNode = tempNode->parent; // ���� ��忡�� 1��ŭ �ö󰡼� Ž��
		}

		return newNode;
	}

	void fixNode(node* curNode) {
		node* parentNode = curNode->parent;
		node* uncleNode = nullptr; // recoloring, restructuring ��Ȳ�� �����ϱ� ���� �θ� ����� ���� ��带 ���Ѵ�.
		if (parentNode->parent->left == parentNode) {
			uncleNode = parentNode->parent->right;
		}
		else if (parentNode->parent->right == parentNode) {
			uncleNode = parentNode->parent->left;
		}

		if (uncleNode == nullptr || uncleNode->color == black) restructuring(curNode); 
		// �θ� ����� ���� ��尡 black�� ��� restructuring
		else if (uncleNode->color == red) recoloring(curNode); // �θ� ����� ���� ��尡 red�� ��� recoloring
	}

	void recoloring(node* curNode) {
		if (curNode->parent->parent != root) { // root node�� �ƴ� ��쿡�� grandNode�� red node�� �����Ѵ�.
			curNode->parent->parent->color = red;
		}

		// parentNode, uncleNode�� black node�� �����Ѵ�.
		curNode->parent->parent->left->color = black;
		curNode->parent->parent->right->color = black;
	}

	void restructuring(node* curNode) {

		node* parentNode = curNode->parent;
		node* grandNode = parentNode->parent;

		if (curNode == parentNode->left && parentNode == grandNode->right) {
			// case1 RL ȸ�� 
			grandNode->right = rotateR(parentNode);
			grandNode->right->parent = grandNode;
			grandNode = rotateL(grandNode);

			// ���� �ٲ� subtree�� root node�� black node, left node�� red node�� �����Ѵ�.
			grandNode->color = black;
			grandNode->left->color = red;
		}
		else if (curNode == parentNode->right && parentNode == grandNode->left) {
			// case2 LR ȸ��
			grandNode->left = rotateL(parentNode);
			grandNode->left->parent = grandNode;
			grandNode = rotateR(grandNode);

			// ���� �ٲ� subtree�� root node�� black node, right node�� red node�� �����Ѵ�
			grandNode->color = black;
			grandNode->right->color = red;
		}
		else if (curNode == parentNode->left && parentNode == grandNode->left) {
			// case3 LLȸ��
			grandNode = rotateR(grandNode);

			// ���� �ٲ� subtree�� root node�� black node, right node�� red node�� �����Ѵ�
			grandNode->color = black;
			grandNode->right->color = red;
		}
		else if (curNode == parentNode->right && parentNode == grandNode->right) {
			// case4 RRȸ��
			grandNode = rotateL(grandNode);

			// ���� �ٲ� subtree�� root node�� black node, right node�� red node�� �����Ѵ�
			grandNode->color = black;
			grandNode->left->color = red;
		}
	}

	node* rotateL(node* small) {
		node* mid = small->right; // ���� ���� ����� right node�� parent node�� ��

		bool isLeft = true; // ���� ���� ��尡 ���� �ڽ��̾����� ������ �ڽ��̾����� Ȯ��
		if (small->parent != nullptr) {
			if (small->parent->left == small) isLeft = true;
			else if (small->parent->right == small)isLeft = false;
		}

		small->right = mid->left; // parent node�� �� ����� �ڽ� Ʈ�� �̵�
		if (small->left != nullptr) mid->left->parent = small;

		if (small->parent == nullptr) root = mid; // ���� �ٲ� �θ� ��尡 root�� �� ��� 
		else if (small->parent != nullptr && isLeft) small->parent->left = mid; 
		// ���� ���� ��尡 �θ� ����� left node���� ��� left�� mid ����
		else if (small->parent != nullptr && !isLeft) small->parent->right = mid; 
		// ���� ���� ��尡 �θ� ����� right node���� ��� right�� mid ����
		
		mid->parent = small->parent; // parent �缳��
		small->parent = mid;

		mid->left = small; // tree ���� �ϼ�

		return mid; // ���� ������� parent ��� ��ȯ
	}

	node* rotateR(node* big) {
		node* mid = big->left; // ���� ū ����� left node�� parent node�� ��
		
		bool isLeft = true; // grandNode�� ���� �ڽ����� ������ �ڽ����� Ȯ��
		if (big->parent != nullptr) {
			if (big->parent->left == big) isLeft = true;
			else if (big->parent->right == big)isLeft = false;
		}

		big->left = mid->right; // parent node�� �� ����� �ڽ� Ʈ�� �̵�
		if (mid->right != nullptr) mid->right->parent = big;

		if (big->parent == nullptr)	root = mid; // ���� �ٲ� �θ� ��尡 root�� �� ��� 
		else if (big->parent != nullptr && isLeft) big->parent->left = mid;
		// ���� ���� ��尡 �θ� ����� left node���� ��� left�� mid ����
		else if (big->parent != nullptr && !isLeft) big->parent->right = mid;
		// ���� ���� ��尡 �θ� ����� right node���� ��� right�� mid ����

		mid->parent = big->parent; // parent �缳��
		big->parent = mid;
		
		mid->right = big; // tree ���� �ϼ�

		return mid; // ���� ������� parent ��� ��ȯ
	}

	node* findNode(int A) {
		node* curNode = root;
		while (curNode != nullptr) {
			if (curNode->A == A) return curNode; // ��ġ�ϴ� �� ã���� ��ȯ
			else if (curNode->A > A) { // ã�� ���� ���� ��� ������ ������ left child�� �̵�
				curNode = curNode->left;
			}
			else { // ã�� ���� ���� ��� ������ ũ�� right child�� �̵�
				curNode = curNode->right;
			}
		}
		return nullptr; // ã�� ��尡 ������ null�� ����� ���� nullptr ��ȯ
	}

	void updateNode(node* n, string N, int S, int P) { // �Ű������� ���� ���� ������Ʈ
		n->N = N;
		n->S = S;
		n->P = P;
	}

	void discount(int x, int y, int ratio) { 
		preorder(root, x, y, ratio); // root���� preorder Ž�� �ؼ� ���� ����
	}

	void preorder(node* curNode, int x, int y, int ratio) {

		if (curNode != nullptr) {
			if (curNode->A >= x && curNode->A <= y) { // preorder �������� id�� ������ x�� y ���̸� ����
				curNode->P = curNode->P * (100 - ratio) / 100;
			}
			preorder(curNode->left, x, y, ratio); // preorder ��� ����
			preorder(curNode->right, x, y, ratio);
		}
	}
};

int main() {

	ios_base::sync_with_stdio(false);
	cin.tie(NULL);
	cout.tie(NULL);

	redBlackTree tree;

	int t;
	cin >> t;

	for (int i = 0; i < t; i++) {

		string instruct;
		cin >> instruct;

		if (instruct == "I") { // ���ø����̼� ��� 
			int A, S, P;
			string N;
			cin >> A >> N >> S >> P;
			node* n = tree.findNode(A); // �ش� ���ø����̼��� ��ϵǾ� �ִٸ� insert ������ ��ġ�� �ʴ´�.
			if (n == nullptr) {
				n = tree.insertNode(A, N, S, P); // ��� ����
			}
			cout << tree.findDepth(n) << '\n'; // depth ���

		}
		else if (instruct == "F") { // ���ø����̼� �˻�
			int A;
			cin >> A;

			node* n = tree.findNode(A); // ��� �˻� 
			if (n != nullptr) { // ã�� ����� ���� ���
				cout << tree.findDepth(n) << " " << n->N << " " << n->S << " " << n->P << '\n';
			}
			else { // ��尡 �������� �ʴ� ���
				cout << "NULL" << '\n';
			}
		}
		else if (instruct == "R") { // ���ø����̼� ������Ʈ
			int A, S, P;
			string N;
			cin >> A >> N >> S >> P;
			node* n = tree.findNode(A); // id�� �´� ��� ã�ƿ���

			if (n == nullptr) { // ��带 ã�� �� ���ٸ� NULL ���
				cout << "NULL" << '\n';
			}
			else { // ã�ƿ� ��带 update�ϰ� depth ���
				tree.updateNode(n, N, S, P);
				cout << tree.findDepth(n) << '\n';
			}
		}
		else if (instruct == "D") { // ���ø����̼� ����
			int x, y, P;
			cin >> x >> y >> P;

			tree.discount(x, y, P); // ��� �� �������ִ� �Լ��� ���� �� ������ ����
		}
	}
}
