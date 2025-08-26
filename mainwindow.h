#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ListaSimple.h"
#include "RenderLista.h"
#include "Pila.h"
#include "RenderPila.h"
#include "Cola.h"
#include "RenderCola.h"
#include "ListaDoble.h"
#include "RenderListaDoble.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class QGraphicsScene;

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private slots:
    void BotonClic();
    void BotonEliminarClic();
    void BotonInsertarClic();
    void BotonBuscarClic();
    void GuardarListaSimpleClic();
    void CargarListaSimpleClic();

    void BotonIngresarPilaClic();
    void BotonEliminarPilaClic();
    void BotonMirarTopeClic();
    void BotonBuscarPilaClic();
    void GuardarPilaClic();
    void CargarPilaClic();

    void BotonEncolarClic();
    void BotonDesencolarClic();
    void BotonMirarFrenteClic();
    void BotonBuscarColaClic();
    void GuardarColaClic();
    void CargarColaClic();


    void BotonIngresarDLLClic();
    void BotonEliminarDLLClic();
    void BotonInsertarDLLClic();
    void BotonBuscarDLLClic();
    void GuardarListaDobleClic();
    void CargarListaDobleClic();

private:
    Ui::MainWindow* ui;

    QGraphicsScene* Escena;
    ListaSimple Lista;
    RenderLista Render;

    QGraphicsScene* EscenaPila;
    Pila EstructuraPila;
    RenderPila RenderDePila;
    QGraphicsScene* EscenaCola;
    Cola EstructuraCola;
    RenderCola RenderDeCola;


    QGraphicsScene* EscenaListaDoble = nullptr;
    ListaDoble EstructuraListaDoble;
    RenderListaDoble RenderDeListaDoble;
};

#endif
