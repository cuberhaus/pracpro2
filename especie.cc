#include <iostream>
#include "especie.hh"

especie::especie() {
    string id;
    string gen;
}
especie::especie(string & id const, string & gen const) {
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

