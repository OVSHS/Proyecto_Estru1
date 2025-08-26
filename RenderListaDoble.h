#ifndef RENDERLISTADOBLE_H
#define RENDERLISTADOBLE_H
#pragma once
#include <QGraphicsScene>
#include <QVector>
#include <QPen>
#include <QBrush>
#include <QPointF>
#include <QRectF>
#include "ListaDoble.h"

class RenderListaDoble {
public:
    RenderListaDoble();

    void ConfigurarEscena(QGraphicsScene* escenaDestino, const QRectF& areaInicial);
    void ReorganizarDesdeListaDoble(const ListaDoble& lista);
    void AnimarAgregarAlFinal(const ListaDoble& lista);
    void AnimarInsertarEnPosicion(int posicionObjetivo, const ListaDoble& lista);
    void AnimarEliminarEnPosicion(int posicionObjetivo, const ListaDoble& lista);
    void ResaltarIndice(int indiceObjetivo);

private:
    QGraphicsScene* Escena;
    QRectF AreaBase;
    QVector<QGraphicsEllipseItem*> ElementosNodo;
    double RadioNodo = 18.0;
    double SeparacionNodos = 72.0;
    double CentroY = 230.0;
    double DesfaseFlechas = 20.0;
    QPen LapizNegro;
    QBrush BrochaBlanca;

    QPointF PosicionNodo(int indiceObjetivo) const;
    void DibujarFlechaAdelante(int indiceOrigen, int indiceDestino);
    void DibujarFlechaAtras(int indiceOrigen, int indiceDestino);
    void DibujarNullExtremos(int cantidadNodos);
    void AjustarSceneRect(const ListaDoble& lista);
};

#endif // RENDERLISTADOBLE_H
