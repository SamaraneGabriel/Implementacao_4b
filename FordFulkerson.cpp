#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include "Grafo.h"

using namespace std;

class FordFulkerson {
private:
    vector<vector<pair<int, int>>> adj; // Lista de adjacência com capacidade e nó destino

public:
    FordFulkerson(Grafo &grafo) {
        int n = grafo.getVertices().size();
        adj.resize(n);

        // Configurar as capacidades a partir das arestas do grafo
        for (auto &aresta : grafo.getArestas()) {
            int u = aresta.getU();
            int v = aresta.getV();
            int w = aresta.getW();
            adj[u].emplace_back(v, w);
            adj[v].emplace_back(u, 0); // Adicionar aresta reversa para fluxo residual com capacidade 0
        }
    }

    // BFS para encontrar um caminho aumentante
    bool bfs(int source, int sink, vector<int> &parent, vector<int> &capacity) {
        fill(parent.begin(), parent.end(), -1);
        fill(capacity.begin(), capacity.end(), 0);
        queue<pair<int, int>> q;
        q.push({source, INT_MAX});

        while (!q.empty()) {
            int cur = q.front().first;
            int flow = q.front().second;
            q.pop();

            for (auto &[next, cap] : adj[cur]) {
                if (parent[next] == -1 && cap > 0) { // Aresta não visitada e com capacidade disponível
                    parent[next] = cur;
                    int new_flow = min(flow, cap);
                    if (next == sink) {
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

    // Encontrar o fluxo máximo e o corte mínimo
    int maxFlowMinCut(int source, int sink, vector<int> &minCutSet) {
        int flow = 0;
        vector<int> parent(adj.size());
        vector<int> capacity(adj.size());

        while (bfs(source, sink, parent, capacity)) {
            int new_flow = capacity[sink];
            flow += new_flow;
            int cur = sink;
            while (cur != source) {
                int prev = parent[cur];

                // Atualizar capacidades residuais
                for (auto &[next, cap] : adj[prev]) {
                    if (next == cur) {
                        cap -= new_flow;
                        break;
                    }
                }
                for (auto &[next, cap] : adj[cur]) {
                    if (next == prev) {
                        cap += new_flow;
                        break;
                    }
                }
                cur = prev;
            }
        }

        // Identificar o corte mínimo
        vector<bool> visited(adj.size(), false);
        queue<int> q;
        q.push(source);
        visited[source] = true;

        while (!q.empty()) {
            int cur = q.front();
            q.pop();

            for (auto &[next, cap] : adj[cur]) {
                if (!visited[next] && cap > 0) {
                    visited[next] = true;
                    q.push(next);
                }
            }
        }

        // Conjunto do lado "source" no corte
        for (int i = 0; i < visited.size(); i++) {
            if (visited[i])
                minCutSet.push_back(i);
        }

        return flow;
    }
};
