/** @file especie.hh
    @brief Especificación de la clase especie
*/

#ifndef especie_hh
#define especie_hh

#ifndef NO_DIAGRAM
#include <iostream>
#include <string>
#include <map>
#endif

using namespace std;

/** @class especie
 * @brief Contiene un id y un gen
 */
class especie {
    private:

        /** @brief Parámetro k para el cálculo de los kmeros */
        static int k;

        /** @brief Identificador de la espécie */
        string id;

        /** @brief Gen de la espécie */
        string gen;

        /** @brief Kmeros para el cálculo de las distancias*/ 
        map <string, int> kmeros;

        /** @brief Genera los kmeros correspondientes al gen de la especie
         * \pre Gen esta inicializado
         * \post El mapa de kmeros se ha actualizado
         */
        void generar_kmeros();

    public:

    // Constructoras
    
    /** @brief Constructora
     * \pre True
     * \post El resultado es una especie nueva
     */
    especie();

    /** @brief Crea una especie dado un id y un gen
     * \pre True
     * \post Crea una especie con id y un gen
     */
    especie(const string & id , const string & gen);

    // Modificadoras

    // Consultoras

    string consultar_id() const;
    /** @brief
     * \pre True
     * \post El resultado es el gen del P.I.
     */
    
    string consultar_gen() const;
    /** @brief Lee el parámetro k de la entrada
     *  \pre True
     *  \post El parámetro k ha sido actualizado
     */

    static void insertar_k(int k); // passar com a parametre

    static double distancia(const especie &e1, const especie &e2);


    // Entrada / Salida
    
    void leer_especie();
    /** @brief Consulta el id del P.I.
     * \pre True
     * \post El resultado es el id del P.I.
     */
};
#endif
