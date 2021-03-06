#include <iostream>
#include <iomanip>
#include <ctime>
#include <fstream>
#include <cstring>
#include <string>
#include <algorithm>
#include <utility>

using namespace std;

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

struct vender {
    string comprador;
    int subtotal_c[50]; //Guarda total de cada producto
    string item[50];
    int price[50];
    int total_c;
};

struct user {
    char usuario[40];
    double pass;
    char tipo[40];
    int total_v; //Total del vendedor
    user *ant;
    user *sig;
    vender buyer[50]; //Guarda los compradores en cada vendedor
};

user *nuevo_u = NULL;
user *ptr_u = NULL;
user *inicio_u = NULL;
user *fin_u = NULL;

int buscar(string name);

void Lectura_prod();
void Lectura_users();

void Verify_user();
bool check_user;

void Administrador(); //Prototipos
void Altas();
void Bajas();
void Consultas();
void Modificaciones();
void Inventario();

void Usuarios();
void Altas_u();
void Bajas_u();
void Consultas_u();
void Modificaciones_u();
void Mostrar_u();

void Ventas();
void Corte_caja();

void Escritura_prods();
void Escritura_users();

int total = 0;
int total_u = 0;

int gran_total = 0;
int total_costos = 0;

int main() {
    Lectura_users();
    Lectura_prod();
    string tipo_temp;
    int opc;
    do {
        cout << "Menú\n";
        cout << "1. Administrador\n";
        cout << "2. Ventas\n";
        cout << "3. Salir\n";
        cout << "Opción: ";
        cin>>opc;
        cout << endl;
        switch (opc) {
            case 1: Verify_user();
            {
                if (check_user == true) {
                    tipo_temp = ptr_u->tipo;
                    if (tipo_temp == "Administrador")
                        Administrador();
                    else
                        cout << "*El usuario ingresado no tiene privilegios de Administrador*\n" << endl;
                } else
                    if (check_user == false)
                    cout << "***Ha excedido el límite de intentos***\n" << endl;
                break;
            }
                break;
            case 2:Verify_user();
            {
                if (check_user == true) {
                    tipo_temp = ptr_u->tipo;
                    if (tipo_temp == "Vendedor")
                        Ventas();
                    else
                        cout << "*El usuario ingresado no es Vendedor*\n" << endl;
                } else
                    if (check_user == false)
                    cout << "Ha excedido el límite de intentos\n";
                break;
            }
                break;
            case 3:Corte_caja();
            {
                Escritura_prods();
                Escritura_users();
                cout << "Cerrando Sistema\n";
            }
                break;
            default: cout << "Opción inválida\n";
        }
    } while (opc != 3);
    return 0;
}

void Lectura_users() {
    fstream archivo_u("usuarios.bin", ios::in | ios::binary);
    char user_t[40];
    char tipo_t[40];
    double pass_t;
    int i = 0;
    if (!archivo_u) cout << "Error en la apertura del archivo" << endl;
    else {
        cout << setw(8) << "Usuario" << setw(6) << "Pass" << setw(15) << "Tipo" << endl;
        // lectura adelantada
        archivo_u.read(user_t, 40);
        archivo_u.read(reinterpret_cast<char *> (&pass_t), sizeof (pass_t));
        archivo_u.read(tipo_t, 40);
        nuevo_u = new(user); //Iniciando lista (directamente, sin arreglo de estructuras))
        total_u += 1;
        strcpy(nuevo_u->usuario, user_t);
        nuevo_u->pass = pass_t;
        strcpy(nuevo_u->tipo, tipo_t);
        nuevo_u->ant = NULL;
        nuevo_u->sig = NULL;
        inicio_u = nuevo_u;
        fin_u = nuevo_u;
        i++;
        while (archivo_u.eof() == false) {
            // lectura del siguiente registro
            archivo_u.read(user_t, 40);
            archivo_u.read(reinterpret_cast<char *> (&pass_t), sizeof (pass_t));
            archivo_u.read(tipo_t, 40);
            nuevo_u = new(user);
            total_u += 1;
            strcpy(nuevo_u->usuario, user_t);
            nuevo_u->pass = pass_t;
            strcpy(nuevo_u->tipo, tipo_t);
            fin_u->sig = nuevo_u;
            nuevo_u->ant = fin_u;
            nuevo_u->sig = NULL;
            fin_u = nuevo_u;
            i++;
        }
        archivo_u.close();
        delete(fin_u);
        fin_u = fin_u->ant;
        fin_u->sig = NULL;
        total_u -= 1;
        ptr_u = inicio_u;
        while (ptr_u != NULL) {
            cout << setw(8) << ptr_u->usuario << setw(6) << ptr_u->pass << setw(18) << ptr_u->tipo << endl;
            ptr_u = ptr_u->sig;
        }

    }
}

int buscar_u(string name) {
    int i = 0;
    ptr_u = inicio_u;
    while (i < total_u && ptr_u->usuario != name) {
        i++;
        ptr_u = ptr_u->sig;
    }
    if (i == total_u)
        return-1;
    else
        return i;
}

