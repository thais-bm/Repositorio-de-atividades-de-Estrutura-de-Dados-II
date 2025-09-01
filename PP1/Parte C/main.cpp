#include <iostream>
#include <utility>
#include <vector>
#include <list>

// ESPEC-1: criando apelidos
using Vertex = unsigned int;
using uint = unsigned int;
using Weight = float;
using VertexWeightPair = std::pair<Vertex, Weight>;

class WeightedGraphAL
{
private:
    // ESPEC-2: Atributos da classe WGAL
    uint num_vertices;
    uint num_edges;
    std::list<VertexWeightPair> *adj;

    bool jaExisteEdge(Vertex u, Vertex v);

public:
    WeightedGraphAL(uint num_vert);
    ~WeightedGraphAL();
    void addEdge(Vertex u, Vertex v, Weight w);
    std::list<VertexWeightPair> getAdj(const Vertex u);
    uint getNumVertices() const { return num_vertices; }
    uint getEdges() const { return num_edges; }
};

// ESPEC-3: Construtor
WeightedGraphAL::WeightedGraphAL(uint num_vert)
{
    num_vertices = num_vert;
    num_edges = 0;
    adj = new std::list<VertexWeightPair>[num_vertices];
}

// ESPEC-4: Destrutor
WeightedGraphAL::~WeightedGraphAL()
{
    delete[] adj;
    adj = nullptr;
}

// extra: ve se ja existe
bool WeightedGraphAL::jaExisteEdge(Vertex u, Vertex v)
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

// ESPEC-5: Add-edge
void WeightedGraphAL::addEdge(Vertex u, Vertex v, Weight w)
{
    // teste de validade
    // nao pode: menor que zero, maior que num_vertices e iguais
    if (int(u) < 0 || int(v) < 0 || u >= num_vertices || v >= num_vertices || u == v)
    {
        throw std::invalid_argument("Valores invalidos");
    }
    // verificar duplicidade
    if (jaExisteEdge(u,v))
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

// ESPEC-6: Pegar o ADJ
std::list<VertexWeightPair> WeightedGraphAL::getAdj(const Vertex u)
{
    if (u >= num_vertices)
    {
        throw std::invalid_argument("Valores invalidos");
    }

    return adj[u];
}

// ESPEC-7: o print lista de adjacencias
void printAdjacencyList(WeightedGraphAL &graph)
{
    std::cout << "num_vertices: " << graph.getNumVertices() << std::endl;
    std::cout << "num_edges: " << graph.getEdges() << std::endl;

    for (uint u = 0; u < graph.getNumVertices(); ++u)
    {
        std::cout << u << ": ";
        std::list<VertexWeightPair> list = graph.getAdj(u);
        for (const auto &v : list)
        {
            std::cout << "(" << v.first << ", " << v.second << "), ";
        }
        std::cout << std::endl;
    }
}

int main(int argc, char const *argv[])
{
    uint n, m = 0;

    std::cin >> n; // ordem do grafo
    std::cin >> m; // numero de arestas

    WeightedGraphAL graph = WeightedGraphAL(n);

    uint u, v = 0;
    Weight w = 0; // representam aresta vertices q foram uma aresta {u,v}

    for (size_t i = 0; i < m; i++)
    {
        std::cin >> u >> v >> w;
        graph.addEdge(u, v, w);
    }

    printAdjacencyList(graph);

    return 0;
}
