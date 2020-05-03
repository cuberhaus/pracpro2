#include <iostream>
#include "especie.hh"

void especie::generar_kmeros() {
    int n = gen.size();
    string kmero;
    for (int i = 0; i < n-k-1; ++i) {
        kmero = gen.substr(i,k); 
        kmeros[kmero] += 1;
    }
    /* DEBUG: imprimeix els kmeros
    for (map <string, int>::iterator it = kmeros.begin(); it != kmeros.end(); ++it) {
        cout << it->first; 
        cout << ' ';
    }
    cout << endl;
    */
}
especie::especie() {
    string id;
    string gen;
}
especie::especie(const string & id , const string & gen ) {
    this->id = id;
    this->gen = gen;
    generar_kmeros();
}

void especie::leer_especie() {
    cin >> id >> gen;
}
string especie::consultar_id() const {
    return id;
}
string especie::consultar_gen() const {
    return gen;
}

int especie::k = 0;

void especie::insertar_k(int k) {
    especie::k = k;
}

