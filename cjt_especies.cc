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

especie cjt_especies::obtener_especie(const string& id) const {
    map <string, especie>::const_iterator it = espmap.find(id);
    return it->second; 
}
bool cjt_especies::existe_especie(const string &id) const {
    map<string,especie>::const_iterator it = espmap.find(id);
    if (it != espmap.end()) return true;
    return false;
}

void cjt_especies::borrar_conjunto() {
    espmap.clear();
    distmap.clear();
}
void cjt_especies::insertar_especie(const especie &e) {
    espmap.insert(make_pair(e.consultar_id(),e));
}
void cjt_especies::imprimir_conjunto() const {
    for(map <string, especie>::const_iterator it = espmap.begin(); it != espmap.end(); ++it) {
        cout << it->second.consultar_id() << ' ' << it->second.consultar_gen();
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
