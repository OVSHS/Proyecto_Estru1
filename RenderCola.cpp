#include "RenderCola.h"
#include <QVariantAnimation>
#include <QGraphicsTextItem>
#include <QTimer>

RenderCola::RenderCola() : Escena(nullptr), LapizNegro(Qt::black), BrochaBlanca(Qt::white) {}

void RenderCola::ConfigurarEscena(QGraphicsScene* escena, const QRectF& area) {
    Escena = escena;
    Area = area;
    if (Escena) Escena->setSceneRect(Area);
}

QPointF RenderCola::PosicionNodo(int indice) const {
    double x = 160.0 + indice * (RadioNodo * 2.0 + Separacion);
    double y = CentroY;
    return QPointF(x, y);
}

void RenderCola::DibujarFlecha(int desde, int hasta) {
    QPointF a = PosicionNodo(desde);
    QPointF b = PosicionNodo(hasta);
    double sx = a.x() + RadioNodo;
    double ex = b.x() - RadioNodo;
    double y = a.y();
    Escena->addLine(sx, y, ex, y, LapizNegro);
    double t = 8.0;
    Escena->addLine(ex, y, ex - t, y - t, LapizNegro);
    Escena->addLine(ex, y, ex - t, y + t, LapizNegro);
}

void RenderCola::AjustarSceneRectCola(const Cola& cola) {
    int c = cola.Tamano();
    double ancho = 160.0 + (c > 0 ? (c - 1) * (RadioNodo * 2.0 + Separacion) + RadioNodo : 0) + 140.0;
    QRectF r(Area.left(), Area.top(), std::max(Area.width(), ancho), Area.height());
    Escena->setSceneRect(r);
}

void RenderCola::ReorganizarDesdeCola(const Cola& cola) {
    if (!Escena) return;
    Escena->clear();
    Nodos.clear();
    int c = cola.Tamano();
    for (int i = 0; i < c; ++i) {
        int v = 0;
        if (!cola.ObtenerValorDesdeFrente(i, v)) continue;
        QPointF p = PosicionNodo(i);
        auto* e = Escena->addEllipse(-RadioNodo, -RadioNodo, RadioNodo*2.0, RadioNodo*2.0, LapizNegro, BrochaBlanca);
        e->setPos(p);
        Nodos.append(e);
        auto* t = new QGraphicsTextItem(QString::number(v), e);
        t->setDefaultTextColor(Qt::black);
        QRectF br = t->boundingRect();
        t->setPos(-br.width()/2.0, -br.height()/2.0);
        if (i > 0) DibujarFlecha(i - 1, i);
    }
    auto* n = Escena->addText("null");
    n->setDefaultTextColor(Qt::black);
    QPointF pnull = PosicionNodo(std::max(0, c - 1));
    n->setPos(pnull.x() + 60.0, pnull.y() - 8.0);
    AjustarSceneRectCola(cola);
}

void RenderCola::AnimarEncolar(const Cola& cola) {
    if (!Escena) return;
    int c = cola.Tamano();
    if (c <= 0) return;

    int indiceNuevo = c - 1;
    QPointF destino = PosicionNodo(indiceNuevo);
    QPointF inicio(destino.x() + 160.0, destino.y());

    auto* e = Escena->addEllipse(-RadioNodo, -RadioNodo, RadioNodo*2.0, RadioNodo*2.0, LapizNegro, BrochaBlanca);
    e->setOpacity(0.0);
    e->setPos(inicio);
    int valor = 0;
    cola.ObtenerValorDesdeFrente(indiceNuevo, valor);
    auto* t = new QGraphicsTextItem(QString::number(valor), e);
    t->setDefaultTextColor(Qt::black);
    QRectF br = t->boundingRect();
    t->setPos(-br.width()/2.0, -br.height()/2.0);

    auto* mover = new QVariantAnimation(Escena);
    mover->setDuration(350);
    mover->setStartValue(inicio);
    mover->setEndValue(destino);
    QObject::connect(mover, &QVariantAnimation::valueChanged, [e](const QVariant& v){ e->setPos(v.toPointF()); });
    mover->start(QAbstractAnimation::DeleteWhenStopped);

    auto* apar = new QVariantAnimation(Escena);
    apar->setDuration(350);
    apar->setStartValue(0.0);
    apar->setEndValue(1.0);
    QObject::connect(apar, &QVariantAnimation::valueChanged, [e](const QVariant& v){ e->setOpacity(v.toDouble()); });
    QObject::connect(apar, &QVariantAnimation::finished, [this, &cola](){ ReorganizarDesdeCola(cola); });
    apar->start(QAbstractAnimation::DeleteWhenStopped);
}

void RenderCola::AnimarDesencolar(const Cola& cola) {
    if (!Escena) return;
    if (Nodos.isEmpty()) { ReorganizarDesdeCola(cola); return; }

    QGraphicsEllipseItem* primero = Nodos.first();
    QPointF pos0 = primero->pos();
    QPointF fuera(pos0.x() - 160.0, pos0.y());

    auto* salida = new QVariantAnimation(Escena);
    salida->setDuration(350);
    salida->setStartValue(pos0);
    salida->setEndValue(fuera);
    QObject::connect(salida, &QVariantAnimation::valueChanged, [primero](const QVariant& v){ primero->setPos(v.toPointF()); });

    auto* fade = new QVariantAnimation(Escena);
    fade->setDuration(350);
    fade->setStartValue(1.0);
    fade->setEndValue(0.0);
    QObject::connect(fade, &QVariantAnimation::valueChanged, [primero](const QVariant& v){ primero->setOpacity(v.toDouble()); });
    QObject::connect(fade, &QVariantAnimation::finished, [this, &cola](){ ReorganizarDesdeCola(cola); });

    salida->start(QAbstractAnimation::DeleteWhenStopped);
    fade->start(QAbstractAnimation::DeleteWhenStopped);
}

void RenderCola::ResaltarIndice(int indiceObjetivo) {
    if (indiceObjetivo < 0 || indiceObjetivo >= Nodos.size()) return;
    QGraphicsEllipseItem* nodoDestino = Nodos.at(indiceObjetivo);
    QBrush brochaAnterior = nodoDestino->brush();
    nodoDestino->setBrush(QBrush(Qt::yellow));
    QTimer::singleShot(600, [nodoDestino, brochaAnterior](){ nodoDestino->setBrush(brochaAnterior); });
}
