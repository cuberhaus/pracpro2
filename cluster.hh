#ifndef cluster_hh
#define cluster_hh
/** @class cluster
 * @brief Contiene un cluster
 */
class cluster {

    private:
        double dist;
        string id;
        map <string, double> distmap;
    public: 
        /** @brief Constructora 
         * \pre True
         * \post El resultado es un cluster con distancia 0
         */
        cluster();
        /** @brief Constructora

         * \pre True
         * \post El resultado es un cluster con distancia dist y ID id
         */
        cluster(const double dist, const string id);
        /** @brief Devuelve un bintree de clusters 
         * \pre True
         * \post El resultado es el bintree de clusters del P.I.
         */
        obtener_bin_tree();
        /** @brief Devuelve una distancia
         * \pre dist >= 0
         * \post El resultado es la distancia del P.I.
         */
        obtener_distancia();
}
#endif
