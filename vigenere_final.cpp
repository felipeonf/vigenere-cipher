#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>

using namespace std;

// Função para remover sinais de pontuação e espaços de uma string
string removerPontuacaoEEspacos(string texto) {
    string textoLimpo = "";
    for (char c : texto) {
        if (isalpha(c)) { // Verificar se o caractere é uma letra
            textoLimpo += c;
        }
    }
    return textoLimpo;
}

// Função para cifrar um texto usando a cifra de Vigenère
string cifrar(string mensagem, string chave) {
    string cifra = "";
    int k = 0;

    for (int i = 0; i < mensagem.length(); i++) {
        // Verifica se é uma letra
        if (isalpha(mensagem[i]) != 0) {
            int deslocamento = chave[k] - 'a'; // O deslocamento é um número entre 0 e 25 que representa quantos caracteres a letra deve ser deslocada

            
            if (islower(mensagem[i])) {
                cifra += (char)((mensagem[i] + deslocamento - 'a' + 26) % 26 + 'a');
            }
            
            else {
                cifra += (char)((mensagem[i] + deslocamento - 'A' + 26) % 26 + 'A');
            }
           
            k++;
            if (k == chave.length()) k = 0;
        }
        else {
            cifra += mensagem[i];
        }
    }
    return cifra;
}



// Função para estimar o tamanho da chave
int estimarTamanhoChave(string cifra) {
    int tamanhoCifra = 0;
    int tamanhoFrequencia = 0;
    int tamanhoEstimado = 0;
    vector<int> intervalos;
    vector<int> frequencia(19, 0); // 19 possíveis tamanhos de chave

    // Remove caracteres não desejados da cifra
    string cifraLimpa = removerPontuacaoEEspacos(cifra);
    tamanhoCifra = cifraLimpa.length();

    // Calcula os trigramas e frequências
    for (int i = 0; i < tamanhoCifra - 3; i++) {
        char trigrama[3] = {cifraLimpa[i], cifraLimpa[i + 1], cifraLimpa[i + 2]};
        for (int k = i + 1; k < tamanhoCifra - 2; k++) {
            if (trigrama[0] == cifraLimpa[k] && trigrama[1] == cifraLimpa[k + 1] && trigrama[2] == cifraLimpa[k + 2]) {
                if (find(intervalos.begin(), intervalos.end(), k - i) == intervalos.end()) {
                    intervalos.push_back(k - i);
                }
                break;
            }
        }
    }

    // Calcula a frequência das diferenças de intervalo
    tamanhoFrequencia = intervalos.size();
    for (int i = 0; i < tamanhoFrequencia; i++) {
        for (int k = 2; k < 20; k++) {
            if (intervalos[i] % k == 0) {
                frequencia[k - 2]++;
            }
        }
    }

    // Exibe as possíveis chaves com base nas frequências
    cout << "\nAnálise de possíveis tamanhos de chave por frequência. Escolha um tamanho de chave possível:\n";
    for (int i = 0; i < 10; i++) {
        cout << "Possíveis tamanhos de chave: " << i + 2 << " = " << frequencia[i] << endl;
    }
    cout << "\nDigite o tamanho da chave: ";
    cin >> tamanhoEstimado;

    return tamanhoEstimado;
}

