#include <iostream>
#include "cjt_especies.hh"
#include "cjt_clusters.hh"
/*
void cjt_clusters::inicializa_clusters() {
    for (int i; i < c.mida_files(); ++i) {
        string id = c.consultar_id(i);
        for (int j; j < c.mida_columnes(i)) {
            string id2 = c.consultar_id(i,j);  
            clustdist [id][id2] = c.consultar_dist(i,j); 
        }
    }
}
*/
cjt_clusters::cjt_clusters() {
}
void cjt_clusters::insertar_especie(const string &id1, const string &id2, const double dist) {
    if (dist != -1) {
        clustdist[id1][id2] = dist;
        return;
    }
        clustdist[id1];
}
void cjt_clusters::imprimir_tabla_distancias() const {
    map <string, map<string, double >>::const_iterator it1;
    map<string,double>::const_iterator it2;
    for (it1 = clustdist.begin(); it1 != clustdist.end(); ++it1) {
        cout << it1->first << ':' ;
        for (it2 = it1->second.begin(); it2 != it1->second.end(); ++it2) {
            cout << ' ' << it2->first << " (" << it2->second << ")";
        }
        cout << endl;
    }
}


/*
        {
            for
            for() {
                distmap(it).(it)
                cluster e
                BinTree tre(e)
                clustmap.insert(id,distmap(it)
            }
        }
 */
