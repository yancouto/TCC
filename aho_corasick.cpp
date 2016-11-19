/* Yan Soares Couto - TCC - Algoritmos em sequências */
#include <string>
#include <map>
#include <vector>
#include <iostream>
#include <sstream>
#include <fstream>

using std::map;
using std::vector;
using std::string;

/*
	Representa um nó da trie
	A lista de adjacência é guardada em adj
	mrk é verdadeiro se é um nó marcado
	fail é o link de falha
	occurrence é o link de ocorrência
	count e occs são usados para os algoritmos e guardam,
	respectivamente, o número de ocorrências e a lista
	de ocorrências.
*/
struct node {
	map<char, node*> adj;
	bool mrk;
	node *fail, *occurrence;

	int count;
	vector<int> occs;

	node() : mrk(false), fail(NULL), occurrence(NULL) {}
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
	Entrada: node r (raiz da trie)
	Retorno: nenhum
	Reseta os campos occs e count de todos os nós
	Tempo: O(tamanho da trie)
*/
void clean(node &r) {
	r.occs.clear();
	r.count = 0;
	for(auto x : r.adj)
		clean(*x.second);
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
	Entrada: node r (raiz da trie), node v, node parent (pai de v),
	         char c (caractere de parent para v)
	Retorno: nenhum
	Calcula os links de falha e ocorrência de v, dado que os links
	de parent e todos os nós com profundidade menor já foram calculados
	Tempo: Varia, mas é O(1) amortizado
*/
void auxiliary_links(node &r, node &v, node &parent, char c) {
	v.fail = parent.fail;
	while(v.fail != &r && v.fail->adj.count(c) == 0)
		v.fail = v.fail->fail;
	if(v.fail->adj.count(c) != 0)
		v.fail = v.fail->adj[c];
	if(v.fail->mrk)
		v.occurrence = v.fail;
	else
		v.occurrence = v.fail->occurrence;
}

/*
	Entrada: node r (raiz da trie)
	Retorno: uma ordenação topológica reversa da trie
	Calcula os links de falha e ocorrência de todos os nós da trie
	Tempo: O(tamanho da trie)
*/
vector<node*> preprocessamento(node &r) {
	vector<node*> Q;
	int used = 0;
	for(auto e : r.adj) {
		node *v = e.second;
		Q.push_back(v);
		v->fail = &r;
		v->occurrence = NULL;
	}
	while(used != Q.size()) {
		node *u = Q[used++];
		for(auto e : u->adj) {
			auxiliary_links(r, *e.second, *u, e.first);
			Q.push_back(e.second);
		}
	}
	return Q;
}

/*
	Entrada: node r (raiz da trie), string P
	Retorno: nenhum
	É necessário que a trie já tenha sido preprocessada.
	Calcula as ocorrências de todas strings da trie em P.
	Guarda a posição direita destas ocorrências no campo
	occs dos nós correspondentes.
	Tempo: O(|P|.|lgE| + x) onde x é o número de ocorrências
*/
void lista_ocorrencias(node &r, const string &P) {
	node *u = &r;
	for(int i = 0; i < P.size(); i++) {
		while(u != &r && u->adj.count(P[i]) == 0)
			u = u->fail;
		if(u->adj.count(P[i]) != 0)
			u = u->adj[P[i]];
		if(u->mrk)
			u->occs.push_back(i);
		node *x = u;
		while(x->occurrence != NULL) {
			x = x->occurrence;
			x->occs.push_back(i);
		}
	}
}

/*
	Entrada: node r (raiz da trie), string P
	Retorno: nenhum
	É necessário que a trie já tenha sido preprocessada.
	Calcula o número de ocorrências para cada string da trie
	em P. Guarda esse número no campo count de cada nó.
	Tempo: O(|P|.|lgE| + |tamanho da trie|)
*/
void conta_ocorrencias(node &r, vector<node*> &top, const string &P) {
	node *u = &r;
	for(int i = 0; i < P.size(); i++) {
		while(u != &r && u->adj.count(P[i]) == 0)
			u = u->fail;
		if(u->adj.count(P[i]) != 0)
			u = u->adj[P[i]];
		u->count++;
	}
	for(int i = top.size() - 1; i >= 0; i--)
		top[i]->fail->count += top[i]->count;
}

/*
	Função auxiliar usada na main. Imprime as ocorrências
	das strings da trie, ou seu número.
*/
void print_occs(const node &r, string &cur) {
	if(r.mrk && (r.count > 0 || r.occs.size() > 0)) {
		std::cout << cur << ":";
		if(r.count > 0) std::cout << ' ' << r.count;
		else
			for(int p : r.occs)
				std::cout << ' ' << p - cur.size() + 1;
		std::cout << '\n';
	}
	for(auto x : r.adj) {
		cur.push_back(x.first);
		print_occs(*x.second, cur);
		cur.pop_back();
	}
}

/*
	Teste dos algoritmos neste arquivo. Lê um nome de arquivo da
	entrada padrão e adiciona todas as strings (separadas por espaços)
	do arquivo em uma trie.
	Depois, lê o nome de um segundo arquivo e determina todas as
	ocorrências de alguma string da trie no texto do arquivo.
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
	std::cout << "Trie construida com " << count << " palavras e " << count_nodes(r) << " nos!" << std::endl;
	vector<node*> top = preprocessamento(r);
	std::cout << "Trie preprocessada!\n" << std::endl;

	while(true) {
		std::cout << "Digite o nome de um arquivo: ";
		if(!std::getline(std::cin, filename)) return 0;
		std::ifstream f2(filename);
		if(!f2.is_open()) {
			std::cerr << "Falha ao ler o arquivo." << std::endl;
			continue;
		}
		std::stringstream buffer;
		buffer << f2.rdbuf();
		string S = buffer.str();
		std::cout << "Arquivo lido!\nOperacoes:\n1 - Listar ocorrencias\n2 - Contar ocorrencias\n"
		"\nDigite a operacao desejada: ";
		int op;
		std::cin >> op;
		std::cin.ignore(1);
		clean(r);
		if(op == 1)
			lista_ocorrencias(r, S);
		else
			conta_ocorrencias(r, top, S);
		string aux = "";
		print_occs(r, aux);
		std::cout << std::endl;
	}
}
