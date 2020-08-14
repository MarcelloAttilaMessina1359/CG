/**
 * @file
 * Logik-Modul.
 * Das Modul kapselt die Programmlogik. Wesentliche Programmlogik ist die
 * Verwaltung des Rotationswinkels des Wuerfels. Die Programmlogik ist
 * weitgehend unabhaengig von Ein-/Ausgabe (io.h/c) und
 * Darstellung (scene.h/c).
 *
 * Bestandteil eines Beispielprogramms fuer Displaylisten mit OpenGL & GLUT.
 *
 * @author copyright (C) Fachhochschule Wedel 1999-2011. All rights reserved.
 */

#include <string.h>

#ifdef MACOSX
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

/* ---- Eigene Header einbinden ---- */
#include "logic.h"
#include "types.h"
#include "io.h"
#include "stringOutput.h"

/* ---- Konstanten ---- */
#define X 0
#define Y 1
#define ONE 1

#define PLAYER_MOVE_COOLDOWN 0.3f

/** Anzahl der Wuerfeldrehungen pro Sekunde. */

/* ---- Globale Daten ---- */

//Die Position des Spielers auf dem Spielfeld
static Position playerPosition = {1, 1};

static wmLevel emptyLevel = { };

/** Das erste Level */
static wmLevel level1 =
{
	{WM_WALL, WM_WALL, WM_WALL, WM_WALL, WM_WALL, WM_WALL, WM_WALL, WM_WALL, WM_WALL},
	{WM_WALL, WM_FREE, WM_FREE, WM_FREE, WM_FREE, WM_FREE, WM_FREE, WM_FREE, WM_WALL},
	{WM_WALL, WM_FREE, WM_WALL, WM_FREE, WM_WALL, WM_WALL, WM_WALL, WM_FREE, WM_WALL},
	{WM_WALL, WM_WATER, WM_SUGARXXL, WM_FREE, WM_FREE, WM_FREE, WM_WALL, WM_FREE, WM_WALL},
	{WM_WALL, WM_FREE, WM_WALL, WM_WALL, WM_WALL, WM_FREE, WM_WALL, WM_FREE, WM_WALL},
	{WM_WALL, WM_FREE, WM_WALL, WM_WALL, WM_WALL, WM_FREE, WM_WALL, WM_FREE, WM_WALL},
	{WM_WALL, WM_SAND, WM_WALL, WM_WALL, WM_WALL, WM_FREE, WM_WALL, WM_FREE, WM_WALL},
	{WM_WALL, WM_TARGET, WM_FREE, WM_FREE, WM_FREE, WM_FREE, WM_WALL, WM_FREE, WM_WALL},
	{WM_WALL, WM_WALL, WM_WALL, WM_WALL, WM_WALL, WM_WALL, WM_WALL,WM_WALL, WM_WALL}
};

/** Das zweite Level */
static wmLevel level2 =
{
	{WM_WALL, WM_WALL, WM_WALL, WM_WALL, WM_WALL, WM_WALL, WM_WALL, WM_WALL, WM_WALL},
	{WM_WALL, WM_FREE, WM_WALL, WM_FREE, WM_FREE, WM_FREE, WM_WATER, WM_FREE, WM_WALL},
	{WM_WALL, WM_FREE, WM_SAND, WM_FREE, WM_WALL, WM_WALL, WM_WALL, WM_SUGARXXL, WM_WALL},
	{WM_WALL, WM_FREE, WM_FREE, WM_FREE, WM_FREE, WM_FREE, WM_WALL, WM_FREE, WM_WALL},
	{WM_WALL, WM_FREE, WM_WALL, WM_WALL, WM_WALL, WM_FREE, WM_SAND, WM_FREE, WM_WALL},
	{WM_WALL, WM_FREE, WM_FREE, WM_FREE, WM_FREE, WM_FREE, WM_WALL, WM_FREE, WM_WALL},
	{WM_WALL, WM_SAND, WM_WALL, WM_SUGARXXL, WM_WALL, WM_FREE, WM_FREE, WM_FREE, WM_WALL},
	{WM_WALL, WM_FREE, WM_FREE, WM_FREE, WM_FREE, WM_FREE, WM_WALL, WM_TARGET, WM_WALL},
	{WM_WALL, WM_WALL, WM_WALL, WM_WALL, WM_WALL, WM_WALL, WM_WALL,WM_WALL, WM_WALL}
};

