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

class GrafoPonderado {
private:
    uint num_vertices;
    uint num_edges;
    int size = 8; // tamanho padrao 8x8
    std::vector<std::vector<VertexWeightPair>> adj;
    

    void criar_grafo(); // ele usa o size privado
    void insertion_sort(std::vector<VertexWeightPair> &vec);

    bool ja_existe_edge(Vertex u, Vertex v);

public:

    GrafoPonderado(uint size = 8);
    ~GrafoPonderado();


    void add_edge(Vertex u, Vertex v, Weight w);
    std::vector<VertexWeightPair> get_adj(Vertex u) const;
    uint get_edges() const { return num_edges; }
    uint get_vertices() const { return num_vertices; }
    Weight get_peso(Vertex u, Vertex v) const { return adj[u][v].second; }
    int get_tamanho() const { return size; }
    const std::vector<std::vector<VertexWeightPair>>& get_adj() const { return adj; }
};

GrafoPonderado::GrafoPonderado(uint size) : size(size)
{
    num_vertices = size * size; // Exemplo: 8 x 8 = 64 vertices
    num_edges = 0;
    adj.resize(num_vertices);
}

GrafoPonderado::~GrafoPonderado()
{
    //automatico
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

std::vector<VertexWeightPair> GrafoPonderado::get_adj(Vertex u) const {
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
    resultado.movimentos.assign(num_vertices, std::numeric_limits<int>::max());

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
            int novos_movimentos = resultado.movimentos[u] + 1; // salvando os novos movimentos

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