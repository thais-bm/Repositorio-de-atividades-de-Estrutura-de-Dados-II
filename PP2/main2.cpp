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
    std::vector<Vertex> *adj;

    // movimentos possiveis feitos pelo cavaleiro
    std::vector<std::pair<int, int>> movimentos = {
        {1, 2}, {1, -2}, {-1, 2}, {-1, -2}, {2, 1}, {2, -1}, {-2, 1}, {-2, -1}};

    void criar_Grafo();                           // possui grafo do tabuleiro INTEIRO
    void insertionSort(std::vector<Vertex> &vec); // algoritmo de ordenacao

public:
    Tabuleiro();
    ~Tabuleiro();
    void add_edge(Vertex u, Vertex v);
    std::vector<Vertex> get_adj(Vertex u);

    uint get_edges();
    uint get_vertices();
};

Tabuleiro::Tabuleiro()
{
    num_vertices = 64;
    num_edges = 0;
    adj = new std::vector<Vertex>[num_vertices];
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
    // Percorre todas as linhas e todas as colunas
    for (int linha = 0; linha < 8; ++linha)
    {
        for (int coluna = 0; coluna < 8; ++coluna)
        {

            // Converte (linha, coluna) para o índice do vértice de origem ÚNICO ( a1 virou 0, b1 virou 1. c1 virou 2....)
            int vertice_origem = linha * 8 + coluna;

            // Aplica todos os movimentos possiveis pelo cavaleiro
            for (const auto &mov : movimentos)
            {
                int nova_linha = linha + mov.first;
                int nova_coluna = coluna + mov.second;

                // Verifica se a nova posição ta certa (bulgo ta dentro do tabuleiro)
                if (nova_linha >= 0 && nova_linha < 8 && nova_coluna >= 0 && nova_coluna < 8)
                {
                    // Se sim, converte para o indice unico de (possivel) destino e adiciona a aresta na lista de adjancencia
                    int vertice_destino = nova_linha * 8 + nova_coluna;

                    // porque quando acontecer o vertice 2,10, nao ter que rodar o coamndo 10, 2 e duplicar
                    if (vertice_origem < vertice_destino)
                    {
                        add_edge(vertice_origem, vertice_destino);
                    }
                }
            }
        }
    }

    // Aplicar o algoritmo de ordenacao em todos os 64 vertices
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

    for (int i = 1; i < vec.size(); i++)
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

    std::vector<std::string> knights; // ainda e string
    std::pair<int, int> kingPosition; // vem convertido de notacao xadrez para um par de inteiros
    const std::vector<std::pair<int, int>> moviments = {
        {2, 1}, {2, -1}, {-2, 1}, {-2, -1}, {1, 2}, {1, -2}, {-1, 2}, {-1, -2}};

    void auxiliarProcess(Tabuleiro &grafo, Vertex start); // algoritmo de busca por largura que recebe um inicial

    // essa funcao recebe o rei e, com base nos movimentos, determina as casas que representam ameaca pra ele. se o cavalo chega nessas, o rei ta ameaçado de morte
    std::vector<int> threatPositionForKing(std::pair<int, int> kingPosition);

public:
    std::pair<unsigned int, unsigned int> ChessNotToPos(const std::string &position);

    BFS(std::vector<std::string> knights, std::string king)
    {
        this->kingPosition = ChessNotToPos(king);
        this->knights = knights;
    }

    void process(Tabuleiro &tabubu, std::string rei);
    void alcancaRei(Tabuleiro &grafo);
};

std::pair<unsigned int, unsigned int> BFS::ChessNotToPos(const std::string &position)
{
    unsigned int col = position[0] - 'a'; // fica entre 0 e 7, logo < 8
    unsigned int row = position[1] - '1'; // fica entre 0 e 7, logo < 8
    return {row, col};
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

        std::vector<Vertex> adjList = grafo.get_adj(u);
        std::vector<Vertex> adjacentes(adjList.begin(), adjList.end()); // conversão list -> vector

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

std::vector<int> BFS::threatPositionForKing(std::pair<int, int> kingPosition)
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
    }
    return threatForKing;
}

void BFS::process(Tabuleiro &tabubu, std::string rei)
{
    std::pair<unsigned int, unsigned int> kingPos = ChessNotToPos(rei);
    unsigned int row = kingPos.first;
    unsigned int col = kingPos.second;
    Vertex start = row * 8 + col;
    auxiliarProcess(tabubu, start);
}

int main(int argc, char const *argv[])
{
    Tabuleiro tabubu = Tabuleiro(); // Cria o tabuleiro e desenha o grafo com todas as possiveis posicoes do cavaleiro a partir de TODOS os pontos do tabuleiro

    // PRINT tabuleiro
    std::cout << "num_vertices: " << tabubu.get_vertices() << std::endl;
    std::cout << "num_edges: " << tabubu.get_edges() << std::endl;

    for (uint u = 0; u < tabubu.get_vertices(); ++u)
    {
        std::cout << u << ": ";
        std::vector<Vertex> list = tabubu.get_adj(u);
        for (const auto &v : list)
        {
            std::cout << v << ", ";
        }
        std::cout << std::endl;
    }

    return 0;
}
