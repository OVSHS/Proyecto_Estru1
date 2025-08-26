#ifndef LISTADOBLE_H
#define LISTADOBLE_H
#pragma once

class ListaDoble {
public:
    ListaDoble();
    ~ListaDoble();

    bool AgregarAlFinal(int valorNuevo);
    bool InsertarPorPosicion(int posicionObjetivo, int valorNuevo);
    bool EliminarPorValor(int valorBuscado);

    int Tamano() const;
    bool EstaVacia() const;
    void Limpiar();

    int Buscar(int valorBuscado) const;
    bool ObtenerValorEn(int indiceBuscado, int& valorSalida) const;

private:
    struct Nodo {
        int Valor;
        Nodo* Siguiente;
        Nodo* Anterior;
    };

    Nodo* Cabeza;
    Nodo* Cola;
    int Cantidad;
};

#endif // LISTADOBLE_H
