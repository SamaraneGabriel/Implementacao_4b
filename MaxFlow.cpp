#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include "Grafo.h"
#define INT_MAX 2147483647

using namespace std;

class MaxFlow
{
private:
    vector<vector<pair<int, int>>> adj;

public:
    MaxFlow(Grafo &grafo)
    {
        int n = grafo.getVertices().size();
        adj.resize(n);
        int i = 0;
        for (auto &aresta : grafo.getArestas())
        {

            int u = aresta.getU();
            int v = aresta.getV();
            int w = aresta.getW();
            adj[u].emplace_back(v, w);
            adj[v].emplace_back(u, 0);
            i++;
        }
    }

    bool bfs(int source, int sink, vector<int> &parent, vector<int> &capacity)
    {
        fill(parent.begin(), parent.end(), -1);
        fill(capacity.begin(), capacity.end(), 0);
        queue<pair<int, int>> q;
        q.push({source, INT_MAX});

        while (!q.empty())
        {
            int cur = q.front().first;
            int flow = q.front().second;
            q.pop();

            for (auto &[next, cap] : adj[cur])
            {
                if (parent[next] == -1 && cap > 0)
                {
                    parent[next] = cur;
                    int new_flow = min(flow, cap);
                    if (next == sink)
                    {
                        capacity[next] = new_flow;
                        return true;
                    }
                    capacity[next] = new_flow;
                    q.push({next, new_flow});
                }
            }
        }
        return false;
    }

    int boykovKolmogorovMinCut(int source, int sink, unordered_map<int, int> &minCutSet)
    {
        vector<int> parent(adj.size(), -1);
        vector<bool> inTree(adj.size(), false);
        queue<int> active;

        active.push(source);
        inTree[source] = true;

        while (!active.empty())
        {
            int cur = active.front();
            active.pop();

            for (auto &[next, cap] : adj[cur])
            {
                if (!inTree[next] && cap > 0)
                {
                    parent[next] = cur;
                    inTree[next] = true;

                    if (next == sink)
                    {
                        int bottleneck = INT_MAX;
                        int node = sink;

                        while (node != source)
                        {
                            int prev = parent[node];
                            for (auto &[n, c] : adj[prev])
                            {
                                if (n == node)
                                {
                                    bottleneck = min(bottleneck, c);
                                    break;
                                }
                            }
                            node = prev;
                        }

                        node = sink;
                        while (node != source)
                        {
                            int prev = parent[node];

                            for (auto &[n, c] : adj[prev])
                            {
                                if (n == node)
                                {
                                    c -= bottleneck;
                                    break;
                                }
                            }
                            for (auto &[n, c] : adj[node])
                            {
                                if (n == prev)
                                {
                                    c += bottleneck;
                                    break;
                                }
                            }
                            node = prev;
                        }

                        fill(parent.begin(), parent.end(), -1);
                        fill(inTree.begin(), inTree.end(), false);
                        active = queue<int>(); // Limpa fila
                        active.push(source);
                        inTree[source] = true;
                        break;
                    }

                    active.push(next);
                }
            }
        }

        // Identificar o corte mínimo
        vector<bool> visited(adj.size(), false);
        queue<int> q;
        q.push(source);
        visited[source] = true;

        while (!q.empty())
        {
            int cur = q.front();
            q.pop();

            for (auto &[next, cap] : adj[cur])
            {
                if (!visited[next] && cap > 0)
                {
                    visited[next] = true;
                    q.push(next);
                }
            }
        }

        for (int i = 0; i < visited.size(); i++)
        {
            if (!visited[i])
                minCutSet[i] = 0;
            else
                minCutSet[i] = 1;
        }

        int maxFlow = 0;
        for (auto &[next, cap] : adj[source])
        {
            maxFlow += cap;
        }
        return maxFlow;
    }
};
