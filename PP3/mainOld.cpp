#include <iostream>
#include <list>
#include <vector>
#include <cstdlib>
#include <limits>
#include <iomanip>
#include <string>
#include <cmath>

using Vertex = unsigned int;
using uint = unsigned int;
using Weight = float;
using VertexWeightPair = std::pair<Vertex, Weight>;

struct Exercito
{
    std::string cor;
    std::vector<std::string> inimigos;
    Vertex posicao;
    std::list<Vertex> rota = {};
    Weight peso_da_rota = 0;
    bool pode_mover = true;
    uint movimentos = 0;
};

struct Casa
{
    std::vector<Exercito> exercitos;
    bool tormenta;
};

std::pair<char, char> int_to_chess(Vertex u, int size)
{
    char tamanho_tabuleiro = '0' + size;
    char letra_u = (u % size) + 'a';
    char numero_u = tamanho_tabuleiro - (u / size);

    return {letra_u, numero_u};
}

std::string int_to_notation(Vertex u, int size)
{
    std::pair<char, char> par = int_to_chess(u, size);
    std::string output = "  ";
    output[0] = par.first;
    output[1] = par.second;
    return output;
}

template <typename T>
bool sequential_check(const T &target, const std::vector<T> &vector)
{
    for (T item : vector)
    {
        if (item == target)
        {
            return true;
        }
    }
    return false;
}

std::vector<uint> algoritmo_movimentos(Vertex u, int size)
{
    int u1 = u / size;
    int u2 = u % size;
    std::vector<std::pair<int, int>> movimentos = {{1, 2}, {1, -2}, {-1, 2}, {-1, -2}, {2, 1}, {2, -1}, {-2, 1}, {-2, -1}};
    std::vector<Vertex> posfinal;

    for (std::pair<int, int> movimento : movimentos)
    {
        int v1 = u1 + movimento.first;
        int v2 = u2 + movimento.second;
        if (v1 < size && v2 < size && v1 >= 0 && v2 >= 0)
        {
            posfinal.push_back((v1 * size) + (v2));
        }
    }

    return posfinal;
}

Weight calculate_edge_weight(Vertex u, Vertex v, uint size)
{
    std::pair<char, char> helper = int_to_chess(u, size);
    char letra_u = helper.first;
    char numero_u = helper.second;
    helper = int_to_chess(v, size);
    char letra_v = helper.first;
    char numero_v = helper.second;

    Weight peso = ((static_cast<int>(letra_u) * (numero_u - '0')) + (static_cast<int>(letra_v) * (numero_v - '0'))) % 19;

    return peso;
}

template <typename T>
std::vector<T> reverse(std::vector<T> original)
{
    std::vector<T> helper;
    int n = original.size() - 1;
    for (int i = n; i >= 0; i--)
    {
        helper.push_back(original[i]);
    }
    return helper;
}

class GrafoPonderado
{
private:
    uint num_vertices;
    uint num_edges;
    int size = 8;
    std::vector<std::vector<VertexWeightPair>> adj;

    void criar_grafo();
    void insertion_sort(std::vector<VertexWeightPair> &vec);

    bool ja_existe_edge(Vertex u, Vertex v);

public:
    GrafoPonderado(uint size = 8);
    ~GrafoPonderado();

    void add_edge(Vertex u, Vertex v, Weight w);
    std::vector<VertexWeightPair> get_adj(Vertex u) const;
    uint get_edges() const { return num_edges; }
    uint get_vertices() const { return num_vertices; }
    Weight get_peso(Vertex u, Vertex v) const { return adj[u][v].second; }
    int get_tamanho() const { return size; }
    const std::vector<std::vector<VertexWeightPair>> &get_adj() const { return adj; }
};

GrafoPonderado::GrafoPonderado(uint size) : size(size)
{
    num_vertices = size * size;
    num_edges = 0;
    adj.resize(num_vertices);
}

GrafoPonderado::~GrafoPonderado()
{
}

bool GrafoPonderado::ja_existe_edge(Vertex u, Vertex v)
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

