#ifndef LISTASIMPLE_H
#define LISTASIMPLE_H

class Nodo;

class ListaSimple {
public:
    ListaSimple();
    ~ListaSimple();

    bool AgregarAlFinal(int valor);
    bool QuitarAlInicio(int& valorSalida);
    void Limpiar();

    bool EliminarPorValor(int valor);
    bool InsertarPorPosicion(int posicion, int valor);
    int  Buscar(int valor) const;

    int Tamano() const;
    bool EstaVacia() const;
    bool ObtenerValorEn(int indice, int& valor) const;

private:
    Nodo* Cabeza;
    Nodo* Cola;
    int Cantidad;

    ListaSimple(const ListaSimple&) = delete;
    ListaSimple& operator=(const ListaSimple&) = delete;
};

#endif
