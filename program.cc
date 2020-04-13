#include <iostream>

using namespace std;

int main() {
    // les distancies s'han de imprimir amb 4 decimals de precisió. ??set::precision(4);
    string s; 
    bool fin = false;
    cjt_especies c;
    cluster clu;
    int k;
    cin >> k;
    while (cin >> s and not fin) {

        if (s == "crea_especie") {
            bool existe;
            especie e;
            e.leer_especie();
            existe = c.existe_especie(e.obtener_id());
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
                cout << e.obtener_gen();
            }
        }
        else if (s == "distancia") {
            string id1,id2;
            bool existe, existe2;
            cin >> id1 >> id2;
            existe = c.existe_especie(id1);
            existe2 = c.existe_especie(id2);
            if (existe and existe2) {
                cout << c.obtener_distancia(c.obtener_especie(id1).obtener_gen(), c.obtener_especie(id2).obtener_gen(), k);
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
            c.calcular_tabladistancias(k); 
        }
        else if (s == "inicializa_clusters") {
            c.inicializar_clusters();
            c.inicializar_distmap();
            c.imprimir_clusters();
            c.inicializar_distar();
            c.imprimir_distmap();
        }
        else if (s == "ejecuta_paso_wpgma") {

        }
        else if (s == "imprime_cluster") {
             
        }
        else if (s == "imprime_arbol_filogenetico") {
        }
        else if (s == "fin") {
            fin = true;
        }
    }
}
