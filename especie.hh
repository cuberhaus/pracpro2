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
 * @brief Contiene un identificador y un gen
 */
class especie {
    private:

        /** @brief Parámetro k para el cálculo de los kmeros */
        static int k;

        /** @brief Identificador de la especie */
        string id;

        /** @brief Gen de la especie */
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

    /** @brief Constructora dado un id y un gen
     * \pre True
     * \post Crea una especie con id y un gen
     */
    especie(const string & id , const string & gen);

    // Modificadoras

    /** @brief Modificadora del parámetro k
     *  \pre True
     *  \post El parámetro k toma el nuevo valor
     */
    static void insertar_k(int k);

    // Consultoras

    /** @brief Consultora del identificador
     * \pre True
     * \post El resultado es el id del P.I.
     */
    string consultar_id() const;

    /** @brief Consultora del gen
     * \pre True
     * \post El resultado es el gen del P.I.
     */
    string consultar_gen() const;

    /** @brief Consultora, calcula la distancia entre dos especies
     * \pre e1 y e2 contienen id y gen
     * \post El resultado es la distancia entre las especies
     */
    static double distancia(const especie &e1, const especie &e2);
};
#endif
