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
#include "stringOutput.h"
/* ---- Globale Daten ---- */

/** Wall-Displaylist-ID  */
static GLuint g_wallListID = 0;
/** Water-Displaylist-ID  */
static GLuint g_waterListID = 0;
/** Sandbag-Displaylist-ID  */
static GLuint g_sandbagListID = 0;
/** Freefield-Displaylist-ID  */
static GLuint g_freeListID = 0;
/** Sugar-Displaylist-ID  */
static GLuint g_sugarListID = 0;
/** square-Displaylist-ID  */
static GLuint g_squareListID = 0;
/** circle-Displaylist-ID  */
static GLuint g_circleListID = 0;
/** hLine-Displaylist-ID  */
static GLuint g_hLineListID = 0;
/** vLine-Displaylist-ID  */
static GLuint g_vLineListID = 0;




/**
 * Zeichnet ein Einheits-Quadrat in der Mitte des Fensters
 */
static void
drawSquare (void)
{
	glBegin(GL_QUADS);
	{
		glVertex2f(EDGE_SIZE / 2, EDGE_SIZE/ 2);
		glVertex2f(-EDGE_SIZE / 2, EDGE_SIZE / 2);
		glVertex2f(-EDGE_SIZE / 2, -EDGE_SIZE / 2);
		glVertex2f(EDGE_SIZE / 2, -EDGE_SIZE / 2);
	}
	glEnd();
}

/**
 * Zeichnet ein Einheits-Kreis in der Mitte des Fensters
 */
static void
drawCircle (void)
{

  /* Quadric erzuegen */
  GLUquadricObj *qobj = gluNewQuadric ();

  if (qobj != 0)
    {
      /* Gefuellt darstellen */
      gluQuadricDrawStyle (qobj, GLU_FILL);
      /* Scheibe zeichnen */
      gluDisk (qobj, INNER, OUTER, SLICES, LOOPS);
      /* Loeschen des Quadrics nicht vergessen */
      gluDeleteQuadric (qobj);
    }
  else
    {
      CG_ERROR (("Quadric konnte nicht erstellt werden!\n"));
    }
}

/**
 * Zeichnet eine Horizontale Linie der Länge 1 im
 * Ursprung
 */
static void
drawHorizontalLine (void)
{
	glBegin(GL_LINES);
	{
		glVertex2f(-EDGE_SIZE / 2, ORIGIN);
		glVertex2f( EDGE_SIZE / 2, ORIGIN);
	}
	glEnd();
}

/**
 * Zeichnet eine Vertikale Linie der Länge 1 im
 * Ursprung
 */
static void
drawVerticalLine (void)
{
	glBegin(GL_LINES);
	{
		glVertex2f(ORIGIN, EDGE_SIZE / 2);
		glVertex2f(ORIGIN, -EDGE_SIZE / 2);
	}
	glEnd();
}

/**
 * Zeichnet eine Mauer
 */
static void
drawWall(void)
{
	//Abstand der Horizontalen Fugen
	GLfloat horizontalGap = EDGE_SIZE / 4;
	//Vertikale Position an der die Fuge Gezeichnet wird
	GLfloat currHorizontalGap = horizontalGap;
	//Reihe des Mauer
	GLint row = 0;
	//Der vertikale Abstand
	GLfloat verticalGap = EDGE_SIZE / 8;
	//Horizontale Position an die die Fuge gezeichnet wird
	GLfloat currVerticalGap = 0.0f;
	//Die Farbe der Mauer
	glColor3f (1.0f, 0.0f, 0.0f);
	//Die Mauer
	glCallList (g_squareListID);
	//Hellgraue Fugen der Mauer
	glColor3f(0.7f, 0.7f, 0.7f);
	//Hinzufügen von Horizontalen Fugen
	while (currHorizontalGap < EDGE_SIZE) {
		glPushMatrix();
		{
			glTranslatef(0.0f, (EDGE_SIZE / 2) - currHorizontalGap, 0.0f);
			glCallList (g_hLineListID);
		}
		glPopMatrix();
		currHorizontalGap += horizontalGap;
	}
	//Hinzufügen vertikaler Fugen
	for (; row < 4; row++)
	{
		if (row % 2)
		{
			currVerticalGap = EDGE_SIZE / 6;
			while (currVerticalGap < EDGE_SIZE)
			{
				glPushMatrix();
				{
					glTranslatef((EDGE_SIZE / 2) - currVerticalGap, (EDGE_SIZE / 2) - verticalGap, 0.0f);
					glScalef(0.0f, EDGE_SIZE / 4, 0.0f);
					glCallList (g_vLineListID);
				}
				glPopMatrix();
				currVerticalGap += EDGE_SIZE / 3;
			}
			verticalGap += EDGE_SIZE / 4;
		}
		else
		{
			currVerticalGap = EDGE_SIZE / 3;
			while (currVerticalGap  < EDGE_SIZE) {
				glPushMatrix();
				{
					glTranslatef((EDGE_SIZE / 2) - currVerticalGap, (EDGE_SIZE / 2) - verticalGap, 0.0f);
					glScalef(0.0f, EDGE_SIZE / 4, 0.0f);
					glCallList (g_vLineListID);
				}
				glPopMatrix();
				currVerticalGap += EDGE_SIZE / 3;
			}
			verticalGap += EDGE_SIZE / 4;
		}
	}
}

