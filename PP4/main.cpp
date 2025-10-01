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

// Nao me decidi sobre o tipo, entao deixarei generico
// Pai = i/2
// Filho esquerdo = 2i
// filho direito = 2i + 1
template <typename T>
class MinHeap
{
private:
    std::vector<T> heap;


    int getParent(int index) { return std::floor((index) / 2); }
    int getLeft(int index) { return 2 * index; }
    int getRight(int index) { return 2 * index + 1; }

public:

    void heapify(int index); // faz as trocas a partir de um indice
    void full_heapify();  // faz as trocas em todo o heap

    MinHeap() = default;
    void insert(const T &value);
    void erase(const T &value);
    T extractMin();
    bool isEmpty() const { return heap.empty(); }
};

// faz as trocas de cima para baixo
template <typename T>
void MinHeap<T>::heapify(int index)
{
    // Pegando o menor entre pai e filhos
    // o menor possivel e o proprio pai que e o index
    int smallest = index;
    int left = getLeft(index);
    int right = getRight(index);

    int heapsize = heap.size();

    // verifica se o filho esq exite e e menor que o pai
    if (left < heapsize && heap[left] < heap[smallest])
    {
        smallest = left;
    }
    else
    {
        smallest = index;
    }

    // verifica se o filho dir exite e e menor que o pai
    if (right < heapsize && heap[right] < heap[smallest])
    {
        smallest = right;
    }

    // se o menor nao e o pai, faz a troca e chama recursivamente ate geral ser trocado
    if (smallest != index)
    {
        T temp = heap[index];
        heap[index] = heap[smallest];
        heap[smallest] = temp;
        heapify(smallest);
    }
}

template <typename T>
void MinHeap<T>::full_heapify()
{
    // Comeca do ultimo pai ate a raiz
    for (int i = getParent(heap.size() - 1); i >= 0; i--)
    {
        heapify(i);
    }
}

template <typename T>
void MinHeap<T>::insert(const T &value)
{
    // Manda pro final do vetor
    heap.push_back(value);

    // calcula o valor do indice do novo elemento
    int index = heap.size() - 1;

    heapify(index);
}

template <typename T>
void MinHeap<T>::erase(const T &value)
{
    // procuro o valor do indice que eu quero matar
    // se nao achar, retorno sem fazer nada
    // se achar, troco com o ultimo elemento e dou um pop_back
    // aí faço heapfity -> vou fazer um full so que é mais facil, mas deve dar pra fazer um heapify so do indice que troquei
    int index = -1;

    for (int i = 0; i < int(heap.size()); i++){
        if (heap[i] == value){
            index = i;
            break;
        }
    }

    // verifica se achou
    if (index == -1){
        return;
    }

    // troca com o ultimo e da pop
    heap[index] = heap.back();
    heap.pop_back();

    // faz o heapify
    full_heapify();
}

template <typename T>
T MinHeap<T>::extractMin() {
    T minValue = heap[0];
    erase(minValue);
    return minValue;
}

// ----------------------------------------- ALGORITMO DE DIJKSTRA --------------------------------

struct DijkstraResult {
    std::vector<Weight> distancias;
    std::vector<int> predecessores;
};


DijkstraResult dijkstra(const GrafoPonderado& grafo, Vertex origem) {
    uint num_vertices = grafo.get_vertices();
    DijkstraResult resultado;
    resultado.distancias.assign(num_vertices, std::numeric_limits<Weight>::infinity());
    resultado.predecessores.assign(num_vertices, -1);

    resultado.distancias[origem] = 0;

    MinHeap<VertexWeightPair> fila_prioridade;
    fila_prioridade.insert({0.0f, origem});
    
    std::vector<bool> visitado(num_vertices, false);

    //std::cout << "se essa mensagem nao aparecer tem erro antes do loop while" << std::endl;

    //int vezes = 1;
    while (!fila_prioridade.isEmpty()) {
        //std::cout << "iteracao numero "<<vezes<<" do loop while de dijkstra iniciada" << std::endl;
        //vezes++;
        Vertex u = fila_prioridade.extractMin().second;

        if (visitado[u]) continue;
        visitado[u] = true;
        grafo.get_adj(0);
        //std::cout << "agora o loop for vai comecar para essa iteracao do loop while, e o index do adj vai ser: " << u << " e o get_adj() desse index resulta em um vetor de tamanho " << grafo.get_adj(int(u)).size() << std::endl;
        //int vezes2 = 1;
        for (const auto& vizinho : grafo.get_adj(u)) {
            //std::cout << "agora o loop numero "<<vezes2<<" do loop while numero "<<vezes2<< std::endl;
            //vezes2++;
            Vertex v = vizinho.first;
            Weight peso_aresta = vizinho.second;

            if (!visitado[v] && resultado.distancias[u] + peso_aresta < resultado.distancias[v]) {
                //std::cout<<"condicional if do loop for foi acionado"<<std::endl;
                resultado.distancias[v] = resultado.distancias[u] + peso_aresta;
                resultado.predecessores[v] = u;
                fila_prioridade.insert({resultado.distancias[v], v});
            }
        }
    }
    return resultado;
}