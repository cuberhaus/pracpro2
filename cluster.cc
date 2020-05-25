/** @file cluster.cc
    @brief Código de la clase cluster
*/
#include "cluster.hh"

cluster::cluster() {
    c_tree = BinTree <pair<string,double>> ();
}

cluster::cluster(pair <string,double> info) {
    BinTree <pair<string,double>> c_tree (info);
    dist = 1;
    this->c_tree = c_tree;
}

cluster::cluster(const pair <string, double>& newclust, const cluster &left, const cluster& right,int dist) {
    this->dist = dist;

    BinTree <pair<string,double>> c_tree(newclust, left.c_tree, right.c_tree);
    this -> c_tree  = c_tree;
}

void imprimir_cluster_aux(const BinTree <pair<string,double>> &c_tree)  {
    if (not c_tree.empty() and c_tree.left().empty() and c_tree.right().empty()) {
        cout << '[' << c_tree.value().first << ']';
    }
    else if (not c_tree.empty()) {
        cout << "[";
        cout << '(' << c_tree.value().first << ", " << c_tree.value().second << ") ";
        imprimir_cluster_aux(c_tree.left());
        imprimir_cluster_aux(c_tree.right());
        cout << ']';
    }
}

void cluster::imprimir_cluster() const {
    imprimir_cluster_aux(c_tree);
}
