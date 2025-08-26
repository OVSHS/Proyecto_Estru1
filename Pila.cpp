#include "Pila.h"
#include "Nodo.h"

Pila::Pila() : Tope(nullptr), Cantidad(0) {}
Pila::~Pila() { Limpiar(); }

bool Pila::Apilar(int valor) {
    bool OperacionExitosa = false;
    if (valor >= 0) {
        Nodo* NuevoNodo = new Nodo(valor);
        NuevoNodo->Siguiente = Tope;
        Tope = NuevoNodo;
        ++Cantidad;
        OperacionExitosa = true;
    }
    return OperacionExitosa;
}

bool Pila::Desapilar(int& valorSalida) {
    bool OperacionExitosa = false;
    if (Tope) {
        Nodo* NodoEliminar = Tope;
        valorSalida = NodoEliminar->Valor;
        Tope = Tope->Siguiente;
        delete NodoEliminar;
        --Cantidad;
        OperacionExitosa = true;
    }
    return OperacionExitosa;
}

bool Pila::Mirar(int& valorSalida) const {
    bool OperacionExitosa = false;
    if (Tope) {
        valorSalida = Tope->Valor;
        OperacionExitosa = true;
    }
    return OperacionExitosa;
}

int Pila::Buscar(int valor) const {
    int IndiceDesdeTope = 0;
    Nodo* Actual = Tope;
    while (Actual) {
        if (Actual->Valor == valor) return IndiceDesdeTope;
        Actual = Actual->Siguiente;
        ++IndiceDesdeTope;
    }
    return -1;
}

int Pila::Tamano() const { return Cantidad; }
bool Pila::EstaVacia() const { return Cantidad == 0; }

void Pila::Limpiar() {
    while (Tope) {
        Nodo* Temp = Tope;
        Tope = Tope->Siguiente;
        delete Temp;
    }
    Cantidad = 0;
}

bool Pila::ObtenerValorDesdeTope(int posicion, int& valor) const {
    bool OperacionExitosa = false;
    if (posicion >= 0 && posicion < Cantidad) {
        Nodo* Actual = Tope;
        int Paso = 0;
        while (Paso < posicion && Actual) {
            Actual = Actual->Siguiente;
            ++Paso;
        }
        if (Actual) {
            valor = Actual->Valor;
            OperacionExitosa = true;
        }
    }
    return OperacionExitosa;
}
