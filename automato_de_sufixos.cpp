/* Yan Soares Couto - TCC - Algoritmos em sequências */
#include <string>
#include <map>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>

using std::map;
using std::string;
using std::vector;

/*
	Representa um nó do autômato
	f é a lista de adjacência
	fail é a a classe da falha de alguma string desta classe
	L é o tamanho da maior string nesta classe de equivalência
	mrk é uma booleana que indica se o nó é final
	count é o número de caminhos do nó até um nó final
*/
struct node {
	map<char, node*> f;

	node *fail;
	int L;
	bool mrk;
	int count;
	node(int a) : fail(NULL), L(a), mrk(false), count(0) {}
};


/*
	Entrada: string P
	Retorno: node r (raiz do autômato)
	Cria o autômato de sufixos de P
	Tempo: O(|P|.|lgE|)
*/
node* build_suffix_automaton(const string &P) {
	int count = P.size() + 1;
	node *root = new node(0);
	node *last = root;
	for(int i = 0; i < P.size(); i++) {
		node *p = last;
		last = new node(i + 1);
		while(p != NULL && p->f.count(P[i]) == 0) {
			p->f[P[i]] = last;
			p = p->fail;
		}
		if(p == NULL)
			last->fail = root;
		else {
			node *y = p->f[P[i]];
			if(y->L == p->L + 1)
				last->fail = y;
			else {
				node *x = new node(p->L + 1);
				count++;
				x->f = y->f;
				x->fail = y->fail;
				y->fail = last->fail = x;
				while(p != NULL && p->f.count(P[i]) != 0 && p->f[P[i]] == y) {
					p->f[P[i]] = x;
					p = p->fail;
				}
			}
		}
	}
	node *p = last;
	while(p != NULL) {
		p->mrk = true;
		p = p->fail;
	}
	std::cout << "Automato criado com " << count << " nos!" << std::endl;
	return root;
}

/*
	Entrada: node r (raiz do automato)
	Retorno: uma ordenação topológica reverse do automato
	Utiliza o campo count dos nós para o algoritmo
	Tempo: no total O(tamanho do autômato)
*/
void ordenacao_topologica_reversa(node *r, vector<node*> &ord) {
	r->count = 1;
	for(auto x : r->f)
		if(x.second->count == 0)
			ordenacao_topologica_reversa(x.second, ord);
	ord.push_back(r);
}

/*
	Entrada: node root (raiz do automato), vetor topr (ordenação
	         topológica reversa do autômato)
	Retorno: nenhum
	Calcula os campos count de cada nó, que guardam o número de
	caminhos deste nó a um nó final
	Tempo: O(tamanho do autômato)
*/
void processa_count(node *root, vector<node*> &topr) {
	for(int i = 0; i < topr.size(); i++) {
		topr[i]->count = 0;
		for(auto x : topr[i]->f)
			topr[i]->count += x.second->count;
		if(topr[i]->mrk)
			topr[i]->count++;
	}
}

/*
	Teste dos algoritmos neste arquivo. Lê um nome de arquivo da
	entrada padrão e cria autômato de sufixos do texto do arquivo.
	Depois, lê uma série de strings e imprime para cada uma o
	número de ocorrências desta no arquivo.
*/
int main() {
	string filename;
	std::cout << "Digite o nome de um arquivo: ";
	std::getline(std::cin, filename);
	std::ifstream f(filename);
	if(!f.is_open()) {
		std::cerr << "Falha ao ler o arquivo." << std::endl;
		return 1;
	}
	std::stringstream buffer;
	buffer << f.rdbuf();
	string S = buffer.str();
	node *root = build_suffix_automaton(S);
	vector<node*> topr;
	ordenacao_topologica_reversa(root, topr);
	processa_count(root, topr);
	std::cout << "Numero de caminhos preprocessado!\n" << std::endl;

	while(true) {
		std::cout << "Digite uma string: ";
		string P;
		if(std::cin >> P) {
			bool ok = true;
			node *p = root;
			for(char c : P) {
				if(p->f.count(c) == 0) {
					ok = false;
					break;
				}
				p = p->f[c];
			}
			int count = ok? p->count : 0;
			std::cout << "A string ocorre " << count << " vezes.\n" << std::endl;
		} else return 0;
	}
}
