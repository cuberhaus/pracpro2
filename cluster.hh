/** @file cluster.hh
    @brief Especificación de la clase cluster
*/

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

        /** @brief Estructura del cluster */
        BinTree <pair<string,double>> c_tree;

    public:
        int dist;

        // Constructoras

        /** @brief Constructora por defecto
         * \pre True
         * \post El resultado es un cluster con distancia 0
         */
        cluster();

        /** @brief Constructora dado un pair
         * \pre True
         * \post El resultado es un cluster con distancia info.second y identificador info.first
         */
        cluster(pair <string, double> info);

        /**  @brief Constructora dado un pair y dos clusters
         * \pre True
         * \post El resultado es la fusión de un cluster con otros dos clusters y con el identificador y la distancia del par
         */
        cluster(const pair <string, double>& newclust, const cluster &left, const cluster& right, int dist);

        // Entrada / Salida

        /** @brief Operación de escritura, imprime la estructura del cluster
         * \pre True
         * \post Se ha escrito en el canal de salida estándard la estructura del cluster
         */
        void imprimir_cluster() const;
};
#endif