void Verify_user() {
    string user_t;
    int pos;
    int pass_t;
    int i;
    i = 0;
    while (i < 3) {
        cout << "Ingrese nombre de usuario: ";
        cin>>user_t;
        cout << "Ingresa contraseña: ";
        cin>> pass_t;
        pos = buscar_u(user_t);
        if (pos == -1)
            cout << "Usuario o contraseña incorrectos. Quedan: " << 2 - i << " intentos\n" << endl;
        else
            if (pass_t == ptr_u->pass) {
            check_user = true;
            break;
        } else
            cout << "Usuario o contraseña incorrectos. Quedan: " << 2 - i << " intentos\n" << endl;
        i++;
    }
    if (i > 2)
        check_user = false;
}

void Lectura_prod() {
    fstream archivo("productos.bin", ios::in | ios::binary);
    char prod[40];
    int id_t, pc_t, pv_t, existencia_t, nr_t;
    int i = 0;
    if (!archivo) cout << "***Error en la apertura del archivo***" << endl;
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
        nuevo->id = id_t;
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
        while (archivo.eof() == false) {
            // lectura del siguiente registro
            archivo.read(reinterpret_cast<char *> (&prod), sizeof (prod));
            archivo.read(reinterpret_cast<char *> (&id_t), sizeof (id_t));
            archivo.read(reinterpret_cast<char *> (&pc_t), sizeof (pc_t));
            archivo.read(reinterpret_cast<char *> (&pv_t), sizeof (pv_t));
            archivo.read(reinterpret_cast<char *> (&existencia_t), sizeof (existencia_t));
            archivo.read(reinterpret_cast<char *> (&nr_t), sizeof (nr_t));
            nuevo = new(inventario);
            total += 1;
            strcpy(nuevo->producto, prod);
            nuevo->id = id_t;
            nuevo->pc = pc_t;
            nuevo->pv = pv_t;
            nuevo->existencia = existencia_t;
            nuevo->nr = nr_t;
            fin->sig = nuevo;
            nuevo->ant = fin;
            nuevo->sig = NULL;
            fin = nuevo;
            i++;
        }
        archivo.close();
        delete(fin);
        fin = fin->ant;
        fin->sig = NULL;
        total -= 1;
        ptr = inicio;
        while (ptr != NULL) {
            cout << setw(5) << ptr->id << setw(12) << ptr->producto << setw(7) << ptr->pc;
            cout << setw(6) << ptr->pv << setw(9) << ptr->existencia << setw(11) << ptr->nr << endl;
            ptr = ptr->sig;
        }
    }
}

int buscar(string name) {
    int i = 0;
    ptr = inicio;
    while (i < total && ptr->producto != name) {
        i++;
        ptr = ptr->sig;
    }
    if (i == total) {
        return-1;
    } else
        return i;
}

void Administrador() {
    int opc;
    int pos;
    do {
        cout << endl;
        cout << "Menú Administrador\n";
        cout << "1. Altas\n";
        cout << "2. Bajas\n";
        cout << "3. Consultas\n";
        cout << "4. Modificaciones\n";
        cout << "5. Mostrar inventario\n";
        cout << "6. Administración de Usuarios\n";
        cout << "7. Regresar\n";
        cout << "Opción: ";
        cin>>opc;
        cout << endl;
        switch (opc) {
            case 1: Altas();
                break;
            case 2: Bajas();
                break;
            case 3: Consultas();
                break;
            case 4: Modificaciones();
                break;
            case 5: Inventario();
                break;
            case 6: Usuarios();
                break;
            case 7:
                break;
            default: cout << "Opción Inválida\n";
        }
    } while (opc != 7);
}

void Altas() {
    string prod;
    int pos;
    int pc_t, pv_t, existencia_t, nr_t;
    while (true) {
        cout << "Ingrese Producto: ";
        cin >> prod;
        if (prod == "*")
            break;
        pos = buscar(prod);
        if (pos == -1) {
            do {
                cout << "Ingrese Precio de Compra: ";
                cin >> pc_t;
                cout << "Ingrese Precio de Venta: ";
                cin >> pv_t;
                if (pv_t <= pc_t)
                    cout << "El precio de Venta debe ser mayor al precio de Compra\n";
            } while (pv_t <= pc_t);
            do {
                cout << "Ingrese cantidad en existencia: ";
                cin >> existencia_t;
                cout << "Ingrese nivel de Reorden: ";
                cin >> nr_t;
                if (existencia_t <= nr_t)
                    cout << "La existencia debe ser mayor al Nivel de Reorden\n";
            } while (existencia_t <= nr_t);
            nuevo = new(inventario);
            strcpy(nuevo->producto, prod.c_str());
            nuevo->pc = pc_t;
            nuevo->pv = pv_t;
            nuevo->existencia = existencia_t;
            nuevo->nr = nr_t;
            total += 1;
            if (inicio == NULL) {
                nuevo->id = 1;
                inicio = nuevo;
                fin = nuevo;
                fin->sig = NULL;
                fin->ant = NULL;
            } else {
                nuevo->ant = fin;
                fin->sig = nuevo;
                nuevo->id = nuevo->ant->id + 1;
            }
            fin = nuevo;
            fin->sig = NULL;
            cout << "*El producto ha sido dado de alta exitosamente*\n" << endl;
        } else
            cout << "El producto " << prod << " ya está dado de alta\n" << endl;
    }
}

