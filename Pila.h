#ifndef PILA_H
#define PILA_H
class Nodo;

class Pila {
public:
    Pila();
    ~Pila();

    bool Apilar(int valor);
    bool Desapilar(int& valorSalida);
    bool Mirar(int& valorSalida) const;
    int  Buscar(int valor) const;
    int  Tamano() const;
    bool EstaVacia() const;
    void Limpiar();
    bool ObtenerValorDesdeTope(int posicion, int& valor) const;

private:
    Nodo* Tope;
    int Cantidad;

    Pila(const Pila&) = delete;
    Pila& operator=(const Pila&) = delete;
};


#endif // PILA_H
