#include <iostream>
#include <list>
#include <vector>
#include <cstdlib>
#include <limits>
#include <iomanip>
#include <string>

// - Struct para representar os exercitos
struct Exercito {
    std::string cor; // cores + tormenta
    const std::pair<int, int> posicao;
    std::vector<std::string> aliados;
};

// -x-x-x-x- Classe Tabuleiro baseada num grafo lista de adjacencias com peso em que geral se movimenta igual cavalo -x-x-x-x-
// Ainda falta implementar o peso nas arestas, faço depois de dormir
// E ajustar pro tabuleiro criar certinho sem bugs
// vo dar um soninho agora
using Vertex = unsigned int;
using uint = unsigned int;
using Weight = float;
using VertexWeightPair = std::pair<Vertex, Weight>;

class Tabuleiro
{
private:
    uint num_vertices;
    uint num_edges;
    int tabuleiro_size = 8; // tamanho padrao 8x8
    std::vector<VertexWeightPair> *adj;
    std::vector<std::pair<int, int>> movimentos = {
        {1, 2}, {1, -2}, {-1, 2}, {-1, -2}, {2, 1}, {2, -1}, {-2, 1}, {-2, -1}
    };

    void criar_grafo(); // ele usa o tabuleiro_size privado
    void insertion_sort(std::vector<VertexWeightPair> &vec);

    void add_edge(Vertex u, Vertex v, Weight w);
    Weight calculate_edge_weight(Vertex u, Vertex v);

    bool ja_existe_edge(Vertex u, Vertex v);
    std::vector<VertexWeightPair> get_adj(Vertex u);
    uint get_edges() const { return num_edges; }
    uint get_vertices() const { return num_vertices; }
    

public:
    Tabuleiro(int size = 8);
    ~Tabuleiro();
};

Tabuleiro::Tabuleiro(int size)
{
    num_vertices = size * size; // Exemplo: 8 x 8 = 64 vertices
    num_edges = 0;
    adj = new std::vector<VertexWeightPair>[num_vertices];
    criar_grafo();
}

Tabuleiro::~Tabuleiro()
{
    delete[] adj;
    adj = nullptr;
}

// extra: ve se ja existe
bool Tabuleiro::ja_existe_edge(Vertex u, Vertex v)
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

void Tabuleiro::add_edge(Vertex u, Vertex v, Weight w)
{
    // teste de validade
    // nao pode: menor que zero, maior que num_vertices e iguais
    if (u >= num_vertices || v >= num_vertices || u == v)
    {
        throw std::invalid_argument("Valores invalidos");
    }

    // verificar duplicidade (tem a verificacao na u<v na criacao do grafo, mas deixarei aqui tbm)
    if (ja_existe_edge(u,v))
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

std::vector<VertexWeightPair> Tabuleiro::get_adj(Vertex u)
{
    if (u >= num_vertices)
    {
        throw std::invalid_argument("Valores invalidos");
    }
    return adj[u];
}

Weight Tabuleiro::calculate_edge_weight(Vertex u, Vertex v){
    // numero unico para notacao xadrez 
    int linha_u = u / tabuleiro_size; // linha = sempre numerico
    int coluna_u = u % tabuleiro_size; // coluna = sempre letra

    char coluna_u = 'a' + coluna_u;
    char linhaChar = '1' + linha_u;

    // vou fazer 
}

void Tabuleiro::criar_grafo()
{
    for (int linha = 0; linha < tabuleiro_size; ++linha)
    {
        for (int coluna = 0; coluna < tabuleiro_size; ++coluna)
        {
            int vertice_origem = linha * tabuleiro_size + coluna; // Convertendo (linha, coluna) para vertice 0-63

            // Adicionando arestas para todos os possiveis movimentos do cavalo
            for (const auto &mov : movimentos)
            {
                int nova_linha = linha + mov.first;
                int nova_coluna = coluna + mov.second;

                // Verificando se a nova posicao esta dentro dos limites do tabuleiro
                if (nova_linha >= 0 && nova_linha < tabuleiro_size && nova_coluna >= 0 && nova_coluna < tabuleiro_size)
                {
                    int vertice_destino = nova_linha * tabuleiro_size + nova_coluna;

                    // verficando se destino é maior que origem (grafo nao direcionado)
                    if (vertice_origem < vertice_destino)
                    {
                        add_edge(vertice_origem, vertice_destino, calculate_edge_weight(vertice_origem, vertice_destino)); // falta arrumar essa funcao de calcular o peso
                    }
                }
            }
        }
    }

    // Depois de tudo, ordenar as listas de adjacencias
    for (uint u = 0; u < num_vertices; ++u)
    {
        insertion_sort(adj[u]);
    }
}

void Tabuleiro::insertion_sort(std::vector<VertexWeightPair> &vec)
{
    if (vec.size() < 2)
    {
        return;
    }

    for (Vertex i = 1; i < vec.size(); i++)
    {
        VertexWeightPair key = vec[i];
        int j = i - 1;

        while (j >= 0 && vec[j] > key)
        {
            vec[j + 1] = vec[j];
            j = j - 1;
        }
        vec[j + 1] = key;
    }
}

// 
template <typename T>
class Heapnode {
private:
    T val;
    int keyval;
    Heapnode* leftnode;
    Heapnode* rightnode;
public:
    T value() {return val;}
    int key() {return keyval;}
    Heapnode* left() {return leftnode;}
    Heapnode* right() {return rightnode;}
    std::pair<Heapnode*, Heapnode*> children() {return {leftnode, rightnode};}
    void setleft(Heapnode* node) {this.leftnode = node;}
    void setright(Heapnode* node) {this.rightnode = node;}
    Heapnode(T value, int key) {
        leftnode = nullptr;
        rightnode = nullptr;
        this->val = value;
        this->keyval = key;
    }
};

template <typename T>
class Minheap {
private:
    Heapnode<T> root;
public:
    Heapnode<T> gettop() {return this->root;}
    Heapnode<T> extracttop();
    void removetop();
    void add(Heapnode<T> node);
    void add(T value, int key) {add(Heapnode<T>(value, key))}
    void heapify(int A);
};

template <typename T>
void Minheap<T>::heapify(int A) {
    Heapnode<T>* current = root;
    Heapnode<T>* prev;
    for (int i = 0; i < A; fin)
}