void Bajas() {
    int i;
    int pos;
    int opc;
    string prod;
    cout << "Bajas\n";
    if (inicio == NULL)
        cout << "***No hay ningún producto registrado***" << endl;
    else {
        while (true) {
            cout << "Ingrese producto: ";
            cin>>prod;
            if (prod == "*")
                break;
            pos = buscar(prod);
            if (pos == -1)
                cout << "El producto: " << prod << ", no existe\n";
            else {
                while (true) {
                    cout << "¿Seguro que desea dar de baja el producto: " << prod << "?\n";
                    cout << "1. Sí\n";
                    cout << "2. No\n";
                    cin >> opc;
                    switch (opc) {
                        case 1:
                        {
                            total -= 1;
                            ptr = inicio;
                            i = 0;
                            while (i != pos) {
                                ptr = ptr->sig;
                                i++;
                            }
                            if (inicio == fin) { //Si solo hay un nodo en la lista
                                delete(inicio);
                                inicio == NULL;
                                fin == NULL;
                                ptr == NULL;
                                nuevo == NULL;
                            } else {
                                if (ptr == inicio) { //Hay varios nodos, solo el 1° se borra
                                    delete(inicio);
                                    inicio = inicio->sig;
                                    inicio->ant = NULL;
                                } else {
                                    if (ptr == fin) { //Hay varios nodos, solo el último se borra
                                        delete(fin);
                                        fin = fin->ant;
                                        fin->sig = NULL;
                                    } else { //Varios nodos, borrar X elemento de la lista
                                        delete(ptr);
                                        ptr->ant->sig = ptr->sig;
                                        ptr->sig->ant = ptr->ant;
                                    }
                                }

                            }
                            cout << "*El producto ha sido dado de baja exitosamente*\n" << endl;
                        }
                            break;
                        case 2:
                            break;
                        default: cout << "Opción Inválida\n";
                    }
                    if (opc < 3)
                        break;
                }
            }
        }
    }
}

void Consultas() {
    int pos;
    int opc;
    string prod;
    int pc_t, pv_t, existencia_t, nr_t;
    cout << "Consultas\n";
    if (inicio == NULL)
        cout << "***No hay ningún producto registrado***" << endl;
    else {
        while (true) {
            cout << "Ingrese producto: ";
            cin>>prod;
            if (prod == "*")
                break;
            pos = buscar(prod);
            if (pos == -1) {
                while (true) {
                    cout << "El producto " << prod << " no existe. ¿Desea darlo de Alta?\n";
                    cout << "1. Sí\n" << "2. No\n";
                    cin >> opc;
                    switch (opc) {
                        case 1:
                        {
                            do {
                                cout << "Ingrese Precio de Compra: ";
                                cin >> pc_t;
                                cout << "Ingrese Precio de Venta: ";
                                cin >> pv_t;
                                if (pv_t <= pc_t)
                                    cout << "El precio de Venta debe ser mayor al precio de Compra\n";
                            } while (pv_t <= pc_t);
                            do {
                                cout << "Ingrese cantidad en existencia: ";
                                cin >> existencia_t;
                                cout << "Ingrese nivel de Reorden: ";
                                cin >> nr_t;
                                if (existencia_t <= nr_t)
                                    cout << "La existencia debe ser mayor al Nivel de Reorden\n";
                            } while (existencia_t <= nr_t);
                            nuevo = new(inventario);
                            strcpy(nuevo->producto, prod.c_str());
                            nuevo->id = total + 1;
                            nuevo->pc = pc_t;
                            nuevo->pv = pv_t;
                            nuevo->existencia = existencia_t;
                            nuevo->nr = nr_t;
                            total += 1;
                            if (inicio == NULL)
                                inicio = nuevo;
                            else {
                                nuevo->ant = fin;
                                fin->sig = nuevo;
                            }
                            fin = nuevo;
                        }
                            break;
                        case 2:
                            break;
                        default: cout << "Opción Inválida\n";
                    }
                    if (opc < 3)
                        break;
                }
            } else {
                cout << "Resultados del producto: " << ptr->producto << endl;
                cout << endl;
                cout << setw(6) << "ID" << setw(12) << "Producto" << setw(6) << "PC" << setw(6) << "PV";
                cout << setw(14) << "Existencia" << setw(6) << "NR" << endl;
                cout << setw(5) << ptr->id << setw(12) << ptr->producto << setw(7) << ptr->pc;
                cout << setw(6) << ptr->pv << setw(9) << ptr->existencia << setw(11) << ptr->nr;
                if (ptr->existencia <= ptr->nr)
                    cout << "*" << endl;
                else
                    cout << endl;
            }
        }
    }
}

