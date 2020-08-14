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
 * Ausbreiten des Wassers.
 */
void
spreadWater (void);

/**
 * Bewegt den Spieler um eine Poition in eine der
 * möglichen Richtungen auf dem Spielfeld
 * @param direction die richtung in die der Spieler bewegt
 *        werden soll
 * @return GL_TRUE wenn der Spieler sich noch bewegt, sonst GL_FALSE
 */
int
movePlayer (double interval);

/**
 * Getter für die Position des Spielers
 * @return Position des Spilers
 */
CGPoint2f * getPlayerPosition (void);

/**
 * Getter für das Spielbrett des Levels
 * @return Das aktuelle Spielbrett
 */
wmLevelPointer getCurrentLevelBoard (void);

/**
 * Getter Für die ID des Levels
 * @return Die id des Levels
 */
Level getLevel (void);

/**
 * Setter für das Level
 * @param level Das zu settende
 */
void setLevel(Level level);

/**
 * Setzt den Bewegunsstatus des Spielers.
 * @param direction Neue Bewegungsrichtung des Spielers
 */
void
setMovement (Direction direction);

/**
 * Gibt die Ausrichtung des Spielers zurueck
 */
Direction
getPlayerRotation (void);

/**
 * Gibt an ob die Kamera gedreht wurde
 */
int
getCameraStatus (void);

/**
 * Methode um die perspective zu setzten
 */
void
setCameraStatus (Perspective perspective);

/**
 * Liefert den Status der ersten Lichtquelle.
 * @return Status der ersten Lichtquelle (an/aus).
 */
int getLight0Status (void);

/**
 * Setzt den Status der ersten Lichtquelle.
 * @param status Status der ersten Lichtquelle (an/aus).
 */
void setLight0Status (int status);

/**
 * Liefert den Status der zweiten Lichtquelle.
 * @return Status der zweiten Lichtquelle (an/aus).
 */
int getLight1Status (void);

/**
 * Setzt den Status der zweiten Lichtquelle.
 * @param status Status der zweiten Lichtquelle (an/aus).
 */
void setLight1Status (int status);

/**
 * Gibt zurueck ob die normalen gezeichnet werden sollen.
 */
int
getShowNormals (void);

/**
 * Setzt, ob die normalen gezeichnet werden sollen.
 * @param status Status, ob Normalen gezeichnet werden sollen.
 */
void
setShowNormals (int status);

#endif
