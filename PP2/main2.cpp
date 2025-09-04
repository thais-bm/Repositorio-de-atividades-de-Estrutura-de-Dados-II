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
    // ele vai ter 64 de tamanho para representar uma tabuleiro 8 x 8
    std::list<Vertex> *adj;

    // movimentos possiveis feitos pelo cavaleiro
    std::vector<std::pair<int, int>> movimentos = {
        {1, 2}, {1, -2}, {-1, 2}, {-1, -2}, {2, 1}, {2, -1}, {-2, 1}, {-2, -1}};

    void criar_Grafo(); // possui grafo do tabuleiro INTEIRO
public:
    Tabuleiro();
    ~Tabuleiro();
    void add_edge(Vertex u, Vertex v);
    std::list<Vertex> get_adj(Vertex u);
    uint get_edges();
    uint get_vertices();
    void remove_edge(Vertex u, Vertex v);
};

Tabuleiro::Tabuleiro()
{
    num_vertices = 64;
    num_edges = 0;
    adj = new std::list<Vertex>[num_vertices];
    criar_Grafo(); // cria o grafo do tabuleiro
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

void Tabuleiro::criar_Grafo()
{
    // Percorre todas as linhas e todas as colunas
    for (int linha = 0; linha < 8; ++linha)
    {
        for (int coluna = 0; coluna < 8; ++coluna)
        {

            // Converte (linha, coluna) para o índice do vértice de origem ÚNICO ( a1 virou 0, b1 virou 1....)
            int vertice_origem = linha * 8 + coluna;

            // Aplica todos os movimentos possiveis pelo cavaleiro
            for (const auto &mov : movimentos)
            {
                int nova_linha = linha + mov.first;
                int nova_coluna = coluna + mov.second;

                // 4. Verifica se a nova posição ta certa (dentro do tabuleiro)
                if (nova_linha >= 0 && nova_linha < 8 && nova_coluna >= 0 && nova_coluna < 8)
                {

                    // Se for válida, converte para o indice unico de (possivel) destino
                    int vertice_destino = nova_linha * 8 + nova_coluna;

                    // Adiciona a aresta na lista de adjacência
                    add_edge(vertice_origem, vertice_destino);
                }
            }
        }
    }
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
    std::pair<int, int> kingPosition; // vem convertido de notacao xadrez para um par de inteiros
    const std::vector<std::pair<unsigned int, unsigned int>> moviments = {
        {2, 1}, {2, -1}, {-2, 1}, {-2, -1}, {1, 2}, {1, -2}, {-1, 2}, {-1, -2}};

    void insertionSort(std::vector<unsigned int> &list);
    void auxiliarProcess(Tabuleiro &grafo, Vertex start);

    // essa funcao recebe o rei e, com base nos movimentos, determina as casas que representam ameaca pra ele
    // se o cavalo chega nessas, o rei ta ameaçado de morte
    std::vector<int> threatPositionForKing(std::pair<int, int> kingPosition)
    {
        std::vector<int> threatForKing;
        for (auto mov : moviments)
        {
            int nova_linha = kingPosition.first + mov.first;
            int nova_coluna = kingPosition.second + mov.second;

            if (nova_linha >= 0 && nova_linha < 8 && nova_coluna >= 0 && nova_coluna < 8)
            {
                // Se for estiver entre 8 x 8, converte para o índice de vertice do tabuleiro
                int ameaca = nova_linha * 8 + nova_coluna;
                threatForKing.push_back(ameaca);
            }
            return threatForKing;
        }
    }

    Tabuleiro grafoDoCavaleiro();

public:
    std::pair<unsigned int, unsigned int> ChessNotToPos(const std::string &position);

    BFS(std::vector<std::string> knights, std::string king)
    {
        this->kingPosition = ChessNotToPos(king);
        this->knights = knights;
    }

    void process(Tabuleiro &tabubu);
    void alcancaRei(Tabuleiro &grafo);
};

std::pair<unsigned int, unsigned int> BFS::ChessNotToPos(const std::string &position)
{
    unsigned int col = position[0] - 'a'; // fica entre 0 e 7, logo < 8
    unsigned int row = position[1] - '1'; // fica entre 0 e 7, logo < 8
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
    std::pair<unsigned int, unsigned int> kingPos = ChessNotToPos(king);
    unsigned int row = kingPos.first;
    unsigned int col = kingPos.second;
    Vertex start = row * 8 + col;
    auxiliarProcess(tabubu, start);
}

int main(int argc, char const *argv[])
{
    // Grafo de lista de adjacencia, mas chamado de Tabuleiro
    Tabuleiro tabubu = Tabuleiro();

    std::cout << "num_vertices: " << tabubu.get_vertices() << std::endl;
    std::cout << "num_edges: " << tabubu.get_edges() << std::endl;

    for (uint u = 0; u < tabubu.get_vertices(); ++u)
    {
        std::cout << u << ": ";
        std::list<Vertex> list = tabubu.get_adj(u);
        for (const auto &v : list)
        {
            std::cout << v << ", ";
        }
        std::cout << std::endl;
    }

    return 0;
}