/** Das dritte Level */
static wmLevel level3 =
{
	{WM_WALL, WM_WALL, WM_WALL, WM_WALL, WM_WALL, WM_WALL, WM_WALL, WM_WALL, WM_WALL},
	{WM_WALL, WM_FREE, WM_FREE, WM_WALL, WM_FREE, WM_WATER, WM_FREE, WM_FREE, WM_WALL},
	{WM_WALL, WM_FREE, WM_WALL, WM_SAND, WM_WALL, WM_FREE, WM_WALL, WM_FREE, WM_WALL},
	{WM_WALL, WM_FREE, WM_FREE, WM_FREE, WM_FREE, WM_SUGARXXL, WM_WALL, WM_SUGARXXL, WM_WALL},
	{WM_WALL, WM_SAND, WM_WALL, WM_SAND, WM_WALL, WM_FREE, WM_FREE, WM_FREE, WM_WALL},
	{WM_WALL, WM_FREE, WM_SAND, WM_FREE, WM_FREE, WM_FREE, WM_FREE, WM_FREE, WM_WALL},
	{WM_WALL, WM_FREE, WM_WALL, WM_WALL, WM_WALL, WM_FREE, WM_WALL, WM_FREE, WM_WALL},
	{WM_WALL, WM_WALL, WM_SAND, WM_FREE, WM_FREE, WM_FREE, WM_WALL, WM_TARGET, WM_WALL},
	{WM_OUTER, WM_WALL, WM_WALL, WM_WALL, WM_WALL, WM_WALL, WM_WALL,WM_WALL, WM_WALL}
};

/** Pointer zum ersten Level */
static wmLevelPointer currentLevelBoard = emptyLevel;

// ID des Aktuellen Levels (defualt Level 1)
static Level currLevel = LEVEL_1;

// Wann wurde der Spieler das letzte mal bewegt
static int lastPlayerMoved = - PLAYER_MOVE_COOLDOWN;

//Die größe des Zucker stücks

/* ---- Funktionen ---- */

/**
 * Prüft ob die übergebene Position innerhalb des Levels ist.
 * @param x X-Koordinate der Position
 * @param y Y-Koordinate der Position
 * @return != 0 Wenn Position valide ist.
 */
static int
isValidPosition (Coord x, Coord y)
{
	return x < LEVELSIZE
				&& x > 0
				&& y < LEVELSIZE
				&& y > 0;
}

/**
 * Prüft ob an der übergebenen Position Sand ist.
 * @param x X-Koordinate der Position
 * @param y Y-Koordinate der Position
 * @return != 0 Wenn Position Sand ist.
 */
static int
isSand (Coord x, Coord y)
{
	return isValidPosition(x, y) && currentLevelBoard[y][x] == WM_SAND;
}

/**
 * Prüft ob an der übergebenen Position ein freies Feld ist.
 * @param x X-Koordinate der Position
 * @param y Y-Koordinate der Position
 * @return != 0 Wenn Position frei ist.
 */
static int
isFree (Coord x, Coord y)
{
	return isValidPosition(x, y) && currentLevelBoard[y][x] == WM_FREE;
}

/**
 * Prüft ob an der übergebenen Position Wasser ist.
 * @param x X-Koordinate der Position
 * @param y Y-Koordinate der Position
 * @return != 0 Wenn Position Wasser ist.
 */
static int
isWater (Coord x, Coord y)
{
	return isValidPosition(x, y) && currentLevelBoard[y][x] == WM_WATER;
}

/**
 * Prüft ob an der übergebenen Position das Ziel ist.
 * @param x X-Koordinate der Position
 * @param y Y-Koordinate der Position
 * @return != 0 Wenn Position das Ziel ist.
 */
static int
isTarget (Coord x, Coord y)
{
	return isValidPosition(x, y) && currentLevelBoard[y][x] == WM_TARGET;
}

/**
 * Prüft ob an der übergebenen Position ein Zuckerstück ist.
 * @param x X-Koordinate der Position
 * @param y Y-Koordinate der Position
 * @return != 0 Wenn Position ein Zuckerstück ist.
 */
static int
isSugar (Coord x, Coord y)
{
  return isValidPosition(x, y)
        && (currentLevelBoard[y][x] == WM_SUGARXXL
            || currentLevelBoard[y][x] == WM_SUGARXL
            || currentLevelBoard[y][x] == WM_SUGARL
            || currentLevelBoard[y][x] == WM_SUGAR);
}

/**
 * Prüft ob das bewegen an die übergebene Position möglich ist.
 * @param x X-Koordinate der Position
 * @param y Y-Koordinate der Position
 * @return != 0 Wenn Bewegung möglich
 */
static int
isValidMove(Coord x, Coord y)
 {
	 return isFree(x, y) || isTarget(x, y);
 }
 
static void
gameOver ()
{
  setPaused (-1);
}

static void
gameWon ()
{
  setPaused (1);
}

/**
 * Ausbreiten des Wassers.
 */
