#include "BinTree.hh"
#include "cluster.hh"


cluster::cluster() {
    dist = 0;
}

cluster::cluster(const double dist, const string id) {
   this -> dist = dist; //(*this).dist = dist
   this -> id = id;
}

cluster::obtener_distancia() { 
    assert(dist >= 0);
    return dist;
}

cluster::obtener_cluster() {
    return bindist;
}
