#include "Cola.h"

Cola::Cola() : Frente(nullptr), Fondo(nullptr), Cantidad(0) {}
Cola::~Cola() { Limpiar(); }

bool Cola::Encolar(int valor) {
    if (valor < 0) return false;
    Nodo* nuevo = new Nodo{valor, nullptr};
    if (!Frente) {
        Frente = Fondo = nuevo;
    } else {
        Fondo->Siguiente = nuevo;
        Fondo = nuevo;
    }
    Cantidad += 1;
    return true;
}

bool Cola::Desencolar(int& valorSalida) {
    if (!Frente) return false;
    Nodo* tmp = Frente;
    valorSalida = tmp->Valor;
    Frente = Frente->Siguiente;
    if (!Frente) Fondo = nullptr;
    delete tmp;
    Cantidad -= 1;
    return true;
}

bool Cola::MirarFrente(int& valor) const {
    if (!Frente) return false;
    valor = Frente->Valor;
    return true;
}

int Cola::Tamano() const { return Cantidad; }
bool Cola::EstaVacia() const { return Cantidad == 0; }

void Cola::Limpiar() {
    while (Frente) {
        Nodo* tmp = Frente;
        Frente = Frente->Siguiente;
        delete tmp;
    }
    Fondo = nullptr;
    Cantidad = 0;
}

bool Cola::ObtenerValorDesdeFrente(int indice, int& valor) const {
    if (indice < 0 || indice >= Cantidad) return false;
    Nodo* actual = Frente;
    for (int i = 0; i < indice; ++i) actual = actual->Siguiente;
    valor = actual->Valor;
    return true;
}

int Cola::Buscar(int valor) const {
    int indice = 0;
    Nodo* actual = Frente;
    while (actual) {
        if (actual->Valor == valor) return indice;
        actual = actual->Siguiente;
        indice += 1;
    }
    return -1;
}
