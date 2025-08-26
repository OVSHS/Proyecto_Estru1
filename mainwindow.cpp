#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QGraphicsScene>
#include <QIntValidator>
#include <QString>
#include <QPushButton>
#include "Persistencia.h"
#include <QFileDialog>
#include "Pila.h"
#include "RenderPila.h"
MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    ui->VistaPila->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    ui->VistaPila->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    ui->VistaPila->setAlignment(Qt::AlignLeft | Qt::AlignTop);

    ui->VistaLista->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    ui->VistaLista->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    ui->VistaLista->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    Escena = new QGraphicsScene(this);
    ui->VistaLista->setScene(Escena);
    ui->VistaLista->viewport()->setStyleSheet("background: white;");
    Render.ConfigurarEscena(Escena, QRectF(0, 0, ui->VistaLista->width(), ui->VistaLista->height()));


    auto* VInt = new QIntValidator(0, 2147483647, this);
    ui->TextoIngresar->setValidator(VInt);
    ui->TextoEliminar->setValidator(VInt);
    ui->TextoInsertar->setValidator(VInt);
    ui->TextoBuscar->setValidator(VInt);

    EscenaPila = new QGraphicsScene(this);
    ui->VistaPila->setScene(EscenaPila);
    ui->VistaPila->viewport()->setStyleSheet("background: white;");
    RenderDePila.ConfigurarEscena(EscenaPila, QRectF(0, 0, ui->VistaPila->width(), ui->VistaPila->height()));

    auto* ValidadorEnteroPila = new QIntValidator(0, 2147483647, this);
    ui->TextoApilar->setValidator(ValidadorEnteroPila);
    ui->BuscarPila->setValidator(ValidadorEnteroPila);

    connect(ui->Boton,         &QPushButton::clicked, this, &MainWindow::BotonClic);
    connect(ui->BotonEliminar, &QPushButton::clicked, this, &MainWindow::BotonEliminarClic);
    connect(ui->BotonInsertar, &QPushButton::clicked, this, &MainWindow::BotonInsertarClic);
    connect(ui->BotonBuscar,   &QPushButton::clicked, this, &MainWindow::BotonBuscarClic);
    connect(ui->GuardarListaSimple, &QPushButton::clicked, this, &MainWindow::GuardarListaSimpleClic);
    connect(ui->CargarListaSimple,  &QPushButton::clicked, this, &MainWindow::CargarListaSimpleClic);

    connect(ui->BotonIngresarPila,  &QPushButton::clicked, this, &MainWindow::BotonIngresarPilaClic);
    connect(ui->BotonEliminarPila,  &QPushButton::clicked, this, &MainWindow::BotonEliminarPilaClic);
    connect(ui->BotonMirarTope,     &QPushButton::clicked, this, &MainWindow::BotonMirarTopeClic);
    connect(ui->BotonBuscarPila,    &QPushButton::clicked, this, &MainWindow::BotonBuscarPilaClic);
    connect(ui->GuardarPila,        &QPushButton::clicked, this, &MainWindow::GuardarPilaClic);
    connect(ui->CargarPila,         &QPushButton::clicked, this, &MainWindow::CargarPilaClic);


    EscenaCola = new QGraphicsScene(this);
    ui->VistaCola->setScene(EscenaCola);
    ui->VistaCola->viewport()->setStyleSheet("background: white;");
    ui->VistaCola->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    ui->VistaCola->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    ui->VistaCola->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    RenderDeCola.ConfigurarEscena(EscenaCola, QRectF(0,0, ui->VistaCola->width(), ui->VistaCola->height()));

    auto* ValCola = new QIntValidator(0, 2147483647, this);
    ui->TextoEncolar->setValidator(ValCola);
    ui->BuscarCola->setValidator(ValCola);

    connect(ui->BotonEncolar,     &QPushButton::clicked, this, &MainWindow::BotonEncolarClic);
    connect(ui->BotonDesencolar,  &QPushButton::clicked, this, &MainWindow::BotonDesencolarClic);
    connect(ui->BotonMirarFrente, &QPushButton::clicked, this, &MainWindow::BotonMirarFrenteClic);
    connect(ui->BotonBuscarCola,  &QPushButton::clicked, this, &MainWindow::BotonBuscarColaClic);
    connect(ui->GuardarCola,      &QPushButton::clicked, this, &MainWindow::GuardarColaClic);
    connect(ui->CargarCola,       &QPushButton::clicked, this, &MainWindow::CargarColaClic);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::BotonClic() {
    QString S = ui->TextoIngresar->text().trimmed();
    if (!S.isEmpty()) {
        int V = S.toInt();
        if (Lista.AgregarAlFinal(V)) {
            Render.AnimarAgregarAlFinal(Lista);
            ui->TextoIngresar->clear();
        }
    }
    return;
}

void MainWindow::BotonEliminarClic() {
    QString S = ui->TextoEliminar->text().trimmed();
    if (!S.isEmpty()) {
        int V = S.toInt();
        int Idx = Lista.Buscar(V);
        if (Idx >= 0) {
            if (Lista.EliminarPorValor(V)) {
                Render.AnimarEliminarEnPosicion(Idx, Lista);
                ui->TextoEliminar->clear();
            }
        }
    }
    return;
}

