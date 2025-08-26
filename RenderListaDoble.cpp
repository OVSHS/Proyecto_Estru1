#include "RenderListaDoble.h"
#include <QGraphicsEllipseItem>
#include <QGraphicsTextItem>
#include <QVariantAnimation>
#include <QTimer>

RenderListaDoble::RenderListaDoble() : Escena(nullptr), LapizNegro(Qt::black), BrochaBlanca(Qt::white) {}

void RenderListaDoble::ConfigurarEscena(QGraphicsScene* escenaDestino, const QRectF& areaInicial) {
    Escena = escenaDestino;
    AreaBase = areaInicial;
    if (Escena) Escena->setSceneRect(AreaBase);
    return;
}

QPointF RenderListaDoble::PosicionNodo(int indiceObjetivo) const {
    double posicionX = 160.0 + indiceObjetivo * (RadioNodo * 2.0 + SeparacionNodos);
    double posicionY = CentroY;
    return QPointF(posicionX, posicionY);
}

void RenderListaDoble::DibujarFlechaAdelante(int indiceOrigen, int indiceDestino) {
    QPointF posicionA = PosicionNodo(indiceOrigen);
    QPointF posicionB = PosicionNodo(indiceDestino);
    double inicioX = posicionA.x() + RadioNodo;
    double finX = posicionB.x() - RadioNodo;
    double lineaY = CentroY - DesfaseFlechas;
    Escena->addLine(inicioX, lineaY, finX, lineaY, LapizNegro);
    double tamPunta = 8.0;
    Escena->addLine(finX, lineaY, finX - tamPunta, lineaY - tamPunta, LapizNegro);
    Escena->addLine(finX, lineaY, finX - tamPunta, lineaY + tamPunta, LapizNegro);
    return;
}

void RenderListaDoble::DibujarFlechaAtras(int indiceOrigen, int indiceDestino) {
    QPointF posicionA = PosicionNodo(indiceOrigen);
    QPointF posicionB = PosicionNodo(indiceDestino);
    double inicioX = posicionA.x() - RadioNodo;
    double finX = posicionB.x() + RadioNodo;
    double lineaY = CentroY + DesfaseFlechas;
    Escena->addLine(inicioX, lineaY, finX, lineaY, LapizNegro);
    double tamPunta = 8.0;
    Escena->addLine(finX, lineaY, finX + tamPunta, lineaY - tamPunta, LapizNegro);
    Escena->addLine(finX, lineaY, finX + tamPunta, lineaY + tamPunta, LapizNegro);
    return;
}

void RenderListaDoble::DibujarNullExtremos(int cantidadNodos) {
    auto* textoIzquierdo = Escena->addText("null");
    textoIzquierdo->setDefaultTextColor(Qt::black);
    textoIzquierdo->setPos(100.0, CentroY - 8.0);

    QPointF posicionDerecha = PosicionNodo(std::max(0, cantidadNodos - 1));
    auto* textoDerecho = Escena->addText("null");
    textoDerecho->setDefaultTextColor(Qt::black);
    textoDerecho->setPos(posicionDerecha.x() + 60.0, CentroY - 8.0);
    return;
}

void RenderListaDoble::AjustarSceneRect(const ListaDoble& lista) {
    if (!Escena) return;
    QRectF limiteItems = Escena->itemsBoundingRect().adjusted(-60, -40, 180, 60);
    QRectF recta = limiteItems;
    if (recta.width() < AreaBase.width()) recta.setWidth(AreaBase.width());
    if (recta.height() < AreaBase.height()) recta.setHeight(AreaBase.height());
    Escena->setSceneRect(recta);
    return;
}