static void
drawSugar (void)
{
	//Random x,y Koordinate
	GLfloat x = 0.0f,
					y = 0.0f;
	//die gezeichneten Punkte
	GLint drawnDots = 0;
	//Random Zähler für Koordinaten initzialisieren
	srand((GLfloat)time(NULL));
	//Weiße farbe für den Zuckerwürfel
	glColor3f(1.0f, 1.0f, 1.0f);
	//Zuckerwürfel Zeichnen
	glCallList (g_squareListID);
	//Graue farbe fpr die Körner
	glColor3f (0.6f, 0.6f, 0.6f);
	//Die größe der Punkte
	glPointSize(5.0f);
	//Punkte auf das Zuckerstück Zeichen
	//glBegin(GL_POINTS);
	//{
		for (; drawnDots < SUGAR_DOTS; drawnDots++) {
			//Random Kooridnaten zwischne -0.5 und 0.5 generieren
			x = (GLfloat) rand () / RAND_MAX / 1.2f - 0.415f;
			y = (GLfloat) rand () / RAND_MAX / 1.2f - 0.415f;
			//(glVertex2f(x, y);
      glPushMatrix();
      {
        glTranslatef(x, y, 0.0f);
        glScalef(0.05f, 0.05f, 0.0f);
        glCallList (g_circleListID);
      }
      glPopMatrix();
		}
	//}
	//glEnd();
}

static void
drawSandbag (void)
{
	//Anzahl der zu zeichnended säcke
	GLint sandBagToDraw= 3;
	//Vertikale Abbstand der Sandsacke in einer Reihe
	GLfloat verticalGap = SANDBAG_HEIGHT;
	//Horizontaler Abstand der Sansack Reihen
	GLfloat horizontalGap = SANDBAG_WIDTH;
	//Der zu zeichnende Sandsack
	GLint sandbag = 0;
	//Farbe für den Hintergrund Der
	//Sandsäcker auf braun Setzen
	glColor3f (0.53f, 0.26f, 0.12f);
	//Hintergrund zeichnen
	glCallList (g_squareListID);
	//Farbe für Sandsäcke setzten
	glColor3f (0.82f, 0.71f, 0.55f);

	for (; sandbag < sandBagToDraw; sandbag++)
	{
		glPushMatrix();
		{
			glTranslatef((EDGE_SIZE / 2) - horizontalGap, (-EDGE_SIZE / 2) + verticalGap, 0.0f);
			glScalef(SANDBAG_WIDTH, SANDBAG_HEIGHT, 0.0f);
			glCallList (g_circleListID);
		}
		glPopMatrix();
		horizontalGap += SANDBAG_WIDTH * 2;
	}

	//Die Gaps updaten
	horizontalGap =  EDGE_SIZE / sandBagToDraw;
	verticalGap += SANDBAG_HEIGHT * 2;
	sandbag = 0;
	sandBagToDraw--;
	//Zweite Reihe zeichnen
	for (; sandbag < sandBagToDraw; sandbag++)
	{
		glPushMatrix();
		{
			glTranslatef((EDGE_SIZE / 2) - horizontalGap, (-EDGE_SIZE / 2) + verticalGap, 0.0f);
			glScalef(SANDBAG_WIDTH, SANDBAG_HEIGHT, 0.0f);
			glCallList (g_circleListID);
		}
		glPopMatrix();
		horizontalGap += SANDBAG_WIDTH * 2;
	}

	horizontalGap =  EDGE_SIZE / sandBagToDraw;
	verticalGap += SANDBAG_HEIGHT * 2;

	glPushMatrix();
	{
		glTranslatef((EDGE_SIZE / 2) - horizontalGap, (-EDGE_SIZE / 2) + verticalGap, 0.0f);
		glScalef(SANDBAG_WIDTH, SANDBAG_HEIGHT, 0.0f);
		glCallList (g_circleListID);
	}
	glPopMatrix();

}

static void
drawWave (void)
{
	//Farbe der welle
	glColor3f(1.0f, 1.0f, 1.0f);

	glPointSize(1);
	//Zeiche die Kurve
	for (float x = -3.5f; x <= 6.8f; x+= 0.01)
	{
		glBegin(GL_POINTS);
		{
			glVertex2f(x, sinf(x));
		}
		glEnd();
	}

}

