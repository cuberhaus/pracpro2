#ifndef cjt_clusters_hh
#define cjt_clusters_hh
#ifndef NO_DIAGRAM
#include <iostream>
#include <map>
#include <string>
#include "BinTree.hh"
#endif
#include "cluster.hh"
using namespace std;
/** @brief Cjt_clusters
 * @brief Contiene un conjunto de clusters
 */
class cjt_clusters {
    private:
        map <string, BinTree<cluster>> clustmap;

    public:
        cjt_clusters();
        /** @brief fusiona los dos clusters a menor distancia en uno nuevo
         * \pre Hay almenos dos clusters guardados
         * \post Se han juntado dos clusters en uno
         */
        void fusiona_clusters();
        /** @brief Se inicia el conjunto de clusters a partir de la tabla de distancias del conjunto de especies
         * \pre En la entrada hay el mapa de distancias de las especies
         * \post Se ha iniciado el mapa de clusters 
         */
        void inicializa_clusters(map<string, map<string, double >> distmap);
        /** @brief Imprime los clusters
         * \pre Hay almenos un cluster en el conjunto
         * \post Se han imprimido los clusters
         */
        void imprimir_clusters() const;
        /** @brief Imprime la tabla de distancias entre clusters
         * \pre Se han inicializado previamente los clusters
         * \post Se ha imprimido la tabla de distancias en el output
         */
        void imprimir_tabla_distancias() const;
        /** @brief Dado un id imprime el cluster correspondiente
         * \pre El id dado corresponde a un cluster del conjunto   
         * \post Imprime el cluster correspondiente al id en el output
         */
        void imprimir_cluster(const string &id) const;
        /** @brief Consulta el numero de elementos que hay en el conjunto
         * \pre True
         * \post El resultado es el numero de elementos en el conjunto
         */
        int consultar_size_conjunto() const;
        /** @brief Imprime la estructura arborescente
         * \pre True
         * \post Se ha imprimido la estructura arborescente en el output 
         */
        void imprimir_estructura() const;
        /** @brief Fusiona todos los clusters hasta que solo queda uno
         * \pre True
         * \post Todos los clusters se han fusionado
         */
        void fusiona_todos_los_clusters();
};
#endif
