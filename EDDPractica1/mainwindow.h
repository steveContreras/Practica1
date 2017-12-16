#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "QGraphicsItem"
#include "QGraphicsScene"
#include "QtGui"
#include "estructuras.h"
#include "QProcess"
#include "fstream"
#include "sstream"
#include "QDir"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_btnIniciar_clicked();

    void on_btnSiguiente_clicked();

private:
    Ui::MainWindow *ui;
    QGraphicsItem *item;
    QGraphicsScene *scene;
    QDir dir;
    QPixmap pixmap;
    int NoTurnos;
    int NoAviones;
    int NoEscritorios;
    int NoEstaciones;
    int Turno = 0;
    int NoMaletas;
    bool registroFull = false;
    string graph,k,r,i;
    QProcess *proceso;

    NodoAvion *AvionActual;
    NodoPasajero *PasajeroActual;
    NodoEscritorio *VentanillaActual;
    NodoServicio *ServicioActual;
    NodoCola *ColaActual;
    NodoDocumento *DocActual;
    NodoMaleta *maletaActual;

    LlegadaAviones *llegadaAviones;
    Desbordaje *desbordaje;
    Escritorios *ventanillas;
    Mantenimiento *estaciones;
    ColaServicio *servicioCola;
    ListaMaletas *Maletas;


    void registrarPasajeros();
    void restartSimulacion();
    void resetCampos();
    void graficarSimulacion();
    void actualizarConsola();
    string dotLlegada();
    string dotServicio();
    string dotRegistro();
    string dotMaletas();

};

#endif // MAINWINDOW_H
