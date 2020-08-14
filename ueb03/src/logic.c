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
#include <stdio.h>

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

/* HilfsFunktion */
#define ABS(X)  ((X>=0)? X : -(X) )
#define ROUND(X)  (X>=0)? (int) (X + 0.5) : (int)-(ABS(X) +0.5)

/* ---- Konstanten ---- */
#define X 0
#define Y 1
#define ONE 1
#define PLAYER_MOVE_COOLDOWN 0.3f
/** Geschwindigkeit (Schritte pro Sekunde) */
#define STEPS_PS  2.5f

/** Anzahl der Wuerfeldrehungen pro Sekunde. */

/* ---- Globale Daten ---- */

/** Die Position des Spielers auf dem Spielfeld */
static CGPoint2f playerPosition = {1, 1};

/** Die Position auf die sich der Spieler bewegen will */
static CGPoint2f nextPlayerPosition = {0, 0};

/** Status der ersten Lichtquelle (an/aus) */
static int g_light0Status = 1;

/** Status der zweiten Lichtquelle (an/aus) */
static int g_light1Status = 1;

/** Status, ob Normalen gezeichnet werden sollen. */
static int g_showNormals = GL_FALSE;

static Direction g_playerRotation = 0;

static wmLevel emptyLevel = { };

/* Gibt an ob die Kameraperspektive verändert wurde */
Perspective c_perspective = BOARD;

