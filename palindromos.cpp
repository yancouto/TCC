/* Yan Soares Couto - TCC - Algoritmos em sequências */
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

using std::vector;
using std::string;
using std::pair;

/*
	Entrada: string S
	Saída: vector M de tamanho |S|, em que M[i] é o alcance de i
	Tempo: O(|S|)
*/
vector<int> alcances(const string &S) {
	int k = -1;
	vector<int> M(S.size());
	for(int i = 0; i < S.size(); i++) {
		if(k != -1 && k + M[k] > i && i + M[2*k - i] < k + M[k]) // Caso (a)
			M[i] = M[2*k - i];
		else {
			int x;
			if(k != -1 && k + M[k] > i)                          // Caso (b)
				x = k + M[k] - i;
			else                                                 // Caso (c)
				x = 0;
			while(i - x > 0 && i + x < S.size() - 1 && S[i - x - 1] == S[i + x + 1])
				x++;
			M[i] = x;
			k = i;
		}
	}
	return M;
}

/*
	Entrada: string S
	Saída: string P tal que P = Fill(S), como discutido na monografia.
	A string Fill(S) é a string S com caracteres '$' inseridos entre
	todos os pares de caracteres adjacentes.
	Tempo: O(|S|)

	Observações: Utilizamos vector<char> para criar a string para garantir
	que o tempo de adicionar um caractere ao final da string atual sej O(1).
	A classe string não tem essa garantia.
*/
string fill(const string &S) {
	vector<char> tmp;
	for(int i = 0; i < S.size(); i++) {
		tmp.push_back(S[i]);
		if(i < S.size() - 1) tmp.push_back('$');
	}
	return string(tmp.begin(), tmp.end()); // transforma tmp em uma string
}

/*
	Entrada: string S
	Saída: pair<int, int> best tal que best.first é a posição de ínicio de uma
	substring palíndroma máxima, e best.second é o tamanho dessa substring.
	Se houverem vários começos possíveis, o algoritmo retorna o primeiro.
	Tempo: O(|S|)
*/
pair<int, int> maior_substring_palindroma(const string &S) {
	vector<int> M = alcances(fill(S));
	pair<int, int> best(0, 1);
	for(int i = 1; i < 2 * S.size() - 1; i++) {
		if(i % 2) { // i é centro de palíndromo par
			int alc = (M[i] + 1) / 2; // ceil(M[i] / 2)
			if(2 * alc > best.second)
				best = pair<int, int>((i - 1) / 2 - alc + 1, 2 * alc);
		} else { // i é centro de palíndromo ímpar
			int alc = M[i] / 2; // floor(M[i] / 2)
			if(2 * alc + 1 > best.second)
				best = pair<int, int>(i / 2 - alc, 2 * alc + 1);
		}
	}
	return best;
}

/*
	Teste dos algoritmos neste arquivo. Lê um nome de arquivo da entrada padrão,
	e imprime a maior substring palíndroma deste arquivo.
*/
int main() {
	string filename, S;
	std::cout << "Digite o nome de um arquivo: ";
	std::getline(std::cin, filename);
	std::ifstream f(filename);
	if(f.is_open()) {
		std::stringstream buffer;
		buffer << f.rdbuf();
		S = buffer.str();
		pair<int, int> best = maior_substring_palindroma(S);
		std::cout << "Maior Substring Palíndroma (tamanho " << best.second << "):\n";
		std::cout << S.substr(best.first, best.second) << std::endl;
	} else std::cout << "Falha ao ler o arquivo." << std::endl;
}
