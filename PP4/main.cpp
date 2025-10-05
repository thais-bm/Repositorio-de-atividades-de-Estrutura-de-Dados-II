#include <iostream>
#include <list>
#include <vector>
#include <cstdlib>
#include <limits>
#include <iomanip>
#include <string>
#include <cmath>

// ----------------------------------------- CLASSE GRAFO PONDERADO -------------------------------
// tem que ajustar o recebimento da quantidade de vertices e grau do grafo
using Vertex = unsigned int;
using uint = unsigned int;
using Weight = float;
using VertexWeightPair = std::pair<Vertex, Weight>;

class GrafoPonderado
{
private:
    uint num_vertices;
    uint num_edges;
    std::vector<std::vector<VertexWeightPair>> adj;

    void insertion_sort(std::vector<VertexWeightPair> &vec);
    bool ja_existe_edge(Vertex u, Vertex v);

public:
    GrafoPonderado(Vertex ordem, uint grau);
    ~GrafoPonderado();
    void add_edge(Vertex u, Vertex v, Weight w);
    std::vector<VertexWeightPair> get_adj(Vertex u) const;
    uint get_edges() const { return num_edges; }
    uint get_vertices() const { return num_vertices; }
    Weight get_peso(Vertex u, Vertex v) const { return adj[u][v].second; }
};

GrafoPonderado::GrafoPonderado(Vertex ordem, uint grau)
{
    num_vertices = ordem;
    num_edges = grau;
    adj.resize(num_vertices);
}

GrafoPonderado::~GrafoPonderado()
{
    // automatico
}

// extra: ve se ja existe
bool GrafoPonderado::ja_existe_edge(Vertex u, Vertex v)
{
    for (const auto &pair : adj[u])
    {
        if (pair.first == v)
        {
            return true;
        }
    }
    return false;
}

void GrafoPonderado::add_edge(Vertex u, Vertex v, Weight w)
{
    // teste de validade
    // nao pode: menor que zero, maior que num_vertices e iguais
    if (u >= num_vertices || v >= num_vertices || u == v)
    {
        throw std::invalid_argument("Valores invalidos");
    }

    // verificar duplicidade (tem a verificacao na u<v na criacao do grafo, mas deixarei aqui tbm)
    if (ja_existe_edge(u, v))
    {
        throw std::invalid_argument("Aresta duplicada");
    }

    // par v,w no final de adj[u]
    VertexWeightPair par = std::make_pair(v, w);
    adj[u].push_back(par);

    // par u,w no final de adj[v]
    par = std::make_pair(u, w);
    adj[v].push_back(par);

    num_edges += 1;
}

std::vector<VertexWeightPair> GrafoPonderado::get_adj(Vertex u) const
{
    if (u >= num_vertices)
    {
        throw std::invalid_argument("Valores invalidos");
    }
    return adj[u];
}

void GrafoPonderado::insertion_sort(std::vector<VertexWeightPair> &vec)
{
    if (vec.size() < 2)
    {
        return;
    }

    for (Vertex i = 1; i < vec.size(); i++)
    {
        // ta ordenando com base no par indice e peso
        // vou deixar de acordo com o indice
        Vertex key = vec[i].first;
        int j = i - 1;

        while (j >= 0 && vec[j].first > key)
        {
            vec[j + 1] = vec[j];
            j = j - 1;
        }
        vec[j + 1] = vec[i];
    }
}

// --------------------------------------- HEAP ---------------------------------------------------
template <typename T>
class MinHeap
{
private:
    std::vector<T> heap;
    int getParent(int index) { return (index - 1) / 2; }
    int getLeft(int index) { return 2 * index + 1; }
    int getRight(int index) { return 2 * index + 2; }

public:
    void heapify(int index);

    MinHeap() = default;
    void insert(const T &value);
    T extractMin();
    bool isEmpty() const { return heap.empty(); }
};

