#include <cstdlib>
#include <stdio.h>
#include <stdlib.h>
#include <list>
#include <string>
#include <iostream>
#include <algorithm>
#include <time.h>
using namespace std;
int acesso=0;
typedef unsigned int uint32;

enum PaddingType { ESQUERDA, DIREITA };

///
/// Verifica se h� strings negativas
///
bool isNegativo(const string& op1, const string& op2) {
    if ((op1[0] == '-' && op2[0] != '-') || (op1[0] != '-' && op2[0] == '-'))
        return true;
    return false;
}

///
/// Preenche um char n vezes na string a direita ou na esquerda
///
string preencherString(const string& paraPreencher,size_t addContPreenchimento,char charPreenchimento,PaddingType tipoPreenchimento = DIREITA) {
    string s(addContPreenchimento, charPreenchimento);
    return tipoPreenchimento == ESQUERDA ? s + paraPreencher : paraPreencher + s;
}

///
/// Verifica se a string � composta de apenas '0's
///
bool isZero(const string& oper) {
    for (size_t i = 0, l = oper.size(); i < l; i++)
        if (oper[i] != '0') return false;
    return true;
}

///
/// Dadas duas strings, ambas representando n�meros,
/// retorna uma nova string que representa a soma delas
///
string incluiNumString(const string& op1, const string& op2, int base) {
    if (op2 == "") return op1;
    if (op1 == "") return op2;
    string total = "";
    size_t op1Tam = op1.size();
    size_t op2Tam = op2.size();
    size_t m = max(op1Tam, op2Tam);

    uint32 carry = 0;
    for (int i = 0; i < static_cast<int>(m); i++) {
        uint32 c0 = i < op1Tam ? op1[op1Tam - 1 - i] - '0' : 0;
        uint32 c1 = i < op2Tam ? op2[op2Tam - 1 - i] - '0' : 0;
        uint32 c = c0 + c1 + carry;
        carry = c / base;
        total += (c % base) + '0';
    }
    if (carry) total += carry + '0';
    reverse(total.begin(), total.end());
    return total;
}

///
/// Remove os zeros da esquerda do n�mero
///
string removeZeroEsquerda(const string& str) {
    // Delete '0's from the ESQUERDA
    if (str.size() > 1 && str[0] == '0') {
        for (size_t i = 0, l = str.size(); i < l; i++) {
            if (str[i] == '0')
                continue;
            return str.substr(i, l - i);
        }
    }
    return str;
}

///
/// Dadas duas strings, ambas representando n�meros, retorna uma nova string
/// que representa a subtra��o delas, para isso, assume que o primeiro par�metro
/// seja sempre maior ou igual ao segundo par�metro
///
string retiraNumString(const string& op1, const string& op2, int base) {
    if (op2 == "") return op1;

    string total = "";
    size_t op1Tam = op1.size();
    size_t op2Tam = op2.size();

    uint32 carry = 0;
    for (int i = 0; i < op1Tam; i++) {
        int c0 = op1[op1Tam - 1 - i] - '0' - carry;
        int c1 = i < op2Tam ? op2[op2Tam - 1 - i] - '0' : 0;
        carry = 0;
        if (c0 < c1) {
            carry = 1;
            c0 += base;
        }
        total += (c0 - c1) + '0';
    }
    reverse(total.begin(), total.end());
    return isZero(total) ? "0" : removeZeroEsquerda(total);
}

///
/// Retorna uma string que representa o produto do primeiro par�metro pelo
/// d�gito que o segundo par�metro representa
///
string mulIntStringByChar(const string& op1, const char op2, int base) {
    if (op2 == '0') return "0";
    if (op2 == '1') return op1;

    int carry = 0;
    int d0 = op2 - '0';
    string r = "";
    for (int j = op1.size() - 1; j >= 0; j--) {
        int d1 = op1[j] - '0';
        int d = (d0 * d1) + carry;
        carry = d / base;
        d %= base;
        r += d + '0';
    }
    if (carry) r += carry + '0';
    reverse(r.begin(), r.end());
    return r;
}

///
/// Aqui ficar a forma padr�o recursiva de multiplica��o
///
string multPadrao(const string& oper1, const string& oper2, int base) {
    //responsavel por contra os acessos recursivos
    acesso++;
    // 2 * O(n)
    if (isZero(oper1) || isZero(oper2))
        return "0";
    // 2 * O(n)
    string op1 = removeZeroEsquerda(oper1);
    string op2 = removeZeroEsquerda(oper2);
    // O(1)
    size_t l1 = op1.size();
    size_t l2 = op2.size();
    if (l1 == 1 || l2 == 1)
        // O(1)
        return mulIntStringByChar(l1==1?op2:op1,l1==1?op1[0]:op2[0],base);
    // Divide
    size_t n = max(l1, l2);
    n += (n & 1);
    // 2 * O(n)
    op1 = preencherString(op1, n - l1, '0', ESQUERDA);
    op2 = preencherString(op2, n - l2, '0', ESQUERDA);
    size_t n2 = n / 2;
    // 4 * O(n)
    string a = op1.substr(0, n2);
    string b = op1.substr(n2, n2);
    string c = op2.substr(0, n2);
    string d = op2.substr(n2, n2);
    // 2 * O(n)
    string _ab = incluiNumString(a, b, base);
    string _cd = incluiNumString(c, d, base);
    // 4 chamadas recursivas
    string ac = multPadrao(  a,  c, base);
    string bc = multPadrao(  b,  c, base);
    string bd = multPadrao(  b,  d, base);
    string ad = multPadrao(_ab,_cd, base);
    // 2 * O(n)
    ad = retiraNumString(ad, ac, base);
    ad = retiraNumString(ad, bd, base);
    // 5 * O(n)
    ac = preencherString(ac, n, '0', DIREITA);
    ad = preencherString(ad, n2, '0', DIREITA);
    string res = incluiNumString(ac, ad, base);
    res = incluiNumString(res, bd, base);
    res = removeZeroEsquerda(res);
    // O(1)
    if(isNegativo(op1,op2)) res="-"+res;
    return res;
    //Total = 4(T)+19n+3
}

