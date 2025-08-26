#include "Persistencia.h"
#include <QFile>
#include <QTextStream>
#include <QStringList>
#include "ListaSimple.h"
#include <QStringConverter>
#include "Pila.h"
#include "Cola.h"
#include "ListaDoble.h"
bool Persistencia::GuardarListaSimple(const QString& RutaArchivo, const ListaSimple& Lista) {
    bool OperacionExitosa = false;
    QFile Archivo(RutaArchivo);
    if (Archivo.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream Salida(&Archivo);
        Salida.setEncoding(QStringConverter::Utf8);
        Salida << "TIPO ListaSimple v1\n";
        int CantidadElementos = Lista.Tamano();
        for (int IndiceActual = 0; IndiceActual < CantidadElementos; ++IndiceActual) {
            int ValorElemento = 0;
            if (Lista.ObtenerValorEn(IndiceActual, ValorElemento)) {
                Salida << QString::number(ValorElemento) << '\n';
            }
        }
        Archivo.close();
        OperacionExitosa = true;
    }
    return OperacionExitosa;
}


bool Persistencia::CargarListaSimple(const QString& RutaArchivo, ListaSimple& Lista) {
    bool OperacionExitosa = false;
    QFile Archivo(RutaArchivo);
    if (Archivo.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream Entrada(&Archivo);
        Entrada.setEncoding(QStringConverter::Utf8);
        QString Cabecera = Entrada.readLine().trimmed();
        QStringList PartesCabecera = Cabecera.split(' ', Qt::SkipEmptyParts);
        bool CabeceraValida = PartesCabecera.size() >= 3 && PartesCabecera.at(0) == "TIPO" && PartesCabecera.at(1) == "ListaSimple";
        if (CabeceraValida) {
            Lista.Limpiar();
            while (!Entrada.atEnd()) {
                QString Linea = Entrada.readLine().trimmed();
                if (!Linea.isEmpty()) {
                    bool ConversionOk = false;
                    int ValorLeido = Linea.toInt(&ConversionOk);
                    if (ConversionOk && ValorLeido >= 0) {
                        Lista.AgregarAlFinal(ValorLeido);
                    }
                }
            }
            OperacionExitosa = true;
        }
        Archivo.close();
    }
    return OperacionExitosa;
}

bool Persistencia::GuardarPila(const QString& RutaArchivo, const Pila& Estructura) {
    bool OperacionExitosa = false;
    QFile Archivo(RutaArchivo);
    if (Archivo.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream Salida(&Archivo);
        Salida.setEncoding(QStringConverter::Utf8);
        Salida << "TIPO Pila v1 ORDEN tope_primero\n";
        int CantidadElementos = Estructura.Tamano();
        for (int IndiceDesdeTope = 0; IndiceDesdeTope < CantidadElementos; ++IndiceDesdeTope) {
            int ValorElemento = 0;
            if (Estructura.ObtenerValorDesdeTope(IndiceDesdeTope, ValorElemento)) {
                Salida << QString::number(ValorElemento) << '\n';
            }
        }
        Archivo.close();
        OperacionExitosa = true;
    }
    return OperacionExitosa;
}

bool Persistencia::CargarPila(const QString& RutaArchivo, Pila& Estructura) {
    bool OperacionExitosa = false;
    QFile Archivo(RutaArchivo);
    if (Archivo.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream Entrada(&Archivo);
        Entrada.setEncoding(QStringConverter::Utf8);
        QString Cabecera = Entrada.readLine().trimmed();
        QStringList Partes = Cabecera.split(' ', Qt::SkipEmptyParts);
        bool CabeceraValida = Partes.size() >= 5 && Partes.at(0) == "TIPO" && Partes.at(1) == "Pila";
        if (CabeceraValida) {
            Estructura.Limpiar();
            while (!Entrada.atEnd()) {
                QString Linea = Entrada.readLine().trimmed();
                if (!Linea.isEmpty()) {
                    bool ConversionOk = false;
                    int ValorLeido = Linea.toInt(&ConversionOk);
                    if (ConversionOk && ValorLeido >= 0) {
                        Estructura.Apilar(ValorLeido);
                    }
                }
            }
            OperacionExitosa = true;
        }
        Archivo.close();
    }
    return OperacionExitosa;
}

