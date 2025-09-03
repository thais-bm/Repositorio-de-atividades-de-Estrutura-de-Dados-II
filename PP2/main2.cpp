#include <iostream>
#include <list>
#include <vector>
#include <cstdlib>
#include <limits>
#include <iomanip>

using Vertex = unsigned int;
using uint = unsigned int;

//  -x-x-x-x- Classe Queue  -x-x-x-x-
template <typename T>
class Queue
{
private:
    std::vector<T> queue;

public:
    void enqueue(T item);
    void dequeue();
    T front();
    int size();
    bool empty();
    Queue() = default;
};

template <typename T>
void Queue<T>::enqueue(T item) { queue.push_back(item); }

template <typename T>
void Queue<T>::dequeue()
{
    if (empty())
    {
        std::cout << "Queue tá vazior" << std::endl;
    }
    queue.erase(queue.begin());
}

template <typename T>
T Queue<T>::front()
{
    if (empty())
    {
        return T();
    }
    return queue.front();
}

template <typename T>
int Queue<T>::size() { return queue.size(); }

template <typename T>
bool Queue<T>::empty() { return queue.empty(); }

// -x-x-x-x- Classe Tabuleiro baseada num grafo lista de adjacencias  -x-x-x-x-
class Tabuleiro
{
private:
    uint num_vertices;
    uint num_edges;
    // ele vai ter 64 de tamanho
    std::list<Vertex> *adj;

public:
    Tabuleiro(uint num_vert);
    ~Tabuleiro();
    void add_edge(Vertex u, Vertex v);
    std::list<Vertex> get_adj(Vertex u);
    uint get_edges();
    uint get_vertices();
    void remove_edge(Vertex u, Vertex v);
};

Tabuleiro::Tabuleiro(uint num_vert)
{
    num_vertices = num_vert;
    num_edges = 0;
    adj = new std::list<Vertex>[num_vertices];
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

std::list<Vertex> Tabuleiro::get_adj(Vertex u)
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

void Tabuleiro::remove_edge(Vertex u, Vertex v)
{
    if (u >= num_vertices || v >= num_vertices || u == v)
    {
        throw std::invalid_argument("Valores invalidos");
    }
    adj[u].remove(v);
    adj[v].remove(u);
    num_edges -= 1;
}

//  -x-x-x-x- Classe BFS: Busca em Largura  -x-x-x-x-
class BFS
{
private:
    enum Color
    {
        white,
        black,
        grey
    };

    std::vector<std::string> knights;
    std::string king;
    const std::vector<std::pair<unsigned int, unsigned int>> moviments = {
        {2, 1}, {2, -1}, {-2, 1}, {-2, -1}, {1, 2}, {1, -2}, {-1, 2}, {-1, -2}};

    void insertionSort(std::vector<unsigned int> &list);
    void auxiliarProcess(Tabuleiro &grafo, Vertex start);

public:
    std::pair<unsigned int, unsigned int> chessPair(const std::string &position);

    BFS(std::vector<std::string> knights, std::string king)
    {
        this->king = king;
        this->knights = knights;
    }

    void process(Tabuleiro &tabubu);
    void alcancaRei(Tabuleiro &grafo);
};

std::pair<unsigned int, unsigned int> BFS::chessPair(const std::string &position)
{
    unsigned int col = position[0] - 'a';
    unsigned int row = position[1] - '1';
    return {row, col};
}

void BFS::insertionSort(std::vector<unsigned int> &list)
{
    unsigned int key, idx_busca = 0;
    if (list.size() < 2)
    {
        return;
    }

    for (auto i = 1; i < list.size(); i++)
    {
        key = list.at(i);
        idx_busca = i - 1;
        while (idx_busca >= 0 && list.at(idx_busca) > key)
        {
            list[idx_busca + 1] = list.at(idx_busca);
            idx_busca = idx_busca - 1;
        }
        list[idx_busca + 1] = key;
    }
}

void BFS::auxiliarProcess(Tabuleiro &grafo, Vertex start)
{
    std::vector<Color> color(grafo.get_vertices(), white);
    Queue<Vertex> fila;

    color[start] = grey;
    fila.enqueue(start);

    while (!fila.empty())
    {
        Vertex u = fila.front();
        fila.dequeue();

        std::cout << "Visitando vertice: " << u << std::endl;

        std::list<Vertex> adjList = grafo.get_adj(u);
        std::vector<Vertex> adjacentes(adjList.begin(), adjList.end()); // conversão explícita
        insertionSort(adjacentes);

        for (Vertex v : grafo.get_adj(u))
        {
            if (color[v] == white)
            {
                color[v] = grey;
                fila.enqueue(v);
            }
        }

        color[u] = black;
    }
}

void BFS::process(Tabuleiro &tabubu)
{
    std::pair<unsigned int, unsigned int> kingPos = chessPair(king);
    unsigned int row = kingPos.first;
    unsigned int col = kingPos.second;
    Vertex start = row * 8 + col;
    auxiliarProcess(tabubu, start);
}

int main(int argc, char const *argv[])
{
    std::vector<std::string> cavaleiros = {"b1", "c3", "e5"};
    std::string rei = "a1";

    Tabuleiro tabuleiro(4);
    BFS busca(cavaleiros, cavaleiros[0]);

    busca.process(tabuleiro);

    return 0;
}
