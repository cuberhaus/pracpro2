#include "BinTree.hh"
#include "cluster.hh"

void imprimir_cluster_aux(const BinTree <pair<string,double>> &c_tree)  {
    if (not c_tree.empty() and c_tree.left().empty() and c_tree.right().empty()) {
        cout << '[' << c_tree.value().first << ']';
    }
    else if (not c_tree.empty()) {
        cout << "[";
        cout << '(' << c_tree.value().first << ", " << c_tree.value().second;
        imprimir_cluster_aux(c_tree.left());
        cout << ']';
        imprimir_cluster_aux(c_tree.right());
        cout << ']';
    }
}

cluster::cluster() {
}
cluster::cluster(BinTree<pair<string,double>> &c_tree) {
    this -> c_tree = c_tree;
}

void cluster::imprimir_cluster() const {
    imprimir_cluster_aux(c_tree);
}
