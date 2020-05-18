/** @file cjt_especies.hh
    @brief Especificación de la clase cjt_especies
*/

#ifndef cjt_especies_hh
#define cjt_especies_hh

#ifndef NO_DIAGRAM
#include <iostream>
#include <set>
#include <map>
#include <string>
#endif

#include "cjt_clusters.hh"
#include "especie.hh"
using namespace std;

/** @class cjt_especies
 * @brief Contiene el conjunto de las especies
 */
class cjt_especies {

    private:

        /** @brief Conjunto de especies */
        map <string, especie> espmap;

        /** @brief Tabla de distancias entre especies */
        map <string, map<string, double >> distmap;

    public:

        // Constructora

        /** @brief Constructora
         * \pre True
         * \post Crea un conjunto de especies vacío
         */
        cjt_especies();
        
        // Modificadoras
        
        /** @brief Modificadora del Conjunto de clusters
         * \pre True
         * \post Dado un conjunto de clusters lo inicializa con las distancias i las especies del conjunto de espécies   
         */
        void inicializa_clusters(cjt_clusters &clusters);
        
        /** @brief Modificadora añade una especie al set de especies
         * \pre especie e
         * \post La especie se ha añadido al parámetro implícito 
         */
        void insertar_especie (const especie& e);

        /** Modificadora borra el conjunto de especies
         * \pre True 
         * \post Se ha eliminado el conjunto de especies del P.I.
         */
        void borrar_conjunto ();

        /** @brief Se elimina una especie del conjunto
         * \pre Tamaño del set es estrictamente > 0, existe una especie en el P.I. con el identificador id
         * \post La especie con identificador id se ha eliminado y se han actualizado las distancias
         */
        void eliminar_especie(const string& id);

        // Consultores
        
        /** @brief Consultora que a partir de un identificador, devuelve la especie correspondiente
         * \pre Existe una especie con el identificador id en el P.I.
         * \post El resultado es la especie con ese identificador 
         */
        especie obtener_especie(const string& id) const;

        /** @brief Consultora de la existencia de una especie con identificador id
         * \pre id
         * \post El resultado es true si existe una especie con ese id sino false
         */
        bool existe_especie (const string& id) const;

        /** @brief Consultora de la distáncia entre especies dados dos identificadores
         * \pre id1, id2
         * \post El resultado es la distáncia entre las dos especies
         */
        double consultar_distancia(const string id1,const string id2) const ;

        // Entrada / Salida
        
        /** @brief Operación de escritura del conjunto de especies
         * \pre True 
         * \post Se ha escrito por el canal de salida estándard el conjunto de especies
         */
        void imprimir_conjunto() const;

        /** @brief Operación de escritura de las distancias entre especies 
         * \pre True
         * \post Se han escrito por el canal de salida estándard las distancias entre especies 
         */
        void imprimir_tabla_distancias() const;
};
#endif
