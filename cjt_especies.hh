#ifndef cjt_especies_hh
#define cjt_especies_hh

class cjt_especies {
    private:
        set <string, especie> espset;
        map <especie, map<string, double >> distmap;
        map <string, map<double, string >> distar;
    public: 
        cjt_especies(const int);
        /** @brief Devuelve una especie dado un string
         * \pre id se encuentra dentro del set espset
         * \post especie con el id de entrada
         */
        especie obtener_especie(const string &id) const; //espset.find(id)
        /** @brief Te dice si existe la especie con ese id
         * \pre id
         * \post si existe una especie con ese id, true, sino false
         */
        bool existe_especie (const string &id) const;
        /** @brief añade una especie al set de especies
         * \pre especie en la entrada
         * \post La especie se ha añadido al set espset
         */
        void insertar_especie (const especie &e);
        /** @brief Obtiene la distància dados dos id 
         * \pre Dos strings en la entrada  
         * \post El resultado es la distància entre las dos especies
         */
        double consultar_distancia(const string& id1, const string& id2) const; 
        /** @brief 
         */
        void eliminar_especie(const string& id); 
        /** @brief Borra las especies del conjunto anterior y se añaden de nuevas desde  el input
         * \pre n especies en el input y el numero n
         * \post N especies en el conjunto
         */
        void leer_conjunto();
        /** @brief Imprime el conjunto de especies
         * \pre El conjunto no es vacio
         * \post Se escriben en el output el conjunto de especies
         */
        void imprimir_conjunto() const;

        void calcular_tabladistancias(); 
        void inicializar_clusters();  // poner las especies dentro del cluster dentro del bintree 
        void inicializar_distmap(); // se guarda en la clase "las coje desde el set"
        void actualizar_dist_map(); // junta las especies con menor distancia (los strings) i calcula las distancias a los demas (clusters)
        void inicializar_distar();  // se guarda en la clase
        void imprimir_distmap() const;

        void imprimir_cluster();
}
