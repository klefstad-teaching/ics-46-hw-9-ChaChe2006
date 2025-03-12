#include "dijkstras.h"
#include <iostream>

using namespace std;

vector<int> dijkstra_shortest_path(const Graph& G, int source, vector<int>& previous){
    vector<int> distances(G.numVertices, INF);
    vector<bool> visited(G.numVertices, false);
    distances[source] = 0;
    previous[source] = -1;

    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> minHeap;
    minHeap.push({0, source});

    while(!minHeap.empty()){
        int u = minHeap.top().first;
        int dist = minHeap.top().second;
        minHeap.pop();

        if(visited[u]){
            continue;
        }

        visited[u] = true;

        for(Edge edge: G[u]){
            int v = edge.dst;
            int w = edge.weight;
            if(!visited[v] && distances[u] + w < distances[v]){
                distances[v] = distances[u] + w;
                previous[v] = u;
                minHeap.push({v, distances[v]});
            }
        }
    }

    return distances;
}

vector<int> extract_shortest_path(const vector<int>& /*distances*/, const vector<int>& previous, int destination){
    vector<int> path;
    for(int v = destination; v != -1; v = previous[v]){
        path.push_back(v);
    }

    int left = 0;
    int right = path.size() - 1;
    while(left < right){
        swap(path[left], path[right]);
        left++;
        right--;
    }

    return path;
}

void print_path(const vector<int>& v, int total){
    for (int i = 0; i < v.size(); ++i) {
        cout << v[i] << " ";
    }
    cout << "\nTotal cost is " << total << endl;
}