void GrafoPonderado::add_edge(Vertex u, Vertex v, Weight w)
{
    if (u >= num_vertices || v >= num_vertices || u == v)
    {
        throw std::invalid_argument("Valores invalidos");
    }

    if (ja_existe_edge(u, v))
    {
        throw std::invalid_argument("Aresta duplicada");
    }

    VertexWeightPair par = std::make_pair(v, w);
    adj[u].push_back(par);

    par = std::make_pair(u, w);
    adj[v].push_back(par);

    num_edges += 1;
}

std::vector<VertexWeightPair> GrafoPonderado::get_adj(Vertex u) const
{
    if (u >= num_vertices)
    {
        throw std::invalid_argument("Valores invalidos");
    }
    return adj[u];
}

void GrafoPonderado::insertion_sort(std::vector<VertexWeightPair> &vec)
{
    if (vec.size() < 2)
    {
        return;
    }

    for (Vertex i = 1; i < vec.size(); i++)
    {
        VertexWeightPair current = vec[i];
        int j = i - 1;

        while (j >= 0 && vec[j].first > current.first)
        {
            vec[j + 1] = vec[j];
            j = j - 1;
        }
        vec[j + 1] = current;
    }
}


GrafoPonderado tabuleiro(uint tam)
{
    GrafoPonderado grafo = GrafoPonderado(tam);

    for (uint i = 0; i < tam * tam; i++)
    {
        std::vector<uint> movimentos = algoritmo_movimentos(i, tam);
        for (uint pos : movimentos)
        {
            try
            {
                grafo.add_edge(i, pos, calculate_edge_weight(i, pos, tam));
            }
            catch (const std::invalid_argument &e)
            {
            }
        }
    }
    return grafo;
}

template <typename T>
class MinHeap
{
private:
    std::vector<T> heap;
    int getParent(int index) { return (index - 1) / 2; }
    int getLeft(int index) { return 2 * index + 1; }
    int getRight(int index) { return 2 * index + 2; }

public:
    void heapify(int index);

    MinHeap() = default;
    void insert(const T &value);
    T extractMin();
    bool isEmpty() const { return heap.empty(); }
};

template <typename T>
void MinHeap<T>::heapify(int index)
{
    int smallest = index;
    int left = getLeft(index);
    int right = getRight(index);
    int heapsize = heap.size();

    if (left < heapsize && heap[left] < heap[smallest])
    {
        smallest = left;
    }

    if (right < heapsize && heap[right] < heap[smallest])
    {
        smallest = right;
    }

    if (smallest != index)
    {
        std::swap(heap[index], heap[smallest]);
        heapify(smallest);
    }
}

template <typename T>
void MinHeap<T>::insert(const T &value)
{
    heap.push_back(value);
    int index = heap.size() - 1;

    while (index != 0 && heap[index] < heap[getParent(index)])
    {
        std::swap(heap[index], heap[getParent(index)]); // vou usar swap pq e mais facil
        index = getParent(index);
    }
}


template <typename T>
T MinHeap<T>::extractMin()
{
    if (heap.empty())
    {
        throw std::runtime_error("Heap ta vaziorrr");
    }

    T minValue = heap[0];

    heap[0] = heap.back();
    heap.pop_back();

    if (!heap.empty())
    {
        heapify(0);
    }

    return minValue;
}

struct DijkstraResult
{
    std::vector<Weight> distancias;
    std::vector<int> predecessores;
    std::vector<int> movimentos;
};

