#include "ListaDoble.h"

ListaDoble::ListaDoble() : Cabeza(nullptr), Cola(nullptr), Cantidad(0) {}
ListaDoble::~ListaDoble() { Limpiar(); }

bool ListaDoble::AgregarAlFinal(int valorNuevo) {
    if (valorNuevo < 0) return false;
    Nodo* nodoNuevo = new Nodo{valorNuevo, nullptr, Cola};
    if (!Cabeza) {
        Cabeza = Cola = nodoNuevo;
    } else {
        Cola->Siguiente = nodoNuevo;
        Cola = nodoNuevo;
    }
    Cantidad = Cantidad + 1;
    return true;
}

bool ListaDoble::InsertarPorPosicion(int posicionObjetivo, int valorNuevo) {
    if (valorNuevo < 0) return false;
    if (posicionObjetivo < 0 || posicionObjetivo > Cantidad) return false;
    if (posicionObjetivo == Cantidad) return AgregarAlFinal(valorNuevo);

    Nodo* nodoNuevo = new Nodo{valorNuevo, nullptr, nullptr};
    if (posicionObjetivo == 0) {
        nodoNuevo->Siguiente = Cabeza;
        nodoNuevo->Anterior = nullptr;
        if (Cabeza) Cabeza->Anterior = nodoNuevo;
        Cabeza = nodoNuevo;
        if (!Cola) Cola = nodoNuevo;
        Cantidad = Cantidad + 1;
        return true;
    }

    Nodo* nodoActual = Cabeza;
    int indiceActual = 0;
    while (indiceActual < posicionObjetivo) {
        nodoActual = nodoActual->Siguiente;
        indiceActual = indiceActual + 1;
    }
    Nodo* nodoAnterior = nodoActual->Anterior;
    nodoNuevo->Siguiente = nodoActual;
    nodoNuevo->Anterior = nodoAnterior;
    if (nodoAnterior) nodoAnterior->Siguiente = nodoNuevo;
    if (nodoActual) nodoActual->Anterior = nodoNuevo;
    Cantidad = Cantidad + 1;
    return true;
}

bool ListaDoble::EliminarPorValor(int valorBuscado) {
    if (!Cabeza) return false;
    Nodo* nodoActual = Cabeza;
    while (nodoActual && nodoActual->Valor != valorBuscado) {
        nodoActual = nodoActual->Siguiente;
    }
    if (!nodoActual) return false;

    Nodo* nodoAnterior = nodoActual->Anterior;
    Nodo* nodoSiguiente = nodoActual->Siguiente;

    if (nodoAnterior) nodoAnterior->Siguiente = nodoSiguiente;
    else Cabeza = nodoSiguiente;

    if (nodoSiguiente) nodoSiguiente->Anterior = nodoAnterior;
    else Cola = nodoAnterior;

    delete nodoActual;
    Cantidad = Cantidad - 1;
    return true;
}

int ListaDoble::Tamano() const { return Cantidad; }
bool ListaDoble::EstaVacia() const { return Cantidad == 0; }

void ListaDoble::Limpiar() {
    Nodo* nodoActual = Cabeza;
    while (nodoActual) {
        Nodo* nodoSiguiente = nodoActual->Siguiente;
        delete nodoActual;
        nodoActual = nodoSiguiente;
    }
    Cabeza = nullptr;
    Cola = nullptr;
    Cantidad = 0;
    return;
}

int ListaDoble::Buscar(int valorBuscado) const {
    int indiceActual = 0;
    Nodo* nodoActual = Cabeza;
    while (nodoActual) {
        if (nodoActual->Valor == valorBuscado) return indiceActual;
        nodoActual = nodoActual->Siguiente;
        indiceActual = indiceActual + 1;
    }
    return -1;
}

bool ListaDoble::ObtenerValorEn(int indiceBuscado, int& valorSalida) const {
    if (indiceBuscado < 0 || indiceBuscado >= Cantidad) return false;
    Nodo* nodoActual = Cabeza;
    int indiceActual = 0;
    while (indiceActual < indiceBuscado) {
        nodoActual = nodoActual->Siguiente;
        indiceActual = indiceActual + 1;
    }
    valorSalida = nodoActual->Valor;
    return true;
}
