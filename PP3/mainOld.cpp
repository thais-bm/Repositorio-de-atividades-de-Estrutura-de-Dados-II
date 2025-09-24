#include <iostream>
#include <list>
#include <vector>
#include <cstdlib>
#include <limits>
#include <iomanip>
#include <string>
#include <cmath>

// - Struct para representar os exercitos
struct Exercito
{
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
        {1, 2}, {1, -2}, {-1, 2}, {-1, -2}, {2, 1}, {2, -1}, {-2, 1}, {-2, -1}};

    void criar_grafo(); // ele usa o tabuleiro_size privado
    void insertion_sort(std::vector<VertexWeightPair> &vec);

    void add_edge(Vertex u, Vertex v, Weight w);
    Weight calculate_edge_weight(Vertex u, Vertex v);

    bool ja_existe_edge(Vertex u, Vertex v);

public:
    Tabuleiro(int size = 8);
    ~Tabuleiro();

    std::vector<VertexWeightPair> get_adj(Vertex u);
    uint get_edges() const { return num_edges; }
    uint get_vertices() const { return num_vertices; }
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

Weight Tabuleiro::calculate_edge_weight(Vertex u, Vertex v)
{
    // transformando o tamanho em numero ASCII
    char tamanho_tabuleiro = '0' + tabuleiro_size; // '0' + 8 = '8' (valor ASCII de '8')

    // numero unico para notacao xadrez
    // formula inversa -> letra = (indice % 8) + 'A' e numero = 8 - (indice / 8)

    char letra_u = (u % tabuleiro_size) + 'a'; // coluna = sempre letra -> resto da divisao pelo tamanho do tabuleiro + valor ascii de 'a'
    char numero_u = tamanho_tabuleiro - (u / tabuleiro_size);     // linha = sempre numerico ->

    char letra_v = (v % tabuleiro_size) + 'a';                // coluna = sempre letra
    char numero_v = tamanho_tabuleiro - (v / tabuleiro_size); // linha = sempre numerico: valor ascii do tamanho do tabuleiro - ( vertice / tamanho do tabuleiro)

    // calculo do peso
    // peso = valor ASCII de letra u * numero u + valor ASCII de letra v * numero v mod 19
    // static_cast<int> para converter char para int (valor ASCII)
    Weight peso = ((static_cast<int>(letra_u) * (numero_u - '0')) + (static_cast<int>(letra_v) * (numero_v - '0'))) % 19;

    std::cout << "Peso de " << letra_u << numero_u << " e " << letra_v << numero_v << ": " << peso << " Calculado com: " << static_cast<int>(letra_u) << " * " << (numero_u - '0') << " + " << static_cast<int>(letra_v) << " * " << (numero_v - '0') << " mod 19" << " = " << peso << std::endl;
    return peso;
}

std::vector<VertexWeightPair> Tabuleiro::get_adj(Vertex u)
{
    if (u >= num_vertices)
    {
        throw std::invalid_argument("Valores invalidos");
    }
    return adj[u];
}