DijkstraResult dijkstra(const GrafoPonderado &grafo, Vertex origem)
{
    uint num_vertices = grafo.get_vertices();
    DijkstraResult resultado;
    resultado.distancias.assign(num_vertices, std::numeric_limits<Weight>::infinity());
    resultado.predecessores.assign(num_vertices, -1);
    resultado.movimentos.assign(num_vertices, std::numeric_limits<int>::max());

    resultado.distancias[origem] = 0;
    resultado.movimentos[origem] = 0;

    MinHeap<std::pair<Weight, Vertex>> fila_prioridade;
    fila_prioridade.insert({0.0f, origem});

    std::vector<bool> visitado(num_vertices, false);

    while (!fila_prioridade.isEmpty())
    {
        Vertex u = fila_prioridade.extractMin().second;

        if (visitado[u])
            continue;
        
        visitado[u] = true;
    
        for (const auto &vizinho : grafo.get_adj(u))
        {
            Vertex v = vizinho.first;
            Weight peso_aresta = vizinho.second;

            Weight nova_distancia = resultado.distancias[u] + peso_aresta; // salvando a nova distancia
            int novos_movimentos = resultado.movimentos[u] + 1; // salvando os novos movimentos

            // verficando se ja foi visitado
            if (!visitado[v])
            {
                // caminho com peso menor
                if (nova_distancia < resultado.distancias[v])
                {
                    resultado.distancias[v] = nova_distancia;
                    resultado.predecessores[v] = u;
                    resultado.movimentos[v] = novos_movimentos; 
                    fila_prioridade.insert({resultado.distancias[v], v});
                }

                // caminho com O MESMO PESO.
                else if (nova_distancia == resultado.distancias[v])
                {
                    // compara os movimentos
                    if (novos_movimentos < resultado.movimentos[v])
                    {
                        resultado.predecessores[v] = u;
                        resultado.movimentos[v] = novos_movimentos; 
                        fila_prioridade.insert({resultado.distancias[v], v});
                    }
                }
            }
        }
    }
    return resultado;
}

std::vector<Vertex> reconstruir_caminho(Vertex origem, Vertex destino, const std::vector<int> &predecessores)
{
    std::vector<Vertex> caminho;
    for (Vertex atual = destino; int(atual) != -1; atual = predecessores[atual])
    {
        caminho.push_back(atual);
        if (atual == origem)
            break;
    }
    caminho = reverse(caminho);
    if (caminho.empty() || caminho[0] != origem)
        return {};
    return caminho;
}

std::vector<Exercito> guerra(const GrafoPonderado &grafo, std::vector<Exercito> &participantes, Vertex hunnus, std::vector<Vertex> tormentas)
{
    std::vector<Casa> casas;

    for (Exercito &exercito : participantes)
    {
        DijkstraResult resultado = dijkstra(grafo, exercito.posicao);
        std::vector<Vertex> auxiliar = reconstruir_caminho(exercito.posicao, hunnus, resultado.predecessores);
        exercito.peso_da_rota += resultado.distancias[hunnus];
        for (Vertex i : auxiliar)
        {
            exercito.rota.push_back(i);
        }
        if (!exercito.rota.empty())
        {
            exercito.rota.pop_front();
        }
    }

    uint n = grafo.get_vertices();
    casas.resize(n);

    for (Vertex i = 0; i < n; i++)
    {
        casas[i] = Casa();
        casas[i].tormenta = sequential_check<uint>(i, tormentas);
        for (const auto& exercito : participantes)
        {
            if (exercito.posicao == i)
            {
                casas[i].exercitos.push_back(exercito);
            }
        }
    }

    bool finalizar_nessa_rodada = false;
    while (!finalizar_nessa_rodada)
    {
        for (Exercito &exercito : participantes)
        {
            if (casas[exercito.posicao].tormenta)
            {
                exercito.pode_mover = false;
                for (const auto& outro : casas[exercito.posicao].exercitos)
                {
                    if (outro.cor != exercito.cor)
                    {
                        exercito.pode_mover = true;
                        break;
                    }
                }
                casas[exercito.posicao].tormenta = false;
            }

            if (exercito.pode_mover && !exercito.rota.empty())
            {
                std::vector<Exercito> &proxima_casa_exercitos = casas[exercito.rota.front()].exercitos;
                for (const auto& outro : proxima_casa_exercitos)
                {
                    if (sequential_check<std::string>(outro.cor, exercito.inimigos))
                    {
                        exercito.pode_mover = false;
                        break; 
                    }
                }
            }
            
            if (exercito.pode_mover && !exercito.rota.empty())
            {
                std::vector<Exercito> &casa_atual_exercitos = casas[exercito.posicao].exercitos;
                for (auto it = casa_atual_exercitos.begin(); it != casa_atual_exercitos.end(); ++it)
                {
                    if (it->cor == exercito.cor)
                    {
                        casa_atual_exercitos.erase(it);
                        break;
                    }
                }

                exercito.posicao = exercito.rota.front();
                exercito.rota.pop_front();
                casas[exercito.posicao].exercitos.push_back(exercito);
                exercito.movimentos++;
            }
            else
            {
                exercito.pode_mover = true;
            }

            if (!casas[hunnus].exercitos.empty())
            {
                finalizar_nessa_rodada = true;
            }
        }
    }

    return casas[hunnus].exercitos;
};

