#include <iostream>
#include "cjt_especies.hh"
#include "cjt_clusters.hh"
#include "cluster.hh"


cjt_clusters::cjt_clusters() {
}

void cjt_clusters::insertar_especie_dist(const string &id1, const string &id2, const double dist) {
    if (dist != -1) {
        clustdist[id1][id2] = dist;
    }
    else clustdist[id1];
}

void cjt_clusters::insertar_cluster(const string &id) {
    pair <string,double> p_esp;
    p_esp.first = id; 
    p_esp.second = 0;
    BinTree <pair<string,double>> c_tree (p_esp) ;
    cluster c (c_tree);
    clustmap [id] = c; 
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

void cjt_clusters::imprimir_cluster(const string &id) const {
    clustmap.at(id).imprimir_cluster();
}

pair <string, string> cjt_clusters::distancia_minima() const {
    map <string, map<string,double>>::const_iterator it1;
    map <string,double>::const_iterator it2;
    it1 = clustdist.begin();
    it2 = it1->second.begin();
    double minim = it2->second;
    string id1 = it1->first;
    string id2 = it2->first;
    for (it1 = clustdist.begin(); it1 != clustdist.end(); ++it1) {
        for (it2 = it1->second.begin(); it2 != it1->second.end(); ++it2) {
            if (it2->second < minim) {
                minim = it2->second;
                id1 = it1->first;
                id2 = it2->first;
            }
        }
    }
    pair <string,string> ids;
    ids.first = id1;
    ids.second = id2;
    return ids;
}
void cjt_clusters::fusiona_clusters() {
    pair <string, string> ids_min;
    ids_min = distancia_minima();
    
}
