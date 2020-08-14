#ifndef __SCENE_H__
#define __SCENE_H__
/**
 * @file
 * Schnittstelle des Darstellungs-Moduls.
 * Das Modul kapselt die Rendering-Funktionalitaet (insbesondere der OpenGL-
 * Aufrufe) des Programms.
 *
 * Bestandteil eines Beispielprogramms fuer Picking mit OpenGL & GLUT.
 *
 * @author copyright (C) Fachhochschule Wedel 1999-2011. All rights reserved.
 */ 
 
/**
 * Setzt ob die Kugeln gezeichnet werden sollen.
 */
void
setShowBalls (GLint show);

/**
 * Gibt zurück, ob die Kugeln gezeichnet werden sollen.
 */
GLint
getShowBalls ();

/**
 * Zeichen-Funktion.
 * Stellt die Szene dar. Ein weisses Rechteck wird an der vorher im Logik-Modul
 * berechnet Position gezeichnet.
 * Ausgabe eines Quadrats.
 */
void drawScene (void);

/**
 * Initialisierung der Szene (inbesondere der OpenGL-Statusmaschine).
 * Setzt Hintergrund- und Zeichenfarbe.
 * @return Rueckgabewert: im Fehlerfall 0, sonst 1.
 */
int initScene (void);

/**
 * Zeichnet die Normalen
 */
 void drawNormals();

/**
 * (De-)aktiviert den Wireframe-Modus.
 */
void toggleWireframeMode (void);

#endif
