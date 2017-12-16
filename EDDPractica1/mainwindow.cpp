#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QMessageBox"
#include "iostream"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    proceso = new QProcess(this);
    QImage imagen("graphSimulacion.jpg");
    item = new QGraphicsPixmapItem(QPixmap::fromImage(imagen));

    scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene);
    scene->addItem(item);

    ventanillas = new Escritorios();
    estaciones = new Mantenimiento();
    servicioCola = new ColaServicio();
    llegadaAviones = new LlegadaAviones();
    Maletas = new ListaMaletas();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::resetCampos()
{
    ui->txtTurnos->setText("");
    ui->txtAviones->setText("");
    ui->txtEscritorios->setText("");
    ui->txtEstaciones->setText("");
}


void MainWindow::on_btnIniciar_clicked()
{
    registroFull = false;
    Turno = 0;
    NoTurnos =  ui->txtTurnos->text().toInt();
    NoAviones = ui->txtAviones->text().toInt();
    NoEscritorios = ui->txtEscritorios->text().toInt();
    NoEstaciones = ui->txtEstaciones->text().toInt();

    if(NoTurnos == 0 || NoAviones == 0 || NoEscritorios == 0 || NoEstaciones == 0)
    {
        QMessageBox::warning(this,"Mensaje","Todos los campos deben ser numeros enteros mayores a cero.");
        return;
    }
    resetCampos();
    if(NoEscritorios > 25)
    {
        QMessageBox::information(this,"Mensaje","El numero maximo de escritorios son 26.");
        return;
    }

    llegadaAviones->insertar(NoAviones);
    estaciones->insertar(NoEstaciones);
    ventanillas->insertar(NoEscritorios);
    ui->btnSiguiente->setEnabled(true);
    graficarSimulacion();
    actualizarConsola();
}


void MainWindow::on_btnSiguiente_clicked()
{
    if(registroFull)
    {
        QMessageBox::information(this,"Mensaje","Los registros estan llenos no se puede asignar mas pasajeros.");
        return;
    }
    if(NoTurnos == 0)
    {
        QMessageBox::information(this,"Mensaje","Se han terminado los turnos.");
        return;
    }

    Turno++;

    AvionActual = llegadaAviones->primero;
    if(AvionActual != nullptr)
    {
    AvionActual->turnos = AvionActual->turnos - 1;
    if(AvionActual->turnos == 0) registrarPasajeros();
    }







    NoTurnos--;
    graficarSimulacion();
    actualizarConsola();

}


void MainWindow::registrarPasajeros()
{
    PasajeroActual = AvionActual->desbordaje->primero;
    registroFull = ventanillas->encolarPasajeros(PasajeroActual);
    //cout <<"encolo"<<endl;
    servicioCola->pushCola(AvionActual->idAvion,AvionActual->mantenimiento);
    //cout <<"PUSHEO"<<endl;
    if(AvionActual->anterior != nullptr) AvionActual = AvionActual->anterior;
    llegadaAviones->eliminar();
    //cout <<"elimino"<<endl;
}


void MainWindow::restartSimulacion()
{
    NoTurnos = 0;
    NoAviones = 0;
    NoEstaciones = 0;
    NoEscritorios = 0;
    resetCampos();
}


