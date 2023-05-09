#include <iostream>
#include <fstream>
#include <vector>
#include <stack>
#include <queue>
#include <algorithm>

using namespace std;

const int INF = 1e9;

class Graph
{
private:
    vector< vector<int> > AMatrix;

    int totalWeightOfGraph(vector< vector<int> > adjMatrix)
    {
        // gledam samo elemente u gornje trokutastoj matrici jer je matrica simetrična
        int sum = 0;
        int n = adjMatrix[0].size();
        for(int i = 0; i < n-1; i++) {
            for(int j = i+1; j < n; j++) {
                sum += adjMatrix[i][j];
            }
        }
        return sum;
    }

    void printGraph(vector< vector<int> > adjMatrix) {
        if(adjMatrix.size() == 0) {
            cout << "Empty graph..." << endl;
        }
        int n = AMatrix[0].size();
        // Stupci
        cout << "    ";
        for(int j=0; j<n; j++) {
            cout << j << " ";
        }
        cout << endl;

        // Retci
        for(int i=0; i<n; i++) {
            cout << i << " | ";
            for(int j=0; j<n; j++) {
                cout << AMatrix[i][j] << " ";
            }
            cout << endl;
        }
    }
    
    /* mjesto za pomoćne funkcije za algoritme */
    vector< pair<int, pair<int, int> > > matrix_to_edges() {
        vector< pair<int, pair<int, int> > > edges;
        if(AMatrix.size() == 0) return edges;
        int n = AMatrix[0].size();
        for(int i = 0; i < n; i++) {
            for(int j = 0; j < n; j++) {
                if(AMatrix[i][j] > 0) {
                    edges.push_back(make_pair(AMatrix[i][j], make_pair(i, j)));
                }
            }
        }
        return edges;
    }

    int find(int u, vector<int>& parent) {
        if(parent[u] == u) return u;
        return parent[u] = find(parent[u], parent);
    }

    void merge(int u, int v, vector<int>& parent, vector<int>& rank) {
        u = find(u, parent);
        v = find(v, parent);
        if(rank[u] > rank[v]) swap(u, v);
        parent[u] = v;
        if(rank[u] == rank[v]) rank[v]++;
    }

public:

    //konstruktor, paziti na kopiranje
    Graph(vector< vector<int> > adjMatrix)
    {
        // deep copy
        AMatrix = adjMatrix;
    }

    int GraphWeight()
    {
        return totalWeightOfGraph(this->AMatrix);
    }

    void print()
    {
        printGraph(AMatrix);
    }

    void primMST(Graph G) {
        if(G.AMatrix.size() == 0) return;
        int n = G.AMatrix[0].size();

        const auto& adj = G.AMatrix;

        // pohranjuje minimalno tezak brid do svakog
        // cvora u stablu
        vector<int> key(n, INF);

        // je li cvor vec posjecen?
        vector<bool> visited(n, false);

        // roditelj svakog cvora u stablu
        vector<int> parent(n, -1);

        priority_queue<pair<int, int>, vector<pair<int, int> >, greater<pair<int, int> > > Q;

        // put do startnog cvora kosta 0
        key[0] = 0;

        // prvi element je kljuc, drugi element je broj cvora
        Q.push(make_pair(0,0));

        while(!Q.empty()) {
            int u = Q.top().second;
            Q.pop();

            if(visited[u]) continue;

            visited[u] = true;
            
            // updateaj kljuceve i dodaj nove cvorove
            // u prioritetni red
            for(int v = 0; v < n; v++) {
                // ako su vrhovi spojeni, v nije posjecen i
                // brid izmedu njih je manji od trenutne
                // tezine za v
                if(adj[u][v] != 0 && !visited[v] && adj[u][v] < key[v]) {
                    key[v] = adj[u][v];
                    Q.push(make_pair(key[v], v));
                    parent[v] = u;
                }
            }
        }

        // ISPIS MST-a
        // krece od 1 jer nulti cvor nema roditelja
        for(int i = 1; i < n; i++) {
            cout << parent[i] << " -- " << i << " == " << key[i] << endl;
        }
    }

    void kruskalMST(Graph G) {
        if(G.AMatrix.size() == 0) return;
        int n = AMatrix[0].size();

        vector<int> parent;
        vector<int> rank;

        auto edges = matrix_to_edges();
        sort(edges.begin(), edges.end());


        for(int i = 0; i < n; i++) {
            parent.push_back(i);
            rank.push_back(0);
        }
        for(auto& edge: edges) {
            int u = edge.second.first;
            int v = edge.second.second;
            int w = edge.first;

            if(find(u, parent) != find(v, parent)) {
                merge(u, v, parent, rank);
                cout << u << " -- " << v << " == " << w << endl;
            }
        }
    }

};

int main() {
    int n, m;
    ifstream infile("inputgraph.txt");
    infile >> n >> m;
    vector< vector<int> > adj(n, vector<int>(n, 0));
    int u, v, w;
    for(int i=0; i<m; i++) {
        infile >> u >> v >> w; 
        adj[u][v] = w;
        adj[v][u] = w;
    }
    infile.close();

    Graph G(adj);
    G.print();
    cout << "Tezina grafa: " << G.GraphWeight() << endl;
    G.primMST(G);
    cout << endl;
    G.kruskalMST(G);
    return 0;
}