#include "ListaSimple.h"
#include "Nodo.h"

ListaSimple::ListaSimple()
    : Cabeza(nullptr), Cola(nullptr), Cantidad(0) {}

ListaSimple::~ListaSimple() {
    Limpiar();
}

bool ListaSimple::AgregarAlFinal(int valor) {
    if (valor < 0)
        return false;
    Nodo* nuevo = new Nodo(valor);
    if (!Cabeza) {
        Cabeza = Cola = nuevo;
    } else {
        Cola->Siguiente = nuevo;
        Cola = nuevo;
    }
    ++Cantidad;
    return true;
}

bool ListaSimple::QuitarAlInicio(int& valorSalida) {
    if (!Cabeza)
        return false;
    Nodo* temp = Cabeza;
    valorSalida = temp->Valor;
    Cabeza = Cabeza->Siguiente;
    if (!Cabeza) Cola = nullptr;
    delete temp;
    --Cantidad;
    return true;
}

void ListaSimple::Limpiar() {
    while (Cabeza) {
        Nodo* temp = Cabeza;
        Cabeza = Cabeza->Siguiente;
        delete temp;
    }
    Cola = nullptr;
    Cantidad = 0;
}

bool ListaSimple::EliminarPorValor(int valor) {
    if (!Cabeza) return false;
    Nodo* actual = Cabeza;
    Nodo* anterior = nullptr;
    while (actual && actual->Valor != valor) {
        anterior = actual;
        actual = actual->Siguiente;
    }
    if (!actual) return false;
    if (!anterior) {
        Cabeza = actual->Siguiente;
        if (!Cabeza) Cola = nullptr;
    } else {
        anterior->Siguiente = actual->Siguiente;
        if (actual == Cola) Cola = anterior;
    }
    delete actual;
    --Cantidad;
    return true;
}

bool ListaSimple::InsertarPorPosicion(int posicion, int valor) {
    if (valor < 0)
        return false;
    if (posicion < 0 || posicion > Cantidad)
        return false;
    Nodo* nuevo = new Nodo(valor);
    if (posicion == 0) {
        nuevo->Siguiente = Cabeza;
        Cabeza = nuevo;
        if (Cantidad == 0) Cola = nuevo;
        ++Cantidad;
        return true;
    }
    if (posicion == Cantidad) {
        Cola->Siguiente = nuevo;
        Cola = nuevo;
        ++Cantidad;
        return true;
    }
    Nodo* actual = Cabeza;
    for (int i = 0; i < posicion - 1; ++i) actual = actual->Siguiente;
    nuevo->Siguiente = actual->Siguiente;
    actual->Siguiente = nuevo;
    ++Cantidad;
    return true;
}

int ListaSimple::Buscar(int valor) const {
    int i = 0;
    for (Nodo* n = Cabeza; n; n = n->Siguiente, ++i) {
        if (n->Valor == valor)
            return i;
    }
    return -1;
}

int ListaSimple::Tamano() const {
    return Cantidad;
}

bool ListaSimple::EstaVacia() const {
    return Cantidad == 0;
}

bool ListaSimple::ObtenerValorEn(int indice, int& valor) const {
    if (indice < 0 || indice >= Cantidad)
        return false;
    Nodo* actual = Cabeza;
    for (int i = 0; i < indice; ++i)
        actual = actual->Siguiente;
    valor = actual->Valor;
    return true;
}
