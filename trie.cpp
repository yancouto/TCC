/* Yan Soares Couto - TCC - Algoritmos em sequências */
#include <string>
#include <map>
#include <iostream>
#include <fstream>

using std::map;
using std::string;

/*
	Representa um nó da trie
	A lista de adjacência é guardada em adj
	mrk é verdadeiro se é um nó marcado
*/
struct node {
	map<char, node*> adj;
	bool mrk;

	node() : mrk(false) {}
};

/*
	Entrada: node r (raiz da trie), string P (a ser adicionada)
	Retorno: nenhum
	Adiciona a string P à trie com raiz r
	Tempo: O(|P|.|lgE|)
*/
void add(node &r, const string &P) {
	node *v = &r;
	for(char c : P) {
		if(v->adj[c] == NULL)
			v->adj[c] = new node;
		v = v->adj[c];
	}
	v->mrk = true;
}

/*
	Entrada: node r (raiz da trie), string P (a ser consultada)
	Retorno: Tamanho do maior prefixo comum de P com alguma
	string representada na trie
	Tempo: O(|P|.|lgE|)
*/
int maior_prefixo_comum(node &r, const string &P) {
	node *v = &r;
	for(int i = 0; i < P.size(); i++) {
		if(v->adj.count(P[i]) == 0)
			return i;
		v = v->adj[P[i]];
	}
	return (int) P.size();
}

/*
	Entrada: node r (raiz da trie), string P (a ser consultada)
	Retorno: um booleano que indica se a string P está armazenada
	na trie com raiz r
	Tempo: O(|P|.|lgE|)
*/
bool ocorre(node &r, const string &P) {
	node *v = &r;
	for(char c : P) {
		if(v->adj.count(c) == 0)
			return false;
		v = v->adj[c];
	}
	return v->mrk;
}

/*
	Entrada: node r (raiz da trie)
	Retorno: número de nós da trie
	Tempo: O(tamanho da trie)
*/
int count_nodes(const node &r) {
	int count = 1;
	for(auto x : r.adj)
		count += count_nodes(*x.second);
	return count;
}

/*
	Teste dos algoritmos neste arquivo. Lê um nome de arquivo da
	entrada padrão e adiciona todas as strings (separadas por espaços)
	do arquivo em uma trie.
	Depois, lê uma série de strings e imprime para cada uma, o
	tamanho do maior prefixo comum destas strings com alguma string da
	trie, e se esta string está exatamente no arquivo.
*/
int main() {
	string filename, S;
	std::cout << "Digite o nome de um arquivo: ";
	std::getline(std::cin, filename);
	std::ifstream f(filename);
	if(!f.is_open()) {
		std::cerr << "Falha ao ler o arquivo." << std::endl;
		return 1;
	}
	node r;
	int count = 0;
	while(f >> S) {
		add(r, S);
		count++;
	}
	std::cout << "Trie construida com " << count << " palavras e " << count_nodes(r) << " nos!\n" << std::endl;
	while(true) {
		std::cout << "Digite uma string: ";
		string S;
		if(std::cin >> S) {
			std::cout << "Maior prefixo comum: " << maior_prefixo_comum(r, S) << '\n';
			std::cout << "A string " << (ocorre(r, S)? "" : "nao ") << "ocorre exatamente na trie.\n" << std::endl;
		} else return 0;
	}
}
