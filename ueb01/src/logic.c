/**
 * @file
 * Logik-Modul.
 * Das Modul kapselt die Programmlogik. Wesentliche Programmlogik ist die
 * Verwaltung und Aktualisierung der Position und Bewegung eines Rechtecks. Die
 * Programmlogik ist weitgehend unabhaengig von Ein-/Ausgabe (io.h/c) und
 * Darstellung (scene.h/c).
 *
 * Bestandteil eines Beispielprogramms fuer Animationen mit OpenGL & GLUT.
 *
 * @author copyright (C) Fachhochschule Wedel 1999-2011. All rights reserved.
 */
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <float.h>
/* ---- Eigene Header einbinden ---- */
#include "logic.h"
#include "debugGL.h"

/** Geschwindgeit (Rotation pro sekunde) */
#define ROTATION_PS 20.0f

/* ---- Konstanten ---- */
/** Geschwindigkeit (Schritte pro Sekunde) */
#define STEPS_PS     1.0f

#define DEBUG

/* ---- Globale Daten ---- */
/** der Mittelpunkt des Quadrats */
static CGPoint2f g_ballCenter = { 0.0f, 0.0f };

/** Geschwindigkeit des Balls. */
static float g_ballSpeed = STEPS_PS;

/** Richtung, in die der Ball fliegt. */
static CGVector2f g_ballDirection = { -0.5f, 0.5f };

/** der Mittelpunkt des Schlägers des Spielers */
static CGPoint2f g_PlayerPaddleCenter = { BORDER_LEFT + PADDLE_WIDTH / 2, 0.0f};

/** die Rotation des Paddels des Spielers*/
static PaddleRotation g_PlayerPaddleRotation = 0.0f;

/** der Mittelpunkt des Schlägers der KI*/
static CGPoint2f g_KIPaddleCenter = { BORDER_RIGHT - PADDLE_WIDTH / 2, 0.0f};

/** aktuelle Zeichenfarbe */
static CGColor3f g_currentColors[5] = { {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f},
{0.0f, 0.0f, 1.0f}, {1.0f, 0.0f, 1.0f}, {0.0f, 1.0f, 1.0f}
};
/** die Farbe */
static int currentColor = 0;
/**
 * Bewegungsstatus des Schlägers des Spielers. Fuer alle vier Richtungen wird angegeben, ob
 * sich das Rechteck in die jeweilige Richtung bewegt.
 */
static GLboolean g_movement[4] = { GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE };

/* ---- Funktionen ---- */

/**
 * Berechnet die Entfernung eines Punktes p zu einem Geraden-Abschnitt mit Endpunkten a und b
 */
static float
distanceToLineSegment(CGVector2f a, CGVector2f b, CGVector2f p)
{
  CGVector2f ba = {
    b[0] - a[0],
    b[1] - a[1]
  };
  
  CGVector2f ap = {
    a[0] - p[0],
    a[1] - p[1]
  };
    
  float scalar = (ap[0] * ba[0] + ap[1] * ba[1]) / (ba[0] * ba[0] + ba[1] * ba[1]);
  CGVector2f distanceVector = {
    ap[0] - ba[0] * scalar,
    ap[1] - ba[1] * scalar
  };
  
  float distance = sqrt(
    distanceVector[0] * distanceVector[0]
    + distanceVector[1] * distanceVector[1]
  );
  
  return distance;
}

/**
 * Reflektiert den Richtungsvektor der Ballbewegung an dem Uebergebenen Vektor
 * @param surface Oberflaeche an der der Ball reflektiert
 */
static void
reflectBall (CGVector2f surface)
{
  float surfaceLength = sqrt(surface[0] * surface[0] + surface[1] * surface[1]);
  CGVector2f orthogonalVector = {
    - surface[1] / surfaceLength,
    surface[0] / surfaceLength
  };
  float dotDirection = g_ballDirection[0] * orthogonalVector[0] + g_ballDirection[1] * orthogonalVector[1];
  g_ballDirection[0] = g_ballDirection[0] - 2 * dotDirection * orthogonalVector[0];
  g_ballDirection[1] = g_ballDirection[1] - 2 * dotDirection * orthogonalVector[1];
    
}

/**
 * Prueft, ob das Rechteck mit dem Rahmen kollidiert ist.
 * @return Rahmenseite, mit der kollidiert wurde.
 */