/** Das erste Level */
static wmLevel level1 =
{
	{WM_WALL, WM_WALL, WM_WALL, WM_WALL, WM_WALL, WM_WALL, WM_WALL, WM_WALL, WM_WALL},
	{WM_WALL, WM_FREE, WM_FREE, WM_FREE, WM_FREE, WM_FREE, WM_FREE, WM_FREE, WM_WALL},
	{WM_WALL, WM_FREE, WM_WALL, WM_FREE, WM_WALL, WM_WALL, WM_WALL, WM_FREE, WM_WALL},
	{WM_WALL, WM_WALL, WM_SUGARXXL, WM_FREE, WM_FREE, WM_FREE, WM_WALL, WM_FREE, WM_WALL},
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
int
movePlayer (double interval)
{
    GLfloat xOffset = 0;
    GLfloat yOffset = 0;
		GLint nextX = 0;
		GLint nextY = 0;
    GLint move = GL_FALSE;

		if (g_playerRotation == RIGHT)
	 	{
			xOffset += STEPS_PS * (float) interval;
			nextX = 1;
		}
		else if (g_playerRotation == DOWN)
	 	{
			yOffset += STEPS_PS * (float) interval;
			nextY = 1;
		}
		else if (g_playerRotation == LEFT)
	 	{
			xOffset -= STEPS_PS * (float) interval;
			nextX = 1;
		}
		else if (g_playerRotation == UP)
	 	{
			yOffset -= STEPS_PS * (float) interval;
			nextY = -1;
		}
    if (isValidMove(nextPlayerPosition[X], nextPlayerPosition[Y]))
    {
      move = GL_TRUE;
    } else if (isSand(nextPlayerPosition[X], nextPlayerPosition[Y])
        && isFree(nextPlayerPosition[X] + nextX, nextPlayerPosition[Y] + nextY))
    {
      // Sand weg bewegen und dann Spieler bewegen
      currentLevelBoard[(int) nextPlayerPosition[Y]][(int) nextPlayerPosition[X]] = WM_FREE;
      currentLevelBoard[(int) nextPlayerPosition[Y] + nextY][(int) nextPlayerPosition[X] + nextX] = WM_SAND;
      
      move = GL_TRUE;

    }
    if (move)
    {
      playerPosition[X] += xOffset;
      playerPosition[Y] += yOffset;
      if (g_playerRotation == UP && playerPosition[Y] < nextPlayerPosition[Y])
      {
        playerPosition[Y] = nextPlayerPosition[Y];
      } else if (g_playerRotation == DOWN && playerPosition[Y] > nextPlayerPosition[Y]) {
        playerPosition[Y] = nextPlayerPosition[Y];
      } else if (g_playerRotation == LEFT && playerPosition[X] < nextPlayerPosition[X]) {
        playerPosition[X] = nextPlayerPosition[X];
      } else if (g_playerRotation == RIGHT && playerPosition[X] > nextPlayerPosition[X]) {
        playerPosition[X] = nextPlayerPosition[X];
      }
      if (isTarget (playerPosition[X] + BOARD_SHIFT, playerPosition[Y] + BOARD_SHIFT))
      {
        gameWon ();
      }
    } else {
      nextPlayerPosition[X] = playerPosition[X];
      nextPlayerPosition[Y] = playerPosition[Y];
    }
    return playerPosition[X] != nextPlayerPosition[X]
      || playerPosition[Y] != nextPlayerPosition[Y];

}

/**
 * Getter für die Position des Spielers
 * @return Position des Spilers
 */
CGPoint2f * getPlayerPosition (void)
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

/**
 * Gibt an ob die Kamera gedreht wurde
 */
int
getCameraStatus (void)
{
	return c_perspective;
}

/**
 * Methode um die perspective zu setzten
 */
void
setCameraStatus (Perspective perspective)
{
	c_perspective = perspective;
}

/**
 * Setzt den Bewegunsstatus des Spielers.
 * @param direction Bewegungsrichtung deren Status veraendert werden soll.
 * @param status neuer Status der Bewegung: GL_TRUE->Rechteck bewegt sich in Richtung
 * direction, GL_FALSE->Rechteck bewegt sich nicht in Richtung direction.
 */
void
setMovement (Direction direction)
{
  if (direction <= LEFT)
    {
			g_playerRotation = direction;
      switch (direction) {
        case UP:
          nextPlayerPosition[X] = playerPosition[X];
          nextPlayerPosition[Y] = playerPosition[Y] - 1;
          break;
        case DOWN:
          nextPlayerPosition[X] = playerPosition[X];
          nextPlayerPosition[Y] = playerPosition[Y] + 1;
          break;
        case RIGHT:
          nextPlayerPosition[X] = playerPosition[X] + 1;
          nextPlayerPosition[Y] = playerPosition[Y];
          break;
        case LEFT:
          nextPlayerPosition[X] = playerPosition[X] - 1;
          nextPlayerPosition[Y] = playerPosition[Y];
          break;
      }
    }
}


Direction getPlayerRotation (void)
{
	switch (g_playerRotation) {
		case UP :
			return 180;
		case DOWN :
			return 0;
		case RIGHT :
			return 90;
		case LEFT :
			return 270;
	}
  return g_playerRotation;
}

/**
 * Liefert den Status der ersten Lichtquelle.
 * @return Status der ersten Lichtquelle (an/aus).
 */
int
getLight0Status (void)
{
  return g_light0Status;
}

/**
 * Setzt den Status der ersten Lichtquelle.
 * @param status Status der ersten Lichtquelle (an/aus).
 */
void
setLight0Status (int status)
{
  g_light0Status = status;
}

/**
 * Liefert den Status der zweiten Lichtquelle.
 * @return Status der zweiten Lichtquelle (an/aus).
 */
int
getLight1Status (void)
{
  return g_light1Status;
}

/**
 * Setzt den Status der zweiten Lichtquelle.
 * @param status Status der zweiten Lichtquelle (an/aus).
 */
void
setLight1Status (int status)
{
  g_light1Status = status;
}

/**
 * Gibt zurueck ob die normalen gezeichnet werden sollen.
 */
int
getShowNormals (void)
{
  return g_showNormals;
}

/**
 * Setzt, ob die normalen gezeichnet werden sollen.
 * @param status Status, ob Normalen gezeichnet werden sollen.
 */
void
setShowNormals (int status)
{
  g_showNormals = status;
}
