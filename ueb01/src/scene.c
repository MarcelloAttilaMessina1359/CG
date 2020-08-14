#define DEBUG
/**
 * @file
 * Darstellungs-Modul.
 * Das Modul kapselt die Rendering-Funktionalitaet (insbesondere der OpenGL-
 * Aufrufe) des Programms.
 *
 * Bestandteil eines Beispielprogramms fuer Animationen mit OpenGL & GLUT.
 *
 * @author copyright (C) Fachhochschule Wedel 1999-2011. All rights reserved.
 */

#include <math.h>
#include <stdlib.h>
#include <time.h>

/* ---- System Header einbinden ---- */
#ifdef WIN32
#include <windows.h>
#endif

#ifdef MACOSX
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

/* ---- Eigene Header einbinden ---- */
#include "scene.h"
#include "logic.h"
#include "types.h"
#include "debugGL.h"

/**
 * Form des Balls:
 * 0 => Ball
 * 1 => Dreieck
 * 2 => Stern
 */
int ballModel = 0;

/**
 * veraendert die Form des Balls.
 */
void
changeBallModel (void) {
  switch (ballModel) {
    case 0:
      ballModel = 1;
      break;
    case 1:
      ballModel = 2;
      break;
    case 2:
      ballModel = 0;
      break;
  }
}

/**
 * Zeichnet ein Einheits-Quadrat in der Mitte des Fensters
 */
static void
drawSquad (void)
{
	glBegin(GL_QUADS);
	{
		glVertex2f(QUAD_WIDTH / 2, QUAD_HEIGHT/ 2);
		glVertex2f(-QUAD_WIDTH / 2, QUAD_HEIGHT / 2);
		glVertex2f(-QUAD_WIDTH / 2, -QUAD_HEIGHT / 2);
		glVertex2f(QUAD_WIDTH / 2, -QUAD_HEIGHT / 2);
	}
	glEnd();
}

/**
 * Zeichnet den Ball je nach ausgewaehltem Ball-Modell.
 */
static void
drawBallModel(void) 
{
	glBegin (GL_TRIANGLE_FAN);
    {
      glVertex2f (0.0f, 0.0f);
      
      GLfloat twicePi = 2.0f * M_PI;

      int triangleCornerAmount = 10;
      
      if (ballModel == 1) {
        triangleCornerAmount = 3;
      } else if (ballModel == 2) {
        triangleCornerAmount = 16;
      }

      if (ballModel == 2) {
        // Stern
        float offset = 0.5f;
        float xCoordinate;
        float yCoordinate;
        for (int i = 0; i <= triangleCornerAmount; ++i) {
          xCoordinate = cos(i * twicePi / triangleCornerAmount);
          yCoordinate = sin(i * twicePi / triangleCornerAmount);
          if ( i % 2 > 0 ) {
            if ( xCoordinate > 0.0f ) {
              xCoordinate -= offset;
            } else if (xCoordinate < 0.0f) {
              xCoordinate += offset;
            }
            if (yCoordinate > 0.0f) {
              yCoordinate -= offset;
            } else if (yCoordinate < 0.0f) {
              yCoordinate += offset;
            }
          }
          glVertex2f ( xCoordinate, yCoordinate );
        }
      } else {
        // Ball oder Dreieck
        for (int i = 0; i <= triangleCornerAmount; ++i) {
          glVertex2f ( cos(i * twicePi / triangleCornerAmount),
                       sin(i * twicePi / triangleCornerAmount));
        }
      }
    }
    glEnd ();
}

/**
 * Zeichnet den Ball
 */
static void
drawBall (void)
{
  /* In der Logik berechnet Position beziehen */
  CGPoint2f *p_circleCenter = getCircleCenter ();
  CGColor3f *ballColor = getBallColor();
  glPushMatrix ();
  {  
	  glColor3f ((*ballColor)[0], (*ballColor)[1], (*ballColor)[2]);
	  glTranslatef((*p_circleCenter)[0], (*p_circleCenter)[1] ,0.0f);
	  glScalef (BALL_RADIUS, BALL_RADIUS, 1.0f);
	  drawBallModel();
  } 
  glPopMatrix ();
}

/**
 * Zeichnet die Schlaeger
 */
static void
drawAllPaddles (void)
{
    CGPoint2f *p_PlayerPaddleCenter = getPlayerPaddleCenter();
	CGPoint2f *p_KIPaddleCenter = getKIPaddleCenter();
	
	glPushMatrix();
	{
		
		glTranslatef((*p_PlayerPaddleCenter)[0], (*p_PlayerPaddleCenter)[1] ,0.0f);
		glRotatef(getPlayerPaddleRotation(), 0.0f, 0.0f, 1.0f);
		glScalef(PADDLE_WIDTH,PADDLE_HEIGHT, 0.0f);		
		drawSquad();
	}
	glPopMatrix();
	glPushMatrix();	
	{
		
		glTranslatef((*p_KIPaddleCenter)[0], (*p_KIPaddleCenter)[1] ,0.0f);
		glScalef(PADDLE_WIDTH,PADDLE_HEIGHT, 0.0f);
		drawSquad();
	}
	glPopMatrix();
}

/**
 * Zeichnet einen Rahmen um den Bereich [BORDER_TOP/BORDER_BOTTOM]
 * 
 */
static void
drawAllBorders (void)
{
  glPushMatrix();
  {
	
	glTranslatef(0.0f,BORDER_TOP - BORDER_HEIGHT / 2, 0.0f);
	glScalef(BORDER_WIDTH, BORDER_HEIGHT,0.0f);
	drawSquad();
  }
  glPopMatrix();
  glPushMatrix();
  {
	glTranslatef(0.0f, BORDER_BOTTOM + BORDER_HEIGHT / 2, 0.0f);
	glScalef(BORDER_WIDTH, BORDER_HEIGHT,0.0f);
	drawSquad();
  }
  glPopMatrix();
}

/**
 * Zeichen-Funktion.
 * Stellt die Szene dar.
 * Ein Ball, zwei Schlaeger, Rand oben, Rand unten
 */
void
drawScene (void)
{
  /* Zeichenfarbe */
  glColor3f (1.0f, 1.0f, 1.0f);
  
  /* Paddel malen */
  drawAllPaddles();

  /* Rahmen zeichnen */
  drawAllBorders ();
  
  /* Quadrat zeichnen */
  drawBall ();    
}

/**
 * Initialisierung der Szene (inbesondere der OpenGL-Statusmaschine).
 * Setzt Hintergrund- und Zeichenfarbe.
 * @return Rueckgabewert: im Fehlerfall 0, sonst 1.
 */
int
initScene (void)
{
  /* Setzen der Farbattribute */
  /* Hintergrundfarbe */
  glClearColor (0.0f, 0.0f, 0.0f, 0.0f);
  /* Zeichenfarbe */
  glColor3f (1.0f, 1.0f, 1.0f);
  /* Ball Farbe */
  calcColors();  
  /* Linienbreite */
  glLineWidth (3.0f);

  /* Alles in Ordnung? */
  return (GLGETERROR == GL_NO_ERROR);
}
