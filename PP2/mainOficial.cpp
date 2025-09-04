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
        std::cout << "Queue ta vaziorrrr" << std::endl;
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
    std::vector<Vertex> *adj;
    std::vector<std::pair<int, int>> movimentos = {
        {1, 2}, {1, -2}, {-1, 2}, {-1, -2}, {2, 1}, {2, -1}, {-2, 1}, {-2, -1}};

    void criar_Grafo();  
    void insertionSort(std::vector<Vertex> &vec); 

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
            int vertice_origem = linha * 8 + coluna;
            for (const auto &mov : movimentos)
            {
                int nova_linha = linha + mov.first;
                int nova_coluna = coluna + mov.second;
                if (nova_linha >= 0 && nova_linha < 8 && nova_coluna >= 0 && nova_coluna < 8)
                {

                    int vertice_destino = nova_linha * 8 + nova_coluna;

                    
                    if (vertice_origem < vertice_destino)
                    {
                        add_edge(vertice_origem, vertice_destino);
                    }
                }
            }
        }
    }

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

class AtaqueDosCavaleiros
{
private:
    std::vector<std::string> cavaleiros;
    std::string reiPosicao;
    std::vector<int> ameacasRei(); 
    int melhorCaminhoAoRei(Tabuleiro &tabubu, int no_inicio, const std::vector<int> &listaAmeacas);

    std::pair<int, int> ChessNotToPos(const std::string &position);

public:
    AtaqueDosCavaleiros(std::vector<std::string> knights, std::string king);
    void solucionar( Tabuleiro &tabubu);
};

AtaqueDosCavaleiros::AtaqueDosCavaleiros(std::vector<std::string> knights, std::string king)
{
    this->reiPosicao = king;
    this->cavaleiros = knights;
}

std::pair<int, int> AtaqueDosCavaleiros::ChessNotToPos(const std::string &position)
{
    return {position[1] - '1', position[0] - 'a'};
}

std::vector<int> AtaqueDosCavaleiros::ameacasRei()
{
    const std::vector<std::pair<int, int>> movimentos = {
        {2, 1}, {2, -1}, {-2, 1}, {-2, -1}, {1, 2}, {1, -2}, {-1, 2}, {-1, -2}};

    int coluna = reiPosicao[0] - 'a'; 
    int linha = reiPosicao[1] - '1';  

    std::vector<int> listaAmeacas;
    for (auto mov : movimentos)
    {
        int nova_linha = linha + mov.first;
        int nova_coluna = coluna + mov.second;
        if (nova_linha >= 0 && nova_linha < 8 && nova_coluna >= 0 && nova_coluna < 8)
        {
            listaAmeacas.push_back(nova_linha * 8 + nova_coluna); 
        }
    }

    return listaAmeacas;
}

int AtaqueDosCavaleiros::melhorCaminhoAoRei(Tabuleiro &tabubu, int no_inicio, const std::vector<int> &listaAmeacas)
{
    int distanciaDoRei = 0;
    for (auto pos : listaAmeacas)
    {
        if (no_inicio == pos)
        {
            return distanciaDoRei; 
        }
    }

    std::vector<int> distancia(tabubu.get_vertices(), -1);
    Queue<Vertex> fila;                                    

    distancia[no_inicio] = 0;
    fila.enqueue(no_inicio);

    while (!fila.empty())
    {
        Vertex u = fila.front();
        fila.dequeue();

        for (int v : tabubu.get_adj(u))
        {
            if (distancia[v] == -1)
            {
                distancia[v] = distancia[u] + 1;
                for (auto pos : listaAmeacas)
                {
                    if (v == pos)
                    {
                        return distancia[v];
                    }
                }
                fila.enqueue(v);
            }
        }
    }
    return -1;
}

void AtaqueDosCavaleiros::solucionar(Tabuleiro &tabubu)
{
    std::vector<int> posicao_alvo = ameacasRei();
    std::vector<int> resultados;

    int minimo_mov = 3000;

    for (const auto &cavalo : this->cavaleiros)
    {
        std::pair<int, int> cava_pos = ChessNotToPos(cavalo);      
        Vertex no_inicio = cava_pos.first * 8 + cava_pos.second;
        int move = melhorCaminhoAoRei(tabubu, no_inicio, posicao_alvo); 
        resultados.push_back(move);
        if (move != -1 && move < minimo_mov)
        {
            minimo_mov = move;
        }
    }


    bool first = true;
    for (int move_count : resultados)
    {
        if (move_count == minimo_mov)
        {
            if (!first)
                std::cout << " ";
            std::cout << move_count;
            first = false;
        }
    }
    std::cout << std::endl;
}

int main(int argc, char const *argv[])
{
    Tabuleiro tabubu = Tabuleiro();

    int num_tests;
    std::cin >> num_tests;

    while (num_tests--) {
        std::vector<std::string> knights(4);
        std::string king;
        for (int i = 0; i < 4; ++i) {
            std::cin >> knights[i];
        }
        std::cin >> king;

        AtaqueDosCavaleiros solver(knights, king);
        solver.solucionar(tabubu);
    }

    return 0;
}

/*
                ESNUPI


              XXXX
             X    XX
            X  ***  X                XXXXX
           X  *****  X            XXX     XX
        XXXX ******* XXX      XXXX          XX
      XX   X ******  XXXXXXXXX                XX XXX
    XX      X ****  X                           X** X
   X        XX    XX     X                      X***X
  X         //XXXX       X                      XXXX
 X         //   X                             XX
X         //    X          XXXXXXXXXXXXXXXXXX/
X     XXX//    X          X                                                             OOOOOOOOOOOOOOOO
X    X   X     X         X                                             OOOOOOOOOOOOOOOOOO                       
X    X    X    X        X                                       AWOOOOO
 X   X    X    X        X                    XX
 X    X   X    X        X                 XXX  XX
  X    XXX      X        X               X  X X  X
  X             X         X              XX X  XXXX
   X             X         XXXXXXXX\     XX   XX  X
    XX            XX              X     X    X  XX
      XX            XXXX   XXXXXX/     X     XXXX
        XXX             XX***         X     X
           XXXXXXXXXXXXX *   *       X     X
                        *---* X     X     X
                       *-* *   XXX X     X
                       *- *       XXX   X
                      *- *X          XXX
                      *- *X  X          XXX
                     *- *X    X            XX
                     *- *XX    X             X
                    *  *X* X    X             X
                    *  *X * X    X             X
                   *  * X**  X   XXXX          X
                   *  * X**  XX     X          X
                  *  ** X** X     XX          X
                  *  **  X*  XXX   X         X
                 *  **    XX   XXXX       XXX
                *  * *      XXXX      X     X
               *   * *          X     X     X
 =======*******   * *           X     X      XXXXXXXX\
        *         * *      /XXXXX      XXXXXXXX\      )
   =====**********  *     X                     )  \  )
     ====*         *     X               \  \   )XXXXX
=========**********       XXXXXXXXXXXXXXXXXXXXXX


*/