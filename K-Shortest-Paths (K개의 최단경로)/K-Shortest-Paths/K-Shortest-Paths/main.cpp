
#include <iostream>
#include <vector>
#include <queue>
#include <random>
using namespace std;


mt19937 randomGen(0x69420);
int RandomInt(int lowerBound, int upperBound) { return uniform_int_distribution<int>(lowerBound, upperBound)(randomGen); }


class PersistentHeap
{
public:
	using lint = long long;
	using lpair = pair<lint, lint>;

	struct Node {
		Node* son[2];   // { leftson, rightson }
		lpair value;     // { sidetrack, next }

		Node() {
			son[0] = son[1] = NULL;
			value = lpair(-1e18, -1e18);
		}
		Node(lpair _value) {
			son[0] = son[1] = NULL;
			value = _value;
		}
	};


private:
	size_t mHeapTableSize;
	vector<Node*> mHeapTable;


public:
	PersistentHeap() :PersistentHeap(0) {}
	PersistentHeap(size_t heapSize) :mHeapTableSize(heapSize), mHeapTable(vector<Node*>(heapSize, NULL)) {}


public:
	Node*& operator[](int index) { return mHeapTable[index]; }
	void Assign(size_t heapSize) { mHeapTableSize = heapSize; mHeapTable.assign(heapSize, NULL); }


public:
	static Node* NodeCopy(Node* node) {   // node를 복사하고 반환
		if (node == NULL) return NULL;
		Node* newNode = new Node();
		newNode->son[0] = node->son[0];
		newNode->son[1] = node->son[1];
		newNode->value = node->value;
		return newNode;
	}

	static Node* NodeMerge(Node* x, Node* y) {   // 주어진 두 개의 node를 합치고 최상위 노드를(root)를 반환한다
		if (!x) return y;
		if (!y) return x;
		if (x->value > y->value) swap(x, y);
		int idx = RandomInt(0, 1);
		if (x->son[idx]) x->son[idx] = NodeCopy(x->son[idx]);
		x->son[idx] = NodeMerge(x->son[idx], y);
		return x;
	}
};


class WeightGraph
{
public:
	using lint = long long;
	using lpair = pair<lint, lint>;
	using kspPqVal = pair<lint, PersistentHeap::Node*>;

	struct Edge {
		int next, id;
		lint weight;
		Edge(int _next, lint _weight, int _id) :next(_next), weight(_weight), id(_id) {}
	};


private:
	const lint INF = 2e18;
	const lint BIG = 1e18;

	PersistentHeap mPersistentHeap;                  // 퍼시스턴트 힙

	int mSource, mSink, mGraphSize, mEdgeCount;      // 그래프의 시작점, 그래프의 끝점, 그래프의 크기 (=정점의 개수), 간선의 개수
	vector<vector<Edge>> mGraph;                     // 그래프
	vector<vector<Edge>> mRevGraph;                  // 역방향 그래프

	vector<int> mNext;                              // 정점 이동 기록 (RevGraph)
	vector<int> mEdgeID;                            // 사용된 Edge (RevGraph)
	vector<int> mVisit;                             // 정점 방문 순서 (RevGraph)
	vector<lint> mDist;                             // 최단경로 (RevGraph)


public:
	WeightGraph() { Assign(0); }
	WeightGraph(size_t graphSize) { Assign(graphSize); }



public:   // 그래프 구성
	void Assign(size_t graphSize) {
		mGraphSize = graphSize;
		mEdgeCount = 0;
		mGraph.assign(graphSize, vector<Edge>());
		mRevGraph.assign(graphSize, vector<Edge>());
	}

