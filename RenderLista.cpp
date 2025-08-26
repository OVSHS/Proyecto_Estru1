#include "RenderLista.h"
#include "ListaSimple.h"

#include <QGraphicsScene>
#include <QGraphicsEllipseItem>
#include <QGraphicsTextItem>
#include <QGraphicsPolygonItem>
#include <QVariantAnimation>
#include <QTimer>
#include <QString>

RenderLista::RenderLista() {
    LapizNegro = QPen(Qt::black);
    BrochaBlanca = QBrush(Qt::white);
    BrochaNegra = QBrush(Qt::black);
}

void RenderLista::ConfigurarEscena(QGraphicsScene* escena, const QRectF& area) {
    Escena = escena;
    Area = area;
    if (Escena) {
        Escena->setSceneRect(Area);
        Escena->setBackgroundBrush(Qt::white);
    }
}

QPointF RenderLista::PosicionNodo(int indice) const {
    double X = InicioX + indice * (RadioNodo * 2.0 + Separacion);
    double Y = Area.center().y();
    return QPointF(X, Y);
}

void RenderLista::DibujarFlecha(int indiceOrigen, int indiceDestino) {
    if (!Escena)
        return;
    QPointF A = PosicionNodo(indiceOrigen);
    QPointF B = PosicionNodo(indiceDestino);
    Escena->addLine(A.x() + RadioNodo, A.y(), B.x() - RadioNodo, B.y(), LapizNegro);
    double X = B.x() - RadioNodo;
    double Y = B.y();
    double S = 8.0;
    QPolygonF P;
    P << QPointF(X, Y) << QPointF(X - S, Y - S * 0.6) << QPointF(X - S, Y + S * 0.6);
    Escena->addPolygon(P, LapizNegro, BrochaNegra);
}

void RenderLista::ActualizarNull(int indiceUltimo) {
    if (!Escena)
        return;
    QPointF C = PosicionNodo(indiceUltimo);
    double X = C.x() + RadioNodo + Separacion * 0.6;
    double Y = C.y();
    if (!TextoNull) TextoNull = Escena->addText(QStringLiteral("null"));
    TextoNull->setDefaultTextColor(Qt::black);
    QRectF R = TextoNull->boundingRect();
    TextoNull->setPos(X - R.width() / 2.0, Y - R.height() / 2.0);
}

void RenderLista::ReorganizarDesdeLista(const ListaSimple& lista) {

    if (!Escena)
        return;
    Escena->clear();
    TextoNull = nullptr;
    Nodos.clear();
    int N = lista.Tamano();
    for (int i = 0; i < N; ++i) {
        int V = 0;
        if (!lista.ObtenerValorEn(i, V)) continue;
        QPointF C = PosicionNodo(i);
        auto* E = Escena->addEllipse(-RadioNodo, -RadioNodo, RadioNodo * 2.0, RadioNodo * 2.0, LapizNegro, BrochaBlanca);
        E->setPos(C);
        E->setOpacity(1.0);
        Nodos.append(E);
        auto* T = new QGraphicsTextItem(QString::number(V), E);
        T->setDefaultTextColor(Qt::black);
        QRectF R = T->boundingRect();
        T->setPos(-R.width() / 2.0, -R.height() / 2.0);
        if (i > 0) DibujarFlecha(i - 1, i);
    }
    if (N > 0) ActualizarNull(N - 1);

}

void RenderLista::AnimarAgregarAlFinal(const ListaSimple& lista) {
    if (!Escena)
        return;
    int N = lista.Tamano();
    if (N <= 0)
        return;
    int V = 0;
    if (!lista.ObtenerValorEn(N - 1, V))
        return;

    QPointF Destino = PosicionNodo(N - 1);
    QPointF Inicio(Destino.x() + 160.0, Destino.y());

    auto* E = Escena->addEllipse(-RadioNodo, -RadioNodo, RadioNodo * 2.0, RadioNodo * 2.0, LapizNegro, BrochaBlanca);
    E->setPos(Inicio);
    E->setOpacity(0.0);
    Nodos.append(E);

    auto* T = new QGraphicsTextItem(QString::number(V), E);
    T->setDefaultTextColor(Qt::black);
    QRectF R = T->boundingRect();
    T->setPos(-R.width() / 2.0, -R.height() / 2.0);

    if (N > 1) DibujarFlecha(N - 2, N - 1);
    ActualizarNull(N - 1);

    auto* A1 = new QVariantAnimation(Escena);
    A1->setDuration(450);
    A1->setStartValue(Inicio);
    A1->setEndValue(Destino);
    QObject::connect(A1, &QVariantAnimation::valueChanged, [E](const QVariant& Vp){ E->setPos(Vp.toPointF()); });
    A1->start(QAbstractAnimation::DeleteWhenStopped);

    auto* A2 = new QVariantAnimation(Escena);
    A2->setDuration(450);
    A2->setStartValue(0.0);
    A2->setEndValue(1.0);
    QObject::connect(A2, &QVariantAnimation::valueChanged, [E](const QVariant& Vp){ E->setOpacity(Vp.toDouble()); });
    A2->start(QAbstractAnimation::DeleteWhenStopped);
}

