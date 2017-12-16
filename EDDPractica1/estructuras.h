#ifndef ESTRUCTURAS_H
#define ESTRUCTURAS_H
#include "string"
#include "sstream"
using namespace std;

typedef struct NodoAvion NodoAvion;
typedef struct NodoPasajero NodoPasajero;
typedef struct LlegadaAviones LlegadaAviones;
typedef struct Desbordaje Desbordaje;
typedef struct NodoMaleta NodoMaleta;
typedef struct ListaMaletas ListaMaletas;
typedef struct NodoDocumento NodoDocumento;
typedef struct NodoEscritorio NodoEscritorio;
typedef struct Escritorios Escritorios;
typedef struct NodoServicio NodoServicio;
typedef struct Mantenimiento Mantenimiento;
typedef struct NodoCola NodoCola;
typedef struct ColaServicio ColaServicio;


struct NodoAvion
{
    int idAvion;
    int tipoAvion;
    int pasajeros;
    int turnos;
    int mantenimiento;
    NodoAvion *anterior;
    NodoAvion *siguiente;
    Desbordaje *desbordaje;
    void insertarPasajeros(int total);
    NodoAvion(int idAvion_, int tipoAvion_, int pasajeros_, int mantenimiento_,int turnos_);
};


struct NodoPasajero
{
    int idPasajero;
    int noMaletas;
    int noDocumentos;
    int noTurnos;
    NodoPasajero *siguiente;
    NodoPasajero(int idp, int nom, int nod, int notu);
};


struct NodoDocumento
{
    int idPasajero;
    int numero;
    NodoDocumento *siguiente;
    NodoDocumento(int idp, int num);
};


struct NodoEscritorio
{
    string id;
    int capacidad;
    int idCliente;
    string estado;
    int documentos;
    int turnosRestantes;
    NodoPasajero *pPasajero;
    NodoDocumento *topDocumento;
    NodoEscritorio *anterior;
    NodoEscritorio *siguiente;
    NodoEscritorio(string ide);
};


struct NodoMaleta
{
    int idMaleta;
    int numero;
    NodoMaleta *siguiente;
    NodoMaleta *anterior;
    NodoMaleta(int id, int num);
};


struct NodoServicio
{
    int idAvion;
    int idEstacion;
    string estado;
    int turnosRestantes;
    NodoServicio *siguiente;
    NodoServicio(int id);
};


struct NodoCola
{
    int idAvion;
    int turnosServicio;
    NodoCola *siguiente;
    NodoCola(int id, int turnos);
};


struct LlegadaAviones
{
    NodoAvion *primero;
    NodoAvion *ultimo;
    int tipo;
    int pasajeros;
    int mante;
public:
    LlegadaAviones();
    void eliminar();
    void insertar(int total);
    int random(int min, int max);
    NodoAvion *crearAvion(int id);
};


struct Desbordaje
{
private:
    int maletas;
    int documentos;
    int turnos;
public:
    int totalMaletas;
    NodoPasajero *ultimo;
    NodoPasajero *primero;
    ListaMaletas *listaMaletas;
    Desbordaje();
    void eliminar(int idp);
    void insertar(int total);
    int random(int min, int max);
    NodoPasajero *crearPasajero(int id);

};


struct ListaMaletas
{
public:
    NodoMaleta *primero;
    NodoMaleta *ultimo;
    ListaMaletas();
    void insertar(int id, int total);
    void eliminar(int id, int total);

};


struct Escritorios
{
private:
    char c;
    string s;
    stringstream ss;
public:
    NodoEscritorio *actual;
    NodoEscritorio *ultimo;
    NodoEscritorio *primero;
    NodoPasajero *aPasajero;
    Escritorios();
    void eliminarLista();
    void popDocumentos();
    void insertar(int total);
    void sacarPasajero(NodoEscritorio *eActual);
    void nuevoRegistro(NodoEscritorio *ventana);
    void pushDocumentos();
    bool encolarPasajeros(NodoPasajero *pasajero);
private:
    string nuevoId(int val);
};


struct Mantenimiento
{
public:
    NodoServicio *primero;
    NodoServicio *ultimo;
    NodoServicio *actual;
    Mantenimiento();
    void eliminarLista();
    void insertar(int total);
    void eliminarAvion(int idAvion);
    void ingresarAvion(int idAvion,int turnos);
};


struct ColaServicio
{
public:
    NodoCola *primero;
    NodoCola *ultimo;
    ColaServicio();
    NodoCola *popCola();
    void pushCola(int idAvion,int turnos);
};





#endif // ESTRUCTURAS_H
