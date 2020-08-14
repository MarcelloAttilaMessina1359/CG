#ifndef __IO_H__
#define __IO_H__
/**
 * @file
 * Schnittstelle des Ein-/Ausgabe-Moduls.
 * Das Modul kapselt die Ein- und Ausgabe-Funktionalitaet (insbesondere die GLUT-
 * Callbacks) des Programms.
 *
 * Bestandteil eines Beispielprogramms fuer Displaylisten mit OpenGL & GLUT.
 *
 * @author copyright (C) Fachhochschule Wedel 1999-2011. All rights reserved.
 */

/**
 * Initialisiert das Programm (inkl. I/O und OpenGL) und startet die
 * Ereignisbehandlung.
 * @param title Beschriftung des Fensters
 * @param width Breite des Fensters
 * @param height Hoehe des Fensters
 */
int initAndStartIO (char *title, int width, int height, int argc, char **argv);

/**
 * Setzt den Wert für paused.
 * paused != 0 pausiert das Spiel.
 * @param isPaused Der Wert für paused
 */
void setPaused (int isPaused);

/**
 * Gibt zurück ob das Spiel pausiert ist.
 * @return == 0 wenn pausiert.
 */
int getPaused ();

#endif
