#ifndef __LOGIC_H__
#define __LOGIC_H__
/**
 * @file
 * Schnittstelle des Logik-Moduls.
 * Das Modul kapselt die Programmlogik. Wesentliche Programmlogik ist die
 * Verwaltung des Rotationswinkels des Wuerfels. Die Programmlogik ist
 * weitgehend unabhaengig von Ein-/Ausgabe (io.h/c) und
 * Darstellung (scene.h/c).
 *
 * Bestandteil eines Beispielprogramms fuer Displaylisten mit OpenGL & GLUT.
 *
 * @author copyright (C) Fachhochschule Wedel 1999-2011. All rights reserved.
 */
 /* ---- Eigene Header einbinden ---- */
#include "types.h"

/**
 * Verfeinern des Vertex-Arrays
 */
void
increaseWidth ();

/**
 * Vertex-Array gröber machen.
 */
void
decreaseWidth ();

/**
 * Erhöhen der angeklickten Stelle.
 * @param index Index der angeklickten Stelle im VertexArray
 */
void
increaseHeight (GLint index);

/**
 * Senken der angeklickten Stelle.
 * @param index Index der angeklickten Stelle im VertexArray
 */
void
decreaseHeight (GLint index);
/**
 * Zeichne alle Normalen
 */
void
enableNormals (GLint enableNormals);

 /**
  * Ist normalen zeichen gewüncht
  */
  int
normalsEnabled (void);

/**
 * Gibt die Breite des Quadrats zurück.
 */
GLint
getWidth ();

/**
 * Gibt die Anzahl an Indices zurück.
 */
GLint
getNumberOfIndices ();

/**
 * Gibt das Index-Array zurück.
 */
GLint*
getIndices ();

/**
 * Gibt das Vertex-Array zurück.
 */
Vertex*
getVertices ();

/**
 * Gibt die Normalen für das Vertex-Array zurück.
 */
CGVector3f*
getNormals ();

/**
 * Gibt die Textur-Koordinaten zurück.
 */
CGVector2f*
getTexCoords ();

/**
 * Initialisiert das Vertex-Array und das Index-Array
 */
void
initVertices ();

/**
 * Zeichne alle Normalen
 */
void
enableNormals (GLint enableNormals);

 /**
  * Ist normalen zeichen gewüncht
  */
  int
normalsEnabled (void);

#endif
