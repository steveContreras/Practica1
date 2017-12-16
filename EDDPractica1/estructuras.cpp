#include "estructuras.h"
#include <stdlib.h>
#include <cstdlib>
#include <time.h>
#include <iostream>
#include <ctime>

using namespace std;

/*************************************   NODOS DE LAS LISTAS  ***********************************
*/
NodoAvion::NodoAvion(int idAvion_, int tipoAvion_, int pasajeros_, int mantenimiento_, int turnos_)
{
    this->idAvion = idAvion_;
    this->tipoAvion = tipoAvion_;
    this->pasajeros = pasajeros_;
    this->mantenimiento = mantenimiento_;
    this->turnos = turnos_;
    this->siguiente = nullptr;
    this->anterior = nullptr;
    this->desbordaje = new Desbordaje();

}

NodoPasajero::NodoPasajero(int idp, int nom, int nod, int notu)
{
    this->idPasajero = idp;
    this->noMaletas = nom;
    this->noDocumentos = nod;
    this->noTurnos = notu;
    this->siguiente = nullptr;
}

NodoMaleta::NodoMaleta(int id, int num)
{
    this->idMaleta = id;
    this->numero = num;
    this->siguiente = nullptr;
    this->anterior = nullptr;
}

NodoDocumento::NodoDocumento(int idp,int num)
{
    this->idPasajero = idp;
    this->numero = num;
    this->siguiente = nullptr;
}

NodoEscritorio::NodoEscritorio(string ide)
{
    this->id = ide;
    this->capacidad = 0;
    this->documentos = 0;
    this->idCliente = 0;
    this->turnosRestantes = 0;
    this->estado = "Libre";
    this->anterior = nullptr;
    this->siguiente = nullptr;
    this->topDocumento = nullptr;
    this->pPasajero = nullptr;
}

NodoServicio::NodoServicio(int id)
{
    this->idAvion = 0;
    this->idEstacion = id;
    this->estado = "Libre";
    this->turnosRestantes = 0;
    this->siguiente = nullptr;
}

NodoCola::NodoCola(int id, int turnos)
{
    this->idAvion = id;
    this->turnosServicio = turnos;
    this->siguiente = nullptr;
}


/***********************************      CLASE LLEGADA DE AVIONES    ****************************
*/
LlegadaAviones::LlegadaAviones()
{
    this->tipo = 0;
    this->mante = 0;
    this->pasajeros = 0;
    this->ultimo = nullptr;
    this->primero = nullptr;
}

void LlegadaAviones::insertar(int total)
{
    for(int i = 0; i < total; i++)
    {
        if(primero != nullptr)
        {
            NodoAvion *tem = ultimo;
            ultimo =  crearAvion(i+1);
            ultimo->siguiente = tem;
            tem->anterior = ultimo;
        }
        else
            primero = ultimo = crearAvion(i+1);

        ultimo->insertarPasajeros(ultimo->pasajeros);
    }

}

NodoAvion *LlegadaAviones::crearAvion(int id)
{
    tipo = random(1,3);
    if(tipo == 1)
    {
        pasajeros = random(5,10);
        mante = random(1,3);
    }
    else if(tipo == 2)
    {
        pasajeros = random(15,25);
        mante = random(2,4);
    }
    else
    {
        pasajeros = random(30,40);
        mante = random(3,6);
    }

    return new NodoAvion(id,tipo,pasajeros,mante,tipo);
}

int LlegadaAviones::random(int min, int max)
{
    srand(time(0));
    return (min + rand() % (max-min));
}

void LlegadaAviones::eliminar()
{
    if(primero == nullptr) return;

    NodoAvion *tem = primero;

    if(primero == ultimo)
    {
        delete tem;
        primero = ultimo = nullptr;
        tem = nullptr;
    }
    else
    {
        primero = primero->anterior;
        primero->siguiente = nullptr;
        delete tem;
        tem = nullptr;
    }
}


/*****************************         CLASE DESBORDAJE          *********************************
*/
Desbordaje::Desbordaje()
{
    this->totalMaletas = 0;
    this->primero = nullptr;
    this->ultimo = nullptr;
    this->listaMaletas = new ListaMaletas();
}

void NodoAvion::insertarPasajeros(int total)
{
    desbordaje->insertar(total);
}

void Desbordaje::insertar(int total)
{
    for(int i=0; i<total;i++)
    {
        if(primero != nullptr)
        {
            ultimo->siguiente = crearPasajero(i+1);
            ultimo = ultimo->siguiente;
        }
        else
            primero = ultimo = crearPasajero(i+1);

        listaMaletas->insertar(ultimo->idPasajero,ultimo->noMaletas);
        totalMaletas = totalMaletas + ultimo->noMaletas;
    }
}

