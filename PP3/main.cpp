#include <iostream>
#include <list>
#include <vector>
#include <cstdlib>
#include <limits>
#include <iomanip>
#include <string>
#include <cmath>

// -x-x-x-x- Classe Tabuleiro baseada num grafo lista de adjacencias com peso em que geral se movimenta igual cavalo -x-x-x-x-
// Ainda falta implementar o peso nas arestas, faço depois de dormir
// E ajustar pro tabuleiro criar certinho sem bugs
// vo dar um soninho agora
using Vertex = unsigned int;
using uint = unsigned int;
using Weight = float;
using VertexWeightPair = std::pair<Vertex, Weight>;


// ----------------------------- STRUCTS E FUNCOES GERAIS PUBLICAS ------------------------
// - Struct para representar os exercitos
struct Exercito {
    std::string cor; // cores
    std::vector<std::string> inimigos;
    Vertex posicao;
    std::list<Vertex> rota = {}; //usaria pilha mas nao foi permitido a biblioteca e se eu copiar uma pilha aqui o codigo vai ficar ainda mais gigante
    Weight peso_da_rota = 0;
    bool pode_mover = true;
    uint movimentos = 0;
};

struct Casa {
    std::vector<Exercito> exercitos;
    bool tormenta;
};

std::pair<char, char> int_to_chess (Vertex u, int size) {
    //retorna { {letra u, numero u}, {letra v, numero v} }
    // transformando o tamanho em numero ASCII
    char tamanho_tabuleiro = '0' + size; // '0' + 8 = '8' (valor ASCII de '8')

    // numero unico para notacao xadrez
    // formula inversa -> letra = (indice % 8) + 'A' e numero = 8 - (indice / 8)

    char letra_u = (u % size) + 'a'; // coluna = sempre letra -> resto da divisao pelo tamanho do tabuleiro + valor ascii de 'a'
    char numero_u = tamanho_tabuleiro - (u / size);     // linha = sempre numerico ->

    return {letra_u, numero_u};
}

std::string int_to_notation (Vertex u, int size) {
    std::pair<char, char> par = int_to_chess(u, size);
    std::string output = "  ";
    output[0] = par.first;
    output[1] = par.second;
    return output;
}

template <typename T>
bool sequential_check (T& target, const std::vector<T>& vector) {
    for (T item : vector) {
        if (item == target) {
            return true;
        }
    }
    return false;
}

