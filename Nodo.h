#ifndef NODO_H
#define NODO_H

class Nodo {
public:
    explicit Nodo(int valor);
    ~Nodo();

    int   Valor;
    Nodo* Siguiente;

private:
    Nodo(const Nodo&) = delete;
    Nodo& operator=(const Nodo&) = delete;
};

#endif