// Função para realizar o ataque de força bruta
string ataque(string cifra, int tamanhoChave, int idioma) {
    int tamanhoCifra = 0;
    int y = 0;
    string chave = "";

    // Dicionários de frequência para letras em inglês e português
    map<int, double> frequenciaIngles = {
        {'a', 8.167}, {'b', 1.492}, {'c', 2.782}, {'d', 4.253}, {'e', 12.702},
        {'f', 2.228}, {'g', 2.015}, {'h', 6.094}, {'i', 6.966}, {'j', 0.153},
        {'k', 0.772}, {'l', 4.025}, {'m', 2.406}, {'n', 6.749}, {'o', 7.507},
        {'p', 1.929}, {'q', 0.095}, {'r', 5.987}, {'s', 6.327}, {'t', 9.056},
        {'u', 2.758}, {'v', 0.978}, {'w', 2.36}, {'x', 0.15}, {'y', 1.974},
        {'z', 0.074}
    };

    map<int, double> frequenciaPortugues = {
        {'a', 14.63}, {'b', 1.04}, {'c', 3.88}, {'d', 4.99}, {'e', 12.57},
        {'f', 1.02}, {'g', 1.30}, {'h', 1.28}, {'i', 6.18}, {'j', 0.4},
        {'k', 0.02}, {'l', 2.78}, {'m', 4.74}, {'n', 5.05}, {'o', 10.73},
        {'p', 2.52}, {'q', 1.20}, {'r', 6.53}, {'s', 7.81}, {'t', 4.34},
        {'u', 4.63}, {'v', 1.67}, {'w', 0.01}, {'x', 0.21}, {'y', 0.01},
        {'z', 0.47}
    };

    // Dicionário para armazenar frequências da cifra
    map<int, double> frequenciaCifra;
    for (int i = 'a'; i <= 'z'; ++i) {
        frequenciaCifra[i] = 0.0;
    }

    string cifraLimpa = removerPontuacaoEEspacos(cifra);
    tamanhoCifra = cifraLimpa.length();
    y = tamanhoCifra / tamanhoChave;
    double x = 100.0 / static_cast<double>(y);

    // Processo de decifração por força bruta
    for (int i = 0; i < tamanhoChave; ++i) {
        for (int j = 'a'; j <= 'z'; ++j) {
            frequenciaCifra[j] = 0.0;
        }

        // Calculando a frequência das letras na cifra
        for (int j = 0; j < y; ++j) {
            frequenciaCifra[static_cast<int>(cifraLimpa[(j * tamanhoChave) + i])] += x;
        }

        chave += '0';
        double minDiferenca = 999999.0;

        // Teste todas as possíveis rotações para encontrar a chave
        for (int w = 0; w < 26; ++w) {
            double diferenca = 0.0;
            int ind = 0;
            for (int z = 'a'; z <= 'z'; ++z) {
                if (z + w > 'z') {
                    ind = z + w - 26;
                } else {
                    ind = z + w;
                }
                if (idioma == 1) {
                    if (frequenciaIngles[z] > frequenciaCifra[ind]) {
                        diferenca += (frequenciaIngles[z] - frequenciaCifra[ind]);
                    } else {
                        diferenca += (frequenciaCifra[ind] - frequenciaIngles[z]);
                    }
                } else {
                    if (frequenciaPortugues[z] > frequenciaCifra[ind]) {
                        diferenca += (frequenciaPortugues[z] - frequenciaCifra[ind]);
                    } else {
                        diferenca += (frequenciaCifra[ind] - frequenciaPortugues[z]);
                    }
                }
            }

            // Atualizando a chave se uma diferença menor for encontrada
            if (diferenca < minDiferenca) {
                minDiferenca = diferenca;
                chave[chave.length() - 1] = 'a' + w;
            }
        }
    }

    // Imprima a chave encontrada
    cout << "\nChave: " << chave << endl;
    return chave;
}

string decifrar(string textoCifrado, string chave) {
    string textoDecifrado = "";
    int k = 0;

    for (int i = 0; i < textoCifrado.length(); i++) {
        // Verifica se é uma letra
        if (isalpha(textoCifrado[i])) {
            int shift = chave[k] - 'a';

            if (islower(textoCifrado[i])) {
                textoDecifrado += (char)((textoCifrado[i] - shift - 'a' + 26) % 26 + 'a');
            }
           
            else {
                textoDecifrado += (char)((textoCifrado[i] - shift - 'A' + 26) % 26 + 'A');
            }
            
            k += 1;
            if (k == chave.length())  {
                k = 0;
            }
        }
        else {
            textoDecifrado += textoCifrado[i];
        }
    }
    return textoDecifrado;
}



// Função principal
int main() {
    string texto, chave, textoCifrado, textoDecifrado;
    int opcao, idioma;

    while (true) {
        cout << "Escolha uma opção:" << endl;
        cout << "1. Cifrar um texto" << endl;
        cout << "2. Decifrar um texto" << endl;
        cout << "3. Realizar um ataque por força bruta" << endl;
        cout << "4. Sair" << endl;
        cout << "Opção: ";
        cin >> opcao;
        cin.ignore(); // Limpar o buffer do teclado

        switch (opcao) {
            case 1:
                cout << "Digite o texto a ser cifrado: ";
                getline(cin, texto);
                cout << "Digite a chave: ";
                getline(cin, chave);
                textoCifrado = cifrar(texto, chave);
                cout << "Texto cifrado: " << textoCifrado << endl;
                break;
            case 2:
                cout << "Digite o texto a ser decifrado: ";
                getline(cin, textoCifrado);
                cout << "Digite a chave: ";
                getline(cin, chave);
                texto = decifrar(textoCifrado, chave);
                cout << "Texto decifrado: " << texto << endl;
                break;
            case 3:
                cout << "Digite o texto cifrado: ";
                getline(cin, textoCifrado);
                cout << "Digite o idioma (1 para inglês, 2 para português): ";
                cin >> idioma;
                chave = ataque(textoCifrado, estimarTamanhoChave(textoCifrado), idioma);
                cout << "Chave encontrada: " << chave << endl;
                textoDecifrado = decifrar(textoCifrado, chave);
                cout << "Texto Decifrado: " << textoDecifrado << endl;
                break;
            case 4:
                cout << "Encerrando o programa." << endl;
                return 0;
            default:
                cout << "Opção inválida. Tente novamente." << endl;
                break;
        }
    }

    return 0;
}