void RenderListaDoble::ReorganizarDesdeListaDoble(const ListaDoble& lista) {
    if (!Escena) return;
    Escena->clear();
    ElementosNodo.clear();

    int cantidadNodos = lista.Tamano();
    for (int indiceActual = 0; indiceActual < cantidadNodos; ++indiceActual) {
        int valorNodo = 0;
        bool obtenido = lista.ObtenerValorEn(indiceActual, valorNodo);
        if (!obtenido) continue;

        QPointF posicionObjetivo = PosicionNodo(indiceActual);
        auto* itemNodo = Escena->addEllipse(-RadioNodo, -RadioNodo, RadioNodo*2.0, RadioNodo*2.0, LapizNegro, BrochaBlanca);
        itemNodo->setPos(posicionObjetivo);
        ElementosNodo.append(itemNodo);

        auto* itemTexto = new QGraphicsTextItem(QString::number(valorNodo), itemNodo);
        itemTexto->setDefaultTextColor(Qt::black);
        QRectF rectTexto = itemTexto->boundingRect();
        itemTexto->setPos(-rectTexto.width() / 2.0, -rectTexto.height() / 2.0);

        if (indiceActual > 0) {
            DibujarFlechaAdelante(indiceActual - 1, indiceActual);
            DibujarFlechaAtras(indiceActual, indiceActual - 1);
        }
    }

    DibujarNullExtremos(cantidadNodos);
    AjustarSceneRect(lista);
    return;
}

void RenderListaDoble::AnimarAgregarAlFinal(const ListaDoble& lista) {
    if (!Escena) return;
    int cantidadNodos = lista.Tamano();
    if (cantidadNodos <= 0) return;

    int indiceNuevo = cantidadNodos - 1;
    QPointF destino = PosicionNodo(indiceNuevo);
    QPointF inicio(destino.x(), destino.y() - 180.0);

    auto* itemNodo = Escena->addEllipse(-RadioNodo, -RadioNodo, RadioNodo*2.0, RadioNodo*2.0, LapizNegro, BrochaBlanca);
    itemNodo->setOpacity(0.0);
    itemNodo->setPos(inicio);

    int valorNodo = 0;
    lista.ObtenerValorEn(indiceNuevo, valorNodo);
    auto* itemTexto = new QGraphicsTextItem(QString::number(valorNodo), itemNodo);
    itemTexto->setDefaultTextColor(Qt::black);
    QRectF rectTexto = itemTexto->boundingRect();
    itemTexto->setPos(-rectTexto.width() / 2.0, -rectTexto.height() / 2.0);

    auto* animMover = new QVariantAnimation(Escena);
    animMover->setDuration(360);
    animMover->setStartValue(inicio);
    animMover->setEndValue(destino);
    QObject::connect(animMover, &QVariantAnimation::valueChanged, [itemNodo](const QVariant& valor){ itemNodo->setPos(valor.toPointF()); });
    animMover->start(QAbstractAnimation::DeleteWhenStopped);

    auto* animAparecer = new QVariantAnimation(Escena);
    animAparecer->setDuration(360);
    animAparecer->setStartValue(0.0);
    animAparecer->setEndValue(1.0);
    QObject::connect(animAparecer, &QVariantAnimation::valueChanged, [itemNodo](const QVariant& valor){ itemNodo->setOpacity(valor.toDouble()); });
    QObject::connect(animAparecer, &QVariantAnimation::finished, [this, &lista](){ ReorganizarDesdeListaDoble(lista); });
    animAparecer->start(QAbstractAnimation::DeleteWhenStopped);
    return;
}

