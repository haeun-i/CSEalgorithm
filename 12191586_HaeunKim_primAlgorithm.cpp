#include <iostream>
#include <vector>
#include <cstring>
#include <algorithm>
using namespace std;

vector<pair<int, int>> graph[10001]; // 그래프 정보를 입력받는 벡터
int* tree; // tree의 각 vertex의 상태가 tree(1), fringe(-1), unseen(0) 중 무엇인지 체크하는 배열
int* fringe; // fringe 상태인 vertex가 가지는 최소 가중치가 저장되는 배열

int n, m, q;

vector<int> print_vector; // 출력 될 결과값을 저장하는 배열
int minimum = 0; // 산책로 설치에 들어가는 비용의 총합


int find_min() { 	
	/* 그래프의 node가 모두 tree node가 되었을 때 알고리즘이 종료되는데, 이를 체크하기 위함. 
	min 값이 1이면 모든 노드가 트리 노드인 것*/
	int min = 10001;
	for (int i = 1; i <= n; i++) {
		if (min > tree[i]) min = tree[i];
	}

	return min;
}


void prim(int s) {
	
	tree[s] = 1; // 첫 노드를 tree 노드로 지정
	print_vector.push_back(s); 

	if (!graph[s].empty()) {
		for (int i = 0; i < graph[s].size(); i++) {
			if (tree[graph[s][i].first] == 0) { // 첫 노드와 인접한 노드들을 찾는다.
				if (fringe[graph[s][i].first] > graph[s][i].second) fringe[graph[s][i].first] = graph[s][i].second; 
				// 가중치가 더 작은 값으로 교체(decreaseKey). 
				// 현재는 초기값인 10001이 입력되어 있으므로 무조건 값이 배열이 입력되므로, 가중치를 fringe 배열에 입력하는 과정이라고 생각하면 된다.
				tree[graph[s][i].first] = -1; // fringe로 설정
			}
		}
	}

	while (find_min() != 1) {
		// fringe node들 중에 가장 가중치가 작은 node의 idx와 weight 값을 얻는다.
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

		tree[min_idx] = 1; // 그 idx에 해당하는 값을 tree node로 지정
		print_vector.push_back(min_idx);
		minimum += fringe[min_idx]; // 가중치의 총합을 구하기 위해 더해준다.

		if (!graph[min_idx].empty()) { // 추가된 노드와 인접한 노드들을 탐색한다.
			for (int i = 0; i < graph[min_idx].size(); i++) {
				if (tree[graph[min_idx][i].first] == 0) { // unseen이면 fringe로 설정
					tree[graph[min_idx][i].first] = -1; 
				}
				if (fringe[graph[min_idx][i].first] > graph[min_idx][i].second) { 
					// 가중치가 더 작은 값으로 교체(decreaseKey)나 입력 (fringe 배열에 10001이 아닌 값이 들어있었으면 교체, 아니면 입력이라고 볼 수 있다.)
					fringe[graph[min_idx][i].first] = graph[min_idx][i].second;
				}
			}
		}
	}

	// 다음 과정을 위해 사용한 배열들을 초기화한다.
	fill(tree, tree + n + 1, 0); 
	fill(fringe, fringe + n + 1, 10001);
}

int main() {
	ios_base::sync_with_stdio(false);
	cin.tie(0);
	cout.tie(0);

	cin >> n >> m >> q;

	tree = new int[n + 1]; // 입력값에 맞춰 배열의 크기를 초기화한다.
	fringe = new int[n + 1]; 

	fill(tree, tree+n+1, 0); // 각 배열의 값을 초기화한다.
	fill(fringe, fringe+n+1, 10001); // fringe배열에는 가중치의 최소값을 찾아 저장해야하므로, 초기화는 가중치의 가능한 최대값인 10001로 초기화한다.

	for (int i = 0; i < m; i++) { // graph의 정보를 입력받는다.
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

			cout << minimum << " "; // 가중치 합 출력
			for (int i = 0; i < n; i++) {
				cout << print_vector[i] << " "; // prim 알고리즘 실행을 통해 저장된 값 출력
			}
			cout << endl;
			
			print_vector.clear(); // 다음 과정을 위해 초기화
			minimum = 0;
		}
	}

}