#include <iostream>
#include <list>
#include <vector>
#include <cstdlib>
#include <limits>
#include <iomanip>

// -x-x-x-x- Classe Tabuleiro baseada num grafo lista de adjacencias em que geral se movimenta igual cavalo -x-x-x-x-
using Vertex = unsigned int;
using uint = unsigned int;

class Tabuleiro
{
private:
    uint num_vertices;
    uint num_edges;
    std::vector<Vertex> *adj;
    std::vector<std::pair<int, int>> movimentos = {
        {1, 2}, {1, -2}, {-1, 2}, {-1, -2}, {2, 1}, {2, -1}, {-2, 1}, {-2, -1}};

    void criar_Grafo();  
    void insertionSort(std::vector<Vertex> &vec); 
    void add_edge(Vertex u, Vertex v);
    std::vector<Vertex> get_adj(Vertex u);
    uint get_edges();
    uint get_vertices();

public:
    Tabuleiro();
    ~Tabuleiro();
};

Tabuleiro::Tabuleiro()
{
    num_vertices = 64;
    num_edges = 0;
    adj = new std::vector<Vertex>[num_vertices];
    criar_Grafo();
}

Tabuleiro::~Tabuleiro()
{
    delete[] adj;
    adj = nullptr;
}

void Tabuleiro::add_edge(Vertex u, Vertex v)
{
    if (u >= num_vertices || v >= num_vertices || u == v)
    {
        throw std::invalid_argument("Valores invalidos");
    }

    adj[u].push_back(v);
    adj[v].push_back(u);
    num_edges += 1;
}

std::vector<Vertex> Tabuleiro::get_adj(Vertex u)
{
    if (u >= num_vertices)
    {
        throw std::invalid_argument("Valores invalidos");
    }
    return adj[u];
}

uint Tabuleiro::get_edges()
{
    return num_edges;
}

uint Tabuleiro::get_vertices()
{
    return num_vertices;
}

void Tabuleiro::criar_Grafo()
{
    for (int linha = 0; linha < 8; ++linha)
    {
        for (int coluna = 0; coluna < 8; ++coluna)
        {
            int vertice_origem = linha * 8 + coluna; // Convertendo (linha, coluna) para vertice 0-63

            // Adicionando arestas para todos os possiveis movimentos do cavalo
            for (const auto &mov : movimentos)
            {
                int nova_linha = linha + mov.first;
                int nova_coluna = coluna + mov.second;

                // Verificando se a nova posicao esta dentro dos limites do tabuleiro
                if (nova_linha >= 0 && nova_linha < 8 && nova_coluna >= 0 && nova_coluna < 8)
                {
                    int vertice_destino = nova_linha * 8 + nova_coluna;

                    // verficando se destino é maior que origem (grafo nao direcionado)
                    if (vertice_origem < vertice_destino)
                    {
                        add_edge(vertice_origem, vertice_destino);
                    }
                }
            }
        }
    }

    // Depois de tudo, ordenar as listas de adjacencias
    for (uint u = 0; u < num_vertices; ++u)
    {
        insertionSort(adj[u]);
    }
}

void Tabuleiro::insertionSort(std::vector<Vertex> &vec)
{
    if (vec.size() < 2)
    {
        return;
    }

    for (Vertex i = 1; i < vec.size(); i++)
    {
        Vertex key = vec[i];
        int j = i - 1;

        while (j >= 0 && vec[j] > key)
        {
            vec[j + 1] = vec[j];
            j = j - 1;
        }
        vec[j + 1] = key;
    }
}


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