void Modificaciones() {
    int opc;
    int pos;
    string nombre;
    cout << "Modificaciones\n";
    if (inicio == NULL)
        cout << "***No hay ningún producto registrado***" << endl;
    else {
        while (true) {
            cout << "Ingrese producto a Modificar: ";
            cin>>nombre;
            if (nombre == "*")
                break;
            pos = buscar(nombre);
            if (pos == -1)
                cout << "El producto: " << nombre << ", no existe\n";
            else {
                while (true) {
                    cout << "Qué desea Modificar?\n";
                    cout << "1. Precio de Compra\n";
                    cout << "2. Precio de Venta\n";
                    cout << "3. Existencia\n";
                    cout << "4. Nivel de Reorden\n";
                    cout << "5. TODO\n";
                    cout << "6. Regresar\n";
                    cin>>opc;
                    switch (opc) {
                        case 1:
                        {
                            cout << "El Precio de Compra actual es de: " << ptr->pc << endl;
                            cout << "Ingrese el nuevo Precio de Compra: ";
                            cin >> ptr->pc;
                            cout << "El Precio de Compra ha sido Modificado\n" << endl;
                        }
                            break;
                        case 2:
                        {
                            cout << "El Precio de Venta actual es de: " << ptr->pv << endl;
                            cout << "Ingrese el nuevo Precio de Venta: ";
                            cin >> ptr->pv;
                            cout << "El Precio de Venta ha sido Modificado\n" << endl;
                        }
                            break;
                        case 3:
                        {
                            cout << "La Existencia actual es de: " << ptr->existencia << endl;
                            cout << "Ingrese la nueva Existencia: ";
                            cin >> ptr->existencia;
                            cout << "La Existencia ha sido Modificada\n" << endl;
                        }
                            break;
                        case 4:
                        {
                            cout << "El Nivel de Reorden actual es de: " << ptr->nr << endl;
                            cout << "Ingrese el nuevo Nivel de Reorden: ";
                            cin >> ptr->nr;
                            cout << "El Nivel de Reorden ha sido Modificado\n" << endl;
                        }
                            break;
                        case 5:
                        {
                            cout << "Datos actuales:\n";
                            cout << setw(6) << "ID" << setw(12) << "Producto" << setw(6) << "PC" << setw(6) << "PV";
                            cout << setw(14) << "Existencia" << setw(6) << "NR" << endl;
                            cout << setw(5) << ptr->id << setw(12) << ptr->producto << setw(7) << ptr->pc;
                            cout << setw(6) << ptr->pv << setw(9) << ptr->existencia << setw(11) << ptr->nr;
                            if (ptr->existencia <= ptr->nr)
                                cout << "*" << endl;
                            else
                                cout << endl;
                            cout << "Ingrese los nuevos datos:\n";
                            cout << "Precio de compra: ";
                            cin >> ptr->pc;
                            cout << "Precio de Venta: ";
                            cin >> ptr->pv;
                            cout << "Existencia: ";
                            cin >> ptr->existencia;
                            cout << "Nivel de Reorden: ";
                            cin >> ptr->nr;
                            cout << "*Los datos han sido Modificados*\n" << endl;
                        }
                            break;
                        case 6:
                            break;
                        default: cout << "Opción Inválida\n";
                    }
                    if (opc == 6)
                        break;
                }
            }
        }
    }
}

void Inventario() {
    int i, j, k;
    int id_temp, pc_temp, pv_temp, ex_temp, nr_temp;
    int opc;
    string prod_temp;
    string salir;
    ptr = inicio;
    cout << "Inventario\n";
    if (inicio == NULL)
        cout << "***No hay ningún producto registrado***" << endl;
    else {
        do {
            cout << "Mostrar inventario por:\n";
            cout << "1. ID\n" << "2. Producto\n";
            cin>>opc;
            switch (opc) {
                case 1:
                {
                    /*k = total;
                    ptr = inicio;
                    for (i = 0; i < k - 1; i++)
                        for (j = 0; j < (k - 1) - i; j++)
                            if (ptr->id > ptr->sig->id) {
                                id_temp = ptr->sig->id;
                                ptr->sig->id = ptr->id;
                                ptr->id = id_temp;
                     * 
                                prod_temp = ptr->sig->producto;
                                strcpy(ptr->sig->producto, ptr->producto);
                                strcpy(ptr->producto, prod_temp.c_str());
                     * 
                                pc_temp = ptr->sig->pc;
                                ptr->sig->pc = ptr->pc;
                                ptr->pc = pc_temp;
                     * 
                                pv_temp = ptr->sig->pv;
                                ptr->sig->pv = ptr->pv;
                                ptr->pv = pv_temp;
                     * 
                                ex_temp = ptr->sig->existencia;
                                ptr->sig->existencia = ptr->existencia;
                                ptr->existencia = ex_temp;
                     * 
                                nr_temp = ptr->sig->nr;
                                ptr->sig->nr = ptr->nr;
                                ptr->nr = nr_temp;
                     * 
                     * 
                     * 
                                ptr = ptr->sig;
                            }*/
                    ptr = inicio;
                    while (true) {
                        cout << setw(5) << ptr->id << setw(12) << ptr->producto << setw(7) << ptr->pc;
                        cout << setw(6) << ptr->pv << setw(9) << ptr->existencia << setw(11) << ptr->nr;
                        if (ptr->existencia <= ptr->nr)
                            cout << "*" << endl;
                        else
                            cout << endl;
                        if (ptr->sig == NULL)
                            break;
                        else
                            ptr = ptr->sig;
                    }
                    /*    ptr = inicio;
                        while (ptr->sig != NULL) {
                            if (*ptr->id > *ptr->sig->id)
                                swap(ptr, ptr->sig);
                            ptr = ptr->sig;
                        }
                        cout << setw(6) << "ID" << setw(12) << "Producto" << setw(6) << "PC" << setw(6) << "PV";
                        cout << setw(14) << "Existencia" << setw(6) << "NR" << endl;
                        ptr = inicio;
                        while (true) {
                            cout << setw(5) << ptr->id << setw(12) << ptr->producto << setw(7) << ptr->pc;
                            cout << setw(6) << ptr->pv << setw(9) << ptr->existencia << setw(11) << ptr->nr;
                            if (ptr->existencia <= ptr->nr)
                                cout << "*" << endl;
                            else
                                cout << endl;
                            if (ptr->sig== NULL)
                                break;
                            else
                                ptr = ptr->sig;
                        }*/
                }
                    while (true) {
                        cout << "Presione ''*'' para salir\n";
                        cin>>salir;
                        if (salir == "*")
                            break;
                    }
                    break;
                case 2:
                {
                    ptr = inicio;
                    /*while (ptr->sig != NULL) {
                        if (ptr->producto > ptr->sig->producto)
                            swap(*ptr, *ptr->sig);
                        ptr = ptr->sig;
                    }*/
                    cout << setw(6) << "ID" << setw(12) << "Producto" << setw(6) << "PC" << setw(6) << "PV";
                    cout << setw(14) << "Existencia" << setw(6) << "NR" << endl;
                    ptr = inicio;
                    while (true) {
                        cout << setw(5) << ptr->id << setw(12) << ptr->producto << setw(7) << ptr->pc;
                        cout << setw(6) << ptr->pv << setw(9) << ptr->existencia << setw(11) << ptr->nr;
                        if (ptr->existencia <= ptr->nr)
                            cout << "*" << endl;
                        else
                            cout << endl;
                        if (ptr->sig == NULL)
                            break;
                        else
                            ptr = ptr->sig;
                    }
                }
                    while (true) {
                        cout << "Presione ''*'' para salir\n";
                        cin>>salir;
                        if (salir == "*")
                            break;
                    }
                    break;
                default: cout << "Opción Inválida\n";
            }
        } while (opc > 2);
    }
}

