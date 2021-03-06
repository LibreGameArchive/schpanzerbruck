#ifndef PYMOTEURJEU_HEADER
#define PYMOTEURJEU_HEADER

#include <Python.h>
#include <structmember.h>
#include <MoteurJeu.hpp>
#include <MoteurCombat.hpp>
#include "pyMoteurCombat.hpp"
#include <string>

using namespace std;

typedef struct {
    PyObject_HEAD
    ws::MoteurJeu* instc;    // L'instance C++ associée
} pyMoteurJeu;

#endif