std::vector<uint> algoritmo_movimentos (Vertex u, int size) {
    int u1 = u / size; //coluna
    int u2 = u % size; //linha
    std::vector<std::pair<int, int>> movimentos = {{1, 2}, {1, -2}, {-1, 2}, {-1, -2}, {2, 1}, {2, -1}, {-2, 1}, {-2, -1}};
    std::vector<Vertex> posfinal;

    for (std::pair<int, int> movimento : movimentos) {
        int v1 = u1 + movimento.first;
        int v2 = u2 + movimento.second;
        //std::cout << "de coluna "<<u1+1<<" linha "<<u2+1<<" para coluna "<<v1+1<<" linha "<<v2+1<<"\n";
        if (v1 < size && v2 < size && v1 >= 0 && v2 >= 0) {
            posfinal.push_back((v1*size) + (v2));
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

    //std::cout << "Peso de " << letra_u << numero_u << " e " << letra_v << numero_v << ": " << peso << " Calculado com: " << static_cast<int>(letra_u) << " * " << (numero_u - '0') << " + " << static_cast<int>(letra_v) << " * " << (numero_v - '0') << " mod 19" << " = " << peso << std::endl;
    return peso;
}

//existe uma biblioteca com essa funcao mas nao tava nas especificadas permitidas no documento do enunciado
template <typename T>
std::vector<T> reverse(std::vector<T> original) {
    std::vector<T> helper;
    int n = original.size() - 1;
    for (int i = n; i >= 0; i--) {
        helper.push_back(original[i]);
    }
    return helper;
}

// ----------------------------------------- CLASSE GRAFO PONDERADO -------------------------------

class GrafoPonderado {
private:
    uint num_vertices;
    uint num_edges;
    int size = 8; // tamanho padrao 8x8
    std::vector<std::vector<VertexWeightPair>> adj;
    

    void criar_grafo(); // ele usa o size privado
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
    const std::vector<std::vector<VertexWeightPair>>& get_adj() const { return adj; }
};

GrafoPonderado::GrafoPonderado(uint size) : size(size)
{
    num_vertices = size * size; // Exemplo: 8 x 8 = 64 vertices
    num_edges = 0;
    adj.resize(num_vertices);
}

GrafoPonderado::~GrafoPonderado()
{
    //automatico
}

// extra: ve se ja existe
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

std::vector<VertexWeightPair> GrafoPonderado::get_adj(Vertex u) const {
    if (u >= num_vertices)
    {
        throw std::invalid_argument("Valores invalidos");
    }
    return adj[u];
}

/*
implementar na classe tabuleiro
void GrafoPonderado::criar_grafo()
{
    for (int coluna = 0; coluna < size; ++coluna)
    {
        for (int linha = 0; linha < size; ++linha)
        {
            int vertice_origem = linha * size + coluna; // Convertendo (linha, coluna) para vertice 0-63

            // Adicionando arestas para todos os possiveis movimentos dos exercitos com base em cavalo
            for (const auto &mov : movimentos)
            {
                int nova_coluna = coluna + mov.first;
                int nova_linha = linha + mov.second;

                // Verificando se a nova posicao esta dentro dos limites do tabuleiro
                if (nova_linha >= 0 && nova_linha < size && nova_coluna >= 0 && nova_coluna < size)
                {
                    int vertice_destino = nova_linha * size + nova_coluna;

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
*/
void GrafoPonderado::insertion_sort(std::vector<VertexWeightPair> &vec)
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


//   ---------------------------------------------------- TABULEIRO -------------------------


GrafoPonderado tabuleiro(uint tam) {
    GrafoPonderado grafo = GrafoPonderado(tam);
    
    for (uint i = 0; i < tam*tam; i++) {
        std::vector<uint> movimentos = algoritmo_movimentos(i, tam);
        for (uint pos : movimentos) {
            //std::cout << "adicionando vertice conectando de " << int_to_notation(i, tam) << " a " << int_to_notation(pos, tam) << std::endl;
            try {
                grafo.add_edge(i, pos, calculate_edge_weight(i, pos, tam));
            } catch (const std::invalid_argument& e) {}
        }
    }
    return grafo;
}

// --------------------------------------- HEAP ---------------------------------------------------

// Nao me decidi sobre o tipo, entao deixarei generico
// Pai = i/2
// Filho esquerdo = 2i
// filho direito = 2i + 1
template <typename T>
class MinHeap
{
private:
    std::vector<T> heap;


    int getParent(int index) { return std::floor((index) / 2); }
    int getLeft(int index) { return 2 * index; }
    int getRight(int index) { return 2 * index + 1; }

public:

    void heapify(int index); // faz as trocas a partir de um indice
    void full_heapify();  // faz as trocas em todo o heap

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

    int heapsize = heap.size();

    // verifica se o filho esq exite e e menor que o pai
    if (left < heapsize && heap[left] < heap[smallest])
    {
        smallest = left;
    }
    else
    {
        smallest = index;
    }

    // verifica se o filho dir exite e e menor que o pai
    if (right < heapsize && heap[right] < heap[smallest])
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

    for (int i = 0; i < int(heap.size()); i++){
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

template <typename T>
T MinHeap<T>::extractMin() {
    T minValue = heap[0];
    erase(minValue);
    return minValue;
}

// DEBUG DO TABULEIRO
void printAdjacencyList(GrafoPonderado graph)
{
    std::cout << "num_vertices: " << graph.get_vertices() << std::endl;
    std::cout << "num_edges: " << graph.get_edges() << std::endl;

    int tamanho = graph.get_tamanho();

    for (uint u = 0; u < graph.get_vertices(); ++u)
    {
        std::cout << int_to_chess(u, tamanho).first << int_to_chess(u, tamanho).second << ": ";
        std::vector<VertexWeightPair> list = graph.get_adj(u);
        for (VertexWeightPair v : list)
        {
            std::cout << "(" << int_to_notation(v.first, tamanho) << ", " << v.second << "), ";
        }
        std::cout << std::endl;
    }
}

// - x - x - Struct de algoritmo de caminho minimo Dijkstra - x - x -
// n sei por que motivo seria um struct
// nao vou usar isso
/*
struct Dijkstra{
    Exercito* exercito;
    // atributos do algoritmo de dijistra
    // nao sei como escreve o nome dele
};*/

/*
template <typename T>
void BuscaDijkstra(GrafoPonderado grafo, Vertex start, Vertex end) {
    MinHeap<T> minheap = MinHeap<T>();
    for (VertexWeightPair par : grafo.get_adj()) {
        
    }
}
*/


// ----------------------------------------- ALGORITMO DE DIJKSTRA --------------------------------

struct DijkstraResult {
    std::vector<Weight> distancias;
    std::vector<int> predecessores;
};


DijkstraResult dijkstra(const GrafoPonderado& grafo, Vertex origem) {
    uint num_vertices = grafo.get_vertices();
    DijkstraResult resultado;
    resultado.distancias.assign(num_vertices, std::numeric_limits<Weight>::infinity());
    resultado.predecessores.assign(num_vertices, -1);

    resultado.distancias[origem] = 0;

    MinHeap<VertexWeightPair> fila_prioridade;
    fila_prioridade.insert({0.0f, origem});
    
    std::vector<bool> visitado(num_vertices, false);

    //std::cout << "se essa mensagem nao aparecer tem erro antes do loop while" << std::endl;

    //int vezes = 1;
    while (!fila_prioridade.isEmpty()) {
        //std::cout << "iteracao numero "<<vezes<<" do loop while de dijkstra iniciada" << std::endl;
        //vezes++;
        Vertex u = fila_prioridade.extractMin().second;

        if (visitado[u]) continue;
        visitado[u] = true;
        grafo.get_adj(0);
        //std::cout << "agora o loop for vai comecar para essa iteracao do loop while, e o index do adj vai ser: " << u << " e o get_adj() desse index resulta em um vetor de tamanho " << grafo.get_adj(int(u)).size() << std::endl;
        //int vezes2 = 1;
        for (const auto& vizinho : grafo.get_adj(u)) {
            //std::cout << "agora o loop numero "<<vezes2<<" do loop while numero "<<vezes2<< std::endl;
            //vezes2++;
            Vertex v = vizinho.first;
            Weight peso_aresta = vizinho.second;

            if (!visitado[v] && resultado.distancias[u] + peso_aresta < resultado.distancias[v]) {
                //std::cout<<"condicional if do loop for foi acionado"<<std::endl;
                resultado.distancias[v] = resultado.distancias[u] + peso_aresta;
                resultado.predecessores[v] = u;
                fila_prioridade.insert({resultado.distancias[v], v});
            }
        }
    }
    return resultado;
}

std::vector<Vertex> reconstruir_caminho(Vertex origem, Vertex destino, const std::vector<int>& predecessores) {
    //std::cout << "reconstruindo caminho " <<origem<<"-"<<destino << std::endl;
    std::vector<Vertex> caminho;
    for (Vertex atual = destino; int(atual) != -1; atual = predecessores[atual]) {
        //std::cout << "atual: " << atual << std::endl;
        caminho.push_back(atual);
        if (atual == origem) break;
    }
    //std::cout << "fim do loop" << std::endl;
    caminho = reverse(caminho);
    //std::cout << caminho[0] <<" "<< origem << std::endl;
    if (caminho.empty() || caminho[0] != origem) return {};
    //std::cout << "fim da reconstrucao" << std::endl;
    return caminho;
}

//------------------------------------ EXECUCAO FINAL DA GUERRA DOS REINOS ------------------------------

//args: grafo: grafo, participantes: lista de exercitos participando, hunnus: posicao vertex do destino final, tormentas: lista de posicoes vertex das tormentas, modo_debug e debug_mapa: segredo :)
std::vector<Exercito> guerra (const GrafoPonderado& grafo, std::vector<Exercito>& participantes, Vertex hunnus, std::vector<Vertex> tormentas, bool modo_debug = false, bool debug_mapa = false) {

    std::vector<Casa> casas;

    //calcular rota de cada exercito
    for (Exercito& exercito : participantes) {
        if (modo_debug) {std::cout << "\npreparado o exercito "<<exercito.cor<< std::endl;}
        DijkstraResult resultado = dijkstra(grafo, exercito.posicao);
        std::vector<Vertex> auxiliar = reconstruir_caminho(exercito.posicao, hunnus, resultado.predecessores);
        exercito.peso_da_rota += resultado.distancias[hunnus];
        if (modo_debug) {std::cout <<"a rota ficou com tamanho "<< auxiliar.size() << " e rota de custo total " << exercito.peso_da_rota << "\nrota do exercito:" << std::endl;}
        for (Vertex i : auxiliar) {
            if (modo_debug) {std::cout << int_to_notation(i, grafo.get_tamanho()) << " ";}
            exercito.rota.push_back(i);
        }

        exercito.rota.pop_front(); //nao precisa manter na rota a casa em que o exercito ja ta
    }

    //preparar todas as casas
    
    uint n = grafo.get_vertices();
//relacao entre lista de casas e mapa (grafo): mesmo tamanho correspondencia de index. ex: casas[40] corresponde a grafo.get_adj()[40]
    casas.resize(n);
    if (modo_debug) {std::cout << "\narrumando as casas" << std::endl;}
    for (Vertex i = 0; i < n; i++) {
        casas[i] = Casa();
        casas[i].tormenta = sequential_check<uint>(i, tormentas);
        for (Exercito exercito : participantes) {
            if (exercito.posicao == i) {casas[i].exercitos.push_back(exercito);}
        }
    }

    //inicio da simulacao e controle de rodadas
    if (modo_debug) {std::cout << "\nobjetivo: alcançar " << int_to_notation(hunnus, grafo.get_tamanho()) << std::endl << std::endl;}
    bool finalizar_nessa_rodada = false;
    while (!finalizar_nessa_rodada) {
        if (modo_debug) {std::cout << "inicia rodada" << std::endl;}
        //simulacao da rodada de cada exercito
        for (Exercito& exercito : participantes) {
            if (modo_debug) {std::cout << "vez de " << exercito.cor << std::endl;}
            //antes de tentar se mover, verifica se tem tormenta onde ele ta, se sim, e se nao houver alianca, passa a rodada removendo ela
            if (casas[exercito.posicao].tormenta) {
                if (modo_debug) {std::cout << exercito.cor << " iniciou a rodada enfrentando uma tormenta" << std::endl;}
                exercito.pode_mover = false;
                for (Exercito outro : casas[exercito.posicao].exercitos) {
                    if (outro.cor != exercito.cor) {
                        exercito.pode_mover = true;
                        break;
                    }
                }
                if (exercito.pode_mover) {
                    if (modo_debug) {std::cout << exercito.cor << " derrotou a tormenta com seus aliados e pode prosseguir" << std::endl;}
                } else {if (modo_debug) {std::cout << exercito.cor << " perdeu a vez lutando com tormenta" << std::endl;}}
                casas[exercito.posicao].tormenta = false;
            }
            //verificando se o exercito vai tentar se mover pra onde ja tem inimigos
            if (exercito.pode_mover && !exercito.rota.empty()) {
                std::vector<Exercito>& helper = casas[exercito.rota.front()].exercitos; //conteudo de ponteiro para lista de exercitos da proxima posicao na rota
                
                if (modo_debug) {std::cout << "proxima casa na rota tem "<<helper.size()<< " exercitos nela, verificando quais sao eles" << std::endl;}
                for (Exercito outro : helper) {
                    if (modo_debug) {std::cout << outro.cor << " ja tava la ";}
                    if (sequential_check<std::string>(outro.cor, exercito.inimigos)) {
                        exercito.pode_mover = false;
                        if (modo_debug) {std::cout << "e agora "<<exercito.cor<<" perde uma rodada" << std::endl;}
                    } else { if (modo_debug) {std::cout << "e agora criaram uma alianca" << std::endl;} }
                }
                if (modo_debug && helper.empty()) {std::cout << "tava vazio que sorte" << std::endl;}
            }
            //movimento, se possivel
            if (exercito.pode_mover && !exercito.rota.empty()) {
                std::vector<Exercito>& helper = casas[exercito.posicao].exercitos; //conteudo de ponteiro pra bla bla bla mesma coisa
                if (modo_debug) {std::cout << exercito.cor << " se moveu de "<< int_to_notation(exercito.posicao, grafo.get_tamanho());}
                
                //remover o exercito da lista de exercitos da casa de onde ta saindo
                int n = helper.size();
                for (int i = 0; i < n; i++) {
                    if (exercito.cor == helper[i].cor) {
                        helper.erase(helper.begin() + i);
                        break;
                    }
                }

                //atualizando a nova posicao do exercito, removendo a anterior da lista, e se adicionando na lista de exercitos da casa nova
                exercito.posicao = exercito.rota.front();
                exercito.rota.pop_front();
                casas[exercito.posicao].exercitos.push_back(exercito);
                exercito.movimentos++;

                if (modo_debug) {std::cout << " para "<<int_to_notation(exercito.posicao, grafo.get_tamanho()) << std::endl;}

                
            } else { //se tiver acabado de nao se mover porque perdeu a vez na rodada passada, recupera os movimentos
                if (modo_debug) {std::cout << "exercito "<<exercito.cor<<" nao se moveu mas na proxima ja pode" << std::endl;}
                exercito.pode_mover = true;
            }

            //verifica se acabou de chegar no vilao, se sim, espera os outros exercitos terminarem de se mover antes de terminar a simulacao
            finalizar_nessa_rodada = (!casas[hunnus].exercitos.empty());
            if (modo_debug && finalizar_nessa_rodada) {std::cout << "temos um vencedor, entao essa foi a ultima rodada de "<<exercito.cor<< std::endl;}

            //interface opcional (desligada no juiz online)
            if (modo_debug) {
                int tam = grafo.get_tamanho();
                std::string mapa = "";
                std::string header = "  ";
                for (int i = 0; i < tam; i++) {
                    std::string headerpart = "               ";
                    headerpart[7] = 'A' + i;
                    header += headerpart;
                } header += "\n";
                for (int i = 0; i < tam; i++) {
                    mapa += "  "; for (int j = 0; j < tam; j++) {mapa += "  -----------  ";} mapa += "\n";
                    mapa += '0'+tam-i; mapa += " "; for (int j = 0; j < tam; j++) {mapa += " |           | ";} mapa += "\n";
                    mapa += "  "; for (int j = 0; j < tam; j++) {mapa += "  -----------  ";} mapa += "\n\n";
                }

                int basetop = 15*tam + 2;
                for (int i = 0; i < tam*tam; i++) {
                    int pos = ( basetop + ((i%tam) * 15) + 5 );
                    for (int j = 0; j < i / tam; j++) {
                        pos += (45 * tam + 10);
                    }
                    std::string name = "";
                    if (i == int(hunnus)) {
                        name = "hunnus";
                    }
                    if (!casas[i].exercitos.empty()) {
                        if (casas[i].exercitos.size() > 1) {name = "alianca";} else {name = casas[i].exercitos[0].cor;}
                    }
                    mapa.replace(pos + 5 - name.size()/2, name.size(), name);
                    if (casas[i].tormenta) {
                        mapa.replace(pos - 2 - (15 * tam), 8, "tormenta");
                    }
                }

                if (debug_mapa) {std::cout << "mapa ate agora:\n" << header << mapa << std::endl;}

                std::cout << "fim da vez de "<<exercito.cor<<", envia qualquer input nao vazio pra continuar" << std::endl;
                std::string variavelinutil;
                std::cin >> variavelinutil;
                std::cout << variavelinutil << "\n\n\n";
            }
        }
    }

    //fim do jogo, retorno dos vencedores
    if (modo_debug) {
        std::cout << "\n\nacabou!!1! vencedores:" << std::endl;
        for (Exercito exercito : casas[hunnus].exercitos) {
            std::cout << exercito.cor << " ";
        }
    }

    //retorna lista dos structs Exercito que tavam no destino quando o jogo acabou (aka vencedores)
    //so vai precisar de um pouco de adaptacao pra funcionar no main do jeito que o exercicio pede
    return casas[hunnus].exercitos;
};


// ----------------------------------------------- o main ---------------------------------

int main()
{
    int tamanho = 5;
    GrafoPonderado mapa = tabuleiro(tamanho);
    std::vector<Exercito> exercitos;
    exercitos.push_back(Exercito{.cor = "azul", .inimigos = std::vector<std::string>{"amarelo", "verde", "vermelho"}, .posicao = 12});
    exercitos.push_back(Exercito{.cor = "verde", .inimigos = std::vector<std::string>{"azul", "amarelo", "vermelho"}, .posicao = 0});
    exercitos.push_back(Exercito{.cor = "amarelo", .inimigos = std::vector<std::string>{"azul", "vermelho"}, .posicao = 1});
    exercitos.push_back(Exercito{.cor = "vermelho", .inimigos = std::vector<std::string>{"azul", "amarelo"}, .posicao = 5});

    //printAdjacencyList(mapa);
    
    exercitos = guerra(mapa, exercitos, 24, std::vector<uint>{8, 7, 23, 2, 16}, true, true);

    for (Exercito exercito : exercitos) {
        std::cout << exercito.cor << " " << exercito.movimentos << " " << exercito.peso_da_rota << " ";
    }
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



*/
