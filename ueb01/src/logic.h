#ifndef __LOGIC_H__
#define __LOGIC_H__
/**
 * @file
 * Schnittstelle des Logik-Moduls.
 * Das Modul kapselt die Programmlogik. Wesentliche Programmlogik ist die
 * Verwaltung und Aktualisierung der Position und Farbe eines Rechtecks. Die
 * Programmlogik ist weitgehend unabhaengig von Ein-/Ausgabe (io.h/c) und
 * Darstellung (scene.h/c).
 *
 * Bestandteil eines Beispielprogramms fuer Animationen mit OpenGL & GLUT.
 *
 * @author copyright (C) Fachhochschule Wedel 1999-2011. All rights reserved.
 */

/* ---- Eigene Header einbinden ---- */
#include "types.h"

/**
 * Berechnet neue Position des Rechtecks.
 * @param interval Dauer der Bewegung in Sekunden.
 */
void calcPosition (double interval);

/**
 * Berechnet neue Position des Schlägers des Spielers.
 * @param interval Dauer der Bewegung in Sekunden.
 */
void
calcPositionPaddle (double interval);

/**
 * Berechnet neue Position des Schlägers der KI.
 * @param interval Dauer der Bewegung in Sekunden.
 */
void 
calcPositionKIPaddle(double interval);

/**
 * Liefert aktuelle Postion (des Mittelpunktes) des Rechtecks.
 * @return Postion (des Mittelpunktes) des Rechtecks.
 */
CGPoint2f *getCircleCenter (void);

/**
 * Liefert Zeiger auf aktuelle Postion (des Mittelpunktes) des Schlägers
 * des Spielers.
 * @return Zeiger auf Postion (des Mittelpunktes) des Schlägers
 * des Spielers.
 */
PaddleRotation 
getPlayerPaddleRotation (void);

/**
 * Berechnet neue Farbe des Balls
 */
void
calcColors ();

/**
 * Setzt den Bewegunsstatus des Rechtecks.
 * @param direction Bewegungsrichtung deren Status veraendert werdene soll.
 * @param status neuer Status der Bewegung: GL_TRUE->Rechteck bewegt sich in Richtung
 * direction, GL_FALSE->Rechteck bewegt sich nicht in Richtung direction.
 */
void
setMovement (CGDirection direction, GLboolean status);

/**
 * Liefert den Zeiger auf die aktuelle Farbe des Balls * 
 */
CGColor3f *
getBallColor (void);

/**
 * Liefert Zeiger auf aktuelle Postion (des Mittelpunktes) des
 * schlägers des Spielers.
 * @return Zeiger auf Postion (des Mittelpunktes) des Schlägers
 * des Spielers.
 */
CGPoint2f * getPlayerPaddleCenter (void);

/**
 * Liefert Zeiger auf aktuelle Postion (des Mittelpunktes) des Schlägers der KI.
 * @return Zeiger auf Postion (des Mittelpunktes) des Schlägers
 * der KI.
 */
CGPoint2f * getKIPaddleCenter (void);

void
setAngle(CGSide side);

#endif
