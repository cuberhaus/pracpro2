#ifndef cluster_hh
#define cluster_hh
/** @class cluster
 * @brief Contiene un cluster
 */
class cluster {

    private:
        BinTree <cluster> bindist;
        double dist;
        string id;

    public: 
        /** @brief Constructora 
         */
        cluster();
        /** @brief Imprime el cluster 
         * \pre tclusterrue
         * \post Imprime el cluster en el terminal 
         */
        imprime_cluster();
}
#endif
