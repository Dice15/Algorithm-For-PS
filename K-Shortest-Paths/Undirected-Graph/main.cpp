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
		lpair value;     // { sidetrack, edgeIdx }

		Node() {
			son[0] = son[1] = NULL;
			value = lpair(-1e18, -1e18);
		}
		Node(lpair _value) {
			son[0] = son[1] = NULL;
			value = _value;
		}
	};

	vector<Node*> heapTable;


public:
	PersistentHeap() :PersistentHeap(0) {}
	PersistentHeap(size_t heapSize) : heapTable(vector<Node*>(heapSize, NULL)) {}

	Node*& operator[](int index) { return heapTable[index]; }
	void Assign(size_t heapSize) { heapTable.assign(heapSize, NULL); }


public:
	static Node* NodeCopy(Node* node) {   // node�� �����ϰ� ��ȯ
		if (node == NULL) return NULL;
		Node* newNode = new Node();
		newNode->son[0] = node->son[0];
		newNode->son[1] = node->son[1];
		newNode->value = node->value;
		return newNode;
	}

	static Node* NodeMerge(Node* x, Node* y) {   // �־��� �� ���� node�� ��ġ�� �ֻ��� ��带(root)�� ��ȯ�Ѵ�
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

	static const lint INF = 2e18;
	static const lint BIG = 1e18;

	struct Edge {
		int from, to;
		lint weight;
		Edge(int _from, int _to, lint _weight) :from(_from), to(_to), weight(_weight) {}
	};

	PersistentHeap mPersistentHeap;                  // �۽ý���Ʈ ��

	int mSource, mSink, mGraphSize;                 // �׷����� ������, �׷����� ����, �׷����� ũ�� (=������ ����), ������ ����
	vector<vector<int>> mUndirectedGraph;           // ����׷���
	vector<vector<int>> mGraph;                     // �׷���
	vector<vector<int>> mRevGraph;                  // ������ �׷���
	vector<Edge> mEdges;                            // ����

	vector<int> mNext;                              // ���� �̵� ��� (RevGraph)
	vector<int> mEdgeID;                            // ���� Edge (RevGraph)
	vector<int> mVisit;                             // 1)���� �湮 ���� (RevGraph), 2)���� �湮 ���� Ȯ�� (Undirected Graph)
	vector<lint> mDist;                             // �ִܰ�� (RevGraph)


public:
	WeightGraph() { Assign(0); }
	WeightGraph(size_t graphSize) { Assign(graphSize); }

	void Assign(size_t graphSize) {
		mGraphSize = graphSize;
		mUndirectedGraph.assign(graphSize, vector<int>());
		mGraph.assign(graphSize, vector<int>());
		mRevGraph.assign(graphSize, vector<int>());
	}

	void Add_UndirectedEdge(int u, int v, lint weight) {
		mUndirectedGraph[u].push_back((int)mEdges.size());
		mUndirectedGraph[v].push_back((int)mEdges.size());
		mEdges.push_back(Edge(u, v, weight));
	}

	int Dfs(int prev, int curr) {
		if (mVisit[curr] != -1) return mVisit[curr];

		mVisit[curr] = 0;
		for (auto& edgeIdx : mUndirectedGraph[curr]) {
			auto& edge = mEdges[edgeIdx];

			if (edge.to == prev) continue;
			if (Dfs(edge.from, edge.to) == 1) {
				
				cout << edge.from  << " -> " << edge.to  << ", " << edge.weight << '\n';

				mGraph[edge.from].push_back(edgeIdx);
				mRevGraph[edge.to].push_back(edgeIdx);
				mVisit[curr] = 1;
			}
		}
		return mVisit[curr];
	}

	void ConstructDirectedGraph() {
		mVisit.assign(mGraphSize, -1);
		mVisit[mSink] = 1;
		Dfs(-1, mSource);
	}


public:   
	// ������ �׷����� ���� �ִܰ�θ� ����
	void DijkstraForRevGraph()   
	{
		mNext.assign(mGraphSize, -1);         // Rev Dijkstra���� ������ �̵��� ���
		mEdgeID.assign(mGraphSize, -1);       // Rev Dijkstra���� ����� ������ ���
		mVisit.clear();                       // Rev Dijkstra���� �湮�ߴ� ���� ����
		mDist.assign(mGraphSize, INF);        // Sink���� �� ���������� �ִܰŸ�

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

			for (auto& edgeIdx : mRevGraph[curr]) Enque(curr, mEdges[edgeIdx].from, mEdges[edgeIdx].weight + pathWeight, edgeIdx);
		}
	}


	// Persistent-Heap ����
	void ConstructPersistentHeap()   
	{
		mPersistentHeap.Assign(mGraphSize);

		for (auto& vertex : mVisit) {
			if (mNext[vertex] != -1)
				mPersistentHeap[vertex] = PersistentHeap::NodeCopy(mPersistentHeap[mNext[vertex]]);

			for (auto& edgeIdx : mGraph[vertex]) {
				if (edgeIdx == mEdgeID[vertex]) continue;   // �ִܰ�ο��� ���� �����̶�� ��ŵ

				lint sidetrack = mEdges[edgeIdx].weight + mDist[mEdges[edgeIdx].to] - mDist[vertex];   // sidetrack ����ġ = sidetrack(u, v, w) =  w + d[v] - d[u];

				if (sidetrack < BIG)   // ���� ������ heap�� ���� ������ ���� ��ģ��
					mPersistentHeap[vertex] = PersistentHeap::NodeMerge(mPersistentHeap[vertex], new PersistentHeap::Node(lpair(sidetrack, edgeIdx)));
			}
		}
	}


	// Ksp�� �����ϰ� �� ����� ��ȯ�Ѵ�
	vector<lint> Ksp(int source, int sink, int kValue)
	{
		mSource = source;
		mSink = sink;

		ConstructDirectedGraph();
		DijkstraForRevGraph();
		ConstructPersistentHeap();

		if (mDist[mSource] > BIG) return vector<lint>(kValue, -1);  // �ִܰ�ΰ� 1���� ���� ���

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

			int next = mEdges[node->value.second].to;
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
		graph.Add_UndirectedEdge(u, v, w);
	}

	for (auto& pathWeight : graph.Ksp(S, T, K))
		cout << pathWeight << '\n';

	return 0;
}