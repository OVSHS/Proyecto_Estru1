#include "RenderPila.h"
#include "Pila.h"
#include <QGraphicsScene>
#include <QGraphicsEllipseItem>
#include <QGraphicsTextItem>
#include <QGraphicsPolygonItem>
#include <QVariantAnimation>
#include <QTimer>
#include <QString>

RenderPila::RenderPila() {
    LapizNegro = QPen(Qt::black);
    BrochaBlanca = QBrush(Qt::white);
    BrochaNegra = QBrush(Qt::black);
}

void RenderPila::ConfigurarEscena(QGraphicsScene* escena, const QRectF& area) {
    Escena = escena;
    Area = area;
    if (Escena) {
        Escena->setSceneRect(Area);
        Escena->setBackgroundBrush(Qt::white);
    }
}

QPointF RenderPila::PosicionNodo(int indiceDesdeTope) const {
    double X = Area.center().x();
    double Y = InicioY + indiceDesdeTope * (RadioNodo * 2.0 + Separacion);
    return QPointF(X, Y);
}

void RenderPila::DibujarFlecha(int indiceOrigen, int indiceDestino) {
    if (!Escena) return;
    QPointF A = PosicionNodo(indiceOrigen);
    QPointF B = PosicionNodo(indiceDestino);
    Escena->addLine(A.x(), A.y() + RadioNodo, B.x(), B.y() - RadioNodo, LapizNegro);
    double X = B.x();
    double Y = B.y() - RadioNodo;
    double S = 8.0;
    QPolygonF P;
    P << QPointF(X, Y) << QPointF(X - S * 0.6, Y - S) << QPointF(X + S * 0.6, Y - S);
    Escena->addPolygon(P, LapizNegro, BrochaNegra);
}

void RenderPila::AjustarSceneRectPila(const Pila& pila) {
    int Cantidad = pila.Tamano();
    double AltoContenido = InicioY
                           + (Cantidad > 0 ? (Cantidad - 1) * (RadioNodo * 2.0 + Separacion) + RadioNodo : 0)
                           + Margen;
    QRectF RectNuevo(Area.left(), Area.top(),
                     Area.width(),
                     std::max(Area.height(), AltoContenido));
    Escena->setSceneRect(RectNuevo);
}

void RenderPila::ReorganizarDesdePila(const Pila& pila) {
    if (!Escena) return;
    Escena->clear();
    Nodos.clear();

    int Cantidad = pila.Tamano();
    for (int Indice = 0; Indice < Cantidad; ++Indice) {
        int Valor = 0;
        if (!pila.ObtenerValorDesdeTope(Indice, Valor)) continue;

        QPointF Centro = PosicionNodo(Indice);
        auto* NodoGrafico = Escena->addEllipse(-RadioNodo, -RadioNodo, RadioNodo * 2.0, RadioNodo * 2.0, LapizNegro, BrochaBlanca);
        NodoGrafico->setPos(Centro);
        Nodos.append(NodoGrafico);

        auto* Texto = new QGraphicsTextItem(QString::number(Valor), NodoGrafico);
        Texto->setDefaultTextColor(Qt::black);
        QRectF Br = Texto->boundingRect();
        Texto->setPos(-Br.width() / 2.0, -Br.height() / 2.0);

        if (Indice > 0) DibujarFlecha(Indice - 1, Indice);
    }

    AjustarSceneRectPila(pila);
}