template <typename T>
void MinHeap<T>::heapify(int index)
{
    int smallest = index;
    int left = getLeft(index);
    int right = getRight(index);
    int heapsize = heap.size();

    if (left < heapsize && heap[left] < heap[smallest])
    {
        smallest = left;
    }

    if (right < heapsize && heap[right] < heap[smallest])
    {
        smallest = right;
    }

    if (smallest != index)
    {
        std::swap(heap[index], heap[smallest]);
        heapify(smallest);
    }
}

template <typename T>
void MinHeap<T>::insert(const T &value)
{
    heap.push_back(value);
    int index = heap.size() - 1;

    while (index != 0 && heap[index] < heap[getParent(index)])
    {
        std::swap(heap[index], heap[getParent(index)]); // vou usar swap pq e mais facil
        index = getParent(index);
    }
}

template <typename T>
T MinHeap<T>::extractMin()
{
    if (heap.empty())
    {
        throw std::runtime_error("Heap ta vaziorrr");
    }

    T minValue = heap[0];

    heap[0] = heap.back();
    heap.pop_back();

    if (!heap.empty())
    {
        heapify(0);
    }

    return minValue;
}

// ----------------------------------------- ALGORITMO DE DIJKSTRA --------------------------------
struct DijkstraResult
{
    std::vector<Weight> distancias;
    std::vector<int> predecessores;
    std::vector<int> movimentos;
};

DijkstraResult dijkstra(const GrafoPonderado &grafo, Vertex origem)
{
    uint num_vertices = grafo.get_vertices();
    DijkstraResult resultado;
    resultado.distancias.assign(num_vertices, std::numeric_limits<Weight>::infinity());
    resultado.predecessores.assign(num_vertices, -1);
    resultado.movimentos.assign(num_vertices, -99999);

    resultado.distancias[origem] = 0;
    resultado.movimentos[origem] = 0;

    MinHeap<std::pair<Weight, Vertex>> fila_prioridade;
    fila_prioridade.insert({0.0f, origem});

    std::vector<bool> visitado(num_vertices, false);

    while (!fila_prioridade.isEmpty())
    {
        Vertex u = fila_prioridade.extractMin().second;

        if (visitado[u])
            continue;

        visitado[u] = true;

        for (const auto &vizinho : grafo.get_adj(u))
        {
            Vertex v = vizinho.first;
            Weight peso_aresta = vizinho.second;

            Weight nova_distancia = resultado.distancias[u] + peso_aresta; // salvando a nova distancia
            int novos_movimentos = resultado.movimentos[u] + 1;            // salvando os novos movimentos

            // verficando se ja foi visitado
            if (!visitado[v])
            {
                // caminho com peso menor
                if (nova_distancia < resultado.distancias[v])
                {
                    resultado.distancias[v] = nova_distancia;
                    resultado.predecessores[v] = u;
                    resultado.movimentos[v] = novos_movimentos;
                    fila_prioridade.insert({resultado.distancias[v], v});
                }

                // caminho com O MESMO PESO.
                else if (nova_distancia == resultado.distancias[v])
                {
                    // compara os movimentos
                    if (novos_movimentos < resultado.movimentos[v])
                    {
                        resultado.predecessores[v] = u;
                        resultado.movimentos[v] = novos_movimentos;
                        fila_prioridade.insert({resultado.distancias[v], v});
                    }
                }
            }
        }
    }
    return resultado;
}

// ------------------------------------- UNION FIND GENERICO ----------------------------
template <typename T>
class UnionFind
{
private:
    std::vector<T> parent;

public:
    UnionFind(T size);
    T findSet(T item);
    void unionSets(T set1, T set2);
};

template <typename T>
UnionFind<T>::UnionFind(T size)
{
    parent.resize(size);
    for (T i = 0; i < size; ++i)
    {
        parent[i] = i;
    }
}