///
/// Executa a multiplica��o de karatsuba nas duas cadeias de par�metros
/// (ambas representando n�meros)
///
string karatsuba(const string& oper1, const string& oper2, int base){
    //responsavel por contra os acessos recursivos
    acesso++;
    // 2 * O(n)
    if (isZero(oper1) || isZero(oper2))
        return "0";
    // 2 * O(n)
    string op1 = removeZeroEsquerda(oper1);
    string op2 = removeZeroEsquerda(oper2);
    // O(1)
    size_t l1 = op1.size();
    size_t l2 = op2.size();
    if (l1 == 1 || l2 == 1)
        // O(1)
        return mulIntStringByChar(l1==1?op2:op1,l1==1?op1[0]:op2[0],base);
    // Divide
    size_t n = max(l1, l2);
    n += (n & 1);
    // 2 * O(n)
    op1 = preencherString(op1, n - l1, '0', ESQUERDA);
    op2 = preencherString(op2, n - l2, '0', ESQUERDA);
    size_t n2 = n / 2;
    // 4 * O(n)
    string a = op1.substr(0, n2);
    string b = op1.substr(n2, n2);
    string c = op2.substr(0, n2);
    string d = op2.substr(n2, n2);
    // 2 chamadas recursivas
    string ac = karatsuba(a, c, base);
    string bd = karatsuba(b, d, base);
    // 2 * O(n)
    string _ab = incluiNumString(a, b, base);
    string _cd = incluiNumString(c, d, base);
    // Terceira chamada recursiva
    string abcd = karatsuba(_ab, _cd, base);
    // 2 * O(n)
    abcd = retiraNumString(abcd, ac, base);
    abcd = retiraNumString(abcd, bd, base);
    // 5 * O(n)
    ac = preencherString(ac, n, '0', DIREITA);
    abcd = preencherString(abcd, n2, '0', DIREITA);
    string res = incluiNumString(ac, abcd, base);
    res = incluiNumString(res, bd, base);
    res = removeZeroEsquerda(res);
    // O(1)
    if(isNegativo(op1,op2)) res="-"+res;
    return res;
    //Total = 3(T)+19n+3
}

int main(){
    printf("UFF - Pos-graduacao em Computacao\nAnalise e Sintese de Algoritmos - 2019-2.\n");
    printf("Professor Luis Antonio Brasil Kowada\nAluno Flavio Miranda de Farias\n");
    printf("Analise de metodos de multiplicacao de strings numericas.\n");
    while(1){
        fflush(stdin);
        //Para gerar multiplos testes de um mesmo valor, mudar o valor inicial
        int base,t1,t2,i,testes=1;
        //Para n�o exibir as strings, nao escolher 's'
        char repetir,exibir,temp;
        std::string s1,s2;
        printf("\nDeseja visualizar as strings? (s/n): ");
        scanf(" %c",&exibir);
        //exibir='n';//Para n�o exibir as strings em modo de teste, descomentar
        printf("Digite a Base: ");
        scanf(" %d",&base);
        printf("Tamanho da string 1: ");
        scanf(" %d",&t1);
        printf("Tamanho da string 2: ");
        scanf(" %d",&t2);
        if(exibir=='s') printf("\nString 1(char[]): ");
        for(i=0;i<t1;i++){
            temp=(rand()%base)+48;
            s1.push_back(temp);
            if(exibir=='s') printf("%c ",temp);
        }
        if(exibir=='s') printf("\n\nString 2(char[]): ");
        for(i=0;i<t2;i++){
            temp=(rand()%base)+48;
            s2.push_back(temp);
            if(exibir=='s') printf("%c ",temp);
        }
        cout << "\n---------------------------------------------------------------------";
        while(testes>0){
            cout << "\nCronometragem do algoritmo Karatsuba..." << endl;
            clock_t start = clock();
            if(exibir=='s') cout << "\nProduto= " << karatsuba(s1, s2, base)+"\n" << endl;
            else karatsuba(s1, s2, base);
            clock_t time = clock() - start;
            cout << "Tempo gasto pelo Karatsuba = " << static_cast<float>(time) / CLOCKS_PER_SEC << " segundos." << endl;
            cout << "Segundo a funcao 3(T)+19n+3, foram realizados " << acesso << " acessos recursivos." << endl;
            acesso=0;

            cout << "\nCronometragem do algoritmo de Multiplicacao Padrao..." << endl;
            start = clock();
            if(exibir=='s') cout << "\nProduto= " << multPadrao(s1, s2, base)+"\n" << endl;
            else multPadrao(s1, s2, base);
            time = clock() - start;
            cout << "Tempo gasto pela Multiplicacao Padrao = " << static_cast<float>(time) / CLOCKS_PER_SEC << " segundos." << endl;
            cout << "Segundo a funcao 4(T)+19n+3, foram realizados " << acesso << " acessos recursivos." << endl;
            acesso=0;
            testes--;
            cout << "---------------------------------------------------------------------";
        }
        printf("\nRepetir? (s/n): ");
        scanf(" %c",&repetir);
        system("cls");
        if(repetir!='s')
            return 0;
    }
}