static CGSide
checkCollision (void)
{
  CGSide
    res = sideNone
  ;
  // Ueberschreitet den rechten Rand
  if (g_ballCenter[0] > BORDER_RIGHT) 
	{
		res = sideRight;
		// Ueberschreitet den linken Rand
	} else if ( g_ballCenter[0] < BORDER_LEFT) 
	{
		res = sideLeft;
		//KI schlägt Ball zuruek
	} else if (g_ballDirection[0] > 0.0f
		&& g_KIPaddleCenter[0] - g_ballCenter[0] <= (BALL_RADIUS + PADDLE_WIDTH / 2.0f)
		&& g_ballCenter[1] < g_KIPaddleCenter[1] + (PADDLE_HEIGHT / 2)
		&& g_ballCenter[1] > g_KIPaddleCenter[1] - (PADDLE_HEIGHT / 2))
    {
		res = sideKIPaddle;
		// Trifft oberen Rand
    } else if (g_ballDirection[1] > 0.0f &&
           g_ballCenter[1] + BALL_RADIUS >= BORDER_TOP - BORDER_HEIGHT)
    {
		
      res = sideTop;
      // Trifft unteren Rand
    } else if (g_ballDirection[1] < 0.0f &&
           g_ballCenter[1] - BALL_RADIUS <= BORDER_BOTTOM + BORDER_HEIGHT)
    {
	  
      res = sideBottom;
    } 
    if (g_ballDirection[0] < 0.0f)
    {
      // Spieler schlaegt zurueck
      float toRad = M_PI / 180;
      CGVector2f initialPlayerPaddleRight = {
        0.0f,
        (g_PlayerPaddleCenter[1] + PADDLE_HEIGHT / 2.0f)
        - (g_PlayerPaddleCenter[1] - PADDLE_HEIGHT / 2.0f)
      };
      CGVector2f playerPaddleRight = {
        - sin(g_PlayerPaddleRotation * toRad) * initialPlayerPaddleRight[1],
        cos (g_PlayerPaddleRotation * toRad) * initialPlayerPaddleRight[1]
      };
      // Schlaeger ist rotiert
      CGVector2f playerPaddleBottomRight = {
        g_PlayerPaddleCenter[0] + PADDLE_WIDTH / 2.0f - playerPaddleRight[0] / 2.0f,
        g_PlayerPaddleCenter[1] - playerPaddleRight[1] / 2.0f
      };
      CGVector2f playerPaddleTopRight = {
        g_PlayerPaddleCenter[0] + PADDLE_WIDTH / 2.0f + playerPaddleRight[0] / 2.0f,
        g_PlayerPaddleCenter[1] + playerPaddleRight[1] / 2.0f
      };

      float distance = distanceToLineSegment (playerPaddleBottomRight, playerPaddleTopRight, g_ballCenter);
      if (distance < BALL_RADIUS
        && g_ballCenter[1] < playerPaddleTopRight[1] 
        && g_ballCenter[1] > playerPaddleBottomRight[1]) {
        reflectBall (playerPaddleRight);
        res = sidePlayerPaddle;
      }
    }

  return res;
}

/**
 * Berechnet neue Position des Schlägers des Spielers.
 * @param interval Dauer der Bewegung in Sekunden.
 */
void
calcPositionPaddle (double interval)
{
  //Gegen den Uhrzeigersinn drehen(Darf nicht mehr als 35° drehen)
  if (g_movement[dirLeft]
		&& g_PlayerPaddleRotation < PADDLE_RANGE / 2)
    {
      g_PlayerPaddleRotation += ROTATION_PS * (float) interval;
    }
  //Uhrzeigersinn drehen (Darf nicht mehr als -35° drehen)
  if (g_movement[dirRight]
		&& g_PlayerPaddleRotation > -PADDLE_RANGE / 2)
    {
      g_PlayerPaddleRotation -= ROTATION_PS * (float) interval;
    }
  //Nach oben bewegen (Kollision mit dem Rand wird auch überprüft)
  if (g_movement[dirUp]
		&& g_PlayerPaddleCenter[1] 
			+ (PADDLE_HEIGHT / 2) < BORDER_TOP - BORDER_HEIGHT)
    {
      g_PlayerPaddleCenter[1] += STEPS_PS * (float) interval;
    }
  //Nach unten bewegen (Kollision mit dem Rand wird auch überprüft)
  if (g_movement[dirDown]
		&& g_PlayerPaddleCenter[1]
			- (PADDLE_HEIGHT / 2) > BORDER_BOTTOM + BORDER_HEIGHT)
    {
      g_PlayerPaddleCenter[1] -= STEPS_PS * (float) interval;
    }
}

/**
 * Berechnet neue Position des Schlägers der KI.
 * @param interval Dauer der Bewegung in Sekunden.
 */
