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

	node(int A, string N, int S, int P) { // 노드 생성자
		this->A = A;
		this->N = N;
		this->S = S;
		this->P = P;
		this->color = red; // 무조건 처음에는 red node로 생성한다.

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

	int findDepth(node* curNode) { // 재귀를 통해 depth 구하기
		if (curNode->parent == nullptr) return 0;
		return findDepth(curNode->parent) + 1;
	}

	node* insertNode(int A, string N, int S, int P) {
		node* newNode = new node(A, N, S, P); // 삽입 할 노드 생성
		node* parentNode = nullptr; // 노드가 삽입 될 위치


		if (root == nullptr) {
			newNode->color = black; // root 노드는 black node로 변경
			root = newNode;
		}
		else {
			node* curNode = root;
			while (curNode != nullptr) { // 노드가 삽입 될 위치 찾기
				parentNode = curNode;
				if (curNode->A == A) return newNode;
				else if (curNode->A > A) curNode = curNode->left;
				else if (curNode->A < A) curNode = curNode->right;
			}

			// 노드 삽입
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
			// 현재 노드에서 올라가면서 추가적으로 recoloring, resturcturing이 필요한 상황 체크
			if (tempNode->color == red && tempNode->parent->color == red) { // double red가 발생하면 fix
				fixNode(tempNode);
			}
			tempNode = tempNode->parent; // 현재 노드에서 1만큼 올라가서 탐색
		}

		return newNode;
	}

	void fixNode(node* curNode) {
		node* parentNode = curNode->parent;
		node* uncleNode = nullptr; // recoloring, restructuring 상황을 구분하기 위해 부모 노드의 형제 노드를 구한다.
		if (parentNode->parent->left == parentNode) {
			uncleNode = parentNode->parent->right;
		}
		else if (parentNode->parent->right == parentNode) {
			uncleNode = parentNode->parent->left;
		}

		if (uncleNode == nullptr || uncleNode->color == black) restructuring(curNode); 
		// 부모 노드의 형제 노드가 black인 경우 restructuring
		else if (uncleNode->color == red) recoloring(curNode); // 부모 노드의 형제 노드가 red인 경우 recoloring
	}

	void recoloring(node* curNode) {
		if (curNode->parent->parent != root) { // root node가 아닌 경우에만 grandNode를 red node로 변경한다.
			curNode->parent->parent->color = red;
		}

		// parentNode, uncleNode를 black node로 변경한다.
		curNode->parent->parent->left->color = black;
		curNode->parent->parent->right->color = black;
	}

	void restructuring(node* curNode) {

		node* parentNode = curNode->parent;
		node* grandNode = parentNode->parent;

		if (curNode == parentNode->left && parentNode == grandNode->right) {
			// case1 RL 회전 
			grandNode->right = rotateR(parentNode);
			grandNode->right->parent = grandNode;
			grandNode = rotateL(grandNode);

			// 새로 바뀐 subtree의 root node를 black node, left node를 red node로 변경한다.
			grandNode->color = black;
			grandNode->left->color = red;
		}
		else if (curNode == parentNode->right && parentNode == grandNode->left) {
			// case2 LR 회전
			grandNode->left = rotateL(parentNode);
			grandNode->left->parent = grandNode;
			grandNode = rotateR(grandNode);

			// 새로 바뀐 subtree의 root node를 black node, right node를 red node로 변경한다
			grandNode->color = black;
			grandNode->right->color = red;
		}
		else if (curNode == parentNode->left && parentNode == grandNode->left) {
			// case3 LL회전
			grandNode = rotateR(grandNode);

			// 새로 바뀐 subtree의 root node를 black node, right node를 red node로 변경한다
			grandNode->color = black;
			grandNode->right->color = red;
		}
		else if (curNode == parentNode->right && parentNode == grandNode->right) {
			// case4 RR회전
			grandNode = rotateL(grandNode);

			// 새로 바뀐 subtree의 root node를 black node, right node를 red node로 변경한다
			grandNode->color = black;
			grandNode->left->color = red;
		}
	}

	node* rotateL(node* small) {
		node* mid = small->right; // 가장 작은 노드의 right node가 parent node가 됨

		bool isLeft = true; // 가장 작은 노드가 왼쪽 자식이었는지 오른쪽 자식이었는지 확인
		if (small->parent != nullptr) {
			if (small->parent->left == small) isLeft = true;
			else if (small->parent->right == small)isLeft = false;
		}

		small->right = mid->left; // parent node가 될 노드의 자식 트리 이동
		if (small->left != nullptr) mid->left->parent = small;

		if (small->parent == nullptr) root = mid; // 새로 바뀐 부모 노드가 root가 된 경우 
		else if (small->parent != nullptr && isLeft) small->parent->left = mid; 
		// 제일 작은 노드가 부모 노드의 left node였던 경우 left에 mid 연결
		else if (small->parent != nullptr && !isLeft) small->parent->right = mid; 
		// 제일 작은 노드가 부모 노드의 right node였던 경우 right에 mid 연결
		
		mid->parent = small->parent; // parent 재설정
		small->parent = mid;

		mid->left = small; // tree 구조 완성

		return mid; // 새로 만들어진 parent 노드 반환
	}

	node* rotateR(node* big) {
		node* mid = big->left; // 가장 큰 노드의 left node가 parent node가 됨
		
		bool isLeft = true; // grandNode가 왼쪽 자식인지 오른쪽 자식인지 확인
		if (big->parent != nullptr) {
			if (big->parent->left == big) isLeft = true;
			else if (big->parent->right == big)isLeft = false;
		}

		big->left = mid->right; // parent node가 될 노드의 자식 트리 이동
		if (mid->right != nullptr) mid->right->parent = big;

		if (big->parent == nullptr)	root = mid; // 새로 바뀐 부모 노드가 root가 된 경우 
		else if (big->parent != nullptr && isLeft) big->parent->left = mid;
		// 제일 작은 노드가 부모 노드의 left node였던 경우 left에 mid 연결
		else if (big->parent != nullptr && !isLeft) big->parent->right = mid;
		// 제일 작은 노드가 부모 노드의 right node였던 경우 right에 mid 연결

		mid->parent = big->parent; // parent 재설정
		big->parent = mid;
		
		mid->right = big; // tree 구조 완성

		return mid; // 새로 만들어진 parent 노드 반환
	}

	node* findNode(int A) {
		node* curNode = root;
		while (curNode != nullptr) {
			if (curNode->A == A) return curNode; // 일치하는 값 찾으면 반환
			else if (curNode->A > A) { // 찾는 값이 현재 노드 값보다 작으면 left child로 이동
				curNode = curNode->left;
			}
			else { // 찾는 값이 현재 노드 값보다 크면 right child로 이동
				curNode = curNode->right;
			}
		}
		return nullptr; // 찾는 노드가 없으면 null값 출력을 위해 nullptr 반환
	}

	void updateNode(node* n, string N, int S, int P) { // 매개변수를 통해 정보 업데이트
		n->N = N;
		n->S = S;
		n->P = P;
	}

	void discount(int x, int y, int ratio) { 
		preorder(root, x, y, ratio); // root부터 preorder 탐색 해서 할인 적용
	}

	void preorder(node* curNode, int x, int y, int ratio) {

		if (curNode != nullptr) {
			if (curNode->A >= x && curNode->A <= y) { // preorder 과정에서 id의 범위가 x와 y 사이면 할인
				curNode->P = curNode->P * (100 - ratio) / 100;
			}
			preorder(curNode->left, x, y, ratio); // preorder 재귀 수행
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

		if (instruct == "I") { // 애플리케이션 등록 
			int A, S, P;
			string N;
			cin >> A >> N >> S >> P;
			node* n = tree.findNode(A); // 해당 애플리케이션이 등록되어 있다면 insert 과정을 거치지 않는다.
			if (n == nullptr) {
				n = tree.insertNode(A, N, S, P); // 노드 삽입
			}
			cout << tree.findDepth(n) << '\n'; // depth 출력

		}
		else if (instruct == "F") { // 애플리케이션 검색
			int A;
			cin >> A;

			node* n = tree.findNode(A); // 노드 검색 
			if (n != nullptr) { // 찾은 노드의 정보 출력
				cout << tree.findDepth(n) << " " << n->N << " " << n->S << " " << n->P << '\n';
			}
			else { // 노드가 존재하지 않는 경우
				cout << "NULL" << '\n';
			}
		}
		else if (instruct == "R") { // 애플리케이션 업데이트
			int A, S, P;
			string N;
			cin >> A >> N >> S >> P;
			node* n = tree.findNode(A); // id에 맞는 노드 찾아오기

			if (n == nullptr) { // 노드를 찾을 수 없다면 NULL 출력
				cout << "NULL" << '\n';
			}
			else { // 찾아온 노드를 update하고 depth 출력
				tree.updateNode(n, N, S, P);
				cout << tree.findDepth(n) << '\n';
			}
		}
		else if (instruct == "D") { // 애플리케이션 할인
			int x, y, P;
			cin >> x >> y >> P;

			tree.discount(x, y, P); // 노드 값 할인해주는 함수에 범위 및 할인율 전달
		}
	}
}