void
spreadWater ()
{
  wmLevel levelcopy;
  memcpy (levelcopy, currentLevelBoard, sizeof (wmLevel));
  for (int yCoord = 0; yCoord < LEVELSIZE; ++yCoord)
  {
    for (int xCoord = 0; xCoord < LEVELSIZE; ++xCoord)
    {
      if (isWater (xCoord, yCoord)) {
        if (isValidPosition (xCoord - 1, yCoord) && isFree (xCoord - 1, yCoord))
        {
          levelcopy[yCoord][xCoord - 1] = WM_WATER;
        } else if (isSugar (xCoord - 1, yCoord))
        {
          levelcopy[yCoord][xCoord - 1] = levelcopy[yCoord][xCoord - 1] + 1;
        }

        if (isValidPosition (xCoord + 1, yCoord) && isFree (xCoord + 1, yCoord))
        {
          levelcopy[yCoord][xCoord + 1] = WM_WATER;
        } else if (isSugar (xCoord + 1, yCoord))
        {
          levelcopy[yCoord][xCoord + 1] = levelcopy[yCoord][xCoord + 1] + 1;
        }

        if (isValidPosition (xCoord, yCoord + 1) && isFree (xCoord, yCoord + 1))
        {
          levelcopy[yCoord + 1][xCoord] = WM_WATER;
        } else if (isSugar (xCoord, yCoord + 1))
        {
          levelcopy[yCoord + 1][xCoord] = levelcopy[yCoord + 1][xCoord] + 1;
        }

        if (isValidPosition (xCoord, yCoord - 1) && isFree (xCoord, yCoord - 1))
        {
          levelcopy[yCoord - 1][xCoord] = WM_WATER;
        } else if (isSugar (xCoord, yCoord - 1))
        {
          levelcopy[yCoord - 1][xCoord] = levelcopy[yCoord - 1][xCoord] + 1;
        }
      }
    }
  }
  memcpy (currentLevelBoard, levelcopy, sizeof (wmLevel));
  if (isWater (playerPosition[X], playerPosition[Y]))
  {
    gameOver ();
  }
}

/**
 * Bewegt den Spieler um eine Poition in eine der
 * möglichen Richtungen auf dem Spielfeld
 * @param direction die richtung in die der Spieler bewegt
 *        werden soll
 */
void
movePlayer (Direction direction)
{
  int thisCallTime = glutGet (GLUT_ELAPSED_TIME);
  if ((thisCallTime - lastPlayerMoved) / 1000.0f >= PLAYER_MOVE_COOLDOWN)
  {
    lastPlayerMoved = thisCallTime;
    int xOffset = 0;
    int yOffset = 0;
    switch (direction)
    {
      case RIGHT :
        xOffset += ONE;
        break;
      case DOWN :
        yOffset += ONE;
        break;
      case LEFT :
        xOffset -= ONE;
        break;
      case UP :
        yOffset -= ONE;
        break;
    }

    if (isValidMove(playerPosition[X] + xOffset, playerPosition[Y] + yOffset))
    {
      // Bewegen möglich
      playerPosition[X] += xOffset;
      playerPosition[Y] += yOffset;
    } else if (isSand(playerPosition[X] + xOffset, playerPosition[Y] + yOffset)
        && isFree(playerPosition[X] + xOffset * 2, playerPosition[Y] + yOffset * 2))
    {
      // Sand weg bewegen und dann Spieler bewegen
      currentLevelBoard[playerPosition[Y] + yOffset][playerPosition[X] + xOffset] = WM_FREE;
      currentLevelBoard[playerPosition[Y] + yOffset * 2][playerPosition[X] + xOffset * 2] = WM_SAND;

      playerPosition[X] += xOffset;
      playerPosition[Y] += yOffset;
    }

    if (isTarget (playerPosition[X], playerPosition[Y]))
    {
      gameWon ();
    }
  }
}

/**
 * Getter für die Position des Spielers
 * @return Position des Spilers
 */
Position * getPlayerPosition (void)
{
  return &playerPosition;
}

/**
 * Getter für das Spielbrett des Levels
 * @return Das aktuelle Spielbrett
 */
wmLevelPointer getCurrentLevelBoard (void)
{
  return currentLevelBoard;
}

/**
 * Getter Für die ID des Levels
 * @return Die id des Levels
 */
Level
getLevel (void)
{
  return currLevel;
}
/**
 * Setter für das Level
 * @param level Das zu settende
 */
void
setLevel(Level level)
{
  currLevel = level;

	switch(currLevel)
	{
		case LEVEL_1 :
			playerPosition[X] = 7;
			playerPosition[Y] = 7;
      memcpy (currentLevelBoard, level1, sizeof (wmLevel));
			break;
		case LEVEL_2 :
			playerPosition[X] = ONE;
			playerPosition[Y] = ONE;
      memcpy (currentLevelBoard, level2, sizeof (wmLevel));
			break;
		case LEVEL_3 :
			playerPosition[X] = ONE;
			playerPosition[Y] = ONE;
      memcpy (currentLevelBoard, level3, sizeof (wmLevel));
		break;
	}

}
