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
 */
void movePlayer (Direction direction);

/**
 * Getter für die Position des Spielers
 * @return Position des Spilers
 */
Position * getPlayerPosition (void);

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




#endif
