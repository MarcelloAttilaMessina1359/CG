#ifndef __TYPES_H__
#define __TYPES_H__
/**
 * @file
 * Typenschnittstelle.
 * Das Modul kapselt die Typdefinitionen und globalen Konstanten des Programms.
 *
 * Bestandteil eines Beispielprogramms fuer Animationen mit OpenGL & GLUT.
 *
 * @author copyright (C) Fachhochschule Wedel 1999-2011. All rights reserved.
 */

/* ---- System Header einbinden ---- */
#ifdef WIN32
#include <windows.h>
#endif

#ifdef MACOSX
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

/* ---- Konstanten ---- */
/** x-Koordinate linker Rand */
#define BORDER_LEFT   -0.9f
/** x-Koordinate rechter Rand */
#define BORDER_RIGHT   0.9f

/** y-Koordinate oberer Rand */
#define BORDER_TOP     0.9f
/** y-Koordinate unterer Rand */
#define BORDER_BOTTOM -0.9f

/** Breite des Rechtecks */
#define QUAD_WIDTH     1.0f
/** Breite des Rechtecks */
#define QUAD_HEIGHT    1.0f

/** Breite des Rechtecks */
#define BALL_RADIUS     0.05f

/** Breite des Schlägers */
#define PADDLE_WIDTH 0.1f

/** Höhe des Schlägers */
#define PADDLE_HEIGHT 0.4f

/** Breite des Randes */
#define BORDER_WIDTH 1.8f
/** Höhe des Randes */
#define BORDER_HEIGHT 0.1f

/** Der Winkel in dem Sich der Schläger Rotieren darf */
#define PADDLE_RANGE 70.0f

/** Winkel in dem der Ball nach Rechts angespielt wird*/
#define SPAWN_RANGE_RIGHT 45
/** Winkel in dem der Ball nach Links angespielt wird*/
#define SPAWN_RANGE_LEFT 135


/* ---- Typedeklarationen ---- */
/** Punkt im 2D-Raum */
typedef GLfloat CGPoint2f[2];

/** Vektor im 2D-Raum */
typedef GLfloat CGVector2f[2];

/** RGB-Farbwert */
typedef GLfloat CGColor3f[3];

/** Rotations Winkel des Schlägers*/
typedef GLfloat PaddleRotation;

/** Kollisionsseiten. */
enum e_Side
{ sideNone, sideLeft, sideRight, sideTop, sideBottom, sidePlayerPaddle, sideKIPaddle };

/** Datentyp fuer Kollisionsseiten. */
typedef enum e_Side CGSide;

/** Bewegungsrichtungen. */
enum e_Direction
{ dirLeft, dirRight, dirUp, dirDown };

/** Datentyp fuer Bewegungsrichtungen. */
typedef enum e_Direction CGDirection;

#endif
