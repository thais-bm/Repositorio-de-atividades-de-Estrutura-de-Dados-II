#include <iostream>
#include <vector>
#include <list>
#include <stdexcept>

// ESPEC-B1: criando apelidos
using uint = unsigned int;
using Vertex = uint;
using Weight = uint;

// ESPEC-B2: criando o GraphAL parte privada
class GraphAM {
private:
  uint num_vertices;                      
  uint num_edges;                         
  std::vector<std::vector<Weight>> adj;   

public:
  GraphAM(uint num_vertices);
  ~GraphAM();
  void add_edge(Vertex u, Vertex v);
  void remove_edge(Vertex u, Vertex v);
  std::list<Vertex> get_adj(Vertex u);
  uint get_num_vertices() const;
  uint get_num_edges() const;
  const std::vector<std::vector<Weight>>& get_adj_matrix() const;
};

void Print_Adjacency_Matrix(const GraphAM& g);

// ESPEC-B3: implementandoo construtor
GraphAM::GraphAM(uint num_vert)
{
  num_vertices = num_vert;
  num_edges = 0;
  adj.resize(num_vert, std::vector<Weight>(num_vert, 0));
}
// ESPEC-B4: implementando o destrutor
GraphAM::~GraphAM() {
}
uint GraphAM::get_num_vertices() const {
  return num_vertices;
}
uint GraphAM::get_num_edges() const {
  return num_edges;
}

// ESPEC-B5: implementando o add_edge
void GraphAM::add_edge(Vertex u, Vertex v) {
  if (u >= num_vertices || v >= num_vertices || u == v) {
    throw std::invalid_argument("Valores invalidos");
  }

  if (adj[u][v] == 0) {
      adj[u][v] = 1;
      adj[v][u] = 1;
      num_edges++;
    }
  }

// ESPEC-B6: implementando o add_adj
std::list<Vertex> GraphAM::get_adj(Vertex u){
  if (u >= num_vertices)
  {
    throw std::invalid_argument("Valores invalidos");
  }
std::list<Vertex> adjacentes;
  for (Vertex v = 0; v < num_vertices; v++) {
    if (adj[u][v] != 0) {
      adjacentes.push_back(v);
    }
  }
  return adjacentes;
}

// ESPEC-B7: Implementacao do metodo get_adj_matrix
const std::vector<std::vector<Weight>>& GraphAM::get_adj_matrix() const {
    return adj;
}

// ESPEC-B8: criando funcao Print_Adjacency_Matrix
void Print_Adjacency_Matrix(const GraphAM& g) {
  const auto& adj_matrix = g.get_adj_matrix();
  for(size_t i = 0; i < adj_matrix.size(); ++i) {
    for(size_t j = 0; j < adj_matrix[i].size(); ++j) {
      std::cout << adj_matrix[i][j];
// Imprime um espaco depois de cada numero, exceto o ultimo da linha
      if (j < adj_matrix[i].size() - 1) {
          std::cout << " ";
      }
    }
    std::cout << std::endl;
  }
}

// ESPEC-B9: Implementacao da funcao main
int main() {

  uint n = 0;
  uint m = 0;
  
  std::cin >> n >> m;
  
  GraphAM g(n);
  Vertex u = 0, v = 0;

  for (uint i = 0; i < m; ++i) {
    std::cin >> u >> v;
    g.add_edge(u, v);
  }
  // Impressao do cabecalho
  std::cout << "num_vertices: " << g.get_num_vertices() << std::endl;
  std::cout << "num_edges: " << g.get_num_edges() << std::endl;
  // Impressao da matriz
  Print_Adjacency_Matrix(g);
  return 0;
}