int notation_to_int(const std::string& notation, int size)
{
    char letra = notation[0]; // 'k'
    int numero = std::stoi(notation.substr(1)); // '10'

    int coluna = letra - 'a'; // 10
    int linha = size - numero; // size - 10

    return linha * size + coluna;
}

std::vector<std::string> separar_palavras(const std::string& linha) {
    std::vector<std::string> palavras;
    std::string atual;

    for (char c : linha) {
        if (c == ' ') {
            if (!atual.empty()) {
                palavras.push_back(atual);
                atual.clear();
            }
        } else {
            atual += c;
        }
    }

    if (!atual.empty()) {
        palavras.push_back(atual);
    }

    return palavras;
}


int main()
{
    // tamanho e numero dos exercitos
    int tamanho, num_exercitos;
    std::cin >> tamanho;
    std::cin >> num_exercitos;

    std::vector<Exercito> exercitos;

    std::cin.ignore(); // faz a limpeza
    
    // adicionando os exercitos
    for (int i = 0; i < num_exercitos; ++i)
    {
        std::string linha;
        std::getline(std::cin, linha); // le a linha gigante
        std::vector<std::string> texto = separar_palavras(linha); // separa em um array de palavras

        std::string cor_exercito = texto[0]; // cor
        std::string pos_notacao = texto[1]; // posicao
        std::vector<std::string> inimigos_exercito(texto.begin() + 2, texto.end()); // a partir da posicao 2 inimigos
        
        Vertex pos_vertice = notation_to_int(pos_notacao, tamanho);
        exercitos.push_back({cor_exercito, inimigos_exercito, pos_vertice});
    }

    std::string hunnus_notacao;
    std::cin >> hunnus_notacao;
    Vertex hunnus_pos = notation_to_int(hunnus_notacao, tamanho);

    int num_tormentas;
    std::cin >> num_tormentas;
    std::vector<Vertex> tormentas;
    for (int i = 0; i < num_tormentas; ++i)
    {
        std::string tormenta_notacao;
        std::cin >> tormenta_notacao;
        tormentas.push_back(notation_to_int(tormenta_notacao, tamanho));
    }

    GrafoPonderado mapa = tabuleiro(tamanho);
    std::vector<Exercito> vencedores = guerra(mapa, exercitos, hunnus_pos, tormentas);

    // deixando ordem alfabetica so pra garantir a ordem de print
    int n = vencedores.size(); // se tem varios vencedores
    if (n > 1) { 
    for (int i = 0; i < n - 1; ++i) {
        for (int j = 0; j < n - i - 1; ++j) {
            if (vencedores[j].cor > vencedores[j+1].cor) {
                std::swap(vencedores[j], vencedores[j+1]); // swap pela preguica
            }
        }
    }
}

    for (size_t i = 0; i < vencedores.size(); ++i)
    {
        const auto &vencedor = vencedores[i];
        std::cout << vencedor.cor << " " << vencedor.movimentos + 1 << " " << vencedor.peso_da_rota;

        if (i < vencedores.size() - 1)
        {
            std::cout << " ";
        }
    }
    std::cout << std::endl;

    return 0;
}

/*


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