void Desbordaje::eliminar(int idp)
{
    if(primero == nullptr) return;
    NodoPasajero *tem = primero;

    if(tem->idPasajero != idp) return;

    while(tem != ultimo)
    {
        primero = primero->siguiente;
        delete tem;
        tem = primero;
    }
    if(primero == ultimo) primero = ultimo = tem = nullptr;
}

int Desbordaje::random(int min, int max)
{
    srand(time(0));
    return (min + rand() % (max-min));
}

NodoPasajero *Desbordaje::crearPasajero(int id)
{
    maletas = random(1,4);
    documentos = random(1,10);
    turnos = random(1,3);

    return new NodoPasajero(id,maletas,documentos,turnos);
}


/*************************************   CLASE MALETAS    **************************************
*/
ListaMaletas::ListaMaletas()
{
    this->primero = nullptr;
    this->ultimo = nullptr;
}

void ListaMaletas::insertar(int id, int total)
{
    for(int i = 0; i < total; i++)
    {
        if(primero != nullptr)
        {
        NodoMaleta *tem = ultimo;
        ultimo = new NodoMaleta(id,i+1);
        ultimo->siguiente = tem;
        tem->anterior = ultimo;
        primero->siguiente = ultimo;
        ultimo->anterior = primero;
        }
        else{
            primero = ultimo = new NodoMaleta(id,i+1);
            primero->anterior = ultimo;
            ultimo->siguiente = primero;
        }
    }
}

void ListaMaletas::eliminar(int id,int total)
{
    if(primero == nullptr)return;
    NodoMaleta *tem = primero;

    if(tem->idMaleta != id) return;

    if(tem == primero)
    {
        for(int i = 0; i< total; i++)
        {
            primero = primero->anterior;
            delete tem;
            tem = primero;
        }
        if(primero == ultimo )               //solo hay un nodo
        {
            primero = ultimo = tem = nullptr;
            return;
        }
        primero->siguiente = ultimo;
        ultimo->anterior = primero;
    }


}


/***********************************    CLASE ESCRITORIOS   *************************************
*/
Escritorios::Escritorios()
{
    this->primero = nullptr;
    this->ultimo = nullptr;
}

void Escritorios::insertar(int total)
{
    for(int i = 0; i < total; i++)
    {
        if(primero != nullptr)
        {
            ultimo->siguiente = new NodoEscritorio(nuevoId(i));
            ultimo->siguiente->anterior = ultimo;
            ultimo = ultimo->siguiente;
        }
        else primero = ultimo = new NodoEscritorio(nuevoId(i));
    }
}

void Escritorios::eliminarLista()
{
    if(primero == nullptr) return;
    NodoEscritorio *tem = primero;

    while (tem!=ultimo)
    {
        primero = primero->siguiente;
        primero->anterior = nullptr;
        delete tem;
        tem = primero;
    }
    if(primero == ultimo) primero = ultimo = tem = nullptr;
}

bool Escritorios::encolarPasajeros(NodoPasajero *pasajero)
{
    if(pasajero == nullptr || primero == nullptr) return false;
    actual = primero;
    NodoPasajero *fin = aPasajero =  pasajero;

    while (actual != nullptr && actual->capacidad >= 10  ) actual = actual->siguiente;



    NodoPasajero *pAux;

    while (actual != nullptr && fin != nullptr)
    {
        if(actual->pPasajero != nullptr)
        {
            pAux  = actual->pPasajero;
            while (pAux->siguiente != nullptr) pAux = pAux->siguiente;
            pAux->siguiente = aPasajero;
        }
        else actual->pPasajero = aPasajero;

        while (actual->capacidad <= 10 && fin != nullptr)
        {
            actual->capacidad++;
            fin = fin->siguiente;

            if(actual->capacidad >= 10) aPasajero->siguiente = nullptr;

            aPasajero = fin;
        }

        nuevoRegistro(actual);
        popDocumentos();
        pushDocumentos();

        actual = actual->siguiente;
    }

    if(fin == nullptr) return false;
    return true;
}

void Escritorios::pushDocumentos()
{
    if(actual == nullptr || actual->pPasajero == nullptr) return;
    int docs = actual->pPasajero->noDocumentos;

    while (docs > 0)
    {
        if(actual->topDocumento != nullptr)
        {
            NodoDocumento *tem = actual->topDocumento;
            actual->topDocumento = new NodoDocumento(actual->pPasajero->idPasajero,docs);
            actual->topDocumento->siguiente = tem;
        }
        else actual->topDocumento = new NodoDocumento(actual->pPasajero->idPasajero,docs);

        docs--;
    }
}

