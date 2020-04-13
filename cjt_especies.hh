#ifndef cjt_especies_hh
#define cjt_especies_hh
#include <cassert>

class cjt_especies {
    private:
        set <especies> espset; // not too sure about this
        map <string, map<string, double >> distmap;
        map <string, map<double, string >> distar;
        map <string, clusters> clustmap;
    public: 
        fusiona_clusters() {
            cluster c1,c2;
            c1 = clustmap[id1];
            c2 = clustmap[id2];
            clustmap.erase[id1];
            clustmap.erase[id2];
            double dist;
            dist = (c1.value + c2.value)/2;
            id1 + id2;
            cluster c3 (dist,c1,c2);
        }
        /** @brief Devuelve una especie dado un string
         * \pre id se encuentra dentro del set espset
         * \post especie con el id de entrada
         */
        especie obtener_especie(const string &id) const; //espset.find(id)
        /** @brief Te dice si existe la especie con ese id
         * \pre id
         * \post si existe una especie con ese id, true, sino false
         */
        bool existe_especie (const string &id) const; // si es false assert() "no existe especie con id"
        /** @brief añade una especie al set de especies
         * \pre especie en la entrada
         * \post La especie se ha añadido al set espset
         */
        void insertar_especie (const especie &e);
        /** @brief Obtiene la distància dados dos genes gen1, gen2
         * \pre dos strings en la entrada  
         * \post devuelve la distància entre les dos genes
         */
        double obtener_distancia(const string& gen1, const string& gen2) const; 
        /**
         */
        void eliminar_especie(const string& id); 
        void leer_conjunto();

        calcular_distmap();
        
}
