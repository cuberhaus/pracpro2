#include <iostream>
#include "especie.hh"

especie::especie() {
    string id;
    string gen;
}
especie::especie(const string & id , const string & gen ) {
    this->id = id;
    this->gen = gen;
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

void especie::añadir_k(int k) {
    especie::k = k;
    cout << especie::k;
}