	void Add_Directed_Edge(int u, int v, lint weight) {
		mGraph[u].push_back(Edge(v, weight, mEdgeCount));
		mRevGraph[v].push_back(Edge(u, weight, mEdgeCount));
		mEdgeCount++;
	}


private:   // 역방향 그래프에 대해 최단경로를 구함
	void DijkstraForRevGraph()
	{
		mNext.assign(mGraphSize, -1);         // Rev Dijkstra에서 정점간 이동을 기록
		mEdgeID.assign(mGraphSize, -1);       // Rev Dijkstra에서 사용한 간선을 기록
		mVisit.clear();                       // Rev Dijkstra에서 방문했던 정점 순서
		mDist.assign(mGraphSize, INF);        // Sink에서 각 정점까지의 최단거리

		priority_queue<pair<lint, int>, vector<pair<lint, int>>, greater<pair<lint, int>>> pq;

		auto Enque = [&](int curr, int next, lint pathWeight, int edgeID) {
			if (mDist[next] > pathWeight) {
				mDist[next] = pathWeight;
				mNext[next] = curr;
				mEdgeID[next] = edgeID;
				pq.emplace(pathWeight, next);
			}
		};

		Enque(-1, mSink, 0, -1);
		while (!pq.empty()) {
			auto [pathWeight, curr] = pq.top(); pq.pop();
			if (mDist[curr] != pathWeight) continue;
			mVisit.push_back(curr);
			for (auto& edge : mRevGraph[curr]) Enque(curr, edge.next, edge.weight + pathWeight, edge.id);
		}
	}


private:   // Persistent-Heap 구성
	void ConstructPersistentHeap()
	{
		mPersistentHeap.Assign(mGraphSize);

		for (auto& vertex : mVisit) {
			if (mNext[vertex] != -1)
				mPersistentHeap[vertex] = PersistentHeap::NodeCopy(mPersistentHeap[mNext[vertex]]);

			for (auto& edge : mGraph[vertex]) {
				if (edge.id == mEdgeID[vertex]) continue;   // 최단경로에서 사용된 간선이라면 스킵

				lint sidetrack = edge.weight + mDist[edge.next] - mDist[vertex];   // sidetrack 가중치 = sidetrack(u, v, w) =  w + d[v] - d[u];

				if (sidetrack < BIG)   // 현재 정점의 heap과 다음 정점의 힙을 합친다
					mPersistentHeap[vertex] = PersistentHeap::NodeMerge(mPersistentHeap[vertex], new PersistentHeap::Node(lpair(sidetrack, edge.next)));
			}
		}
	}


public:   // Ksp를 진행하고 그 결과를 반환한다
	vector<lint> Additional_Ksp(int source, int sink, int kValue)
	{
		mSource = source;
		mSink = sink;

		DijkstraForRevGraph();
		ConstructPersistentHeap();

		if (mDist[mSource] > BIG) return vector<lint>(kValue, -1);  // 최단경로가 1개도 없는 경우

		priority_queue<kspPqVal, vector<kspPqVal>, greater<kspPqVal>> pq;
		vector<lint> kspResult;

		kspResult.push_back(mDist[mSource]);
		if (mPersistentHeap[mSource]) pq.emplace(mDist[mSource] + mPersistentHeap[mSource]->value.first, mPersistentHeap[mSource]);

		while (!pq.empty() && (int)kspResult.size() < kValue) {
			auto [pathWeight, node] = pq.top();
			pq.pop();

			kspResult.push_back(pathWeight);

			for (int i = 0; i < 2; i++) {
				if (node->son[i]) pq.emplace(pathWeight - node->value.first + node->son[i]->value.first, node->son[i]);
			}

			int next = node->value.second;
			if (mPersistentHeap[next]) pq.emplace(pathWeight + mPersistentHeap[next]->value.first, mPersistentHeap[next]);
		}
		while ((int)kspResult.size() < kValue) kspResult.push_back(-1);

		return kspResult;
	}
};


int main()
{
	ios_base::sync_with_stdio(false);
	cin.tie(NULL); cout.tie(NULL);

	WeightGraph graph;
	int N, M, S, T, K, u, v, w;

	cin >> N >> M >> S >> T >> K;
	graph.Assign(N);

	for (int i = 0; i < M; i++) {
		cin >> u >> v >> w;
		graph.Add_Directed_Edge(u, v, w);
	}

	for (auto& pathWeight : graph.Additional_Ksp(S, T, K))
		cout << pathWeight << '\n';

	return 0;
}