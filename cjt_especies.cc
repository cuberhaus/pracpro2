#include <iostream>
using namespace std;
#include "especie.hh"
#include "cluster.hh"

void cjt_especies::leer_conjunto() {
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

void cjt_especies::calcular_tabladistancias(int & k const) const {
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
