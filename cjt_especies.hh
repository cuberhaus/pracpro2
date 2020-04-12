#ifndef cjt_especies_hh
#define cjt_especies_hh

class cjt_especies {
    private:
        // set map <string, especies> espmap; // not too sure about this
        map <string, map<string, double>> distmap;
        map <string, clusters> clustmap;
    public: 
        distancia 

        fusiona_clusters() {
            cluster c1,c2;
            c1 = clustmap[id1];
            c2 = clustmap[id2];
            clustmap.erase[id1];
            clustmap.erase[id2];
            double dist;
            dist = (c1.value + c2.value)/2;
            id1 + id2;
            cluster c3 (dist,c1.value,c2.value);
        }
        calcular_distmap() {
        }


}