void RenderListaDoble::AnimarInsertarEnPosicion(int posicionObjetivo, const ListaDoble& lista) {
    if (!Escena) return;
    int cantidadNodos = lista.Tamano();
    if (posicionObjetivo < 0 || posicionObjetivo >= cantidadNodos) { ReorganizarDesdeListaDoble(lista); return; }

    QPointF destino = PosicionNodo(posicionObjetivo);
    QPointF inicio(destino.x(), destino.y() - 180.0);

    auto* itemNodo = Escena->addEllipse(-RadioNodo, -RadioNodo, RadioNodo*2.0, RadioNodo*2.0, LapizNegro, BrochaBlanca);
    itemNodo->setOpacity(0.0);
    itemNodo->setPos(inicio);

    int valorNodo = 0;
    lista.ObtenerValorEn(posicionObjetivo, valorNodo);
    auto* itemTexto = new QGraphicsTextItem(QString::number(valorNodo), itemNodo);
    itemTexto->setDefaultTextColor(Qt::black);
    QRectF rectTexto = itemTexto->boundingRect();
    itemTexto->setPos(-rectTexto.width() / 2.0, -rectTexto.height() / 2.0);

    auto* animMover = new QVariantAnimation(Escena);
    animMover->setDuration(360);
    animMover->setStartValue(inicio);
    animMover->setEndValue(destino);
    QObject::connect(animMover, &QVariantAnimation::valueChanged, [itemNodo](const QVariant& valor){ itemNodo->setPos(valor.toPointF()); });
    animMover->start(QAbstractAnimation::DeleteWhenStopped);

    auto* animAparecer = new QVariantAnimation(Escena);
    animAparecer->setDuration(360);
    animAparecer->setStartValue(0.0);
    animAparecer->setEndValue(1.0);
    QObject::connect(animAparecer, &QVariantAnimation::valueChanged, [itemNodo](const QVariant& valor){ itemNodo->setOpacity(valor.toDouble()); });
    QObject::connect(animAparecer, &QVariantAnimation::finished, [this, &lista](){ ReorganizarDesdeListaDoble(lista); });
    animAparecer->start(QAbstractAnimation::DeleteWhenStopped);
    return;
}

void RenderListaDoble::AnimarEliminarEnPosicion(int posicionObjetivo, const ListaDoble& lista) {
    if (!Escena) return;

    if (posicionObjetivo < 0 || posicionObjetivo >= ElementosNodo.size()) { ReorganizarDesdeListaDoble(lista); return; }
    QGraphicsEllipseItem* itemNodo = ElementosNodo.value(posicionObjetivo, nullptr);
    if (!itemNodo) { ReorganizarDesdeListaDoble(lista); return; }

    QPointF posicionActual = itemNodo->pos();
    QPointF posicionSalida(posicionActual.x(), posicionActual.y() - 140.0);

    auto* animMover = new QVariantAnimation(Escena);
    animMover->setDuration(320);
    animMover->setStartValue(posicionActual);
    animMover->setEndValue(posicionSalida);
    QObject::connect(animMover, &QVariantAnimation::valueChanged, [itemNodo](const QVariant& valor){ itemNodo->setPos(valor.toPointF()); });
    animMover->start(QAbstractAnimation::DeleteWhenStopped);

    auto* animOpacidad = new QVariantAnimation(Escena);
    animOpacidad->setDuration(320);
    animOpacidad->setStartValue(1.0);
    animOpacidad->setEndValue(0.0);
    QObject::connect(animOpacidad, &QVariantAnimation::valueChanged, [itemNodo](const QVariant& valor){ itemNodo->setOpacity(valor.toDouble()); });
    QObject::connect(animOpacidad, &QVariantAnimation::finished, [this, &lista](){ ReorganizarDesdeListaDoble(lista); });
    animOpacidad->start(QAbstractAnimation::DeleteWhenStopped);
    return;
}

void RenderListaDoble::ResaltarIndice(int indiceObjetivo) {
    if (indiceObjetivo < 0 || indiceObjetivo >= ElementosNodo.size()) return;
    QGraphicsEllipseItem* nodoDestino = ElementosNodo.at(indiceObjetivo);
    QBrush brochaAnterior = nodoDestino->brush();
    nodoDestino->setBrush(QBrush(Qt::yellow));
    QTimer::singleShot(600, [nodoDestino, brochaAnterior](){ nodoDestino->setBrush(brochaAnterior); });
}
