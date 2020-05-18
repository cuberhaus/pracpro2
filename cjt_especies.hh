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
        map <string, especie> espmap;
        map <string, map<string, double >> distmap;
    public:

        // Constructora

        /** @brief Constructora
         * \pre True
         * \post Crea un conjunt d'especies buit
         */
        cjt_especies();
        
        // Modificadores
        
        /** @brief Inicialitza els clusters a partir del Conjunt d'especies
         * \pre True
         * \post El conjunt de clusters s'ha inicialitzat
         */
        void inicializa_clusters(cjt_clusters &clusters);
        
        /** @brief añade una especie al set de especies
         * \pre especie en la entrada
         * \post La especie se ha añadido al set espset i se ha añadido a la tabla de distancias
         */
        void insertar_especie (const especie& e);

        /** Borra el conjunto de especies
         * \pre Hay un conjunto de especies
         * \post Se ha eliminado ese conjunto de especies
         */
        void borrar_conjunto ();

        /** @brief Se elimina una especie del conjunto
         * \pre Tamaño del set es estrictamente > 0
         * \post La especie se ha eliminado y se ha actualizado la tabla de distancias
         */
        void eliminar_especie(const string& id);

        // Consultores
        
        /** @brief A partir de un id, devuelve la especie correspondiente
         * \pre Existe una especie con el id del parametro
         * \post Se obtiene la especie con ese id
         */
        especie obtener_especie(const string& id) const;

        /** @brief Te dice si existe la especie con ese id
         * \pre id
         * \post si existe una especie con ese id, true, sino false
         */
        bool existe_especie (const string& id) const;

        /** @brief Obtiene la distància dados dos id
         * \pre Dos strings en la entrada
         * \post El resultado es la distància entre las dos especies
         */
        double consultar_distancia(const string id1,const string id2) const ;

        // Entrada / Salida
        
        /** @brief Imprime el conjunto de especies
         * \pre El conjunto no es vacio
         * \post Se escriben en el output el conjunto de especies
         */
        void imprimir_conjunto() const;

        /** @brief Imprime la tabla de distancias
         * \pre True
         * \post Se ha imprimido en el output la tabla de distancias
         */
        void imprimir_tabla_distancias() const;
};
#endif
