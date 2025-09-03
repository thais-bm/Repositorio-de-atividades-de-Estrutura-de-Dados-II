#include <iostream>
#include <list>
#include <vector>
#include <cstdlib>
#include <limits>
#include <iomanip>

// class Queue
// o prof n quer q a gnt use a biblioteca
// no caso eu so modifique oq a gnt fez no semestre passado
// pode ser q eu n tenha trocado tudo pra std::vector D:
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
    Queue(std::vector<T>);
};

template <typename T>
Queue<T>::Queue(std::vector<T> queue) { this->queue = queue; }

template <typename T>
void Queue<T>::enqueue(T item) { queue.push_back(item); }

template <typename T>
void Queue<T>::dequeue()
{
    if (empty())
    {
        std::cout << "Queue tá vazior" << std::endl;
    }
    queue.remove(front());
}

template <typename T>
T Queue<T>::front()
{
    if (empty())
    {
        return T();
    }
    return queue.pop_front();
}

template <typename T>
int Queue<T>::size() { return queue.size(); }

template <typename T>
bool Queue<T>::empty() { return queue.empty(); }

class BFS
{
private:
    // tabuleiro com tamanho 63
    std::vector<std::string> tabuleiro[64];
    std::vector<std::string> knights;
    std::string king;

    // essas sao todos os moviemtentos possiveis do cavalo
    // fazuelli: 2 para frente e 1 pro lado (em todas as direções, ave maria)
    const std::vector<std::pair<int, int>> moviments = {
        {2, 1}, {2, -1}, {-2, 1}, {-2, -1}, {1, 2}, {1, -2}, {-1, 2}, {-1, -2}};

public:
    BFS(std::vector<std::string> knights, std::string king)
    {
        this->king = king;
        this->knights = knights;
    }

    // ordena de notacao xadrez para valores inteiros
    std::pair<int, int> chessPair(const std::string &position);
    // algoritmo de ordenacao
    void insertionSort(std::vector<int> &list);

    // somar com movimentos
    std::pair<int, int> makeMoviment(std::pair<int, int> knightPos, std::pair<int, int> move);
};

// Converte o par em notação 'letra-numero' para 'numero-numero'
std::pair<int, int> BFS::chessPair(const std::string &position)
{
    int col = position[0] - 'a';
    int row = position[1] - '1';
    return {row, col};
}

// Lógica:
/* Começa pelo 2 elemento
-> Compara com os elementos á sua esquerda
-> desloca os maiores a direta -> insira o elemento atual na posição correta
*/
void BFS::insertionSort(std::vector<int> &list)
{
    int key, idx_busca = 0;
    if (list.size() < 2)
    {
        return; // erro, precisa ter + de 1 elemento
    }

    for (auto i = 1; i < list.size(); i++)
    {
        key = list.at(i);                                  // pega o segundo valor
        idx_busca = i - 1;                                 // volta 1 valor de indice
        while (idx_busca >= 0 && list.at(idx_busca) > key) // o valor de trás for maior que a chave, a gente empurra eles pra direta
        {
            list[idx_busca + 1] = list.at(idx_busca);
            idx_busca = idx_busca - 1; // reduz 1 o valor de j
        }
        list[idx_busca + 1] = key; // se forem iguais ou chegar no inicio da fila, coloca a chave nesse lugar
        // faz isso ate terminar o for
    }
}

// ele vai fazer o movimento ao somar os std::pair e criar um par novo
std::pair<int, int> BFS::makeMoviment(std::pair<int, int> knightPos, std::pair<int, int> move)
{
    return { knight.first + move.first, knight.second + move.second }
}

int main()
{
    int tests;
    std::cin >> tests;

    // loop dos testes
    /*
    while(tests--) {
        std::vector<std::string> knights(4);
        std::string king;

        //os 4 cavalos
        for (int i = 0; i < 4; i++) {
            std::cin >> knights[i];
        }

        //o rei na última posição
        std::cin >> king;

        // BFS bfs(tests, knights, king);
    }
    */
    std::vector<int> list = {1, 9, 8, 5, 3, 5};
    int key, idx_busca = 0;
    if (list.size() < 2)
    {
        return 1; // erro, precisa ter + de 1 elemento
    }

    for (auto i = 1; i < list.size(); i++)
    {
        key = list.at(i);                                  // pega o segundo valor
        idx_busca = i - 1;                                 // volta 1 valor de indice
        while (idx_busca >= 0 && list.at(idx_busca) > key) // o valor de trás for maior que a chave, a gente empurra eles pra direta
        {
            list[idx_busca + 1] = list.at(idx_busca);
            idx_busca = idx_busca - 1; // reduz 1 o valor de j
        }
        list[idx_busca + 1] = key; // se forem iguais ou chegar no inicio da fila, coloca a chave nesse lugar
        // faz isso ate terminar o for
    }

    for (int num : list)
    {
        std::cout << num << "\n";
    }
    return 0;
}
