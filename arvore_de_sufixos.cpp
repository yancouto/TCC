/* Yan Soares Couto - TCC - Algoritmos em sequências */
#include <string>
#include <map>
#include <iostream>
#include <fstream>
#include <sstream>

using std::map;
using std::string;

/*
	Representa um nó da trie comprimida
	f é a lista de adjacência
	l e r são as extremidades do rótulo da aresta incidente
	parent é o pai do nó
	suf é o link de sufixo do nó
	leaves é o número de folhas na subárvore do nó
*/
struct node {
	map<char, node*> f;
	int l, r;
	node *parent, *suf;
	int leaves;
	
	/* retorna o tamanho do rótulo da aresta incidente */
	int len() { return r - l + 1; }
	/* retorna o i-esimo caractere do rótulo da aresta incidente */
	char char_at(const string &P, int i) { return P[l + i]; }

	node(int a, int b, node *p) : l(a), r(b), parent(p), leaves(0) {}
};

/*
	Entrada: string P
	Saída: node *r (raiz da arvore de sufixos)
	Constrói a árvore de sufixos da string P.
	Tempo: O(|P|.|lgE|)
*/
node* build_suffix_tree(string &P) {
	P += '$';
	node *r = new node(0, -1, NULL);
	node *cn = r, *ns = NULL;
	int i = 0, cd = 0, count = 1;
	for(int j = 0; j < P.size(); j++)
		for(; i <= j; i++) {
			if(cd == cn->len() && cn->f.count(P[j]) != 0) {
				cn = cn->f[P[j]];
				cd = 0;
			}
			if(cd < cn->len() && cn->char_at(P, cd) == P[j]) {
				cd++;
				break;
			}
			if(cd == cn->len()) {
				cn->f[P[j]] = new node(j, P.size() - 1, cn);
				count++;
				if(cn != r) {
					cn = cn->suf;
					cd = cn->len();
				}
			} else {
				count += 2;
				node *mid = new node(cn->l, cn->l + cd - 1, cn->parent);
				cn->parent->f[mid->char_at(P, 0)] = mid;
				mid->f[cn->char_at(P, cd)] = cn;
				cn->parent = mid;
				cn->l += cd;
				mid->f[P[j]] = new node(j, P.size() - 1, mid);
				if(ns != NULL)
					ns->suf = mid;
				cn = mid->parent;
				int g;
				if(cn != r) {
					cn = cn->suf;
					g = j - cd;
				} else
					g = i + 1;
				while(g < j && g + cn->f[P[g]]->len() <= j) {
					cn = cn->f[P[g]];
					g += cn->len();
				}
				if(g == j) {
					ns = NULL;
					mid->suf = cn;
					cd = cn->len();
				} else {
					ns = mid;
					cn = cn->f[P[g]];
					cd = j - g;
				}
			}
		}
	std::cout << "Arvore de sufixos construida com " << count << " nos!" << std::endl;
	return r;
}

/*
	Entrada: node r
	Retorno: número de folhas na subarvore de r
	Tempo: no total, linear no tamanho da árvore
*/
int num_folhas(node &r) {
	if(r.f.empty()) r.leaves = 1;
	for(auto x : r.f)
		r.leaves += num_folhas(*x.second);
	return r.leaves;
}

/*
	Teste dos algoritmos neste arquivo. Lê um nome de arquivo da
	entrada padrão e cria a árvore de sufixos do texto do arquivo.
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
	node *r = build_suffix_tree(S);
	num_folhas(*r);
	std::cout << "Numero de folhas preprocessado!\n" << std::endl;

	while(true) {
		std::cout << "Digite uma string: ";
		string P;
		if(std::cin >> P) {
			node *cn = r;
			int cd = 0;
			bool ok = true;
			for(int i = 0; i < P.size(); i++) {
				if(cd == cn->len() && cn->f.count(P[i])) {
					cd = 0;
					cn = cn->f[P[i]];
				}
				if(cd == cn->len() || cn->char_at(S, cd) != P[i]) {
					ok = false;
					break;
				}
				cd++;
			}

			int count = ok? cn->leaves : 0;
			std::cout << "A string ocorre " << count << " vezes.\n" << std::endl;
		} else return 0;
	}
}