void Usuarios() {
    int opc;
    do {
        cout << "Administrador de Usuarios\n";
        cout << "1. Alta de Usuarios\n";
        cout << "2. Baja de Usuarios\n";
        cout << "3. Consulta de Usuarios\n";
        cout << "4. Modificación de Usuarios\n";
        cout << "5. Mostrar Usuarios\n";
        cout << "6. Regresar\n";
        cout << "Opción: ";
        cin>>opc;
        cout << endl;
        switch (opc) {
            case 1: Altas_u();
                break;
            case 2: Bajas_u();
                break;
            case 3: Consultas_u();
                break;
            case 4: Modificaciones_u();
                break;
            case 5: Mostrar_u();
                break;
            case 6:
                break;
            default: cout << "Opción Inválida\n";
        }
    } while (opc != 6);
}

void Altas_u() {
    string aux, tipo_temp;
    int pass1, pass2;
    int pos;
    int opc;
    cout << "Alta de Usuarios\n";
    while (true) {
        cout << "Ingrese Usuario: ";
        cin >> aux;
        if (aux == "*")
            break;
        pos = buscar_u(aux);
        if (pos == -1) {
            do {
                cout << "Ingrese contraseña del usuario " << aux << ": ";
                cin>>pass1;
                cout << "Confirme la contraseña: ";
                cin>>pass2;
                if (pass1 == pass2)
                    break;
                else
                    cout << "Las contraseñas no coinciden, intente de nuevo\n";
            } while (pass1 != pass2);
            do {
                cout << "Seleccione Tipo de Usuario: \n";
                cout << "1. Administrador\n";
                cout << "2. Vendedor\n";
                cout << "Opción: ";
                cin>>opc;
                switch (opc) {
                    case 1: tipo_temp = "Administrador";
                        break;
                    case 2: tipo_temp = "Vendedor";
                        break;
                    default: cout << "Opción Inválida\n";
                }
            } while (opc > 2);
            nuevo_u = new(user);
            strcpy(nuevo_u->usuario, aux.c_str());
            nuevo_u->pass = pass2;
            strcpy(nuevo_u->tipo, tipo_temp.c_str());
            total_u += 1;
            if (inicio_u == NULL) {
                inicio_u = nuevo_u;
                fin_u = nuevo_u;
                fin_u->sig = NULL;
                fin_u->ant = NULL;
            } else {
                nuevo_u->ant = fin_u;
                fin_u->sig = nuevo_u;
            }
            fin_u = nuevo_u;
            fin_u->sig = NULL;
            cout << "*El usuario ha sido dado de alta exitosamente*\n" << endl;
        } else
            cout << "El usuario " << aux << " ya está dado de alta\n" << endl;
    }
}