void Escritorios::popDocumentos()
{
    if(actual == nullptr || actual->topDocumento == nullptr) return;
    NodoDocumento *tem = actual->topDocumento;
    while (tem != nullptr)
    {
        actual->topDocumento = actual->topDocumento->siguiente;
        delete tem;
        tem = actual->topDocumento;
    }
    actual->topDocumento = tem = nullptr;
}

void Escritorios::sacarPasajero(NodoEscritorio *eActual)
{
    if(eActual == nullptr) return;
    actual = primero;

    while (actual != nullptr)
    {
        if(actual == eActual) break;
        actual = actual->siguiente;
    }
    if(actual == nullptr || actual->pPasajero == nullptr) return;

    NodoPasajero *p = actual->pPasajero;
    actual->pPasajero = actual->pPasajero->siguiente;
    delete p;
    p = nullptr;
    popDocumentos();
}

void Escritorios::nuevoRegistro(NodoEscritorio *ventana)
{
    if(ventana == nullptr) return;

    if(ventana->pPasajero != nullptr)
    {
        ventana->documentos = ventana->pPasajero->noDocumentos;
        ventana->estado = "Ocupado";
        ventana->idCliente = ventana->pPasajero->idPasajero;
        ventana->turnosRestantes = ventana->pPasajero->noTurnos;
        return;
    }
    ventana->documentos = 0;
    ventana->estado = "Libre";
    ventana->idCliente = 0;
    ventana->turnosRestantes = 0;
}

string Escritorios::nuevoId(int val)
{
    if(val == 0) return "A";
    else if(val == 1) return "B";
    else if(val == 2) return "C";
    else if(val == 3) return "D";
    else if(val == 4) return "E";
    else if(val == 5) return "F";
    else if(val == 6) return "G";
    else if(val == 7) return "H";
    else if(val == 8) return "I";
    else if(val == 9) return "J";
    else if(val == 10) return "K";
    else if(val == 11) return "L";
    else if(val == 12) return "M";
    else if(val == 13) return "N";
    else if(val == 14) return "O";
    else if(val == 15) return "P";
    else if(val == 16) return "Q";
    else if(val == 17) return "R";
    else if(val == 18) return "S";
    else if(val == 19) return "T";
    else if(val == 20) return "U";
    else if(val == 21) return "V";
    else if(val == 22) return "W";
    else if(val == 23) return "X";
    else if(val == 24) return "Y";
    else return "Z";
}


/*********************************     CLASE DE MANTENIMIENTO     **********************************
*/
void Mantenimiento::insertar(int total)
{
    for(int i = 0; i< total; i++)
    {
        if(primero != nullptr)
        {
            ultimo->siguiente = new NodoServicio(i+1);
            ultimo = ultimo->siguiente;
        }
        else primero = ultimo = new NodoServicio(i+1);
    }
}

Mantenimiento::Mantenimiento()
{
    this->primero = nullptr;
    this->ultimo = nullptr;
    this->actual = nullptr;
}

void Mantenimiento::eliminarLista()
{
    if(primero == nullptr) return;
    NodoServicio *tem = primero;
    while (primero != nullptr)
    {
        primero = primero->siguiente;
        delete tem;
        tem = primero;
    }
    primero = ultimo = tem = nullptr;
}

void Mantenimiento::ingresarAvion(int idAvion, int turnos)
{
    if(primero == nullptr) return;
    actual = primero;
    while (actual != nullptr)
    {
        if(actual->estado == "Libre")
        {
            actual->idAvion = idAvion;
            actual->turnosRestantes = turnos;
            actual->estado = "Ocupado";
            break;
        }
        actual = actual->siguiente;
    }
}

void Mantenimiento::eliminarAvion(int idAvion)
{
    if(primero == nullptr) return;
    actual = primero;
    while (actual != nullptr && actual->idAvion!=idAvion) actual = actual->siguiente;

    if(actual == nullptr) return;

    actual->estado = "Libre";
    actual->idAvion = 0;
    actual->turnosRestantes = 0;
}

void ColaServicio::pushCola(int idAvion, int turnos)
{
    NodoCola *nuevo = new NodoCola(idAvion,turnos);
    if (primero != nullptr)
    {
        nuevo->siguiente = ultimo;
        ultimo = nuevo;
    }
    else primero = ultimo = nuevo;
}

ColaServicio::ColaServicio()
{
    this->primero = nullptr;
    this->ultimo = nullptr;
}

NodoCola *ColaServicio::popCola()
{
    if(primero == nullptr) return nullptr;
    NodoCola *tem = ultimo;

    if(primero == ultimo)
    {
        primero = ultimo = nullptr;
        return tem;
    }

    while (tem->siguiente != primero) tem = tem->siguiente;

    primero = tem;
    tem = tem->siguiente;
    primero->siguiente = nullptr;
    return tem;
}


















