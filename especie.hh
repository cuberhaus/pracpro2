#ifndef especie_hh
#define especie_hh

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

    public:
    /** @brief Constructora
     * pre
     * post
     */
    especie();
    /** @brief Crea una especie dado un id y un gen 
     * \pre True
     * \post Crea una especie con id y un gen 
     */
    especie(string & id const, string & gen const);
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
    void leer_k();
}
#endif
