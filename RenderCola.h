#ifndef RENDERCOLA_H
#define RENDERCOLA_H
#pragma once
#include <QGraphicsScene>
#include <QVector>
#include <QPen>
#include <QBrush>
#include "Cola.h"

class RenderCola {
public:
    RenderCola();
    void ConfigurarEscena(QGraphicsScene* escena, const QRectF& area);
    void ReorganizarDesdeCola(const Cola& cola);
    void AnimarEncolar(const Cola& cola);
    void AnimarDesencolar(const Cola& cola);
    void ResaltarIndice(int indice);

private:
    QGraphicsScene* Escena;
    QRectF Area;
    QVector<QGraphicsEllipseItem*> Nodos;
    double RadioNodo = 18.0;
    double Separacion = 70.0;
    double CentroY = 220.0;
    QPen LapizNegro;
    QBrush BrochaBlanca;

    QPointF PosicionNodo(int indice) const;
    void DibujarFlecha(int desde, int hasta);
    void AjustarSceneRectCola(const Cola& cola);
};
#endif // RENDERCOLA_H