bool Persistencia::GuardarCola(const QString& RutaArchivo, const Cola& EstructuraCola) {
    bool OperacionExitosa = false;
    QFile ArchivoSalida(RutaArchivo);
    if (ArchivoSalida.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream FlujoTextoSalida(&ArchivoSalida);
        FlujoTextoSalida.setEncoding(QStringConverter::Utf8);
        FlujoTextoSalida << "TIPO Cola v1 ORDEN frente_primero\n";
        int CantidadElementos = EstructuraCola.Tamano();
        int IndiceDesdeFrente = 0;
        while (IndiceDesdeFrente < CantidadElementos) {
            int ValorElemento = 0;
            bool Obtenido = EstructuraCola.ObtenerValorDesdeFrente(IndiceDesdeFrente, ValorElemento);
            if (Obtenido) {
                FlujoTextoSalida << ValorElemento << "\n";
            }
            IndiceDesdeFrente = IndiceDesdeFrente + 1;
        }
        OperacionExitosa = true;
    }
    return OperacionExitosa;
}


bool Persistencia::CargarCola(const QString& RutaArchivo, Cola& EstructuraCola) {
    bool OperacionExitosa = false;
    QFile ArchivoEntrada(RutaArchivo);
    if (ArchivoEntrada.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream FlujoTextoEntrada(&ArchivoEntrada);
        FlujoTextoEntrada.setEncoding(QStringConverter::Utf8);
        QString Cabecera = FlujoTextoEntrada.readLine().trimmed();
        if (Cabecera.startsWith("TIPO Cola")) {
            EstructuraCola.Limpiar();
            while (!FlujoTextoEntrada.atEnd()) {
                QString LineaLeida = FlujoTextoEntrada.readLine().trimmed();
                if (!LineaLeida.isEmpty()) {
                    bool EsNumero = false;
                    int ValorLeido = LineaLeida.toInt(&EsNumero);
                    if (EsNumero) {
                        EstructuraCola.Encolar(ValorLeido);
                    }
                }
            }
            OperacionExitosa = true;
        }
    }
    return OperacionExitosa;
}

bool Persistencia::GuardarListaDoble(const QString& RutaArchivo, const ListaDoble& Lista) {
    bool operacionExitosa = false;
    QFile archivoSalida(RutaArchivo);
    if (archivoSalida.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream flujoTexto(&archivoSalida);
        flujoTexto.setEncoding(QStringConverter::Utf8);
        flujoTexto << "TIPO ListaDoble v1 ORDEN izquierda_a_derecha\n";
        int cantidadElementos = Lista.Tamano();
        int indiceActual = 0;
        while (indiceActual < cantidadElementos) {
            int valorElemento = 0;
            bool obtenido = Lista.ObtenerValorEn(indiceActual, valorElemento);
            if (obtenido) flujoTexto << valorElemento << "\n";
            indiceActual = indiceActual + 1;
        }
        operacionExitosa = true;
    }
    return operacionExitosa;
}

bool Persistencia::CargarListaDoble(const QString& RutaArchivo, ListaDoble& Lista) {
    bool operacionExitosa = false;
    QFile archivoEntrada(RutaArchivo);
    if (archivoEntrada.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream flujoTexto(&archivoEntrada);
        flujoTexto.setEncoding(QStringConverter::Utf8);
        QString cabecera = flujoTexto.readLine().trimmed();
        if (cabecera.startsWith("TIPO ListaDoble")) {
            Lista.Limpiar();
            while (!flujoTexto.atEnd()) {
                QString lineaLeida = flujoTexto.readLine().trimmed();
                if (lineaLeida.isEmpty()) continue;
                bool esNumero = false;
                int valorLeido = lineaLeida.toInt(&esNumero);
                if (esNumero) Lista.AgregarAlFinal(valorLeido);
            }
            operacionExitosa = true;
        }
    }
    return operacionExitosa;
}
