#include <iostream>
#include <fstream>
#include<string>
#include <cstring>
#include <iomanip>

using namespace std;

void Lectura_user();

struct user {
    char usuario[40];
    double pass;
    char tipo[40];
    user *ant;
    user *sig;
};

user *ptr = NULL;
user *inicio = NULL;
user *fin = NULL;
user *nuevo = NULL;

int total_u = 0;

int main() {
    fstream archivo("usuarios.bin", ios::out | ios::binary);
    char user_t[40];
    string user_a;
    char tipo_t[40];
    double pass_t;
    if (!archivo) cout << "Error en la apertura del archivo\n";
    else {
        do {
            cout << "Usuario (* para salir): ";
            cin >> user_a;
            if (user_a != "*") {
                strcpy(user_t, user_a.c_str());
                fflush(stdin);
                cout << "Pass: ";
                cin >> pass_t;
                fflush(stdin);
                cout << "Tipo: ";
                cin>> tipo_t;
                fflush(stdin);
                cout << user_t << " " << pass_t << " " << tipo_t << endl;
                archivo.write(user_t, 40);
                archivo.write(reinterpret_cast<char *> (&pass_t), sizeof (pass_t));
                archivo.write(tipo_t, 40);
            }
        } while (user_a != "*");
    }
    archivo.close();

    Lectura_user();
}

void Lectura_user() {
    fstream archivo("usuarios.bin", ios::in | ios::binary);
    char user_t[40];
    char tipo_t[40];
    double pass_t;
    int i = 0;
    if (!archivo) cout << "Error en la apertura del archivo" << endl;
    else {
        cout << setw(6) << "Usuario" << setw(6) << "Pass" << setw(10) << "Tipo" << endl;
        // lectura adelantada
        archivo.read(user_t, 40);
        archivo.read(reinterpret_cast<char *> (&pass_t), sizeof (pass_t));
        archivo.read(tipo_t,40);
        nuevo = new(user); //Iniciando lista (directamente, sin arreglo de estructuras))
        total_u += 1;
        strcpy(nuevo->usuario, user_t);
        nuevo->pass = pass_t;
        strcpy(nuevo->tipo, tipo_t);
        nuevo->ant = NULL;
        nuevo->sig = NULL;
        inicio = nuevo;
        fin = nuevo;
        i++;
        cout << i << endl;
        while (archivo.eof() == false) {
            cout << setw(6) << nuevo->usuario << setw(6) << nuevo->pass << setw(10) << nuevo->tipo << endl;
            // lectura del siguiente registro
            archivo.read(user_t, 40);
            archivo.read(reinterpret_cast<char *> (&pass_t), sizeof (pass_t));
            archivo.read(tipo_t, 40);
            nuevo = new(user);
            total_u += 1;
            strcpy(nuevo->usuario, user_t);
            nuevo->pass = pass_t;
            strcpy(nuevo->tipo, tipo_t);
            nuevo->ant = NULL;
            nuevo->sig = NULL;
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
            cout << ptr->usuario << endl;
            ptr = ptr->sig;
        }
    }
}
