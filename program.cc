/** @author Pol Casacuberta Gil */
/** @date 19/5/2019 */
/** @mainpage

  Nombre del Proyecto: Práctica PRO2 Primavera 2020

  Versión: 1.0

  Lenguaje i Versión: C++ 17

  El programa principal se encuentra en el módulo program.cc.
  Para Almacenar y tratar los datos descritos en el enunciado de la práctica,
  usaremos el módulo especie para representar las especies son sus
  identificadores y genes, el módulo cjt_especies para representar un conjunto
  de especies donde almacenaremos las distancias entre especies i el conjunto.
  Para los clusters usaremos el módulo cluster i el módulo cjt_clusters para
  almacenar un conjunto de estos.

*/

/** @file program.cc
  @brief Programa principal

  1. crea_especie: Crea una especie con el identificador y gen (dos strings) dados. Escribe un mensaje de error si ya existe una especie con el mismo identificador. La especie creada, si no hay error, se agrega al conjunto de especies.
  2. obtener_gen: Dado un identificador de especie, imprime el gen asociado a la especie. Escribe un mensaje de error si no existe una especie con el identificador dado.
  3. distancia: Dados dos identificadores de especies, imprime la distancia entre las dos especies. Se escribe un mensaje de error si alguna de las dos especies cuyos identificadores se dan no existen.
  4. elimina_especie: Dado el identificador de una especie e la elimina del conjunto de especies. Escribe un mensaje de error si la especie con el identificador dado no existe.
  5. existe_especie: Dado el identificador de una especie e imprime una indicación de si dicha especie existe.
  6. lee_cjt_especies: Lee del canal estándar de entrada un entero n ≥ 0 y a continuación una secuencia de n especies. Las n especies dadas tienen identificadores distintos entre sí. Los contenidos previos del conjunto de especies se descartan  y las n especies leídas se agregan al conjunto de especies.
  7. imprime_cjt_especies: Imprime en el canal estándar de salida el conjunto de especies.
  8. tabla_distancias: Imprime la tabla de distancias entre cada par de especies del conjunto de especies.
  9. inicializa_clusters: Inicializa el conjunto de clústers con el conjunto de especies en el estado en el que esté en ese momento, e imprime la tabla de distancias entre clústers. Al imprimir la tabla de distancias se usarán los identificadores de los clústers para indexar filas y columnas.
  10. ejecuta_paso_wpgma: ejecuta un paso del algoritmo WPGMA (fusiona los dos clústers a menor distancia en uno nuevo) e imprime la tabla de distancias entre clústers resultante. Al imprimir la tabla de distancias se usarán los identificadores de los clústers para indexar filas y columnas. En caso de que el número de clústers del conjunto sea menor o igual que uno solamente se debe imprimir un mensaje de error.
  11. imprime_cluster: dado un identificador α, imprime el clúster con el identificador dado, o un error si no existe un clúster con dicho identificador en el conjunto de clústers.
  12. imprime_arbol_filogenetico:imprime el árbol filogenético para el conjunto deespecies actual; dicho árbol es el clúster que agrupa todas las especies, resultante de aplicar el algoritmo WPGMA. El contenido del conjunto de clústers previo se descarta y se reinicializa con el conjunto de especies en el estado en el que esté en ese momento, para a continuación aplicar el algoritmo. El conjunto de clústers final es el que queda después de aplicar el algoritmo.
  Se imprimirá la estructura arborescente del clúster con los identificadores de los clústers (raíces de los subárboles) y la distancia entre cada clúster y sus hojas descendientes.
  13. fin: finaliza la ejecución del programa.

*/

#ifndef NO_DIAGRAM
#include <iostream>
#endif

#include "especie.hh"
#include "cjt_especies.hh"
#include "cjt_clusters.hh"
using namespace std;

void opcions() {
    cout << "OPCIONS: " << endl;
    cout << "crea_especie" << endl <<
         "obtener_gen" << endl <<
         "distancia" << endl <<
         "elimina_especie" << endl <<
         "existe_especie" << endl <<
         "lee_cjt_especies" << endl <<
         "imprime_cjt_especies" << endl <<
         "tabla_distancias" << endl <<
         "inicialitza_clusters" << endl <<
         "ejecuta_paso_wpgma" << endl <<
         "imprime_cluster" << endl <<
         "imprime_arbol_filogenetico" << endl <<
         "fin" << endl;

}

int main() {
    string s;
    bool fin = false;
    cjt_especies c;
    int k;
    cout << "Inserta k" << endl;
    cin >> k;
    especie::insertar_k(k);
    cjt_clusters clusters;
    opcions();
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
            cout << endl;
        }
        else if (s == "ejecuta_paso_wpgma") {
            cout << "# ejecuta_paso_wpgma" << endl;
            if (clusters.size() > 1) {
                clusters.fusiona_clusters();
                clusters.imprimir_tabla_distancias();
                cout << endl;
            }
            else cout << "ERROR: num_clusters <= 1" << endl << endl;
        }
        else if (s == "imprime_cluster") {
            string id;
            cin >> id;
            cout << "# imprime_cluster " << id << endl;

            if (clusters.existe_cluster(id)){
                clusters.imprimir_cluster(id);
                cout << endl;
                cout << endl;
            }
            else cout << "ERROR: El cluster "<< id << " no existe." << endl << endl;
        }

        else if (s == "imprime_arbol_filogenetico") {
            cout << "# imprime_arbol_filogenetico" << endl;
            c.inicializa_clusters(clusters);
            if (clusters.size() > 1) {
                while (clusters.size() > 1) {
                    clusters.fusiona_clusters();
                }
                clusters.imprimir_arbol_filogenetico();
                cout << endl;
                cout << endl;
            }
            else if (clusters.size() == 1) {
                clusters.imprimir_arbol_filogenetico();
                cout << endl;
                cout << endl;
            }
            else if (clusters.size() == 0) {
                cout << "ERROR: El conjunto de clusters es vacio." << endl << endl;
            }
        }

        else if (s == "fin") {
            fin = true;
        }
        opcions();
    }
}
