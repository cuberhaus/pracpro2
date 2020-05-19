/** @file cjt_clusters.hh
    @brief Especificación de la clase cjt_clusters
*/

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

/** @class cjt_clusters
 * @brief Contiene un conjunto de clusters
 */
class cjt_clusters {
    private:

        /** @brief Contiene el conjunto de clusters */
        map <string, cluster> clustmap;

        /** @brief Contiene la tabla de distancias entre clusters */
        map <string, map<string, double>> clustdist;

        /** @brief Calcula la distancia mínima entre clusters
         * \pre True
         * \post El resultado es la distancia mínima entre clusters
         * \coste Lineal
         */
        pair <double, pair<string, string>> distancia_minima() const;

    public:

        // Constructora

        /** @brief Constructora por defecto
         * \pre True
         * \post Crea un conjunto de clusters
         * \coste Constante
         */
        cjt_clusters();

        // Modificadoras

        /** @brief Inserta la distancia de una especie en el conjunto
         * \pre id1, id2 son strings i dist es un double
         * \post Las distancias se han añadido al conjunto
         */
        void insertar_especie_dist(const string &id1, const string &id2, const double dist);

        /** @brief Modificadora del conjunto de clusters y sus distancias, Inserta un cluster al conjunto
         * \pre El id no està en el conjunto
         * \post El cluster se ha añadido al conjunto
         */
        void insertar_cluster(const string &id);

        /** @brief Modificadora del conjunto de clusters y sus distancias, fusiona los dos clusters a menor distancia en uno nuevo
         * \pre Hay almenos dos clusters guardados
         * \post Se han juntado dos clusters en uno
         */
        void fusiona_clusters();

        /** @brief Modificadora del conjunto de clusters y sus distancias, borra el conjunto de clusters
         * \pre True
         * \post El conjunto de clusters está vacio
         */
        void borrar_conjunto();

        // Consultoras

        /** @brief Consultora, si el identificador corresponde con un cluster del conjunto devuelve True
         * \pre True
         * \post El resultado es True si existe el cluster con el identificador dado, sino False
         */
        bool existe_cluster(const string &id) const;

        /** @brief Consultora del numero de elementos que hay en el conjunto
         * \pre True
         * \post El resultado es el numero de elementos en el conjunto
         */
        int size() const;

        // Entrada / Salida

        /** @brief Operación de escritura de la tabla de distancias entre clusters
         * \pre True
         * \post Se ha escrito la tabla de distancias entre clusters por el canal de salida estándard
         */
        void imprimir_tabla_distancias() const;

        /** @brief Operación de escritura que dado un id imprime el cluster correspondiente
         * \pre El id dado corresponde a un cluster del conjunto
         * \post Se ha escrito el cluster correspondiente al id por el canal de salida estándard
         */
        void imprimir_cluster(const string &id) const;

        /** @brief Operación de escritura del árbol filogenético
         * \pre El tamaño del cluster es igual a 1
         * \post Se ha escrito la estructura del árbol por el canal de salida estándard
         */
        void imprimir_arbol_filogenetico() const;
};
#endif