void Bajas_u() {
    int i;
    int pos;
    int opc;
    string aux;
    string tipo_temp;
    cout << "Baja de Usuarios\n";
    while (true) {
        cout << "Ingrese usuario: ";
        cin>>aux;
        if (aux == "*")
            break;
        pos = buscar_u(aux);
        cout << pos << endl;
        cout << "total: " << total_u << endl;
        if (pos == -1)
            cout << "El usuario: " << aux << ", no existe\n";
        else {
            while (true) {
                cout << "¿Seguro que desea dar de baja el usuario: " << aux << "?\n";
                cout << "1. Sí\n";
                cout << "2. No\n";
                cin >> opc;
                switch (opc) {
                    case 1:
                    {
                        total_u -= 1;
                        ptr_u = inicio_u;
                        i = 0;
                        while (i != pos) {
                            ptr_u = ptr_u->sig;
                            i++;
                        }
                        if (inicio_u == fin_u) { //Si solo hay un nodo en la lista
                            tipo_temp = ptr_u->tipo;
                            if (tipo_temp == "Administrador")
                                cout << "***No se puede eliminar, debe haber al menos 1 usuario Administrador***\n" << endl;
                            else {
                                delete(inicio_u);
                                inicio_u == NULL;
                                fin_u == NULL;
                                ptr_u == NULL;
                                nuevo_u == NULL;
                            }
                        } else {
                            if (ptr_u == inicio_u) { //Hay varios nodos, solo el 1° se borra
                                inicio_u = inicio_u->sig;
                                inicio_u->ant = NULL;
                                delete(ptr_u);
                            } else {
                                if (ptr_u == fin_u) { //Hay varios nodos, solo el último se borra
                                    delete(fin_u);
                                    fin_u = fin_u->ant;
                                    fin_u->sig = NULL;
                                } else { //Varios nodos, borrar X elemento de la lista
                                    ptr_u->ant->sig = ptr_u->sig;
                                    ptr_u->sig->ant = ptr_u->ant;
                                    delete(ptr_u);
                                }
                            }

                        }
                        cout << "*El usuario ha sido dado de baja exitosamente*\n" << endl;
                    }
                        break;
                    case 2:
                        break;
                    default: cout << "Opción Inválida\n";
                }
                if (opc < 3)
                    break;
            }
        }
    }
}

void Consultas_u() {
    string aux, tipo_temp;
    int pass1, pass2;
    int i;
    int pos;
    int opc;
    cout << "Consultas de Usuarios\n";
    while (true) {
        cout << "Ingrese usuario: ";
        cin>>aux;
        if (aux == "*")
            break;
        pos = buscar_u(aux);
        if (pos == -1) {
            while (true) {
                cout << "El usuario " << aux << " no existe. ¿Desea darlo de Alta?\n";
                cout << "1. Sí\n" << "2. No\n";
                cin >> opc;
                switch (opc) {
                    case 1:
                    {
                        do {
                            cout << "Ingrese contraseña del usuario " << aux << ": ";
                            cin>>pass1;
                            cout << "Confirme la contraseña: ";
                            cin>>pass2;
                            if (pass1 == pass2)
                                break;
                            else
                                cout << "Las contraseñas no coinciden, intente de nuevo\n";
                        } while (pass1 != pass2);
                        do {
                            cout << "Seleccione Tipo de Usuario: \n";
                            cout << "1. Administrador\n";
                            cout << "2. Vendedor\n";
                            cout << "Opción: ";
                            cin>>opc;
                            switch (opc) {
                                case 1: tipo_temp = "Administrador";
                                    break;
                                case 2: tipo_temp = "Vendedor";
                                    break;
                                default: cout << "Opción Inválida\n";
                            }
                        } while (opc > 2);
                        nuevo_u = new(user);
                        strcpy(nuevo_u->usuario, aux.c_str());
                        nuevo_u->pass = pass2;
                        strcpy(nuevo_u->tipo, tipo_temp.c_str());
                        total_u += 1;
                        if (inicio_u == NULL) {
                            inicio_u = nuevo_u;
                            fin_u = nuevo_u;
                            fin_u->sig = NULL;
                            fin_u->ant = NULL;
                        } else {
                            nuevo_u->ant = fin_u;
                            fin_u->sig = nuevo_u;
                        }
                        fin_u = nuevo_u;
                        fin_u->sig = NULL;
                        cout << "*El usuario ha sido dado de alta exitosamente*\n" << endl;
                    }
                        break;
                    case 2:
                        break;
                    default: cout << "Opción Inválida\n";
                }
                if (opc < 3)
                    break;
            }
        } else {

            cout << "Resultados del usuario: " << ptr_u->usuario << endl;
            cout << endl;
            cout << setw(10) << "Usuario" << setw(7) << "Pass" << setw(15) << "Tipo" << endl;
            cout << setw(10) << ptr_u->usuario << setw(6) << ptr_u->pass << setw(18) << ptr_u->tipo << endl;
            cout << endl;
        }
    }
}

