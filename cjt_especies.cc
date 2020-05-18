/** @file cjt_especies.cc
    @brief Código de la clase cjt_especies
*/
#include "cjt_especies.hh"

cjt_especies::cjt_especies() {
    espmap = map <string,especie>();
    distmap = map <string, map <string, double >>();
}

void cjt_especies::inicializa_clusters(cjt_clusters &clusters) {
    clusters.borrar_conjunto();
    map <string, map<string, double>>::iterator it1;
    map <string,double>::iterator it2;
    for (it1 = distmap.begin(); it1 != distmap.end(); ++it1) {
        string id1 = it1->first;
        clusters.insertar_cluster(it1->first);
        for (it2 = it1->second.begin(); it2 != it1->second.end(); ++it2) {
            string id2 = it2->first;
            double dist = it2->second;
            clusters.insertar_especie_dist(id1,id2,dist);
        }
    }
    if (espmap.size() != 0) {
        it1 = distmap.end();
        --it1;
        double d = -1;
        string s;
        clusters.insertar_especie_dist(it1->first, s, d);
    }
}

especie cjt_especies::obtener_especie(const string& id) const {
    map <string, especie>::const_iterator it = espmap.find(id);
    return it->second;
}

bool cjt_especies::existe_especie(const string &id) const {
    map<string,especie>::const_iterator it = espmap.find(id);
    if (it != espmap.end()) return true;
    return false;
}

void cjt_especies::insertar_especie(const especie &e) {
    espmap.insert(make_pair(e.consultar_id(),e));
    map<string, map<string, double>>::iterator it = distmap.begin();
    for (it = distmap.begin(); it != distmap.end(); ++it) {
        if (it->first > e.consultar_id()) {
            double dist;
            dist = especie::distancia(espmap[it->first], e);
            distmap[e.consultar_id()][it->first] = dist;
        }
        else {
            double dist;
            dist = especie::distancia(espmap[it->first], e);
            it->second[e.consultar_id()] = dist;
        }
    }
    distmap[e.consultar_id()];
}


void cjt_especies::borrar_conjunto() {
    espmap.clear();
    distmap.clear();
}

double cjt_especies::consultar_distancia(const string id1, const string id2) const {
    double x;
    if (id1 < id2) {
        x = distmap.at(id1).at(id2);
    }
    else {
        x = distmap.at(id2).at(id1);
    }
    return x;
}

void cjt_especies::eliminar_especie(const string &id) {
    map <string, especie>::iterator it = espmap.find(id);
    if (it != espmap.end()) espmap.erase(it);
    distmap.erase(id);
    map<string, map<string, double>>::iterator it2;
    for (it2 = distmap.begin(); it2 != distmap.end(); ++it2) {
        it2->second.erase(id);
    }
}

void cjt_especies::imprimir_conjunto() const {
    for(map <string, especie>::const_iterator it = espmap.begin(); it != espmap.end(); ++it) {
        cout << it->second.consultar_id() << ' ' << it->second.consultar_gen();
        cout << endl;
    }
}

void cjt_especies::imprimir_tabla_distancias() const {
    map <string, map<string, double >>::const_iterator it1;
    map<string,double>::const_iterator it2;
    for (it1 = distmap.begin(); it1 != distmap.end(); ++it1) {
        cout << it1->first << ':' ;
        for (it2 = it1->second.begin(); it2 != it1->second.end(); ++it2) {
            cout << ' ' << it2->first << " (" << it2->second << ")";
        }
        cout << endl;
    }
}
