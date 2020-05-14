
#ifndef NO_DIAGRAM
#include <iostream>
#include <string>
#endif

#include "especie.hh"
#include "cjt_especies.hh"
#include "cluster.hh"
#include "cjt_clusters.hh"
using namespace std;

int main() {

    //cout.setf(ios::fixed);
    //cout.precision(4);

    string s;
    bool fin = false;
    //cluster clu;
    cjt_especies c;
    //cjt_clusters clusters;
    int k;
    cin >> k;
    especie::insertar_k(k);
    cjt_clusters clusters;
    while (cin >> s and not fin) {

        if (s == "crea_especie") {
            bool existe;
            string id, gen;
            cin >> id >> gen;
            cout << "# crea_especie " << id << ' ' << gen << endl;
            especie e(id,gen);
            existe = c.existe_especie(e.consultar_id());
            if (not existe) {
                c.insertar_especie(e);
            }
            else cout << "ERROR: La especie " << id << " ya existe." << endl;
            cout << endl;
        }
        else if (s == "obtener_gen") {
            string s;
            especie e;
            bool existe;
            cin >> s;
            cout << "# obtener_gen " << s << endl;
            existe = c.existe_especie(s);
            if (existe) {
                e = c.obtener_especie(s);
                cout << e.consultar_gen() << endl;
            }
            else cout << "ERROR: La especie "<< s << " no existe." << endl;
            cout << endl;

        }

        if (s == "distancia") {

            string id1,id2;
            bool existe, existe2;
            cin >> id1 >> id2;
            cout << "# distancia " << id1 << ' ' << id2 << endl;
            existe = c.existe_especie(id1);
            existe2 = c.existe_especie(id2);
            if (not existe and not existe2) {
                cout << "ERROR: La especie " << id1 << " y la especie " << id2 << " no existen." << endl;
            }
            else if (not existe) {
                cout << "ERROR: La especie " << id1 << " no existe." << endl;
            }
            else if (not existe2) {

                cout << "ERROR: La especie " << id2  << " no existe." << endl;
            }
            else {
                cout << c.consultar_distancia(id1,id2) << endl;
            }
            cout << endl;
        }


        else if (s == "elimina_especie") {
            bool existe;
            string id;
            cin >> id;
            cout << "# elimina_especie " << id << endl;
            existe = c.existe_especie(id);
            if (existe) {
                c.eliminar_especie(id);
            }
            else cout << "ERROR: La especie " << id << " no existe." << endl;
            cout << endl;
        }

        else if (s == "existe_especie") {
            string id;
            cin >> id;
            cout << "# existe_especie " << id << endl;
            if (c.existe_especie(id)) {
                cout << "SI" << endl;
            }
            else cout << "NO" << endl;
            cout << endl;
        }

        else if (s == "lee_cjt_especies") {
            cout << "# lee_cjt_especies" << endl << endl;
            int n;
            cin >> n;
            c.borrar_conjunto();
            for (int i = 0; i < n; ++i) {
                string id, gen;
                cin >> id >> gen;
                especie e(id, gen);
                c.insertar_especie(e);
            }
        }
        else if (s == "imprime_cjt_especies") {
            cout << "# imprime_cjt_especies" << endl;;
            c.imprimir_conjunto();
            cout << endl;
        }

        else if (s == "tabla_distancias") {
            cout << "# tabla_distancias" << endl;
            c.imprimir_tabla_distancias();
            cout << endl;
        }

        else if (s == "inicializa_clusters") {
            cout << "# inicializa_clusters" << endl;
            c.inicializa_clusters(clusters);
            clusters.imprimir_tabla_distancias();
        }
         
        //  else if (s == "ejecuta_paso_wpgma") {
         // clusters.fusiona_clusters();
          //clusters.imprimir_tabla_distancias();
          //}
          else if (s == "imprime_cluster") {
              string id;
              cin >> id;
              cout << "# imprime_cluster " << id << endl;
              clusters.imprimir_cluster(id);
          }
          /*
          else if (s == "imprime_arbol_filogenetico") {
          clusters.inicializa_clusters(c.consultar_tabla_distancias()); // passar directament el conjunt
          clusters.fusiona_todos_los_clusters();
          clusters.imprimir_estructura();
          }
          */
        else if (s == "fin") {
            fin = true;
        }
    }
}

