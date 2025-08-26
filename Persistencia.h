#ifndef PERSISTENCIA_H
#define PERSISTENCIA_H

#include <QString>

class ListaSimple;
class Pila;
class Cola;
class ListaDoble;

class Persistencia {
public:
    static bool GuardarListaSimple(const QString& Ruta, const ListaSimple& Lista);
    static bool CargarListaSimple(const QString& Ruta, ListaSimple& Lista);

    static bool GuardarPila(const QString& Ruta, const Pila& Estructura);
    static bool CargarPila(const QString& Ruta, Pila& Estructura);

    static bool GuardarCola(const QString& RutaArchivo, const Cola& EstructuraCola);
    static bool CargarCola(const QString& RutaArchivo, Cola& EstructuraCola);

    static bool GuardarListaDoble(const QString& Ruta, const ListaDoble& Lista);
    static bool CargarListaDoble(const QString& Ruta, ListaDoble& Lista);
};

#endif
