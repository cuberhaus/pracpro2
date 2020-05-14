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
        //pair <string,double> raiz;
        //double dist;
        //string id;
        BinTree <pair<string,double>> c_tree;
        //map <string, double> distmap;
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

        void imprimir_cluster() const;
        /** @brief Devuelve las distancias a los demás clusters 
         * \pre True
         * \post El resultado son las distancias des de el cluster en el que estamos hasta los otros clusters del P.I.
         */
//         map <string, double> obtener_dist_map(); // passar directament el cluster
        /** @brief Devuelve una distancia
         * \pre dist >= 0
         * \post El resultado es la distancia del P.I.
         */
        //double obtener_distancia();
};
#endif
