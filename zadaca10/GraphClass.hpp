#pragma once
#include <iostream>
#include <vector>

using namespace std;
class Graph
{
private:
    vector<vector<int>> AMatrix;

    int totalWeightOfGraph(vector<vector<int>> adjMatrix)
    {
        int totalweight = 0;
        int n = adjMatrix.size();

        for (int i = 0; i < n; ++i)
            for (int j = i + 1; j < n; ++j)
                    totalweight += adjMatrix[i][j];

        return totalweight;   
    }

    void printGraph(vector<vector<int>> adjMatrix)
    {
        int n = adjMatrix.size();
        
        for (int i = 0; i < n; ++i)
            for (int j = i + 1; j < n; ++j)
                if(adjMatrix[i][j] != 0)
                    cout << i << " - " << j << " (" << adjMatrix[i][j] << ")" << endl; 
    }
    
public:

    Graph(vector<vector<int>> &adjMatrix)
    {
		AMatrix = adjMatrix;
    }

    int GraphWeight()
    {
        return totalWeightOfGraph(AMatrix);
    }

    void print()
    {
        printGraph(AMatrix);
    }

    int numberOfNodes() {
        return AMatrix.size();
    }

    vector<vector<int>>& adjMatrix() {
        return AMatrix;
    }
};