void MainWindow::actualizarConsola()
{
    AvionActual = llegadaAviones->primero;
    i = "\n****************** Turno "+to_string(Turno)+"****************** \n\n"
        + " arribo avion: ";
    if(AvionActual != nullptr) i = i + to_string(AvionActual->idAvion);
    else i = i + "ninguno";
    i = i  + "\nAvion desabordando: ";
    if(AvionActual != nullptr) i = i + to_string(AvionActual->idAvion);
    else i = i + "ninguno";
    i = i + "\n-----------------------------------------------------------------------"
          + "\n     >>>>> Escritorio de registro \n\n";
    VentanillaActual = ventanillas->primero;
    while (VentanillaActual != nullptr)
    {
        i = i + "          Escritorio " + VentanillaActual->id + ": " + VentanillaActual->estado + "\n\t"
              + "Pasajero atendido: " + to_string( VentanillaActual->idCliente) + "\n\t"
              + "Turnos restantes: " + to_string(VentanillaActual->turnosRestantes) + "\n\t"
              + "Cantidad de documentos: " + to_string(VentanillaActual->documentos) + "\n\n";

        VentanillaActual = VentanillaActual->siguiente;
    }

    i = i + "---------------------------------------------------------------------- \n"
          + "     >>>>> Estaciones de servicio \n\n";

    ServicioActual = estaciones->primero;
    while (ServicioActual != nullptr)
    {
        i = i + "          Estacion " + to_string(ServicioActual->idEstacion) + "\n\t"
              + "Avion en mantenimiento: " + to_string(ServicioActual->idAvion) + "\n\t"
              + "Turnos restantes: " + to_string(ServicioActual->turnosRestantes) + "\n\n";

        ServicioActual = ServicioActual->siguiente;
    }

    i = i + "----------------------------------------------------------------------\n"
          + "Cantidad de malestas en el sistema: ";
    AvionActual = llegadaAviones->primero;
    if(AvionActual != nullptr)
        desbordaje = AvionActual->desbordaje;
    else desbordaje = nullptr;

    if(desbordaje != nullptr) i = i + to_string(desbordaje->totalMaletas)+ "\n";
    else i = i + "0 \n";


    i = i + "Turnos restantes: " + to_string(NoTurnos) + "\n\n"
          + "**************** Fin de Turno " + to_string(Turno) + " ****************\n";

    ui->consola->appendPlainText(QString::fromStdString(i));
}


void MainWindow::graficarSimulacion()
{
    graph ="";
    graph = "Digraph simulacion{ \n\n ";
    graph = graph + dotLlegada();
    graph = graph + dotServicio();
    graph = graph + dotRegistro();
    graph = graph + dotMaletas();
    graph = graph + "\n\n }";

    //cout<<graph<<endl;

    ofstream archivo;
    archivo.open("graphSimulacion.dot");
    archivo << graph;
    archivo.close();

    QStringList argumentos;
    argumentos.append("-Tjpg");
    argumentos.append("graphSimulacion.dot");
    argumentos.append("-o");
    argumentos.append("graphSimulacion.jpg");
    proceso->start("dot",argumentos);
    proceso->waitForFinished(5000);

    if (dir.exists("graphSimulacion.jpg"))
        {
            pixmap.load("graphSimulacion.jpg");
            item = new QGraphicsPixmapItem(pixmap);
            scene->clear();
            scene->addItem(item);
        }

}


string MainWindow::dotLlegada()
{
    k = "a";
    r = "";
    if(llegadaAviones->primero == nullptr) return "";
    AvionActual = llegadaAviones->primero;
    r = r + "{rank = same \n";
    while (AvionActual !=nullptr)
    {
        r = r + k + "[shape=box, color=red, label=\" Avion: " + to_string(AvionActual->idAvion)
                  + "\\nTipo: "+ to_string(AvionActual->tipoAvion)
                  + "\\nTurnos: "+to_string(AvionActual->turnos) + "\"];\n";
        if(AvionActual->anterior != nullptr)
            r = r + k + " -> "+ k + "a;\n "+ k + "a -> " + k + ";\n ";

        k = k + "a";
        AvionActual = AvionActual->anterior;
    }
    r = r + "}\n";
    AvionActual = llegadaAviones->primero;
    PasajeroActual = AvionActual->desbordaje->primero;
    if(PasajeroActual == nullptr) return r;

    r = r + "a -> p;\n ";
    k = "p";
    while (PasajeroActual != nullptr)
    {
        r = r + k + "[shape=box, color=blue, label=\" Pasajero: " + to_string( PasajeroActual->idPasajero) +"\"];\n";
        if(PasajeroActual->siguiente != nullptr)
            r = r + k + " -> " + k +"p;\n";
        k = k + "p";
        PasajeroActual = PasajeroActual->siguiente;
    }
    r = r + "\n";
    return r;
}


