#ifndef RENDERPILA_H
#define RENDERPILA_H

#include <QPointer>
#include <QRectF>
#include <QPointF>
#include <QPen>
#include <QBrush>
#include <QList>

class QGraphicsScene;
class QGraphicsTextItem;
class QGraphicsEllipseItem;
class Pila;

class RenderPila {
public:
    RenderPila();

    void ConfigurarEscena(QGraphicsScene* escena, const QRectF& area);
    void ReorganizarDesdePila(const Pila& pila);
    void AnimarApilar(const Pila& pila);
    void AnimarDesapilar(const Pila& pila);
    void ResaltarIndice(int indiceDesdeTope);

private:
    double Margen = 40.0;
    void AjustarSceneRectPila(const Pila& pila);
    QPointF PosicionNodo(int indiceDesdeTope) const;
    void DibujarFlecha(int indiceOrigen, int indiceDestino);

private:
    QPointer<QGraphicsScene> Escena;
    QRectF Area;
    double RadioNodo = 24.0;
    double Separacion = 70.0;
    double InicioY = 120.0;
    QPen LapizNegro;
    QBrush BrochaBlanca;
    QBrush BrochaNegra;
    QList<QGraphicsEllipseItem*> Nodos;
};

#endif // RENDERPILA_H
