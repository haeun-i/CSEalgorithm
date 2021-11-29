#include <iostream>
#include <vector>
#include <cstring>
#include <algorithm>
using namespace std;

vector<pair<int, int>> graph[10001]; // �׷��� ������ �Է¹޴� ����
int* tree; // tree�� �� vertex�� ���°� tree(1), fringe(-1), unseen(0) �� �������� üũ�ϴ� �迭
int* fringe; // fringe ������ vertex�� ������ �ּ� ����ġ�� ����Ǵ� �迭

int n, m, q;

vector<int> print_vector; // ��� �� ������� �����ϴ� �迭
int minimum = 0; // ��å�� ��ġ�� ���� ����� ����


int find_min() { 	
	/* �׷����� node�� ��� tree node�� �Ǿ��� �� �˰����� ����Ǵµ�, �̸� üũ�ϱ� ����. 
	min ���� 1�̸� ��� ��尡 Ʈ�� ����� ��*/
	int min = 10001;
	for (int i = 1; i <= n; i++) {
		if (min > tree[i]) min = tree[i];
	}

	return min;
}


void prim(int s) {
	
	tree[s] = 1; // ù ��带 tree ���� ����
	print_vector.push_back(s); 

	if (!graph[s].empty()) {
		for (int i = 0; i < graph[s].size(); i++) {
			if (tree[graph[s][i].first] == 0) { // ù ���� ������ ������ ã�´�.
				if (fringe[graph[s][i].first] > graph[s][i].second) fringe[graph[s][i].first] = graph[s][i].second; 
				// ����ġ�� �� ���� ������ ��ü(decreaseKey). 
				// ����� �ʱⰪ�� 10001�� �ԷµǾ� �����Ƿ� ������ ���� �迭�� �ԷµǹǷ�, ����ġ�� fringe �迭�� �Է��ϴ� �����̶�� �����ϸ� �ȴ�.
				tree[graph[s][i].first] = -1; // fringe�� ����
			}
		}
	}

	while (find_min() != 1) {
		// fringe node�� �߿� ���� ����ġ�� ���� node�� idx�� weight ���� ��´�.
		int min_weight = 10001;
		int min_idx = 10001;
		for (int j = 1; j <= n; j++) {
			if (tree[j] == -1) {
				if (fringe[j] < min_weight) {
					min_weight = fringe[j];
					min_idx = j;
				}
			}
		}

		tree[min_idx] = 1; // �� idx�� �ش��ϴ� ���� tree node�� ����
		print_vector.push_back(min_idx);
		minimum += fringe[min_idx]; // ����ġ�� ������ ���ϱ� ���� �����ش�.

		if (!graph[min_idx].empty()) { // �߰��� ���� ������ ������ Ž���Ѵ�.
			for (int i = 0; i < graph[min_idx].size(); i++) {
				if (tree[graph[min_idx][i].first] == 0) { // unseen�̸� fringe�� ����
					tree[graph[min_idx][i].first] = -1; 
				}
				if (fringe[graph[min_idx][i].first] > graph[min_idx][i].second) { 
					// ����ġ�� �� ���� ������ ��ü(decreaseKey)�� �Է� (fringe �迭�� 10001�� �ƴ� ���� ����־����� ��ü, �ƴϸ� �Է��̶�� �� �� �ִ�.)
					fringe[graph[min_idx][i].first] = graph[min_idx][i].second;
				}
			}
		}
	}

	// ���� ������ ���� ����� �迭���� �ʱ�ȭ�Ѵ�.
	fill(tree, tree + n + 1, 0); 
	fill(fringe, fringe + n + 1, 10001);
}

int main() {
	ios_base::sync_with_stdio(false);
	cin.tie(0);
	cout.tie(0);

	cin >> n >> m >> q;

	tree = new int[n + 1]; // �Է°��� ���� �迭�� ũ�⸦ �ʱ�ȭ�Ѵ�.
	fringe = new int[n + 1]; 

	fill(tree, tree+n+1, 0); // �� �迭�� ���� �ʱ�ȭ�Ѵ�.
	fill(fringe, fringe+n+1, 10001); // fringe�迭���� ����ġ�� �ּҰ��� ã�� �����ؾ��ϹǷ�, �ʱ�ȭ�� ����ġ�� ������ �ִ밪�� 10001�� �ʱ�ȭ�Ѵ�.

	for (int i = 0; i < m; i++) { // graph�� ������ �Է¹޴´�.
		int s1, s2, cost;
		cin >> s1 >> s2 >> cost;

		graph[s1].push_back(make_pair(s2, cost));
		graph[s2].push_back(make_pair(s1, cost));
	}

	for (int i = 0; i < q; i++) {
		string p;
		int s;
		cin >> p >> s;

		if (p == "P") {
			prim(s);

			cout << minimum << " "; // ����ġ �� ���
			for (int i = 0; i < n; i++) {
				cout << print_vector[i] << " "; // prim �˰��� ������ ���� ����� �� ���
			}
			cout << endl;
			
			print_vector.clear(); // ���� ������ ���� �ʱ�ȭ
			minimum = 0;
		}
	}

}