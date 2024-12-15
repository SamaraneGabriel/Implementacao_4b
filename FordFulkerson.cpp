#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include "Grafo.h"
#define INT_MAX 2147483647

using namespace std;

class FordFulkerson
{
private:
    vector<vector<pair<int, int>>> adj; // Lista de adjacência com capacidade e nó destino

public:
    FordFulkerson(Grafo &grafo)
    {
        int n = grafo.getVertices().size();
        adj.resize(n);
        int i = 0;
        // Configurar as capacidades a partir das arestas do grafo
        for (auto &aresta : grafo.getArestas())
        {
            // cout << "for it: " << i << endl;
            int u = aresta.getU();
            int v = aresta.getV();
            int w = aresta.getW();
            adj[u].emplace_back(v, w);
            adj[v].emplace_back(u, 0); // Adicionar aresta reversa para fluxo residual com capacidade 0
            i++;
        }
    }

    // BFS para encontrar um caminho aumentante
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
                { // Aresta não visitada e com capacidade disponível
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

    // bool bfs_max(int source, int sink, vector<int> &parent, vector<int> &capacity)
    // {
    //     fill(parent.begin(), parent.end(), -1);
    //     fill(capacity.begin(), capacity.end(), 0);

    //     // Fila de prioridade (capacidade como prioridade)
    //     priority_queue<pair<int, int>> pq;
    //     pq.push({INT_MAX, source}); // Capacidade infinita no início

    //     while (!pq.empty())
    //     {
    //         int flow = pq.top().first; // Gargalo atual
    //         int cur = pq.top().second; // Nó atual
    //         pq.pop();

    //         for (auto &[next, cap] : adj[cur])
    //         {
    //             // Visitamos o nó se ele ainda não foi visitado e a capacidade é positiva
    //             if (parent[next] == -1 && cap > 0)
    //             {
    //                 parent[next] = cur;            // Atualiza o caminho
    //                 int new_flow = min(flow, cap); // Atualiza o gargalo
    //                 capacity[next] = new_flow;     // Salva o gargalo no vetor capacity

    //                 if (next == sink)
    //                     return true; // Encontramos o caminho até o sumidouro

    //                 pq.push({new_flow, next}); // Adiciona o próximo nó à fila
    //             }
    //         }
    //     }

    //     return false; // Nenhum caminho encontrado
    // }

    // Encontrar o fluxo máximo e o corte mínimo
    int maxFlowMinCut(int source, int sink, unordered_map<int, int> &minCutSet)
    {
        int flow = 0;
        vector<int> parent(adj.size());
        vector<int> capacity(adj.size());
        while (bfs(source, sink, parent, capacity))
        {
            cout << "entrou maxflow" << endl;
            int new_flow = capacity[sink];
            flow += new_flow;
            int cur = sink;
            while (cur != source)
            {
                int prev = parent[cur];

                // Atualizar capacidades residuais
                for (auto &[next, cap] : adj[prev])
                {
                    if (next == cur)
                    {
                        cap -= new_flow;
                        break;
                    }
                }
                for (auto &[next, cap] : adj[cur])
                {
                    if (next == prev)
                    {
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
            if (visited[i])
                minCutSet[i] = 1;
        }
        cout << "saiu maxflow" << endl;
        return flow;
    }

    int boykovKolmogorovMinCut(int source, int sink, unordered_map<int, int> &minCutSet)
    {
        vector<int> parent(adj.size(), -1);
        vector<bool> inTree(adj.size(), false);
        queue<int> active; // Nós ativos para processar

        // Inicializar árvores
        active.push(source);
        inTree[source] = true;

        while (!active.empty())
        {
            int cur = active.front();
            active.pop();

            // Percorre as arestas do nó atual
            for (auto &[next, cap] : adj[cur])
            {
                if (!inTree[next] && cap > 0)
                {
                    parent[next] = cur;
                    inTree[next] = true;

                    if (next == sink)
                    {
                        // Encontramos um caminho do source ao sink
                        int bottleneck = INT_MAX;
                        int node = sink;

                        // Retrocede para calcular o gargalo
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

                        // Aumentar o fluxo e atualizar capacidades residuais
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

                        // Reiniciar árvores
                        fill(parent.begin(), parent.end(), -1);
                        fill(inTree.begin(), inTree.end(), false);
                        active = queue<int>(); // Limpa fila
                        active.push(source);
                        inTree[source] = true;
                        break;
                    }

                    // Adicionar nó à fila de ativos
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

        // Constrói o conjunto do corte mínimo
        for (int i = 0; i < visited.size(); i++)
        {
            if (!visited[i])
                minCutSet[i] = 0;
            else
                minCutSet[i] = 1;
        }

        // Retornar o valor do fluxo máximo encontrado
        int maxFlow = 0;
        for (auto &[next, cap] : adj[source])
        {
            maxFlow += cap;
        }
        return maxFlow;
    }
};
