#include <iostream>
#include <set>
using namespace std;
#include "especie.hh"
//#include "cluster.hh"
#include "cjt_especies.hh"

cjt_especies::cjt_especies() {
    set <especie> espset;
    map <string, map <string, double >> distmap;
}

void cjt_especies::inicializa_clusters(cjt_clusters &clusters) {
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
    it1 = distmap.end(); 
    --it1;
    double d = -1;
    string s;
    clusters.insertar_especie_dist(it1->first, s, d);
    
    //map<string, especie>::iterator it3;
    //for (it3 = espmap.begin(); != espmap.end(), ) {
    //}
     
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
    int n = distmap.size();
    for (it = distmap.begin(); it != distmap.end() and n > 0; ++it) {
        if (it->first > e.consultar_id()) {
            double dist;
            dist = especie::distancia(espmap[it->first], e);
            distmap[e.consultar_id()][it->first] = dist;
        }
        else {
            //cout << it->first << endl;
            double dist;
            dist = especie::distancia(espmap[it->first], e);
            //cout << dist;
            it->second[e.consultar_id()] = dist;
            --n;
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
/*void cjt_especies::leer_conjunto() {
  int n;
  string id,gen;
  cin >> n;
  estset.clear();
  for (int i = 0; i < n; ++i) {
  cin >> id >> gen;
  especie e(id,gen);
  estset.insert(e);
  }
  }
  */
/*void cjt_especies::calcular_tabladistancias(int & k const) const {
  it1
  it2
  for (int it1 = 0; i < estset.size(); ++i) {
  for () {
  if (it1==it2) {
  cout << 0
  }
  else {
  cout << obtener_distancia((*it1).gen,it2->.gen,k);
  }
  }
  cout << endl;
  }
  }
  fusiona_clusters() {
  cluster c1,c2;
  c1 = clustmap[id1];
  c2 = clustmap[id2];
  clustmap.erase[id1];
  clustmap.erase[id2];
  double dist;
  dist = (c1.value + c2.value)/2;
  id1 + id2;
  cluster c3 (dist,c1,c2);
  }
  */
