#ifndef RENDERLISTA_H
#define RENDERLISTA_H

#include <QPointer>
#include <QRectF>
#include <QPointF>
#include <QPen>
#include <QBrush>
#include <QList>

class QGraphicsScene;
class QGraphicsTextItem;
class QGraphicsEllipseItem;
class ListaSimple;

class RenderLista {
public:
    RenderLista();

    void ConfigurarEscena(QGraphicsScene* escena, const QRectF& area);
    void ReorganizarDesdeLista(const ListaSimple& lista);
    void AnimarAgregarAlFinal(const ListaSimple& lista);
    void ResaltarIndice(int indice);
    void AnimarEliminarEnPosicion(int indice, const ListaSimple& lista);
    void AnimarInsertarEnPosicion(int posicion, const ListaSimple& lista);

private:
    QPointF PosicionNodo(int indice) const;
    void DibujarFlecha(int indiceOrigen, int indiceDestino);
    void ActualizarNull(int indiceUltimo);

private:
    QPointer<QGraphicsScene> Escena;
    QRectF Area;
    double RadioNodo = 24.0;
    double Separacion = 100.0;
    double InicioX = 180.0;
    QGraphicsTextItem* TextoNull = nullptr;
    QPen LapizNegro;
    QBrush BrochaBlanca;
    QBrush BrochaNegra;
    QList<QGraphicsEllipseItem*> Nodos;
};

#endif