void Modificaciones_u() {
    int opc;
    int pos;
    int pass1, pass2;
    int i;
    string aux;
    string tipo_temp;
    cout << "Modificación de Usuarios\n";
    while (true) {
        cout << "Ingrese Usuario a Modificar: ";
        cin>>aux;
        if (aux == "*")
            break;
        pos = buscar_u(aux);
        if (pos == -1)
            cout << "El usuario: " << aux << ", no existe\n";
        else {
            while (true) {
                cout << ptr_u->usuario << endl;
                cout << "Qué desea Modificar?\n";
                cout << "1. Contraseña\n";
                cout << "2. Tipo de Usuario\n";
                cout << "3. TODO\n";
                cout << "4. Regresar\n";
                cin>>opc;
                switch (opc) {
                    case 1:
                    {
                        cout << "La contraseña actual es: " << ptr_u->pass << endl;
                        do {
                            cout << "Ingrese la nueva contraseña: ";
                            cin>>pass1;
                            cout << "Confirme la contraseña: ";
                            cin>>pass2;
                            if (pass1 == pass2)
                                break;
                            else
                                cout << "Las contraseñas no coinciden, intente de nuevo\n";
                        } while (pass1 != pass2);
                        ptr_u->pass = pass2;
                        cout << "La contraseña ha sido cambiada\n" << endl;
                    }
                        break;
                    case 2:
                        tipo_temp = ptr_u->tipo;
                    {
                        if (tipo_temp == "Administrador") {
                            cout << "El usuario es de tipo: " << ptr_u->tipo << endl;
                            tipo_temp = "Vendedor";
                            strcpy(ptr_u->tipo, tipo_temp.c_str());
                            cout << "Se cambiará a tipo: " << ptr_u->tipo << endl;
                        } else {
                            cout << "El usuario es de tipo: " << ptr_u->tipo << endl;
                            tipo_temp = "Administrador";
                            strcpy(ptr_u->tipo, tipo_temp.c_str());
                            cout << "Se cambiará a tipo: " << ptr_u->tipo << endl;
                        }
                        cout << "El tipo de usuario ha sido modificado\n" << endl;
                    }
                        break;
                    case 3:
                        tipo_temp = ptr_u->tipo;
                    {
                        cout << "La contraseña actual es: " << ptr_u->pass << endl;
                        do {
                            cout << "Ingrese la nueva contraseña: ";
                            cin>>pass1;
                            cout << "Confirme la contraseña: ";
                            cin>>pass2;
                            if (pass1 == pass2)
                                break;
                            else
                                cout << "Las contraseñas no coinciden, intente de nuevo\n";
                        } while (pass1 != pass2);
                        ptr_u->pass = pass2;
                        cout << "La contraseña ha sido cambiada\n";
                        do {
                            cout << "Seleccione Tipo de Usuario: \n";
                            cout << "1. Administrador\n";
                            cout << "2. Vendedor\n";
                            cout << "Opción: ";
                            cin>>opc;
                            switch (opc) {
                                case 1:
                                {
                                    tipo_temp = "Administrador";
                                    strcpy(ptr_u->tipo, tipo_temp.c_str());
                                }
                                    break;
                                case 2:
                                {
                                    tipo_temp = "Vendedor";
                                    strcpy(ptr_u->tipo, tipo_temp.c_str());
                                }
                                    break;
                                default: cout << "Opción Inválida\n";
                            }
                        } while (opc > 2);
                    }
                        break;
                    case 4:
                        break;
                    default: cout << "Opción Inválida\n";
                }
                if (opc == 4)
                    break;
            }
            break;
        }
    }
}

void Mostrar_u() {
    string salir;
    cout << "Usuarios\n";
    cout << setw(10) << "Usuario" << setw(7) << "Pass" << setw(15) << "Tipo" << endl;
    ptr_u = inicio_u;
    while (ptr_u != NULL) {
        cout << setw(10) << ptr_u->usuario << setw(6) << ptr_u->pass << setw(18) << ptr_u->tipo << endl;
        ptr_u = ptr_u->sig;
    }
    while (true) {
        cout << "Presione ''*'' para salir\n";
        cin>>salir;
        if (salir == "*")
            break;
    }
}

