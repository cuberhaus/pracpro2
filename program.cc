#ifndef NO_DIAGRAM

#include <iostream>
#include "cluster.hh"
#include "especie.hh"
#endif

#include "cjt_clusters.hh"
#include "cjt_especies.hh"

using namespace std;

int main() {

    cout.setf(ios::fixed);
    cout.precision(4);
        
    string s; 
    bool fin = false;
    cluster clu;
    cjt_especies c();
    cjt_clusters clusters();
    especie e();
    e.leer_k();

    while (cin >> s and not fin) {

        if (s == "crea_especie") {
            bool existe;
            especie e;
            e.leer_especie();
            existe = c.existe_especie(e.consultar_id());
            if (not existe) {
                c.insertar_especie(e);
            }
        }
        else if (s == "obtener_gen") {
            string s;
            especie e;
            bool existe;
            cin >> s;
            existe = c.existe_especie(s);
            if (existe) {
                e = c.obtener_especie(s);
                cout << e.consultar_gen();
            }
        }
        else if (s == "distancia") {
            string id1,id2;
            bool existe, existe2;
            cin >> id1 >> id2;
            existe = c.existe_especie(id1);
            existe2 = c.existe_especie(id2);
            if (existe and existe2) {
               cout << c.consultar_distancia(id1,id2);
            }
        }
        else if (s == "elimina_especie") {
            bool existe;
            string id;
            cin >> id;
            existe = c.existe_especie(id);
            if (existe) {
                c.eliminar_especie(id);
            }
        }
        else if (s == "existe_especie") {
            string id;
            cin >> id;
            if (c.existe_especie(id)) {
                cout << "existe" << endl;
            }
        }
        else if (s == "lee_cjt_especies") {
            c.leer_conjunto();
        }
        else if (s == "imprime_cjt_especies") {
            c.imprimir_conjunto();
        }
        else if (s == "tabla_distancias") {
            c.imprimir_tabla_distancias();
        }
        else if (s == "inicializa_clusters") {
            clusters.inicialitza_clusters(c.consultar_tabla_distancias());
            clusters.imprimir_clusters();
            clusters.imprimir_tabla_distancias();
        }
        else if (s == "ejecuta_paso_wpgma") {
            clusters.fusiona_clusters();
            clusters.imprimir_tabla_distancias()
        }
        else if (s == "imprime_cluster") {
            clusters.imprimir_cluster() 
        }
        else if (s == "imprime_arbol_filogenetico") {
            clusters.inicialitza_clusters();
            clusters.fusiona_todos_los_clusters();
            clusters.imprimir_estructura();
        }
        else if (s == "fin") {
            fin = true;
        }
    }
}
