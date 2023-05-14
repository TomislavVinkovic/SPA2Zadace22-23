#include "GraphClass.hpp"
#include <algorithm>

void DFS(
    Graph& g,
    int u,
    vector<int>& disc,
    vector<int>& low,
    vector<int>& parent,
    vector<pair<int, int>>& bridges
) {
    // ova varijabla je static jer cu pozivati funkciju vise puta
    // alternativa bi bila poslijedivati time ali iovako imam puno parametara
    static int TIME = 0;
    int n = g.numberOfNodes();

    disc[u] = low[u] = TIME;
    TIME++;

    for(int v = 0; v < n; v++) {
        // ako su ova 2 cvora povezana
        if(g.adjMatrix()[u][v]) {
            // v nije posjecen
            if(disc[v] == -1) {
                parent[v] = u;
                DFS(g, v, disc, low, parent, bridges);

                low[u] = min(low[u], low[v]);
                if(low[v] > disc[u]) {
                    bridges.push_back(make_pair(u,v));
                }
            }
            else if(v != parent[u]) {
                low[u] = min(low[u], disc[v]);
            }
        }
    }
}

void findBridges(
    Graph& g,
    vector<pair<int, int>>& bridges,
    vector<bool>& visitedComponents
) {
    int n = g.numberOfNodes();
    vector<int> low(n, -1);
    vector<int> disc(n, -1);
    vector<int> parent(n, -1);

    for(int i = 0; i < n; i++) {
        if(disc[i] == -1) {
            DFS(g, i, disc, low, parent, bridges);
        }
    }
}

void criticalEdges(Graph& g) {
    int n = g.numberOfNodes();
    // ovaj vektor deklariram tu jer zelim pokrenuti algoritam iz potencijalno svih nodeova (graf nije povezan)
    vector<bool> visitedComponents(n, 0);

    // korak 2: za svaki component, runaj algoritam da nades mostove
    vector<pair<int, int>> bridges;
    findBridges(g, bridges, visitedComponents);
    sort(bridges.begin(), bridges.end(), [](auto& a, auto& b) {
        return a.first < b.first;
    });
    for(const auto& p : bridges) {
        cout << p.first << " - " << p.second << endl;
    }
}

int main() {
    int n, m;
    int i, j, w;
    cin >> n >> m;
    vector<vector<int>> adjMatrix(n, vector<int>(n, 0));
    for(int u = 0; u < m; u++) {
        cin >> i >> j >> w;
        adjMatrix[i][j] = adjMatrix[j][i] = w;
    }
    Graph g(adjMatrix);
    criticalEdges(g);
}