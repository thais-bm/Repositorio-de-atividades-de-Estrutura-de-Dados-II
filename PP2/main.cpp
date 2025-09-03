#include <iostream>
#include <list>
#include <vector>
#include <cstdlib>
#include <limits>
#include <iomanip>

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
        std::cout << "Queue ta vazior" << std::endl;
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

// ESPEC-1: criando apelidos
using Vertex = unsigned int;
using uint = unsigned int;

// ESPEC-2: criando o Tabuleiro com lista de adjacencia
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

// ESPEC-3: Codificar o construtor
Tabuleiro::Tabuleiro(uint num_vert)
{
    num_vertices = num_vert;
    num_edges = 0;
    adj = new std::list<Vertex>[num_vertices];
}

// ESPEC-4: Codificar o destrutor
Tabuleiro::~Tabuleiro()
{
    delete[] adj;
    adj = nullptr;
}

// ESPEC-5: Metodo add-edge
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

// ESPEC-6: Criando metodo get-adj
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

void print_adj_list(Tabuleiro &graph)
{
    std::cout << "num_vertices: " << graph.get_vertices() << std::endl;
    std::cout << "num_edges: " << graph.get_edges() << std::endl;

    for (uint u = 0; u < graph.get_vertices(); ++u)
    {
        std::cout << u << ": ";
        std::list<Vertex> list = graph.get_adj(u);
        for (const auto &v : list)
        {
            std::cout << v << ", ";
        }
        std::cout << std::endl;
    }
}


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
    const std::vector<std::pair<int, int>> moviments = {
        {2, 1}, {2, -1}, {-2, 1}, {-2, -1}, {1, 2}, {1, -2}, {-1, 2}, {-1, -2}};

    void insertionSort(std::vector<int> &list);
    void auxiliarProcess(Tabuleiro &grafo, Vertex start);


public:
    std::pair<int, int> chessPair(const std::string &position);

    BFS(std::vector<std::string> knights, std::string king)
    {
        this->king = king;
        this->knights = knights;
    }

    void process(Tabuleiro &tabubu);
    void alcancaRei(Tabuleiro &grafo);
};

std::pair<int, int> BFS::chessPair(const std::string &position)
{
    int col = position[0] - 'a';
    int row = position[1] - '1';
    return {row, col};
}

void BFS::insertionSort(std::vector<int> &list)
{
    int key, idx_busca = 0;
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
            idx_busca = idx_busca - 1; // reduz 1 o valor de j
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
    std::pair<int, int> kingPos = chessPair(king);
    int row = kingPos.first;
    int col = kingPos.second;
    Vertex start = row * 8 + col;
    auxiliarProcess(tabubu, start);
}

void BFS::alcancaRei(Tabuleiro &grafo)
{
    std::vector<int> dist(64, -1); 
    std::vector<Color> color(64, white);
    Queue<Vertex> fila;

    for (const std::string &k : knights)
    {
        std::pair<int, int> aux = chessPair(k);
        int linha = aux.first;
        int coluna = aux.second;
        Vertex origem = linha * 8 + coluna;

        fila.enqueue(origem);
        dist[origem] = 0;
        color[origem] = grey;
    }

    std::pair<int, int> aux = chessPair(king);
    int reiLinha = aux.first;
    int reiColuna = aux.second;
    Vertex destinoRei = reiLinha * 8 + reiColuna;

    while (!fila.empty())
    {
        Vertex u = fila.front();
        fila.dequeue();

        for (Vertex v : grafo.get_adj(u))
        {
            if (color[v] == white)
            {
                color[v] = grey;
                dist[v] = dist[u] + 1;
                fila.enqueue(v);
            }
        }

        color[u] = black;
    }

}

int main()
{
    int tests;
    std::cin >> tests;
     std::vector<std::string> resultados;

    while (tests--)
    {
        std::vector<std::string> knights(4);
        std::string king;

        for (int i = 0; i < 4; i++)
        {
            std::cin >> knights[i];
        }

        std::cin >> king;

        Tabuleiro g(64);
        const std::vector<std::pair<int, int>> movimentos = {
            {2, 1}, {2, -1}, {-2, 1}, {-2, -1}, {1, 2}, {1, -2}, {-1, 2}, {-1, -2}};

        for (int linha = 0; linha < 8; linha++)
        {
            for (int coluna = 0; coluna < 8; coluna++)
            {
                Vertex origem = linha * 8 + coluna;
                for (std::pair<int, int> di : movimentos)
                {
                    int novaLinha = linha + di.first;
                    int novaColuna = coluna + di.second;
                    if (novaLinha >= 0 && novaLinha < 8 &&
                        novaColuna >= 0 && novaColuna < 8)
                    {
                        Vertex destino = novaLinha * 8 + novaColuna;
                        g.add_edge(origem, destino);
                    }
                }
            }
        }

        int reiLinha = king[1] - '1';
        int reiColuna = king[0] - 'a';
        Vertex destinoRei = reiLinha * 8 + reiColuna;

        std::vector<int> movimentosMinimos;

        for (const std::string &k : knights)
        {
            std::vector<int> dist(64, -1);
            std::vector<int> color(64, 0);
            Queue<Vertex> fila;

            int linha = k[1] - '1';
            int coluna = k[0] - 'a';
            Vertex origem = linha * 8 + coluna;

            fila.enqueue(origem);
            dist[origem] = 0;
            color[origem] = 1;

            while (!fila.empty())
            {
                Vertex u = fila.front();
                fila.dequeue();

                for (Vertex v : g.get_adj(u))
                {
                    if (color[v] == 0)
                    {
                        color[v] = 1;
                        dist[v] = dist[u] + 1;
                        fila.enqueue(v);
                    }
                }

                color[u] = 2;
            }

            movimentosMinimos.push_back(dist[destinoRei]);
        }

        int menor = std::numeric_limits<int>::max();
        for (int m : movimentosMinimos)
        {
            if (m != -1 && m < menor)
                menor = m;
        }

        std::string linhaResultado;
        for (int i = 0; i < 4; i++)
        {
            if (movimentosMinimos[i] == menor)
            {
                if (!linhaResultado.empty())
                    linhaResultado += " ";
                linhaResultado += std::to_string(menor-1); 
            }
        }

        resultados.push_back(linhaResultado);
    }

    for (const std::string &linha : resultados)
    {
        std::cout << linha << std::endl;
    }

    return 0;
}
