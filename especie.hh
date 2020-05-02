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
 * El coste de las operaciones es constante
 */
class especie {
    private:
        static int k;
        string id;
        string gen;
        map <string, int> kmeros;
        //map <string, double> d_esp();
        //static distancia
    public:
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
    /** @brief Añade y/o cambia el id y el gen del P.I. por dos strings del input
     * \pre True
     * \post El P.I. tiene id y gen = iguales a los strings de la entrada
     */
    void leer_especie();
    /** @brief Consulta el id del P.I.
     * \pre True
     * \post El resultado es el id del P.I.
     */
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
    static void añadir_k(int k); // passar com a parametre
    /** @brief Genera los kmeros correspondientes al gen de la especie
     * \pre Gen esta inicializado
     * \post El mapa de kmeros se ha actualizado
     */
    void generar_kmeros(); // privat 
    //distancia static
    //static distancia(especie e, especie a) {
    //e.meros
};
#endif