template <typename T>
T UnionFind<T>::findSet(T item)
{
    if (item >= parent.size())
    {
        throw std::out_of_range("Item fora do alcance");
    }
    if (parent[item] != item)
    {
        parent[item] = findSet(parent[item]);
    }
    return parent[item];
}

template <typename T>
void UnionFind<T>::unionSets(T set1, T set2)
{
    T root1 = findSet(set1);
    T root2 = findSet(set2);

    if (root1 != root2)
    {
        parent[root2] = root1;
    }
}
// ------------------------------------- ALGORITMO DE KRUSTAL ----------------------------
struct Edge
{
    Vertex u, v;
    Weight w;
    bool operator<(const Edge &other) const
    {
        return w < other.w;
    }
};

GrafoPonderado kruskal(const GrafoPonderado &grafo)
{
    GrafoPonderado arvore(grafo.get_vertices(), 0);
    UnionFind<Vertex> uf(grafo.get_vertices());

    // Montar lista de arestas a partir da lista de adj
    std::vector<Edge> edges;
    for (Vertex u = 0; u < grafo.get_vertices(); ++u)
    {
        for (auto &par : grafo.get_adj(u))
        {
            Vertex v = par.first;
            Weight w = par.second;

            if (u < v)
            {
                edges.push_back({u, v, w});
            }
        }
    }

    // Ordenar por peso (insertion sort)
    for (size_t i = 1; i < edges.size(); i++)
    {
        Edge key = edges[i];
        int j = i - 1;
        while (j >= 0 && edges[j].w > key.w)
        {
            edges[j + 1] = edges[j];
            j--;
        }
        edges[j + 1] = key;
    }

    // Construir MST
    for (auto &e : edges)
    {
        if (uf.findSet(e.u) != uf.findSet(e.v))
        {
            arvore.add_edge(e.u, e.v, e.w);
            uf.unionSets(e.u, e.v);
        }
    }

    return arvore;
}

// ------------------------------------- Simulacao ------------------------------------------------
// Cada bloco de neuronio a ser acessado representa um grafo ponderado e um estado de saúde
struct bloco_neuronio {
    bool estaDoente;
    GrafoPonderado grafo_interno;
};

/*

    A ideia e criar um grafo ponderado para representar a conexao entre os blocos de neuronios
    Cada bloco de neuronio e um vertice nesse grafo maior
    A conexao entre os blocos de neuronios e representada por uma aresta entre os vertices
    Ai podemos usar o algoritmo de Dijkstra para encontrar o caminho mais curto entre os blocos de neuronios
    Cada bloco de neuronio tambem tem um grafo ponderado interno
    Ai da pra usar o algoritmo de Kruskal para encontrar a arvore geradora minima

*/



int main()
{
    // Criar grafo com 5 vértices
    GrafoPonderado g(5, 0);

    // Adicionar arestas
    g.add_edge(0, 1, 10);
    g.add_edge(0, 2, 6);
    g.add_edge(0, 3, 5);
    g.add_edge(1, 3, 15);
    g.add_edge(2, 3, 4);

    std::cout << "=== Teste Dijkstra (origem = 0) ===\n";
    auto resultado = dijkstra(g, 0);
    for (size_t i = 0; i < resultado.distancias.size(); i++)
    {
        std::cout << "Distancia ate " << i << " = " << resultado.distancias[i]
                  << " | Predecessor = " << resultado.predecessores[i]
                  << " | Movimentos = " << resultado.movimentos[i] << "\n";
    }

    std::cout << "\n=== Teste Kruskal ===\n";
    GrafoPonderado mst = kruskal(g);

    // Imprimir arestas da MST
    std::cout << "Arestas na MST:\n";
    for (Vertex u = 0; u < mst.get_vertices(); ++u)
    {
        for (auto &par : mst.get_adj(u))
        {
            Vertex v = par.first;
            Weight w = par.second;

            if (u < v) // evita duplicidade
                std::cout << u << " - " << v << " (peso " << w << ")\n";
        }
    }

    return 0;
}