void Ventas() {
    string client;
    string prod;
    int pos, opc;
    int cant;
    int i = 0, j = 0;
    int k = 0;
    int total_prod[i];
    int total_buyers = 0;
    time_t tSac = time(NULL);
    struct tm* tmP = localtime(&tSac);
    ptr_u->total_v = 0;
    cout << endl;
    cout << "Menú Ventas\n";
    while (true) {
        cout << "Ingrese nombre del cliente: ";
        cin>>client;
        if (client == "*")
            break;
        ptr_u->buyer[i].total_c = 0;
        ptr_u->buyer[i].subtotal_c[j] = 0;
        ptr_u->buyer[i].comprador = client;
        while (true) {
            cout << "Ingrese producto a Vender: ";
            cin>>prod;
            if (prod == "*")
                break;
            pos = buscar(prod);
            if (pos == -1)
                cout << "El producto; " << prod << " NO existe\n";
            else
                if (ptr->existencia < 1)
                cout << "No hay existencia disponible para este producto\n";
            else {
                ptr_u->buyer[i].item[j] = prod;
                cout << "Ingrese cantidad a vender: ";
                cin>>cant;
                if (cant > ptr->existencia) {
                    do {
                        cout << "No hay " << cant << " piezas disponibles, únicamente hay " << ptr->existencia;
                        cout << ". ¿Desea realizar venta?\n";
                        cout << "1. Sí\n";
                        cout << "2. No\n";
                        cin>>opc;
                        switch (opc) {
                            case 1:
                            {
                                ptr_u->buyer[i].subtotal_c[j] = ptr->existencia * ptr->pv;
                                total_costos = total_costos + (ptr->existencia * ptr->pc);
                                ptr_u->buyer[i].price[j] = ptr->pv;
                                ptr->existencia = 0;
                            }
                                break;
                            case 2:
                                break;
                            default:
                                cout << "Opción Inválida\n";
                        }
                    } while (opc > 2);
                } else {
                    ptr_u->buyer[i].subtotal_c[j] = cant * ptr->pv;
                    total_costos = total_costos + (cant * ptr->pc);
                    ptr_u->buyer[i].price[j] = ptr->pv;
                    ptr->existencia = ptr->existencia - cant;
                }
                j++;
            }
            cout << endl;
        }
        total_prod[i] = j;
        k = j;
        for (j = 0; j < k; j++)
            ptr_u->buyer[i].total_c = ptr_u->buyer[i].total_c + ptr_u->buyer[i].subtotal_c[j];

        cout << endl;
        cout << setw(23) << "Cliente: " << ptr_u->buyer[i].comprador << endl;
        cout << setw(10) << "Producto" << setw(12) << "Cantidad" << setw(17) << "Precio Unitario" << setw(10) << "Subtotal\n";
        for (j = 0; j < total_prod[i]; j++) {
            cout << setw(8) << ptr_u->buyer[i].item[j] << setw(10) << ptr_u->buyer[i].subtotal_c[j] / ptr_u->buyer[i].price[j];
            cout << setw(16) << ptr_u->buyer[i].price[j] << setw(10) << ptr_u->buyer[i].subtotal_c[j] << endl;
        }
        cout << setw(42) << "Total: $" << ptr_u->buyer[i].total_c << endl;
        cout << endl;
        i++;
        j = 0;
    }
    total_buyers = i;

    cout << endl;
    cout << setw(30) << "Abarrotes El Tocho\n";
    cout << setw(30) << "RFC: TOC980615EL1\n";
    cout << setw(30) << "Ticket de Venta\n";
    cout << setw(40) << "Hora: " << tmP->tm_hour << ":" << tmP->tm_min << ":" << tmP->tm_sec << endl;
    cout << setw(40) << "Fecha: " << tmP->tm_mday << "/" << tmP->tm_mon + 1 << "/" << tmP->tm_year + 1900 << endl;
    cout << "Vendedor: " << ptr_u->usuario << endl;
    for (i = 0; i < total_buyers; i++) {
        cout << endl;
        cout << setw(23) << "Cliente: " << ptr_u->buyer[i].comprador << endl;
        cout << setw(10) << "Producto" << setw(12) << "Cantidad" << setw(17) << "Precio Unitario" << setw(10) << "Subtotal\n";
        for (j = 0; j < total_prod[i]; j++) {
            cout << setw(8) << ptr_u->buyer[i].item[j] << setw(10) << ptr_u->buyer[i].subtotal_c[j] / ptr_u->buyer[i].price[j];
            cout << setw(16) << ptr_u->buyer[i].price[j] << setw(10) << ptr_u->buyer[i].subtotal_c[j] << endl;
        }
        cout << setw(42) << "Total: $" << ptr_u->buyer[i].total_c << endl;
        cout << endl;
        ptr_u->total_v = ptr_u->total_v + ptr_u->buyer[i].total_c;
    }
    cout << setw(40) << "Corte de caja del vendedor: $" << ptr_u->total_v << endl;
    cout << endl;
    gran_total = gran_total + ptr_u->total_v;
}

void Corte_caja() {
    cout << "---Gran Total: $" << gran_total << "---" << endl;
    cout << "---Gastos: $" << total_costos << "---" << endl;
    cout << "---Utilidad: $" << gran_total - total_costos << "---" << endl;
    cout << endl;
}

void Escritura_prods() {
    char prod[40];
    int id_t, pc_t, pv_t, existencia_t, nr_t;
    fstream archivo("productos.bin", ios::out | ios::binary);
    ptr = inicio;
    do {
        strcpy(prod, ptr->producto);
        id_t = ptr->id;
        pc_t = ptr->pc;
        pv_t = ptr->pv;
        existencia_t = ptr->existencia;
        nr_t = ptr->nr;
        archivo.write(prod, 40);
        archivo.write(reinterpret_cast<char *> (&id_t), sizeof (id_t));
        archivo.write(reinterpret_cast<char *> (&pc_t), sizeof (pc_t));
        archivo.write(reinterpret_cast<char *> (&pv_t), sizeof (pv_t));
        archivo.write(reinterpret_cast<char *> (&existencia_t), sizeof (existencia_t));
        archivo.write(reinterpret_cast<char *> (&nr_t), sizeof (nr_t));
        ptr = ptr->sig;
    } while (ptr != NULL);
    archivo.close();
}

void Escritura_users() {
    char user_t[40];
    char tipo_t[40];
    double pass_t;
    fstream archivo("usuarios.bin", ios::out | ios::binary);
    ptr_u = inicio_u;
    do {
        strcpy(user_t, ptr_u->usuario);
        pass_t = ptr_u->pass;
        strcpy(tipo_t, ptr_u->tipo);
        archivo.write(user_t, 40);
        archivo.write(reinterpret_cast<char *> (&pass_t), sizeof (pass_t));
        archivo.write(tipo_t, 40);
        ptr_u = ptr_u->sig;
    } while (ptr_u != NULL);
    archivo.close();
}