string MainWindow::dotServicio()
{
    k = "e";
    r = "";
    if(estaciones->primero == nullptr) return "";
    ServicioActual = estaciones->primero;

    while (ServicioActual != nullptr)
    {
        r = r + k + "[shape=box, color=red, label=\" Servicio: " + to_string(ServicioActual->idEstacion)
                  + "\\n Id avion: " + to_string(ServicioActual->idAvion) + "\\n Estado: "+ ServicioActual->estado + "\\n Turnos: "
                  + to_string(ServicioActual->turnosRestantes) + "\"];\n";

        if(ServicioActual->siguiente != nullptr)
            r = r + k + " -> "+ k + "e;\n ";

        k = k + "e";
        ServicioActual = ServicioActual->siguiente;
    }

    if(servicioCola->ultimo == nullptr) return r;

    k = "c";
    ColaActual = servicioCola->ultimo;
    while (ColaActual != nullptr)
    {
        r = r + k + "[shape=box, color=blue, label=\"Avion: " + to_string(ColaActual->idAvion) +"\"];\n";

        if(ColaActual->siguiente != nullptr)
            r = r + k + " -> "+ k + "c;\n ";

        k = k + "c";
        ColaActual = ColaActual->siguiente;
    }
    r = r + "\n";
    return r;
}


string MainWindow::dotRegistro()
{
    k = "r";
    r = i = "";
    if(ventanillas->primero == nullptr) return "";
    VentanillaActual = ventanillas->primero;
    while (VentanillaActual != nullptr)
    {
        r = r + "{rank = same \n";

        if(VentanillaActual->topDocumento != nullptr)
        {
            i = k + "d";
            r = r + k + " -> " + i +";\n";
            DocActual = VentanillaActual->topDocumento;
            while (DocActual != nullptr)
            {
                r = r +  i+ "[color=blue, label=\" Documento: " + to_string( DocActual->numero) +"\"];\n";

                if(DocActual->siguiente != nullptr)
                    r = r + i + " -> " + i +"d;\n";
                i = i + "d";
                DocActual = DocActual->siguiente;
            }
        }

        r = r + k + "[shape=box, color=red, label=\" Registro: " + VentanillaActual->id
                  + "\\n Cliente: " + to_string(VentanillaActual->idCliente)
                  + "\\n Estado: " + VentanillaActual->estado + "\\n Documentos: "
                  + to_string( VentanillaActual->documentos) + "\\n Turnos: "
                  + to_string(VentanillaActual->turnosRestantes) + "\"];\n";

        if(VentanillaActual->pPasajero != nullptr)
        {

            i = k + "p";
            r = r + k + " -> " + i +";\n";
            PasajeroActual = VentanillaActual->pPasajero;
            while (PasajeroActual != nullptr)
            {
                r = r +  i + "[color=blue, label=\" Pasajero: " + to_string( PasajeroActual->idPasajero) +"\"];\n";

                if(PasajeroActual->siguiente != nullptr)
                    r = r + i + " -> " + i +"p;\n";
                i = i + "p";
                PasajeroActual = PasajeroActual->siguiente;
            }

        }

        r = r + "}\n";
        if(VentanillaActual->siguiente != nullptr)
            r = r + k + " -> "+ k + "r;\n "+ k + "r -> " + k + ";\n ";
        k = k + "r";
        VentanillaActual = VentanillaActual->siguiente;
    }

    r = r + "\n";
    return r;
}


string MainWindow::dotMaletas()
{
    AvionActual = llegadaAviones->primero;
    if(AvionActual == nullptr || AvionActual->desbordaje == nullptr) return "";
    desbordaje = AvionActual->desbordaje;

    if(desbordaje->listaMaletas == nullptr) return "";

    Maletas = desbordaje->listaMaletas;
    maletaActual = Maletas->primero;
    r = "";
    k = "m";
    while (maletaActual->siguiente != Maletas->primero)
    {
         r = r + k + "[shape=box, color=blue, label=\" Cliente: " + to_string(maletaActual->idMaleta)
                   + "\\n Numero: " + to_string(maletaActual->numero) + "\"];\n";

         if(maletaActual->siguiente != Maletas->primero)
             r = r + k + " -> "+ k + "m;\n "+ k + "m -> " + k + ";\n ";

         k = k +"m";
         maletaActual = maletaActual->siguiente;
    }
    r = r + k + "[shape=box, color=blue, label=\" Cliente: " + to_string(maletaActual->idMaleta)
              + "\\n Numero: " + to_string(maletaActual->numero) + "\"];\n";

    r = r + k + " -> m;\n m -> " + k + ";\n ";

    return r;
}