void 
calcPositionKIPaddle(double interval)
{
	//Ball befindet sich über dem Zentrum des Balls
	//-> bewege Schläger nach oben
	if (g_ballCenter[1] > g_KIPaddleCenter[1]
		&& g_KIPaddleCenter[1] + (PADDLE_HEIGHT / 2) < BORDER_TOP - BORDER_HEIGHT) 
	{
		
			g_KIPaddleCenter[1] += STEPS_PS * (float) interval;
				
	}
	
	//Ball befindet sich Unter dem Zentrum des Balls
	//-> bewege Schläger nach unten
	if (g_ballCenter[1] < g_KIPaddleCenter[1]
		&& g_KIPaddleCenter[1] - (PADDLE_HEIGHT / 2) > BORDER_BOTTOM + BORDER_HEIGHT)
	{
		
			g_KIPaddleCenter[1] -= STEPS_PS * (float) interval;
	}	
}
/**
 * Reagiert auf Kollisionen des Rechtecks mit dem Rahmen.
 * @param side Rahmenseite, mit der kollidiert wurde.
 */
static void
handleCollision (CGSide side)
{
  switch(side) {
	 
	 case sideLeft :
		  g_ballCenter[0] = -0.15f;
		  g_ballCenter[1] = 0.0f;
      g_ballSpeed = STEPS_PS;
      g_ballDirection[0] = 0.5f;
      g_ballDirection[1] = (float) rand ()/RAND_MAX - 0.5f;
		  break;
	 case sideRight :
		  g_ballCenter[0] = 0.15f;
		  g_ballCenter[1] = 0.0f;
      g_ballSpeed = STEPS_PS;
      g_ballDirection[0] = -0.5f;
      g_ballDirection[1] = (float) rand ()/RAND_MAX - 0.5f;
		  break;
	 case sideTop :
	 case sideBottom :
		  g_ballDirection[1] *= -1;
      g_ballSpeed *= 1.1f;
      break;
	 case sidePlayerPaddle :
      g_ballSpeed *= 1.1f;
		  break;
	 case sideKIPaddle :
		  g_ballDirection[0] *= -1;
      g_ballSpeed *= 1.1f;
		  break;
	default : ;
	}
	 
  }

/**
 * Berechnet neue Position des Rechtecks.
 * @param interval Dauer der Bewegung in Sekunden.
 */
void
calcPosition (double interval)
{
  CGSide
    side = 0
  ;
		side = checkCollision ();
  
		if (side != 0)
		{
		  handleCollision (side);
		  calcColors();
		}  
		g_ballCenter[0] += g_ballDirection[0] *(float) interval * g_ballSpeed;
		g_ballCenter[1] += g_ballDirection[1] * (float) interval * g_ballSpeed;
	  
}

/**
 * Berechnet neue Farbe des Balls
 */
void
calcColors ()
{
	currentColor = rand() % 5;
}


/**
 * Setzt den Bewegunsstatus des Rechtecks.
 * @param direction Bewegungsrichtung deren Status veraendert werden soll.
 * @param status neuer Status der Bewegung: GL_TRUE->Rechteck bewegt sich in Richtung
 * direction, GL_FALSE->Rechteck bewegt sich nicht in Richtung direction.
 */
void
setMovement (CGDirection direction, GLboolean status)
{
  if (direction <= dirDown)
    {
      g_movement[direction] = status;
    }
}

/**
 * Liefert den Zeiger auf die aktuelle Farbe des Balls * 
 */
CGColor3f *
getBallColor (void)
{
	return &g_currentColors[currentColor];
}
/**
 * Liefert Zeiger auf aktuelle Postion (des Mittelpunktes) des Rechtecks.
 * @return Zeiger auf Postion (des Mittelpunktes) des Rechtecks.
 */
CGPoint2f *
getCircleCenter (void)
{
  return &g_ballCenter;
}

/**
 * Liefert Zeiger auf aktuelle Postion (des Mittelpunktes) des Schlägers
 * des Spielers.
 * @return Zeiger auf Postion (des Mittelpunktes) des Schlägers
 * des Spielers.
 */
CGPoint2f *
getPlayerPaddleCenter (void)
{
  return &g_PlayerPaddleCenter;
}

/**
 * Liefert Zeiger auf aktuelle Rotation des Schlägers
 * des Spielers.
 * @return Zeiger auf Rotation des Schlägers
 * des Spielers.
 */
PaddleRotation 
getPlayerPaddleRotation (void)
{
  return g_PlayerPaddleRotation;
}

/**
 * Liefert Zeiger auf aktuelle Postion (des Mittelpunktes) des Schlägers
 * der KI.
 * @return Zeiger auf Postion (des Mittelpunktes) des Schlägers
 * der KI.
 */
CGPoint2f *
getKIPaddleCenter (void)
{
  return &g_KIPaddleCenter;
}
