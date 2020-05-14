#ifndef cluster_hh
#define cluster_hh

#ifndef NO_DIAGRAM
#include <iostream>
#include <map>
#include <string>
#include "BinTree.hh"
#endif

using namespace std;

/** @class cluster
 * @brief Contiene un cluster
 */
class cluster {

    private:
        BinTree <pair<string,double>> c_tree;

    public: 
        /** @brief Constructora 
         * \pre True
         * \post El resultado es un cluster con distancia 0
         */
        cluster();

        /** @brief Constructora
         * \pre True
         * \post El resultado es un cluster con sus distancia y ID
         */
        cluster(BinTree<pair<string,double>> &c_tree);

        /** @brief Imprime la estructura del cluster 
         * \pre True
         * \post Imprime en el output la estructura del cluster 
         */
        void imprimir_cluster() const;
};
#endif