static void
drawWater (void)
{
	//Der Horizontale Abstand
	GLfloat vGap = EDGE_SIZE / 7;
	//Die zu zeichnende Welle
	GLint drawnWave = 0;
	//DIe Anzahl zu zeichnender Wellen (pro reihe)
	GLint toDraw = 3;
	//Farbe für wasser auf hellBlaus setzen
	glColor3f(0.0f, 0.0f, 0.7f);
	//Wasser zeichnen
	glCallList (g_squareListID);
	for (; drawnWave < toDraw; drawnWave++) {
		glPushMatrix();
		{
			glTranslatef(- 0.1f, (EDGE_SIZE / 2) - (vGap + EDGE_SIZE / 7), 0.0f);
			glScalef(0.07f, 0.07f, 0.0f);
			drawWave();
		}
		glPopMatrix();
		vGap += EDGE_SIZE / 4;
	}
}

/**
 * Zeichent ein Freies Feld
 */
static void
drawFreeField (void)
{
	//Graue Farbe für ein leeres Feld
	glColor3f (0.6f, 0.6f, 0.6f);
	glCallList (g_squareListID);
}

static void
drawTarget(void)
{	//Grüne farbe für das Zielfeld
	glColor3f(0.0f, 1.0f, 0.0f);
	glCallList (g_squareListID);
}

/**
 * Methode um den Spieler zu zeichen
 */
static void
drawStickFigure (void)
{
  //Den Kopf zeichnen
  glPushMatrix();
  {
    glColor3f (0.898f, 0.76f, 0.596f);
    glTranslatef(0.0f, (EDGE_SIZE / 2) - (EDGE_SIZE / 4), 0.0f );
    glScalef((EDGE_SIZE / 8), (EDGE_SIZE / 8), 0.0f);
    glCallList (g_circleListID);
  }
  glPopMatrix();
  glPushMatrix ();
  {
    glColor3f (0.0f, 0.9f, 1.0f);
    //Den Körper Zeichen
    glPushMatrix();
    {
      glScalef((EDGE_SIZE / 4), (EDGE_SIZE / 4), 0.0f);
      glCallList (g_vLineListID);
    }
    glPopMatrix();
    //Die Arme zeichnen
    glPushMatrix();
    {
      glTranslatef((EDGE_SIZE / 12), 0.0f, 0.0f);
      glRotatef(45.0f, 0.0f, 0.0f, 1.0f);
      glScalef((EDGE_SIZE / 4), (EDGE_SIZE / 4), 0.0f);
      glCallList (g_vLineListID);
    }
    glPopMatrix();
    glPushMatrix();
    {
      glTranslatef((-EDGE_SIZE / 12), 0.0f, 0.0f);
      glRotatef(-45.0f, 0.0f, 0.0f, 1.0f);
      glScalef((EDGE_SIZE / 4), (EDGE_SIZE / 4), 0.0f);
      glCallList (g_vLineListID);
    }
    glPopMatrix();
    //Die Beine zeichnen
    glPushMatrix();
    {
      glTranslatef((EDGE_SIZE / 12), (-EDGE_SIZE / 5), 0.0f);
      glRotatef(45.0f, 0.0f, 0.0f, 1.0f);
      glScalef((EDGE_SIZE / 4), (EDGE_SIZE / 4), 0.0f);
      glCallList (g_vLineListID);
    }
    glPopMatrix();
    glPushMatrix();
    {
      glTranslatef((-EDGE_SIZE / 12), (-EDGE_SIZE / 5), 0.0f);
      glRotatef(-45.0f, 0.0f, 0.0f, 1.0f);
      glScalef((EDGE_SIZE / 4), (EDGE_SIZE / 4), 0.0f);
      glCallList (g_vLineListID);
    }
    glPopMatrix();
  }
  glPopMatrix ();
}

/**
 *
 */
 static void
 drawPlayer (void)
 {
	 //Die aktuelle Poition des Spileres
 	 Position * playerPosition = getPlayerPosition();
	 	//Die farbe des Spielers Setzen
	 	glColor3f(0.0f, 0.0f, 0.0f);
		glPushMatrix();
		{
		 glTranslatef(-EDGE_SIZE + ((2.0f / 10.0f) * ((*playerPosition)[0] + 1)),
									 EDGE_SIZE - ((2.0f / 10.0f) * ((*playerPosition)[1] + 1)),
									 0.0f);
		 glScalef(2.0f / 10.5f, 2.0f / 10.5f, 0.0f);
		 drawStickFigure();
		}
		glPopMatrix();
 }

/**
 * Erzeugen der Display-Listen fuer Wuerfel und Koordinatenachsen.
 */
