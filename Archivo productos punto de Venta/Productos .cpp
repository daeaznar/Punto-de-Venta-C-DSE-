#include <iostream>
#include <fstream>
#include<string>
#include <cstring>
#include <iomanip>

using namespace std;

void Lectura_prod();

struct inventario {
    int id;
    char producto[40];
    int pc;
    int pv;
    int existencia;
    int nr;
    inventario *ant;
    inventario *sig;
};

inventario *ptr = NULL;
inventario *inicio = NULL;
inventario *fin = NULL;
inventario *nuevo = NULL;

int total = 0;

int main() {
    fstream archivo("productos.bin", ios::out | ios::binary);
    char prod[40];
    string prod_t;
    int id_t, pc_t, pv_t, existencia_t, nr_t;
    if (!archivo) cout << "Error en la apertura del archivo\n";
    else {
        do {
            cout << "Producto (* para salir): ";
            cin >> prod_t;
            if (prod_t != "*") {
                strcpy(prod, prod_t.c_str());
                cout << "ID: ";
                cin >> id_t;
                cout << "PC: ";
                cin>>pc_t;
                cout << "PV: ";
                cin>>pv_t;
                cout << "Existencia: ";
                cin>>existencia_t;
                cout << "NR: ";
                cin>>nr_t;
                cout << prod << " " << id_t << " " << pc_t << " " << pv_t << " " << existencia_t << " " << nr_t << endl;
                //convertir string a char
                archivo.write(prod, 40);
                archivo.write(reinterpret_cast<char *> (&id_t), sizeof (id_t));
                archivo.write(reinterpret_cast<char *> (&pc_t), sizeof (pc_t));
                archivo.write(reinterpret_cast<char *> (&pv_t), sizeof (pv_t));
                archivo.write(reinterpret_cast<char *> (&existencia_t), sizeof (existencia_t));
                archivo.write(reinterpret_cast<char *> (&nr_t), sizeof (nr_t));
            }
        } while (prod_t != "*");
    }
    archivo.close();

    Lectura_prod();
}

void Lectura_prod() {
    fstream archivo("productos.bin", ios::in | ios::binary);
    char prod[40];
    int id_t, pc_t, pv_t, existencia_t, nr_t;
    int i = 0;
    if (!archivo) cout << "Error en la apertura del archivo" << endl;
    else {
        cout << setw(6) << "ID" << setw(12) << "Producto" << setw(6) << "PC" << setw(6) << "PV";
        cout << setw(14) << "Existencia" << setw(6) << "NR" << endl;
        // lectura adelantada
        archivo.read(prod, 40);
        archivo.read(reinterpret_cast<char *> (&id_t), sizeof (id_t));
        archivo.read(reinterpret_cast<char *> (&pc_t), sizeof (pc_t));
        archivo.read(reinterpret_cast<char *> (&pv_t), sizeof (pv_t));
        archivo.read(reinterpret_cast<char *> (&existencia_t), sizeof (existencia_t));
        archivo.read(reinterpret_cast<char *> (&nr_t), sizeof (nr_t));
        nuevo = new(inventario); //Iniciando lista (directamente, sin arreglo de estructuras))
        total += 1;
        nuevo->id = total;
        strcpy(nuevo->producto, prod);
        nuevo->pc = pc_t;
        nuevo->pv = pv_t;
        nuevo->existencia = existencia_t;
        nuevo->nr = nr_t;
        nuevo->ant = NULL;
        nuevo->sig = NULL;
        inicio = nuevo;
        fin = nuevo;
        i++;
        cout << i << endl;
        while (archivo.eof() == false) {
            cout << setw(5) << nuevo->id << setw(12) << nuevo->producto << setw(7) << nuevo->pc;
            cout << setw(6) << nuevo->pv << setw(9) << nuevo->existencia << setw(11) << nuevo->nr << endl;
            // lectura del siguiente registro
            archivo.read(prod, 40);
            archivo.read(reinterpret_cast<char *> (&id_t), sizeof (id_t));
            archivo.read(reinterpret_cast<char *> (&pc_t), sizeof (pc_t));
            archivo.read(reinterpret_cast<char *> (&pv_t), sizeof (pv_t));
            archivo.read(reinterpret_cast<char *> (&existencia_t), sizeof (existencia_t));
            archivo.read(reinterpret_cast<char *> (&nr_t), sizeof (nr_t));
            nuevo = new(inventario);
            total += 1;
            nuevo->id = total;
            strcpy(nuevo->producto, prod);
            nuevo->pc = pc_t;
            nuevo->pv = pv_t;
            nuevo->existencia = existencia_t;
            nuevo->nr = nr_t;
            fin->sig = nuevo;
            nuevo->ant = fin;
            nuevo->sig = NULL;
            fin = nuevo;
            i++;
            cout << i << endl;
        }
        archivo.close();
        ptr = inicio;
        while (ptr != NULL) {
            cout << ptr->producto << endl;
            ptr = ptr->sig;
        }
    }
}
