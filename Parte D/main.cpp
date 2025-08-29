#include <iostream>
#include <utility>
#include <vector>
#include <list>
#include <limits>
#include <iomanip>

// ESPEC-D1: Criando os apelidos 
using uint = unsigned int;
using Vertex = uint;       
using Weight = float;
using VertexWeightPair = std::pair<Vertex, Weight>;        

// ESPEC-D2:Atributos da classe WGAL
class WeightedWeightedGraphAM{
private:
  uint num_vertices;
  uint num_edges;
  std::vector<std::vector<Weight>> adj;
public:
  WeightedWeightedGraphAM(uint num_vertices);
  ~WeightedWeightedGraphAM();
  void add_edge(Vertex u, Vertex v, Weight w);
  void remove_edge(Vertex u, Vertex v);
  std::list<VertexWeightPair> get_adj(Vertex u) const;
  std::vector<std::vector<Weight>> get_adj_matrix() const;
  Weight get_weight(const Vertex& u, const Vertex& v) const;
  uint get_num_vertices() const;
  uint get_num_edges() const;
};

// ESPEC-D3a: Implementacao do construtor
WeightedWeightedGraphAM::WeightedWeightedGraphAM(uint num_vert) {
  num_vertices = num_vert;
  num_edges = 0;

  const Weight infinity = std::numeric_limits<Weight>::infinity();
  adj.assign(num_vertices, std::vector<Weight>(num_vertices, infinity));
}

// ESPEC-D4: Implementacao do Destrutor
WeightedWeightedGraphAM::~WeightedWeightedGraphAM() {
    // A destruicao do std::vector e automatica, liberando a memoria da matriz.
}

// ESPEC-D5: Implementacao do metodo add_edge
void WeightedWeightedGraphAM::add_edge(Vertex u, Vertex v, Weight w) {
  if (u >= num_vertices || v >= num_vertices || u == v) {
    throw std::invalid_argument("Valor de vertice invalido ou laco detectado.");
  }

  if (adj[u][v] == std::numeric_limits<Weight>::infinity()) {
    num_edges++;
  }

  adj[u][v] = w;
  adj[v][u] = w;
}

// ESPEC-D6: Implementacao do metodo get_adj
std::list<VertexWeightPair> WeightedWeightedGraphAM::get_adj(Vertex u) const{
  if (u >= num_vertices) {
    throw std::invalid_argument("Valor de vertice invalido.");
  }
  std::list<VertexWeightPair> adjacents;
  const Weight infinity = std::numeric_limits<Weight>::infinity();
  for (Vertex v = 0; v < num_vertices; ++v) {
    if (adj[u][v] != infinity) {
        adjacents.push_back({v, adj[u][v]});
    }
  }
  return adjacents;
}

// ESPEC-D7: Implementacao do metodo get_adj_matrix
std::vector<std::vector<Weight>>     WeightedWeightedGraphAM::get_adj_matrix() const {
    return adj;
}

// ESPEC-D8: Implementacao do metodo get_weight
Weight WeightedWeightedGraphAM::get_weight(const Vertex& u, const Vertex& v) const {
  if (u >= num_vertices || v >= num_vertices) {
    throw std::invalid_argument("Valor de parsmetro invalido");
  }
  return adj[u][v];
}

// Implementacao dos Getters
uint WeightedWeightedGraphAM::get_num_vertices() const {
    return num_vertices;
}

uint WeightedWeightedGraphAM::get_num_edges() const {
    return num_edges;
}

// ESPEC-D9: Implementacao da funcao Print_Adjacency_Matrix
void Print_Adjacency_Matrix(const WeightedWeightedGraphAM& g) {
  uint n = g.get_num_vertices();
  uint m = g.get_num_edges();
  std::vector<std::vector<Weight>> matrix = g.get_adj_matrix();

  std::cout << "num_vertices: " << n << std::endl;
  std::cout << "num_edges: " << m << std::endl;

  for (Vertex u = 0; u < n; ++u) {
    for (Vertex v = 0; v < n; ++v) {
      Weight weight = matrix[u][v];
        if (v > 0) {
          std::cout << " ";
        }
        if (weight == std::numeric_limits<Weight>::infinity()) {
          std::cout << "inf";
        } else {
          std::cout << weight;
        }
      }
    std::cout << std::endl;
  }
}

// ESPEC-D10: Implementacao main
int main(){
  uint n = 0; 
  uint m = 0; 

  std::cin >> n >> m;

  WeightedWeightedGraphAM graph(n);

  Vertex u = 0, v = 0;
  Weight w = 0.0;
  for (uint i = 0; i < m; ++i) {
    std::cin >> u >> v >> w;

    try {
      graph.add_edge(u, v, w);
    } catch (const std::invalid_argument& e) {
      }
  }

  Print_Adjacency_Matrix(graph);
  return 0;

}


