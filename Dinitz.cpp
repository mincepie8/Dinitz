#include<iostream>
#include<vector>
#include<stack>
#include<fstream>
using namespace std;

const int INF = 1e6;

struct Edge {
	int flow;
	int cap;
};
class Graph {
public:
	vector<vector<Edge>> graph;
	vector<int> visited;
	vector<int> edges;
	vector<int> d;
	vector<int> p;
	stack<int> q;
	int s, t;
	Graph(int v_num) {
		graph.resize(v_num, vector<Edge>(v_num));
		visited.resize(v_num);
		edges.resize(v_num * v_num);
		d.resize(v_num);
		p.resize(v_num);
		fill(visited.begin(), visited.end(), 0);
	}
	void fill_graph() {
		ifstream in;
		in.open("test.txt");
		int a, b, cap;
		while (in >> a && in >> b && in >> cap) {
			a--;
			b--;
			graph[a][b] = { 0, cap };
			graph[b][a] = { 0, 0 };
		}
	}
	bool bfs() {
		fill(d.begin(), d.end(), INF);
		d[s] = 0;
		q.push(s);
		while (!q.empty()) {
			int u = q.top();
			q.pop();
			for (int v = 0; v < graph.size(); v++) {
				if (graph[u][v].flow < graph[u][v].cap && d[v] == INF) {
					d[v] = d[u] + 1;
					q.push(v);
				}
			}
		}
		return d[t] != INF;
	}
	int dfs(int u, int cmin) {
		if (u == t || cmin == 0) {
			return cmin;
		}
		for (int v = p[u]; v < graph.size(); v++) {
			if (d[v] == d[u] + 1) {
				int delta = dfs(v, min(cmin, graph[u][v].cap - graph[u][v].flow));
				if (delta) {
					graph[u][v].flow += delta;
					graph[v][u].flow -= delta;
					return delta;
				}
			}
			p[u]++;
		}
		return 0;
	}
	int find_max_flow() {
		int max_flow = 0;
		while (bfs()) {
			fill(p.begin(), p.end(), 0);
			int flow = dfs(s, INF);
			while (flow) {
				max_flow += flow;
				flow = dfs(s, INF);
			}
		}
		return max_flow;
	}
};
int main() {
	int v_num;
	cout << "Enter the num of vertex: ";
	cin >> v_num;
	Graph A(v_num);
	A.fill_graph();
	cout << "Enter the source: ";
	cin >> A.s;
	A.s--;
	cout << "Enter the sink: ";
	cin >> A.t;
	A.t--;
	cout << "Max flow is: " << A.find_max_flow();
}