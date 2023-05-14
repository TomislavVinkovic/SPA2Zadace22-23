#include "GraphClass.hpp"
#include <queue>
const int INF = 1e9;

int dijkstra(Graph& g, int s, int f) {
    // init single source
    int n = g.numberOfNodes();
    vector<int> d(n, INF);
    vector<int> prev(n, INF);
    d[s] = 0;
    //dijkstra
    priority_queue<int, std::vector<int>, std::greater<int>> q;
    for(int i = 0; i < n; i++) {
        q.push(i);
    }

    // optimiziranje grafa dijkstra algoritmom
    while(!q.empty()) {
        int u = q.top();
        q.pop();
        for(int v = 0; v < n; v++) {
            if(g.adjMatrix()[u][v] > 0) {
                int alt = d[u] + g.adjMatrix()[u][v];
                if(alt < d[v]) {
                    d[v] = alt;
                    prev[v] = u;
                }
            }
        }
    }

    // vracanje tezine s-v
    return d[f];
}

int main() {
    int n, r, s, f;
    int i, j, w;
    cin >> n >> r >> s >> f;
    vector<vector<int>> adjMatrix(n, vector<int>(n, 0));
    for(int u = 0; u < r; u++) {
        cin >> i >> j >> w;
        adjMatrix[i-1][j-1] = adjMatrix[j-1][i-1] = w + 12;
    }
    Graph g(adjMatrix);
    cout << dijkstra(g, s-1, f-1) << endl;
}