void MainWindow::BotonInsertarClic() {
    QString SV = ui->TextoIngresar->text().trimmed();
    QString SP = ui->TextoInsertar->text().trimmed();
    if (!SV.isEmpty() && !SP.isEmpty()) {
        int V = SV.toInt();
        int P = SP.toInt();
        if (Lista.InsertarPorPosicion(P, V)) {
            Render.AnimarInsertarEnPosicion(P, Lista);
            ui->TextoIngresar->clear();
            ui->TextoInsertar->clear();
        }
    }
    return;
}

void MainWindow::BotonBuscarClic() {
    QString S = ui->TextoBuscar->text().trimmed();
    if (!S.isEmpty()) {
        int V = S.toInt();
        int Idx = Lista.Buscar(V);
        if (Idx >= 0) {
            Render.ResaltarIndice(Idx);
        }
    }
    return;
}

void MainWindow::GuardarListaSimpleClic() {
    QString RutaArchivo = QFileDialog::getSaveFileName(this, "Guardar Lista Simple", "ListaSimple.txt", "Texto (*.txt)");
    if (!RutaArchivo.isEmpty()) {
        Persistencia::GuardarListaSimple(RutaArchivo, Lista);
    }
    return;
}

void MainWindow::CargarListaSimpleClic() {
    QString RutaArchivo = QFileDialog::getOpenFileName(this, "Cargar Lista Simple", QString(), "Texto (*.txt)");
    if (!RutaArchivo.isEmpty()) {
        bool CargaCorrecta = Persistencia::CargarListaSimple(RutaArchivo, Lista);
        if (CargaCorrecta) {
            Render.ReorganizarDesdeLista(Lista);
        }
    }
    return;
}

void MainWindow::BotonIngresarPilaClic() {
    QString TextoValor = ui->TextoApilar->text().trimmed();
    if (!TextoValor.isEmpty()) {
        int ValorApilar = TextoValor.toInt();
        if (EstructuraPila.Apilar(ValorApilar)) {
            RenderDePila.AnimarApilar(EstructuraPila);
            ui->TextoApilar->clear();
        }
    }
    return;
}

void MainWindow::BotonEliminarPilaClic() {
    int ValorSalida = 0;
    if (EstructuraPila.Desapilar(ValorSalida)) {
        RenderDePila.AnimarDesapilar(EstructuraPila);
    }
    return;
}

void MainWindow::BotonMirarTopeClic() {
    int ValorTope = 0;
    if (EstructuraPila.Mirar(ValorTope)) {
        RenderDePila.ResaltarIndice(0);
    }
    return;
}

void MainWindow::BotonBuscarPilaClic() {
    QString TextoBuscar = ui->BuscarPila->text().trimmed();
    if (!TextoBuscar.isEmpty()) {
        int ValorBuscar = TextoBuscar.toInt();
        int IndiceDesdeTope = EstructuraPila.Buscar(ValorBuscar);
        if (IndiceDesdeTope >= 0) {
            RenderDePila.ResaltarIndice(IndiceDesdeTope);
        }
    }
    return;
}

void MainWindow::GuardarPilaClic() {
    QString RutaArchivo = QFileDialog::getSaveFileName(this, "Guardar Pila", "Pila.txt", "Texto (*.txt)");
    if (!RutaArchivo.isEmpty()) {
        Persistencia::GuardarPila(RutaArchivo, EstructuraPila);
    }
    return;
}

void MainWindow::CargarPilaClic() {
    QString RutaArchivo = QFileDialog::getOpenFileName(this, "Cargar Pila", QString(), "Texto (*.txt)");
    if (!RutaArchivo.isEmpty()) {
        bool CargaCorrecta = Persistencia::CargarPila(RutaArchivo, EstructuraPila);
        if (CargaCorrecta) {
            RenderDePila.ReorganizarDesdePila(EstructuraPila);
        }
    }
    return;
}
void MainWindow::BotonEncolarClic() {
    QString t = ui->TextoEncolar->text().trimmed();
    if (!t.isEmpty()) {
        int v = t.toInt();
        if (EstructuraCola.Encolar(v)) {
            RenderDeCola.AnimarEncolar(EstructuraCola);
            ui->TextoEncolar->clear();
        }
    }
}

void MainWindow::BotonDesencolarClic() {
    int out = 0;
    if (EstructuraCola.Desencolar(out)) {
        RenderDeCola.AnimarDesencolar(EstructuraCola);
    }
}

void MainWindow::BotonMirarFrenteClic() {
    int v = 0;
    if (EstructuraCola.MirarFrente(v)) {
        RenderDeCola.ResaltarIndice(0);
    }
}

void MainWindow::BotonBuscarColaClic() {
    QString t = ui->BuscarCola->text().trimmed();
    if (!t.isEmpty()) {
        int v = t.toInt();
        int idx = EstructuraCola.Buscar(v);
        if (idx >= 0) RenderDeCola.ResaltarIndice(idx);
    }
}

void MainWindow::GuardarColaClic() {
    QString ruta = QFileDialog::getSaveFileName(this, "Guardar Cola", "Cola.txt", "Texto (*.txt)");
    if (!ruta.isEmpty()) Persistencia::GuardarCola(ruta, EstructuraCola);
}

void MainWindow::CargarColaClic() {
    QString ruta = QFileDialog::getOpenFileName(this, "Cargar Cola", QString(), "Texto (*.txt)");
    if (!ruta.isEmpty()) {
        if (Persistencia::CargarCola(ruta, EstructuraCola)) {
            RenderDeCola.ReorganizarDesdeCola(EstructuraCola);
        }
    }
}

