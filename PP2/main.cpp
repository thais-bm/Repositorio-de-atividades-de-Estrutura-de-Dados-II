#include <iostream>
#include <list>
#include <vector>
#include <cstdlib>
#include <limits>
#include <iomanip>

//class Queue
//o prof n quer q a gnt use a biblioteca
//no caso eu so modifique oq a gnt fez no semestre passado
//pode ser q eu n tenha trocado tudo pra std::list D:
template<typename T> class Queue {
private:
    std::list<T> queue;

public:
    void enqueue(T item);
    void dequeue();
    T front();
    int size();
    bool empty();
    Queue(std::list<T>);
};

template<typename T> Queue<T>::Queue(std::list<T> queue) { this->queue = queue; }

template<typename T> void Queue<T>::enqueue(T item) { queue.push_back(item); }

template<typename T> void Queue<T>::dequeue()
{
    if (empty()) {
        std::cout << "Queue tá vazior" << std::endl;
    }
    queue.remove(front());
}

template<typename T> T Queue<T>::front()
{
    if (empty()) {
        return T();
    }
    return queue.pop_front();
}

template<typename T> int Queue<T>::size() { return queue.size(); }

template<typename T> bool Queue<T>::empty() { return queue.empty(); }


class BFS{
private:
    //tabuleiro com tamanho 63
    std::list<std::string> tabuleiro[64];

    std::vector<std::string> knights;
    std::string king;


    //essas sao todos os moviemtentos possiveis do cavalo
    //fazuelli
    std::vector<std::pair<int,int>> moviments = {
        {2,1}, {2,-1}, {-2,1}, {-2,-1},
        {1,2}, {1,-2}, {-1,2}, {-1,-2}
    };
public:
    std::pair<int,int> chessPair(const std::string& position);
    
    BFS(std::vector<std::string> knights, std::string king) {
        this->king = king;
        this->knights = knights;
    }
};

//função que retorna par do tabuleiro
//da pra fazer conversao tbm mas tem matematica affr
std::pair<int,int> BFS::chessPair(const std::string& position) {
    int col = position[0] - 'a';
    int row = position[1] - '1';
    return {row, col};
}


int main(){
    int tests;
    std::cin >> tests;

    //loop dos testes
    while(tests--) {
        std::vector<std::string> knights(4);
        std::string king;

        //os 4 cavalos
        for (int i = 0; i < 4; i++) {
            std::cin >> knights[i];
        }

        //o rei na última posição
        std::cin >> king;

        BFS bfs(tests, knights, king);
    }

    return 0;
}