void RenderPila::AnimarApilar(const Pila& pila) {
    if (!Escena) return;
    int Cantidad = pila.Tamano();
    if (Cantidad <= 0) return;

    for (int Indice = 0; Indice < Nodos.size(); ++Indice) {
        QGraphicsEllipseItem* Nodo = Nodos.at(Indice);
        QPointF Destino = PosicionNodo(Indice + 1);
        auto* Movimiento = new QVariantAnimation(Escena);
        Movimiento->setDuration(350);
        Movimiento->setStartValue(Nodo->pos());
        Movimiento->setEndValue(Destino);
        QObject::connect(Movimiento, &QVariantAnimation::valueChanged, [Nodo](const QVariant& V){ Nodo->setPos(V.toPointF()); });
        Movimiento->start(QAbstractAnimation::DeleteWhenStopped);
    }

    int ValorTope = 0;
    pila.ObtenerValorDesdeTope(0, ValorTope);
    QPointF DestinoNuevo = PosicionNodo(0);
    QPointF InicioNuevo(DestinoNuevo.x(), DestinoNuevo.y() - 160.0);

    auto* NodoNuevo = Escena->addEllipse(-RadioNodo, -RadioNodo, RadioNodo * 2.0, RadioNodo * 2.0, LapizNegro, BrochaBlanca);
    NodoNuevo->setOpacity(0.0);
    NodoNuevo->setPos(InicioNuevo);

    auto* Texto = new QGraphicsTextItem(QString::number(ValorTope), NodoNuevo);
    Texto->setDefaultTextColor(Qt::black);
    QRectF Br = Texto->boundingRect();
    Texto->setPos(-Br.width() / 2.0, -Br.height() / 2.0);

    auto* Caida = new QVariantAnimation(Escena);
    Caida->setDuration(350);
    Caida->setStartValue(InicioNuevo);
    Caida->setEndValue(DestinoNuevo);
    QObject::connect(Caida, &QVariantAnimation::valueChanged, [NodoNuevo](const QVariant& V){ NodoNuevo->setPos(V.toPointF()); });
    Caida->start(QAbstractAnimation::DeleteWhenStopped);

    auto* Aparicion = new QVariantAnimation(Escena);
    Aparicion->setDuration(350);
    Aparicion->setStartValue(0.0);
    Aparicion->setEndValue(1.0);
    QObject::connect(Aparicion, &QVariantAnimation::valueChanged, [NodoNuevo](const QVariant& V){ NodoNuevo->setOpacity(V.toDouble()); });
    QObject::connect(Aparicion, &QVariantAnimation::finished, [this, &pila](){ ReorganizarDesdePila(pila); });
    Aparicion->start(QAbstractAnimation::DeleteWhenStopped);
}


void RenderPila::AnimarDesapilar(const Pila& pila) {
    if (!Escena) return;
    if (Nodos.isEmpty()) { ReorganizarDesdePila(pila); return; }

    QGraphicsEllipseItem* NodoEliminar = Nodos.first();

    auto* Salida = new QVariantAnimation(Escena);
    Salida->setDuration(350);
    Salida->setStartValue(1.0);
    Salida->setEndValue(0.0);
    QObject::connect(Salida, &QVariantAnimation::valueChanged, [NodoEliminar](const QVariant& V){ NodoEliminar->setOpacity(V.toDouble()); });
    Salida->start(QAbstractAnimation::DeleteWhenStopped);

    QPointF PosInicial = NodoEliminar->pos();
    QPointF PosFuera(PosInicial.x(), PosInicial.y() - 160.0);

    auto* Subida = new QVariantAnimation(Escena);
    Subida->setDuration(350);
    Subida->setStartValue(PosInicial);
    Subida->setEndValue(PosFuera);
    QObject::connect(Subida, &QVariantAnimation::valueChanged, [NodoEliminar](const QVariant& V){ NodoEliminar->setPos(V.toPointF()); });
    QObject::connect(Subida, &QVariantAnimation::finished, [this, &pila](){ ReorganizarDesdePila(pila); });
    Subida->start(QAbstractAnimation::DeleteWhenStopped);
}


void RenderPila::ResaltarIndice(int indiceDesdeTope) {
    if (indiceDesdeTope < 0 || indiceDesdeTope >= Nodos.size()) return;
    QGraphicsEllipseItem* NodoGrafico = Nodos.at(indiceDesdeTope);
    QBrush BrochaAnterior = NodoGrafico->brush();
    NodoGrafico->setBrush(QBrush(Qt::yellow));
    QTimer::singleShot(600, [NodoGrafico, BrochaAnterior](){ NodoGrafico->setBrush(BrochaAnterior); });
}
