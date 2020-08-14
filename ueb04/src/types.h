#ifndef __TYPES_H__
#define __TYPES_H__
/**
 * @file
 * Typenschnittstelle.
 * Das Modul kapselt die Typdefinitionen des Programms.
 *
 * Bestandteil eines Beispielprogramms fuer Picking mit OpenGL & GLUT.
 *
 * @author copyright (C) Fachhochschule Wedel 1999-2011. All rights reserved.
 */

/* ---- System Header einbinden ---- */
#ifdef WIN32
#include <windows.h>
#endif

#ifdef __APPLE__
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

/* ---- Typedeklarationen ---- */
/** Punkt im 3D-Raum */
typedef GLfloat CGPoint3f[3];

/** Vektor im 2D-Raum */
typedef GLfloat CGVector2f[2];

/** Vektor im 3D-Raum */
typedef GLfloat CGVector3f[3];

/** Punkt im 3D-Raum (homogene Koordinaten) */
typedef GLfloat CGPoint4f[4];

/** RGB-Farbwert */
typedef GLfloat CGColor3f[3];

/** RGBA-Farbwert */
typedef GLfloat CGColor4f[4];

/** Vertex */
typedef GLdouble Vertex[7];

/** Mausereignisse. */
enum e_MouseEventType
{ mouseButton, mouseMotion, mousePassiveMotion };

/** Datentyp fuer Mausereignisse. */
typedef enum e_MouseEventType CGMouseEventType;

#endif
