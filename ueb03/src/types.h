#ifndef __TYPES_H__
#define __TYPES_H__
/**
 * @file
 * Typenschnittstelle.
 * Das Modul kapselt die Typdefinitionen des Programms.
 *
 * Bestandteil eines Beispielprogramms fuer Displaylisten mit OpenGL & GLUT.
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
/** RGB-Farbwert */
typedef GLfloat CGColor3f[3];

/** Vektor im 2D-Raum */
typedef GLfloat CGVector2f[2];

/** Vektor im 3D-Raum */
typedef GLfloat CGVector3f[3];

/* Kreis außerer Radiuds*/
#define OUTER .5f

/* Innerer Radius der Scheibe. */
#define INNER 0.0f

/* Anzahl der Segmente, aus denen die konzentrischen Ringe aufgebaut sind. */
#define SLICES 30

/*Anzahl der konzentrischen Ringe um den Mittelpunkt. */
#define LOOPS 1

/* Kanten Länge*/
#define EDGE_SIZE 1.0f

/* Anzahl der Unterteilungsschritte der Kante */
#define SUB_DIVS 20

/* Ursprung des kooridnaten systems*/
#define ORIGIN 0.0f

/* Hoehe/Breite des (quadratischen) Levels */
#define LEVELSIZE (9)

/*Die Punkte auf einem Zuckerstück */
#define SUGAR_DOTS 20

/* Höhe und breite des Sandsacks */
#define SANDBAG_HEIGHT 0.08f

#define SANDBAG_WIDTH 0.16f

/* Breite des Spielers */
#define PLAYER_SIZE 0.3f

/* Verschiebung des Spielers im 3D-Spielfeld */
#define BOARD_SHIFT 0.5f



/*
 * Art von Levelfeldern
 * WM_OUTER: Außenmauer
 * WM_FREE: leeres Feld
 * WM_BLACK: schwarzes Feld
 * WM_WALL: Unzerstörbares Feld
 * WM_TARGET: Zielfeld
 * WM_WATER : Wasser
 * WM_SAND : Sandsack, bewegbares Feld
 * WM_SUGAR : Zuckerstück, bewegbares Feld
 */
typedef enum { WM_OUTER, WM_BLACK, WM_WALL, WM_TARGET,
 WM_WATER, WM_NEWWATER, WM_SAND, WM_SUGARXXL, WM_SUGARXL, WM_SUGARL, WM_SUGAR, WM_FREE} wmFieldType;

/* Spielfeld */
typedef wmFieldType wmLevel[LEVELSIZE][LEVELSIZE];

/* Zeiger auf ein Spielfeld */
typedef wmFieldType (*wmLevelPointer)[LEVELSIZE];

//Id eines Spielfelds
typedef GLint LVLID;
//Koordinate auf dem Spielfeld
typedef GLint Coord;

//Koordinaten auf dem Spielfeld
typedef Coord Position[2];

/** Punkt im 3D-Raum (homogene Koordinaten) */
typedef GLfloat CGPoint4f[4];

/** RGBA-Farbwert */
typedef GLfloat CGColor4f[4];

/** Punkt im 2D-Raum */
typedef GLfloat CGPoint2f[2];

//Bewegungs richtungen des Spielers
typedef enum {UP, RIGHT, DOWN, LEFT} Direction;
//Enum für die unterschiedlichen Kameraperspektiven
typedef enum {BOARD, PLAYER} Perspective;
//Enum typ für die Level
typedef enum {LEVEL_1 = 1, LEVEL_2, LEVEL_3} Level;

#endif
