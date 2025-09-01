#include <iostream>
#include <vector>
#include <list>

// ESPEC-1: criando apelidos
using Vertex = unsigned int;
using uint = unsigned int;

// ESPEC-2: criando o GraphAL
class GraphAL
{
private:
    uint num_vertices;
    uint num_edges;
    std::list<Vertex> *adj;

public:
    GraphAL(uint num_vert);
    ~GraphAL();
    void add_edge(Vertex u, Vertex v);
    std::list<Vertex> get_adj(Vertex u);
    uint get_edges();
    uint get_vertices();
    void remove_edge(Vertex u, Vertex v);
};

// ESPEC-3: Codificar o construtor
GraphAL::GraphAL(uint num_vert)
{
    num_vertices = num_vert;
    num_edges = 0;
    adj = new std::list<Vertex>[num_vertices];
}

// ESPEC-4: Codificar o destrutor
GraphAL::~GraphAL()
{
    delete[] adj;
    adj = nullptr;
}

// ESPEC-5: Metodo add-edge
void GraphAL::add_edge(Vertex u, Vertex v)
{
    // teste de validade
    // nao pode: menor que zero, maior que num_vertices e iguais
    if (u >= num_vertices || v >= num_vertices || u == v)
    {
        throw std::invalid_argument("Valores invalidos");
    }

    adj[u].push_back(v);
    adj[v].push_back(u);
    num_edges += 1;
}

// ESPEC-6: Criando metodo get-adj
std::list<Vertex> GraphAL::get_adj(Vertex u)
{
    if (u >= num_vertices)
    {
        throw std::invalid_argument("Valores invalidos");
    }
    return adj[u];
}

uint GraphAL::get_edges()
{
    return num_edges;
}

uint GraphAL::get_vertices()
{
    return num_vertices;
}

void GraphAL::remove_edge(Vertex u, Vertex v)
{
    if (u >= num_vertices || v >= num_vertices || u == v)
    {
        throw std::invalid_argument("Valores invalidos");
    }
    adj[u].remove(v);
    adj[v].remove(u);
    num_edges -= 1;
}

void print_adj_list(GraphAL& graph) {
    std::cout << "num_vertices: " << graph.get_vertices() << std::endl;
    std::cout << "num_edges: " << graph.get_edges() << std::endl;

    for (uint u = 0; u < graph.get_vertices(); ++u) {
        std::cout << u << ": ";
        std::list<Vertex> list = graph.get_adj(u);
        for (const auto& v : list) {
            std::cout << v << ", ";
        }
        std::cout << std::endl;
    }
}


// ESPEC-8: a main()
int main()
{
    uint n, m = 0;

    std::cin >> n; // ordem do grafo
    std::cin >> m; // numero de arestas

    GraphAL graph = GraphAL(n);

    uint u, v = 0; // representam aresta vertices q foram uma aresta {u,v}

    for (size_t i = 0; i < m; i++)
    {
        std::cin >> u;
        std::cin >> v;
        graph.add_edge(u, v);
    }

    print_adj_list(graph);

    return 0;
}
/* 

ITS COMEBACK OF apoCATlipse!!!
 _._     _,-'""`-._
(,-.`._,'(       |\`-/|
    `-.-' \ )-`( , o o)
          `-    \`_`"'-

    |\__/,|   (`\
  _.|o o  |_   ) )
-(((---(((--------

      |\      _,,,---,,_
ZZZzz /,`.-'`'    -.  ;-;;,_
     |,4-  ) )-,_. ,\ (  `'-'
    '---''(_/--'  `-'\_)  Felix Lee 

*/