static void
initDisplayLists (void)
{
  /* Neun Display-Listen anfordern */
  g_waterListID = glGenLists ((GLsizei) 9);
  if (g_waterListID != 0)
    {
			/* Erste Diplay-Liste */
      glNewList (g_waterListID, GL_COMPILE);
      drawWater();
      glEndList ();

			/* Zweite Diplay-Liste */
			g_wallListID = g_waterListID + 1;
      glNewList (g_wallListID, GL_COMPILE);
      drawWall();
      glEndList ();

			/* Dritte Diplay-Liste */
      g_sandbagListID = g_wallListID + 1;
      glNewList (g_sandbagListID, GL_COMPILE);
      drawSandbag();
      glEndList ();

			/* Vierte Diplay-Liste */
      g_freeListID = g_sandbagListID + 1;
      glNewList (g_freeListID, GL_COMPILE);
      drawFreeField();
      glEndList ();

			/* Fünfte Diplay-Liste */
      g_sugarListID = g_freeListID + 1;
      glNewList (g_sugarListID, GL_COMPILE);
      drawSugar ();
      glEndList ();
      
      g_squareListID = g_sugarListID + 1;
      glNewList (g_squareListID, GL_COMPILE);
      drawSquare ();
      glEndList ();
      
      g_circleListID = g_squareListID + 1;
      glNewList (g_circleListID, GL_COMPILE);
      drawCircle ();
      glEndList ();
      
      g_hLineListID = g_circleListID + 1;
      glNewList (g_hLineListID, GL_COMPILE);
      drawHorizontalLine ();
      glEndList ();
      
      g_vLineListID = g_hLineListID + 1;
      glNewList (g_vLineListID, GL_COMPILE);
      drawVerticalLine ();
      glEndList ();
    }
  else
    {
      CG_ERROR (("Konnte Displaylisten nicht erzeuge\n"));
    }
}

/**
 * Zeichnet das Aktuelle level
 */
static void
drawLevel()
{
	//Die aktuell Zeile des Spielfelds
	GLint row = 0;
	//Die aktuelle Spalte des Spielfeldes
	GLint col = 0;
	wmLevelPointer level = getCurrentLevelBoard();
	//Der Abstand der Felder
	//Initzialisiert mit der hälfte eines Feldes
	GLfloat vGap = 2.0f / 10.0f;
	GLfloat hGap = 2.0f / 10.0f;
	for (;row < LEVELSIZE; row++)
	{
		for (col = 0;col < LEVELSIZE; col++)
		{
			glPushMatrix();
			{
				glTranslatef(-EDGE_SIZE + hGap, EDGE_SIZE - vGap, 0.0f);
				glScalef(2.0f / 10.5f, 2.0f / 10.5f, 0.0f);
				switch (level[row][col])
				{
					case WM_FREE :
						glCallList(g_freeListID);
						break;
					case WM_WALL :
						glCallList(g_wallListID);
						break;
					case WM_WATER :
						glCallList(g_waterListID);
						break;
					case WM_SAND :
						glCallList(g_sandbagListID);
						break;
					case WM_SUGARXXL :
						glCallList(g_sugarListID);
						break;
					case WM_SUGARXL :
						glCallList(g_freeListID);
						glPushMatrix();
						{
							glScalef(EDGE_SIZE / 1.5f, EDGE_SIZE / 1.5f, 0.0f);
							glCallList(g_sugarListID);
						}
						glPopMatrix();
						break;
					case WM_SUGARL :
						glCallList(g_freeListID);
						glPushMatrix();
						{
							glScalef(EDGE_SIZE / 2, EDGE_SIZE / 2, 0.0f);
							glCallList(g_sugarListID);
						}
						glPopMatrix();
						break;
						case WM_SUGAR :
							glCallList(g_freeListID);
							glPushMatrix();
							{
								glScalef(EDGE_SIZE / 3, EDGE_SIZE / 3, 0.0f);
								glCallList(g_sugarListID);
							}
							glPopMatrix();
							break;
					case WM_TARGET :
						drawTarget();
						break;
					default : break;
				}
			}
			glPopMatrix();
			hGap += 2.0f / 10.0f;
		}
			hGap = 2.0f / 10.0f;
			vGap += 2.0f / 10.0f;
	}
}



/**
 * Zeichen-Funktion.
 * Stellt die Szene dar.
 * Ein Ball, zwei Schlaeger, Rand oben, Rand unten
 */
void
drawScene (void)
{
	/* Displaylisten initialisieren */
	 initDisplayLists ();
	 drawLevel();
	 drawPlayer();

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
  /* Linienbreite */
  glLineWidth (3.0f);

  /* Alles in Ordnung? */
  return (GLGETERROR == GL_NO_ERROR);
}
