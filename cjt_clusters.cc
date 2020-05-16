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

pair <double, pair<string, string>> cjt_clusters::distancia_minima() const {
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
    pair <double, pair<string,string>> ids;
    ids.first = minim;
    ids.second.first = id1;
    ids.second.second = id2;
    return ids;
}
void cjt_clusters::fusiona_clusters() {
    pair <double, pair<string, string>> ids_min;
    ids_min = distancia_minima();
    string id1 = ids_min.second.first;
    string id2 = ids_min.second.second;
    string fus;
    fus = ids_min.second.first + ids_min.second.second;
    clustdist[fus]; 
    map<string, map<string,double>>::iterator it, it2;
    it2 = clustdist.end();
    --it2; 
    --it2;
   cout << "hi" << endl; 
   int x = 0;
    for (it = clustdist.begin(); it != clustdist.end() and it != it2; ++it) {
        cout << it->first;
        if (fus > it->first) {
            clustdist[it->first][fus] = (clustdist.at(it->first).at(id1)+clustdist.at(it->first).at(id2)) / 2;
        }
        else if (fus < it->first) {
            ++x; 
            cout << x << endl;
            if (id1 < it->first and id2 < it->first) {
                clustdist[fus][it->first] = (clustdist.at(id1).at(it->first) + clustdist.at(id2).at(it->first)) / 2;
            }
            if (id1 < it->first and id2 > it->first) {
                clustdist[fus][it->first] = (clustdist.at(id1).at(it->first) + clustdist.at(it->first).at(id2)) / 2;
            }
            if (id1 > it->first and id2 < it->first) {
                clustdist[fus][it->first] = (clustdist.at(it->first).at(id1) + clustdist.at(id2).at(it->first)) / 2;
            }
            if (id1 > it->first and id2 > it->first) {
                clustdist[fus][it->first] = (clustdist.at(it->first).at(id1) + clustdist.at(it->first).at(id2)) / 2;
            }
        }
    }
    cout << "we did it";
    clustdist.erase(id1);
    clustdist.erase(id2);


}