void RenderLista::ResaltarIndice(int indice) {
    if (indice < 0 || indice >= Nodos.size()) return;
    QGraphicsEllipseItem* E = Nodos.at(indice);
    QBrush Antes = E->brush();
    E->setBrush(QBrush(Qt::yellow));
    QTimer::singleShot(600, [E, Antes](){ E->setBrush(Antes); });
}

void RenderLista::AnimarEliminarEnPosicion(int indice, const ListaSimple& lista) {
    if (!Escena) return;
    if (indice < 0 || indice >= Nodos.size()) { ReorganizarDesdeLista(lista); return; }

    QGraphicsEllipseItem* El = Nodos.at(indice);
    auto* A = new QVariantAnimation(Escena);
    A->setDuration(350);
    A->setStartValue(1.0);
    A->setEndValue(0.0);
    QObject::connect(A, &QVariantAnimation::valueChanged, [El](const QVariant& Vp){ El->setOpacity(Vp.toDouble()); });
    A->start(QAbstractAnimation::DeleteWhenStopped);

    for (int i = indice + 1; i < Nodos.size(); ++i) {
        QGraphicsEllipseItem* E = Nodos.at(i);
        QPointF Dest = PosicionNodo(i - 1);
        auto* M = new QVariantAnimation(Escena);
        M->setDuration(350);
        M->setStartValue(E->pos());
        M->setEndValue(Dest);
        QObject::connect(M, &QVariantAnimation::valueChanged, [E](const QVariant& Vp){ E->setPos(Vp.toPointF()); });
        M->start(QAbstractAnimation::DeleteWhenStopped);
    }

    QTimer::singleShot(380, [this, &lista](){ ReorganizarDesdeLista(lista); });
}

void RenderLista::AnimarInsertarEnPosicion(int posicion, const ListaSimple& lista) {
    if (!Escena)
        return;
    int N = lista.Tamano();
    if (posicion < 0 || posicion >= N) { ReorganizarDesdeLista(lista);
        return;
    }

    for (int i = posicion; i < Nodos.size(); ++i) {
        QGraphicsEllipseItem* E = Nodos.at(i);
        QPointF Dest = PosicionNodo(i + 1);
        auto* M = new QVariantAnimation(Escena);
        M->setDuration(350);
        M->setStartValue(E->pos());
        M->setEndValue(Dest);
        QObject::connect(M, &QVariantAnimation::valueChanged, [E](const QVariant& Vp){ E->setPos(Vp.toPointF()); });
        M->start(QAbstractAnimation::DeleteWhenStopped);
    }

    int V = 0;
    lista.ObtenerValorEn(posicion, V);
    QPointF Destino = PosicionNodo(posicion);
    QPointF Inicio(Destino.x() + 160.0, Destino.y());

    auto* Enew = Escena->addEllipse(-RadioNodo, -RadioNodo, RadioNodo * 2.0, RadioNodo * 2.0, LapizNegro, BrochaBlanca);
    Enew->setOpacity(0.0);
    Enew->setPos(Inicio);
    auto* T = new QGraphicsTextItem(QString::number(V), Enew);
    T->setDefaultTextColor(Qt::black);
    QRectF R = T->boundingRect();
    T->setPos(-R.width() / 2.0, -R.height() / 2.0);

    auto* A1 = new QVariantAnimation(Escena);
    A1->setDuration(350);
    A1->setStartValue(Inicio);
    A1->setEndValue(Destino);
    QObject::connect(A1, &QVariantAnimation::valueChanged, [Enew](const QVariant& Vp){ Enew->setPos(Vp.toPointF()); });
    A1->start(QAbstractAnimation::DeleteWhenStopped);

    auto* A2 = new QVariantAnimation(Escena);
    A2->setDuration(350);
    A2->setStartValue(0.0);
    A2->setEndValue(1.0);
    QObject::connect(A2, &QVariantAnimation::valueChanged, [Enew](const QVariant& Vp){ Enew->setOpacity(Vp.toDouble()); });
    A2->start(QAbstractAnimation::DeleteWhenStopped);

    QTimer::singleShot(380, [this, &lista](){ ReorganizarDesdeLista(lista); });
}