void Tabuleiro::criar_grafo()
{
    for (int coluna = 0; coluna < tabuleiro_size; ++coluna)
    {
        for (int linha = 0; linha < tabuleiro_size; ++linha)
        {
            int vertice_origem = linha * tabuleiro_size + coluna; // Convertendo (linha, coluna) para vertice 0-63

            // Adicionando arestas para todos os possiveis movimentos dos exercitos com base em cavalo
            for (const auto &mov : movimentos)
            {
                int nova_coluna = coluna + mov.first;
                int nova_linha = linha + mov.second;

                // Verificando se a nova posicao esta dentro dos limites do tabuleiro
                if (nova_linha >= 0 && nova_linha < tabuleiro_size && nova_coluna >= 0 && nova_coluna < tabuleiro_size)
                {
                    int vertice_destino = nova_linha * tabuleiro_size + nova_coluna;

                    // verficando se destino é maior que origem (grafo nao direcionado)
                    if (vertice_origem < vertice_destino)
                    {
                        add_edge(vertice_origem, vertice_destino, calculate_edge_weight(vertice_origem, vertice_destino)); // com calculo de peso
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

// Nao me decidi sobre o tipo, entao deixarei generico
// Pai = i/2
// Filho esquerdo = 2i
// filho direito = 2i + 1
template <typename T>
class MinHeap
{
private:
    std::vector<T> heap;

    void heapify(int index); // faz as trocas a partir de um indice
    void full_heapify();  // faz as trocas em todo o heap

    int getParent(int index) { return std::floor((index) / 2); }
    int getLeft(int index) { return 2 * index; }
    int getRight(int index) { return 2 * index + 1; }

public:
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


    // verifica se o filho esq exite e e menor que o pai
    if (left < heap.size() && heap[left] < heap[smallest])
    {
        smallest = left;
    }
    else
    {
        smallest = index;
    }

    // verifica se o filho dir exite e e menor que o pai
    if (right < heap.size() && heap[right] < heap[smallest])
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

    for (int i = 0; i < heap.size(); i++){
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

// DEBUG DO TABULEIRO
void printAdjacencyList(Tabuleiro &graph)
{
    std::cout << "num_vertices: " << graph.get_vertices() << std::endl;
    std::cout << "num_edges: " << graph.get_edges() << std::endl;

    for (uint u = 0; u < graph.get_vertices(); ++u)
    {
        std::cout << u << ": ";
        std::vector<VertexWeightPair> list = graph.get_adj(u);
        for (const auto &v : list)
        {
            std::cout << "(" << v.first << ", " << v.second << "), ";
        }
        std::cout << std::endl;
    }
}

// - x - x - Struct de algoritmo de caminho minimo Dijkstra - x - x -
struct Dijkstra{
    Exercito* exercito;
    // atributos do algoritmo de dijistra
    // nao sei como escreve o nome dele
};

int main(int argc, char const *argv[])
{
    Tabuleiro tabuleiro;
    printAdjacencyList(tabuleiro);
    return 0;
}

/*

Tabela de conversao notacao ASCII para indice
letra x indice
A     ->     0
B     ->     1
C     ->     2
D     ->     3
E     ->     4
F     ->     5
G     ->     6
H     ->     7


numero x indice
8     ->     0
7     ->     1
6     ->     2
5     ->     3
4     ->     4
3     ->     5
2     ->     6
1     ->     7

formula do indice -> indice = (8 - numero) * 8 + (letra - 'A')
formula inversa -> letra = (indice % 8) + 'A' e numero = 8 - (indice / 8)

                             _____ _   _   ___  _    _   _ _ _ 
                            /  ___| | | | / _ \| |  | | | | | |
                            \ `--.| |_| |/ /_\ \ |  | | | | | |
                             `--. \  _  ||  _  | |/\| | | | | |
                            /\__/ / | | || | | \  /\  / |_|_|_|
                            \____/\_| |_/\_| |_/\/  \/  (_|_|_)
                                   
                                   
                                                                        00                          
                                                                   00000000                         
                                                                0000828000       0000               
                                                             00005718000       000600               
                                                          000027718000       00067400               
                                                        00097 779000       000677700                
                                                      000477774000       00097777600                
                                                   0000577773000       000977   300                 
                                                  0002777779000       00017777 7800                 
                                                0004777771000       0003   7  7400                  
                                               00877   75000      000477      200                   
                                             00057   77600      00097777     1000                   
                                            00017    7800     00001        77000                    
                                           0087     7900     0002777      77800                     
                                          00677    7500    000977        77900                      
                                         00677     100   000877         77600                       
                                        00677     7800 000817            600                        
                                       000777     740000677            7600                         
                                       001          7777777           7600                          
                                      00477                          7900                           
                                     000777                         7000                            
                                     00577                         2000                             
                                     0017                        7900                               
                                    00077                      72000                                
                                    008                       7800                                  
                                    008 001        4885     74000                                   
                                    000 0007    3000000   75000                                     
                                     00 800577 60000001  5000                                       
                                     008 0037  800009  4000                                         
                                      000             0000                                           
                                       00000000000000000                                            
                                      0000666666666669000                                           
                                        0008666666660000                                  0000000   
                                          008666666800                                   005809500  
                                         00089669699000                               000065008580  
                                        0000689609909000                          000006549096800   
                                       00009609680690090000                    000095480000 000     
                                     000808690969086600890000              000085590000             
                                   00009006690966809669008680000       000084560000                 
                                 0000690066680666680966690086900000000004540000                     
                               000066908666600666668086666900096800065480000                        
                              000666808666660066666690066666600096900000                            
                             0086690096666690866666669006666666800969000                            
                            008669006666666809666666666009666666600096800                           
                           0086690066666666006666666666600866666669008600                           
                           0096900966666668096666666666668006666660000000                           
                            0000096666666600666666666666669009660008000                             
                               000896666690866666666666988000000000000                              
                                 0000000000966666669000000000000                                    
                                000923225400000000000000000000                                      
                            000063333590000  00000    0000                                          
                         00006333590000       00000   00000                                         
                      0000433590000            0000   00000                                         
                   000923590000                0000    0000                                         
                00043290000                     0000   0000                                         
             0085290000                         0000   0000                                         
          008560000                              0000  0000                                         
       00000000                                  0000  0000                                         
    0000000                                       000  0000                                         
 00000                                            000  0000                                         
                                                  000   000                                         
                                                   00   00                                          
                                                   00   00                                          

*/