#ifndef COLA_H
#define COLA_H
#pragma once
#include <cstddef>

class Cola {
public:
    Cola();
    ~Cola();
    bool Encolar(int valor);
    bool Desencolar(int& valorSalida);
    bool MirarFrente(int& valor) const;
    int Tamano() const;
    bool EstaVacia() const;
    void Limpiar();
    bool ObtenerValorDesdeFrente(int indice, int& valor) const;
    int Buscar(int valor) const;

private:
    struct Nodo {
        int Valor;
        Nodo* Siguiente;
    };
    Nodo* Frente;
    Nodo* Fondo;
    int Cantidad;
};

#endif // COLA_H
