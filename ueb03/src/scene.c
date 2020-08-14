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

/* ---- Makros ---- */
#ifndef M_PI
/** Die mathematische Konstante PI */
#define M_PI 3.14159265358979323846f
#endif

#ifndef DEG2RAD
/** Winkelumrechnung von Grad nach Radiant */
#define DEG2RAD(x) ((x)/180.0f*M_PI)
/** Winkelumrechnung von Radiant nach Grad */
#define RAD2DEG(x) ((x)/M_PI*180.0f)
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
/** SquareSubDIV-Displaylist-ID  */
static GLuint g_squareSubDivListID = 0;
/** drawCube-Displaylist-ID  */
static GLuint g_cubeivListID = 0;
/** 3dwall-Displaylist-ID  */
static GLuint g_3DWallListID = 0;
/** draw3DFreeField-Displaylist-ID  */
static GLuint g_3DFreeListID = 0;
/** draw3DSugarField-Displaylist-ID  */
static GLuint g_3DSugarListID = 0;
/** draw3DFreeField-Displaylist-ID  */
static GLuint g_sphereListID = 0;
/** drawCylinder-Displaylist-ID  */
static GLuint g_cylinderListID = 0;
/** draw3DStickFigure-Displaylist-ID  */
static GLuint g_3DStickFigureListID = 0;
/** draw3DPlayer-Displaylist-ID  */
static GLuint g_3DPlayerListID = 0;
/** draw3DSandbag-Displaylist-ID  */
static GLuint g_3DSandbagID = 0;
/** draw3DTarget-Displaylist-ID  */
static GLuint g_3DTargetID = 0;
/** draw3DWater-Displaylist-ID  */
static GLuint g_3DWaterListID = 0;
/** wallGaps-Displaylist-ID  */
static GLuint g_wallGaps = 0;



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
 * Zeichnet ein Quadrat mit der Kantenlaenge 1, das aus mehreren kleineren
 * Quadraten zusammen gesetzt ist.
 */
 static void
 drawSubDivSquare ()
 {
   int x, y;

   for (y = 0; y < SUB_DIVS + 1; y++)
     {
			 CGVector3f normal;
       glBegin (GL_QUAD_STRIP);
			 glNormal3f (0.0f, 0.0f, 1.0f);
       for (x = 0; x <= SUB_DIVS + 1; x++)
         {
           glVertex3f (x / (SUB_DIVS + 1.0f) - 0.5f,
                       (y + 1) / (SUB_DIVS + 1.0f) - 0.5f, 0.0f);
           glVertex3f (x / (SUB_DIVS + 1.0f) - 0.5f,
                       y / (SUB_DIVS + 1.0f) - 0.5f, 0.0f);
         }
			 glEnd ();
			 if (getShowNormals())
			 {
				 glBegin(GL_LINES);
				  glGetFloatv(GL_CURRENT_NORMAL, normal);
				  glVertex3f(normal[0], normal[1], normal[2] - 1);
				  glVertex3f(normal[0], normal[1], normal[2] + 0.5);
				 glEnd();
			 }

     }
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
			/* Normalen fuer Quadrics berechnen lassen */
      gluQuadricNormals (qobj, GLU_SMOOTH);
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
		glNormal3f (0.0f, 0.0f, 1.0f);
		glVertex2f(-EDGE_SIZE / 2, ORIGIN);
		glVertex2f( EDGE_SIZE / 2, ORIGIN);
    
	}
	glEnd();
}

static void
drawWallGaps (void)
{
	//Abstand der Horizontalen Fugen
	GLfloat horizontalGap = EDGE_SIZE / 4;
	//Vertikale Position an der die Fuge Gezeichnet wird
	GLfloat currHorizontalGap = horizontalGap;
	//Reihe der Mauer
	GLint row = 0;
	//Der vertikale Abstand
	GLfloat verticalGap = EDGE_SIZE / 8;
	//Horizontale Position an die die Fuge gezeichnet wird
	GLfloat currVerticalGap = 0.0f;

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

/**
 * Zeichnet eine Vertikale Linie der Länge 1 im
 * Ursprung
 */
static void
drawVerticalLine (void)
{
	glBegin(GL_LINES);
	{
		glNormal3f (0.0f, 0.0f, 1.0f);
		glVertex2f(ORIGIN, EDGE_SIZE / 2);
		glVertex2f(ORIGIN, -EDGE_SIZE / 2);
	}
  
  if (getShowNormals())
  {
    GLint normal[3];
    glGetIntegerv(GL_CURRENT_NORMAL, normal);
    glBegin(GL_LINES);
    glVertex3f(0, 0, 0);
    glVertex3f(normal[0], normal[1], normal[2]);
    glEnd();
  }
	glEnd();
}

static void
drawCube (void)
{
	/* Frontflaeche */
  glPushMatrix ();
  {
    glTranslatef (0.0f, 0.0f, 0.5f);
    glCallList(g_squareSubDivListID);

  }
  glPopMatrix ();

  /* rechte Seitenflaeche */
  glPushMatrix ();
  {
    glRotatef (90.0f, 0.0f, 1.0f, 0.0f);
    glTranslatef (0.0f, 0.0f, 0.5f);
    glCallList(g_squareSubDivListID);
  }
  glPopMatrix ();

  /* Rueckseitenflaeche */
  glPushMatrix ();
  {
    glRotatef (180.0f, 0.0f, 1.0f, 0.0f);
    glTranslatef (0.0f, 0.0f, 0.5f);
    glCallList(g_squareSubDivListID);
  }
  glPopMatrix ();

  /* linke Seitenflaeche */
  glPushMatrix ();
  {
    glRotatef (270.0f, 0.0f, 1.0f, 0.0f);
    glTranslatef (0.0f, 0.0f, 0.5f);
    glCallList(g_squareSubDivListID);
  }
  glPopMatrix ();

  /* Deckelflaeche */
  glPushMatrix ();
  {
    glRotatef (-90.0f, 1.0f, 0.0f, 0.0f);
    glTranslatef (0.0f, 0.0f, 0.5f);
    glCallList(g_squareSubDivListID);
  }
  glPopMatrix ();

  /* Bodenflaeche */
  glPushMatrix ();
  {
    glRotatef (90.0f, 1.0f, 0.0f, 0.0f);
    glTranslatef (0.0f, 0.0f, 0.5f);
    glCallList(g_squareSubDivListID);
  }
  glPopMatrix ();
}

static void
drawAllWallGaps (void)
{
	/* Frontflaeche */
  glPushMatrix ();
  {
    glTranslatef (0.0f, 0.0f, 0.5f);
    drawWallGaps();

  }
  glPopMatrix ();

  /* rechte Seitenflaeche */
  glPushMatrix ();
  {
    glRotatef (90.0f, 0.0f, 1.0f, 0.0f);
    glTranslatef (0.0f, 0.0f, 0.5f);
    drawWallGaps();
  }
  glPopMatrix ();

  /* Rueckseitenflaeche */
  glPushMatrix ();
  {
    glRotatef (180.0f, 0.0f, 1.0f, 0.0f);
    glTranslatef (0.0f, 0.0f, 0.5f);
    drawWallGaps();
  }
  glPopMatrix ();

  /* linke Seitenflaeche */
  glPushMatrix ();
  {
    glRotatef (270.0f, 0.0f, 1.0f, 0.0f);
    glTranslatef (0.0f, 0.0f, 0.5f);
    drawWallGaps();
  }
  glPopMatrix ();

  /* Deckelflaeche */
  glPushMatrix ();
  {
    glRotatef (-90.0f, 1.0f, 0.0f, 0.0f);
    glTranslatef (0.0f, 0.0f, 0.5f);
    drawWallGaps();
  }
  glPopMatrix ();

  /* Bodenflaeche */
  glPushMatrix ();
  {
    glRotatef (90.0f, 1.0f, 0.0f, 0.0f);
    glTranslatef (0.0f, 0.0f, 0.5f);
  	drawWallGaps();
  }
  glPopMatrix ();
}

/**
 * Zeichnet eine 3D-Mauer
 */
static void
draw3DWall (void)
{
	/* Material des Fugen */
	float matAmbientGaps[] = { .5f, .5f, .5f , 1.f};
	float matDiffuseGaps[] = { .5f, .5f, .5f , 1.f};
	float matSpecularGaps[] = { .5f, .5f, .5f, 1.f};
	float matShininessGaps[] = { 10.0f };

	/* Setzen der Material-Parameter */
	glMaterialfv (GL_FRONT, GL_AMBIENT, matAmbientGaps);
	glMaterialfv (GL_FRONT, GL_DIFFUSE, matDiffuseGaps);
	glMaterialfv (GL_FRONT, GL_SPECULAR, matSpecularGaps);
	glMaterialfv (GL_FRONT, GL_SHININESS, matShininessGaps);
	glCallList(g_wallGaps);

	/* Material des Wuerfels */
  float matAmbient[] = { 1.0f, 0.0f, 0.0f , 1.f};
  float matDiffuse[] = { 1.0f, 0.0f, 0.0f , 1.0f};
  float matSpecular[] = { 1.0f, 1.f, 1.0f , 1.0f};
  float matShininess[] = { 20.0f };

  /* Setzen der Material-Parameter */
  glMaterialfv (GL_FRONT, GL_AMBIENT, matAmbient);
  glMaterialfv (GL_FRONT, GL_DIFFUSE, matDiffuse);
  glMaterialfv (GL_FRONT, GL_SPECULAR, matSpecular);
  glMaterialfv (GL_FRONT, GL_SHININESS, matShininess);
	glCallList(g_cubeivListID);
}



/**
 * Zeichnet eine Mauer
 */
static void
drawWall(void)
{
	//Die fugen zwischen den Backsteinen
	drawWallGaps ();
	//Die Farbe der Mauer
	glColor3f (1.0f, 0.0f, 0.0f);
	//Die Mauer
	glCallList (g_squareListID);
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
	//Graue farbe fpr die Körner
	glColor3f (0.6f, 0.6f, 0.6f);
	//Die größe der Punkte
	glPointSize(5.0f);
	//Punkte auf das Zuckerstück Zeichen
  for (; drawnDots < SUGAR_DOTS; drawnDots++) {
    //Random Kooridnaten zwischne -0.5 und 0.5 generieren
    x = (GLfloat) rand () / RAND_MAX / 1.2f - 0.415f;
    y = (GLfloat) rand () / RAND_MAX / 1.2f - 0.415f;
    glPushMatrix();
    {
      glTranslatef(x, y, 0.0f);
      glScalef(0.05f, 0.05f, 0.0f);
      glCallList (g_circleListID);
    }
    glPopMatrix();
  }
	//Weiße farbe für den Zuckerwürfel
	glColor3f(1.0f, 1.0f, 1.0f);
	//Zuckerwürfel Zeichnen
	glCallList (g_squareListID);
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

	glColor3f (0.53f, 0.26f, 0.12f);
	//Hintergrund zeichnen
	glCallList (g_squareListID);
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
		glNormal3f (0.0f, 0.0f, 1.0f);
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
	//Farbe für wasser auf hellBlaus setzen
	glColor3f(0.0f, 0.0f, 0.7f);
	//Wasser zeichnen
	glCallList (g_squareListID);
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
	 /** Punkt im 2D-Raum */
   CGPoint2f * playerPosition = getPlayerPosition();
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
						glPushMatrix();
						{
							glScalef(EDGE_SIZE / 1.5f, EDGE_SIZE / 1.5f, 0.0f);
							glCallList(g_sugarListID);
						}
						glPopMatrix();
						glCallList(g_freeListID);
						break;
					case WM_SUGARL :
						glPushMatrix();
						{
							glScalef(EDGE_SIZE / 2, EDGE_SIZE / 2, 0.0f);
							glCallList(g_sugarListID);
						}
						glPopMatrix();
						glCallList(g_freeListID);
						break;
						case WM_SUGAR :
							glPushMatrix();
							{
								glScalef(EDGE_SIZE / 3, EDGE_SIZE / 3, 0.0f);
								glCallList(g_sugarListID);
							}
							glPopMatrix();
							glCallList(g_freeListID);
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
static void
draw3DFreeField (void)
{
	/* Deckelflaeche */
	glPushMatrix ();
	{
		/* Material des Wuerfels */
		float matAmbient[] = { .2f, .2f, .2f, 1.0f};
		float matDiffuse[] = { .2f, .2f, .2f, 1.0f };
		float matSpecular[] = { .2f, .2f, .2f, 1.0f };
		float matShininess[] = { 10.0f };

		/* Setzen der Material-Parameter */
		glMaterialfv (GL_FRONT, GL_AMBIENT, matAmbient);
		glMaterialfv (GL_FRONT, GL_DIFFUSE, matDiffuse);
		glMaterialfv (GL_FRONT, GL_SPECULAR, matSpecular);
		glMaterialfv (GL_FRONT, GL_SHININESS, matShininess);
		glRotatef (-90.0f, 1.0f, 0.0f, 0.0f);
		glTranslatef (0.0f, 0.0f, -0.5f);
		glCallList(g_squareSubDivListID);
	}
	glPopMatrix ();
}

/**
 * Zeichnet ein 3D-Zuckerstück
 */
static void
draw3DSugar ()
{
	/* Material des Wuerfels */
	float matAmbient[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	float matDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	float matSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	float matShininess[] = { 20.0f };

	/* Setzen der Material-Parameter */
	glMaterialfv (GL_FRONT, GL_AMBIENT, matAmbient);
	glMaterialfv (GL_FRONT, GL_DIFFUSE, matDiffuse);
	glMaterialfv (GL_FRONT, GL_SPECULAR, matSpecular);
	glMaterialfv (GL_FRONT, GL_SHININESS, matShininess);
	glCallList(g_cubeivListID);
}

/**
 * Zeichnet eine Kugel mit dem Durchmesser 1 und dem Mittelpunkt (0/0/0).
 */
static void
drawSphere (void)
{
  /* Quadric erzuegen */
  GLUquadricObj *qobj = gluNewQuadric ();
  if (qobj != 0)
    {
			/* Material */
			float matAmbient[] = { .4f, .26f, .12f, 1.0f };
			float matDiffuse[] = { .4f, .26f, .12f, 1.0f };
			float matSpecular[] = { 1.0f, 1.0f, .12f, 1.0f };
      float matShininess[] = { 20.0f };

      /* Setzen der Material-Parameter */
      glMaterialfv (GL_FRONT, GL_AMBIENT, matAmbient);
      glMaterialfv (GL_FRONT, GL_DIFFUSE, matDiffuse);
      glMaterialfv (GL_FRONT, GL_SPECULAR, matSpecular);
      glMaterialfv (GL_FRONT, GL_SHININESS, matShininess);

      /* Normalen fuer Quadrics berechnen lassen */
      gluQuadricNormals (qobj, GLU_SMOOTH);
      if (getShowNormals())
      {
        GLint normal[3];
        glGetIntegerv(GL_CURRENT_NORMAL, normal);
        glBegin(GL_LINES);
        glVertex3f(0, 0, 0);
        glVertex3f(normal[0], normal[1], normal[2]);
        glEnd();
      }

      /* Kugel zeichen */
      gluSphere (qobj, 0.5, 20, 20);

      /* Loeschen des Quadrics nicht vergessen */
      gluDeleteQuadric (qobj);
    }
  else
    {
      CG_ERROR (("Could not create Quadric\n"));
    }
}

/**
 * Zeichnet einen Cylinder der höhe und breite 1 in richtung der z-Achse
 */
static void
drawCylinder (void)
{
	/* Quadric erzuegen */
	GLUquadricObj *qobj = gluNewQuadric ();

	if (qobj != 0)
	{
		/* Normalen fuer Quadrics berechnen lassen */
		gluQuadricNormals (qobj, GLU_SMOOTH);
    if (getShowNormals())
    {
      GLint normal[3];
      glGetIntegerv(GL_CURRENT_NORMAL, normal);
      glBegin(GL_LINES);
      glVertex3f(0, 0, 0);
      glVertex3f(normal[0], normal[1], normal[2]);
      glEnd();
    }

		/* Cylinder zeichnen*/
		gluCylinder (qobj, .5, .5, 1.0, 20, 20);

		/* Obere fläche füllen*/
		glPushMatrix ();
		{
			glTranslatef ( .0f, .0f, 1.0f);

			drawCircle ();
		}
		glPopMatrix ();
		/* Untere fläche füllen*/
		glPushMatrix ();
		{
			glRotatef (90, 1.0f, .0f, .0f);
			drawCircle ();
		}
		glPopMatrix ();

	}
	else
	{
		CG_ERROR (("Could not create Quadric\n"));
	}
}

/**
 * Zeichnet eine 3D-Figur
 */
static void
draw3DStickFigure (void)
{

	/* Kopf zeichnen */
	glPushMatrix ();
	{
		glTranslatef ( .0f, 1.5f, .0f);
		glScalef ( .75f, .75f, .75f);
		glCallList(g_sphereListID);
	}
	glPopMatrix ();
  /* Rumpf zeichnen*/
	glPushMatrix ();
	{
		glTranslatef ( .0f, .25f, .0f);
		glRotatef(-90, 1.0f, .0f, .0f);
		glScalef (.2f, .2f, 1.5f);
		glCallList(g_cylinderListID);
	}
	glPopMatrix ();
 	/* Rechtes Bein zeichnen*/
	glPushMatrix ();
	{
		glRotatef(-90, .0f, 1.0f, .0f);
		glRotatef(60, 1.0f, .0f, .0f);
		glScalef (.2f, .2f, 1.0f);
		glCallList(g_cylinderListID);
	}
	glPopMatrix ();
	/* Linkes Bein zeichnen*/
	glPushMatrix ();
	{
		glRotatef(90, .0f, 1.0f, .0f);
		glRotatef(60, 1.0f, .0f, .0f);
		glScalef (.2f, .2f, 1.0f);
		glCallList(g_cylinderListID);
	}
	glPopMatrix ();
	/* Linken Arm zeichnen*/
	glPushMatrix ();
	{
		glTranslatef ( .0f, .8f, .0f);
		glRotatef(-90, .0f, 1.0f, .0f);
		glRotatef(60, 1.0f, .0f, .0f);
		glScalef (.2f, .2f, 1.0f);
		glCallList(g_cylinderListID);
	}
	glPopMatrix ();
	/* Rechten Arm zeichnen*/
	glPushMatrix ();
	{
		glTranslatef (.0f, .8f, .0f);
		glRotatef(90, .0f, 1.0f, .0f);
		glRotatef(60, 1.0f, .0f, .0f);
		glScalef (.2f, .2f, 1.0f);
		glCallList(g_cylinderListID);
	}
	glPopMatrix ();
}

static void
draw3DPlayer (void)
{

	/* Die aktuelle Poition des Spileres */
 	CGPoint2f * playerPosition = getPlayerPosition();
	/* Material */
	float matAmbient[] = { .4f, .26f, .12f, 1.0f };
	float matDiffuse[] = { .4f, .26f, .12f, 1.0f};
	float matSpecular[] = { 1.0f, 1.0f, .12f , 1.0f};
	float matShininess[] = { 20.0f };

	/* Setzen der Material-Parameter */
	glMaterialfv (GL_FRONT, GL_AMBIENT, matAmbient);
	glMaterialfv (GL_FRONT, GL_DIFFUSE, matDiffuse);
	glMaterialfv (GL_FRONT, GL_SPECULAR, matSpecular);
	glMaterialfv (GL_FRONT, GL_SHININESS, matShininess);
	glPushMatrix ();
	{
    glTranslatef(-9.0f  + 2.0f * (*playerPosition)[0] + 1,
 								 0.0f,
 								 -9.0f + 2.0f * (*playerPosition)[1] + 1);
    glRotatef (getPlayerRotation (), 0.0f, 1.0f, 0.0f);
    glCallList(g_3DStickFigureListID);
	}
	glPopMatrix ();

}

/**
 * Zeichnet 3D-Sandsaecke
 */
static void
draw3DSandbag (void)
{
	/* Boden Flaeche zeichnen */
	glPushMatrix ();
  {
		/* Material des Wuerfels */
		float matAmbient[] = { .4f, .26f, .12f, 1.0f };
		float matDiffuse[] = { .4f, .26f, .12f , 1.0f};
		float matSpecular[] = { 1.0f, 1.0f, .12f , 1.0f};
		float matShininess[] = { 20.0f };
		/* Setzen der Material-Parameter */
		glMaterialfv (GL_FRONT, GL_AMBIENT, matAmbient);
		glMaterialfv (GL_FRONT, GL_DIFFUSE, matDiffuse);
		glMaterialfv (GL_FRONT, GL_SPECULAR, matSpecular);
		glMaterialfv (GL_FRONT, GL_SHININESS, matShininess);
		glRotatef (-90.0f, 1.0f, 0.0f, 0.0f);
		glTranslatef (0.0f, 0.0f, -0.5f);
		glCallList(g_squareSubDivListID);
  }
	glPopMatrix ();

	glPushMatrix ();
	{
		glTranslatef (-0.25f, 0.0f, 0.0f);
		glScalef (EDGE_SIZE / 3, EDGE_SIZE / 3, EDGE_SIZE / 2);
		glCallList(g_sphereListID);
	}
	glPopMatrix ();

	glPushMatrix ();
	{
		glTranslatef (0.0f, 0.0f, 0.0f);
		glScalef (EDGE_SIZE / 3, EDGE_SIZE / 3, EDGE_SIZE / 2);
		glCallList(g_sphereListID);
	}
	glPopMatrix ();

	glPushMatrix ();
	{
		glTranslatef (0.0f, EDGE_SIZE / 3, 0.0f);
		glScalef (EDGE_SIZE / 3, EDGE_SIZE / 3, EDGE_SIZE / 2);
		glCallList(g_sphereListID);
	}
	glPopMatrix ();

	glPushMatrix ();
	{
		glTranslatef (0.25f, 0.0f, 0.0f);
		glScalef (EDGE_SIZE / 3, EDGE_SIZE / 3, EDGE_SIZE / 2);
		glCallList(g_sphereListID);
	}
	glPopMatrix ();

}

/**
 * Zeichnet das 3D-Zielfeld
 */
static void
draw3DTarget (void)
{
	glPushMatrix ();
	{
		/* Material des Wuerfels */
		float matAmbient[] = { .0f, 1.0f, .0f, 1.0f };
		float matDiffuse[] = { .0f, 1.0f, .0f, 1.0f };
		float matSpecular[] = { .7f, 1.0f, .7f, 1.0f };
		float matShininess[] = { 20.0f };

		/* Setzen der Material-Parameter */
		glMaterialfv (GL_FRONT, GL_AMBIENT, matAmbient);
		glMaterialfv (GL_FRONT, GL_DIFFUSE, matDiffuse);
		glMaterialfv (GL_FRONT, GL_SPECULAR, matSpecular);
		glMaterialfv (GL_FRONT, GL_SHININESS, matShininess);
		glRotatef (-90.0f, 1.0f, 0.0f, 0.0f);
		glTranslatef (0.0f, 0.0f, -0.5f);
		glCallList(g_squareSubDivListID);
	}
	glPopMatrix();
}

/**
 * Ein 3d Wasser-Feld
 */
static void
draw3DWater (void)
{
	glCallList(g_3DFreeListID);

	float matAmbient[] = { .0f, .0f, 1.0f, 1.0f/3.0f};
	float matDiffuse[] = { .0f, .0f, 1.0f, 1.0f/3.0f};
	float matSpecular[] = { .7f, 0.7f, 1.0f, 1.0f/3.0f};
	float matShininess[] = { 30.0f };

	/* Setzen der Material-Parameter */
	glMaterialfv (GL_FRONT, GL_AMBIENT, matAmbient);
	glMaterialfv (GL_FRONT, GL_DIFFUSE, matDiffuse);
	glMaterialfv (GL_FRONT, GL_SPECULAR, matSpecular);
	glMaterialfv (GL_FRONT, GL_SHININESS, matShininess);
	glCallList(g_cubeivListID);
}

/**
 * Die 3D-Scene des Levels
 */
static void
draw3DLevel (void)
{
		/* Die aktuell Zeile des Spielfelds */
		GLint row = 0;
		/* Die aktuell Spalte des Spielfelds */
		GLint col = 0;
		/* Das aktuelle Spielfeld */
		wmLevelPointer level = getCurrentLevelBoard();
		/* Initzialisiert mit der hälfte eines Feldes */
		GLfloat vGap = 2.0f;
		GLfloat hGap = 2.0f;

		for (;row < LEVELSIZE; row++)
		{
			for (col = 0;col < LEVELSIZE; col++)
			{
				glPushMatrix();
				{
					glTranslatef(-10.0f + hGap, 0.0f,  -10.0f + vGap);
					glScalef(2.0f , 2.0f, 2.0f);
					switch (level[row][col])
					{
						case WM_WALL :
							glCallList(g_3DWallListID);
							break;
						case WM_FREE :
							glCallList(g_3DFreeListID);
							break;
						case WM_WATER :
							glCallList(g_3DWaterListID);
							break;
						case WM_TARGET :
							glCallList(g_3DTargetID);
							break;
						case WM_SAND :
							glCallList(g_3DSandbagID);
							break;
						case WM_SUGARXXL :
							glCallList(g_3DSugarListID);
							break;
						case WM_SUGARXL :
							draw3DFreeField ();
							glPushMatrix();
							{
								glScalef(EDGE_SIZE / 1.5f, EDGE_SIZE / 1.5f, EDGE_SIZE / 1.5f);
								glCallList(g_3DSugarListID);
							}
							glPopMatrix();
							break;
						case WM_SUGARL :
							draw3DFreeField ();
							glPushMatrix();
							{
								glScalef(EDGE_SIZE / 2, EDGE_SIZE / 2, EDGE_SIZE / 2);
								glCallList(g_3DSugarListID);
							}
							glPopMatrix();
							break;
							case WM_SUGAR :
								draw3DFreeField ();
								glPushMatrix();
								{
									glScalef(EDGE_SIZE / 3, EDGE_SIZE / 3, EDGE_SIZE / 3);
									glCallList(g_3DSugarListID);
								}
								glPopMatrix();
								break;
						default : break;
					}
				}
				glPopMatrix();
				hGap += 2.0f;
			}
			hGap = 2.0f;
			vGap += 2.0f;
		}
	}

	/**
	 * Erzeugen der Display-Listen fuer Wuerfel und Koordinatenachsen.
	 */
	static void
	initDisplayLists (void)
	{
	  /* Neun Display-Listen anfordern */
	  g_waterListID = glGenLists ((GLsizei) 22);
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

				g_squareSubDivListID = g_vLineListID + 1;
				glNewList (g_squareSubDivListID, GL_COMPILE);
				drawSubDivSquare ();
				glEndList ();

				g_cubeivListID = g_squareSubDivListID + 1;
				glNewList (g_cubeivListID, GL_COMPILE);
				drawCube ();
				glEndList ();

				g_3DWallListID = g_cubeivListID + 1;
				glNewList (g_3DWallListID, GL_COMPILE);
				draw3DWall ();
				glEndList ();

				g_3DFreeListID = g_3DWallListID + 1;
				glNewList (g_3DFreeListID, GL_COMPILE);
				draw3DFreeField ();
				glEndList ();

				g_3DSugarListID = g_3DFreeListID + 1;
				glNewList (g_3DSugarListID, GL_COMPILE);
				draw3DSugar ();
				glEndList ();

				g_sphereListID = g_3DSugarListID + 1;
				glNewList (g_sphereListID, GL_COMPILE);
				drawSphere ();
				glEndList ();

				g_cylinderListID = g_sphereListID + 1;
				glNewList (g_cylinderListID, GL_COMPILE);
				drawCylinder ();
				glEndList ();

				g_3DStickFigureListID = g_cylinderListID + 1;
				glNewList (g_3DStickFigureListID, GL_COMPILE);
				draw3DStickFigure ();
				glEndList ();

				g_3DPlayerListID = g_3DStickFigureListID + 1;
				glNewList (g_3DPlayerListID, GL_COMPILE);
				draw3DPlayer ();
				glEndList ();

				g_3DSandbagID = g_3DPlayerListID + 1;
				glNewList (g_3DSandbagID, GL_COMPILE);
				draw3DSandbag ();
				glEndList ();

				g_3DTargetID = g_3DSandbagID + 1;
				glNewList (g_3DTargetID, GL_COMPILE);
				draw3DTarget ();
				glEndList ();

				g_3DWaterListID = g_3DTargetID + 1;
				glNewList (g_3DWaterListID, GL_COMPILE);
				draw3DWater ();
				glEndList ();

				g_wallGaps = g_3DWaterListID + 1;
				glNewList (g_wallGaps, GL_COMPILE);
				drawAllWallGaps ();
				glEndList ();
	    }
	  else
	    {
	      CG_ERROR (("Konnte Displaylisten nicht erzeuge\n"));
	    }
	}

/**
 * Zeichen-Funktion.
 * Stellt die Szene dar.
 * Ein Ball, zwei Schlaeger, Rand oben, Rand unten
 */
void
drawScene2D (void)
{
	/* Displaylisten initialisieren */
	 initDisplayLists ();
	 drawPlayer();
	 drawLevel();

}

/**
 * Zeichen Funktion fuer die 3d-Scene des Spiels
 */
void
drawScene3D (void)
{
	/* Die Position des Spielers */
  CGPoint2f * playerPosition = getPlayerPosition();
  /* Die umgerechnete Position des Spielers */
  GLfloat playerPositionX = -9.0f  + 2.0f * (*playerPosition)[0] + 1;
  GLfloat playerPositionZ = -9.0f  + 2.0f * (*playerPosition)[1] + 1;
	/* Die Rotation des Spielers*/
	GLint playerRotation = getPlayerRotation();
	/* erste (feste) Lichtquelle */
  /* gerichtetes / paralleles Licht von links-unten-vorne */
  CGPoint4f lightPos0 = { 0.0f, 1.0f, 0.0f, 0.0f };
	CGPoint4f lightPos1 = { .0f, 0.0f, .0f, 1.0f };
	CGVector3f lightDirection1 = { 0.0f, -.1f, 0.0f };
	GLint zDirection = 0;
	GLint xDirection = 0;
	/* Lichtberechnung aktivieren */
	glEnable (GL_LIGHTING);

	if (getLight0Status())
	{
		/* Position der ersten Lichtquelle setzen */
    glLightfv (GL_LIGHT0, GL_POSITION, lightPos0);
    /* Erste Lichtquelle aktivieren */
    glEnable (GL_LIGHT0);
	}

	if (getLight1Status())
	{
		glPushMatrix();
		{
			/* Position der Lichtquelle auf den Spieler setzen */
			glTranslatef(playerPositionX, .3f, playerPositionZ);
			/* Richtung der zweiten Lichtquelle aus Rotationswinkel setzen */
			switch (playerRotation) {
		    case 0:
		         zDirection = 1;
		    break;
		    case 90:
		         xDirection = 1;
		    break;
		    case 180:
		         zDirection = -1;
		    break;
		    case 270:
		         xDirection = -1;
		    break;
		  }
			lightDirection1[0] = xDirection;
      lightDirection1[2] = zDirection;
			glLightfv (GL_LIGHT1, GL_POSITION, lightPos1);
			/* Richtung der zweiten Lichtquelle festlegen */
      glLightfv (GL_LIGHT1, GL_SPOT_DIRECTION, lightDirection1);
		}
		glPopMatrix();
		glEnable (GL_LIGHT1);
	}
	draw3DLevel ();
	glCallList(g_3DPlayerListID);

	/* Erste Lichtquelle deaktivieren */
  glDisable (GL_LIGHT0);
	glDisable (GL_LIGHT1);
	/* Lichtberechnung deaktivieren */
  glDisable (GL_LIGHTING);
}

/**
 * Initialisierung der Lichtquellen.
 * Setzt Eigenschaften der Lichtquellen (Farbe, Oeffnungswinkel, ...)
 */
static void
initLight (void)
{
  /* Farbe der ersten Lichtquelle */
  CGPoint4f lightColor0[3] =
    { {0.1f, 0.1f, 0.1f, 1.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {1.5f, 1.5f, 1.5f,
                                                           1.0f}
  };

  /* Farbe der zweiten Lichtquelle */
  CGPoint4f lightColor1[3] =
    { {0.0f, 0.0f, 0.0f, 1.0f}, {2.0f, 2.0f, 2.0f, 1.0f}, {3.0f, 3.0f, 3.0f,
                                                           1.0f}
  };
  /* Oeffnungswinkel der zweiten Lichtquelle */
  GLdouble lightCutoff1 = 10.0f;
  /* Lichtverteilung im Lichtkegel der zweiten Lichtquelle */
  GLdouble lightExponent1 = 30.0f;

  /* Farbe der ersten Lichtquelle setzen */
  glLightfv (GL_LIGHT0, GL_AMBIENT, lightColor0[0]);
  glLightfv (GL_LIGHT0, GL_DIFFUSE, lightColor0[1]);
  glLightfv (GL_LIGHT0, GL_SPECULAR, lightColor0[2]);
  /* Farbe der zweiten Lichtquelle setzen */
  glLightfv (GL_LIGHT1, GL_AMBIENT, lightColor1[0]);
  glLightfv (GL_LIGHT1, GL_DIFFUSE, lightColor1[1]);
  glLightfv (GL_LIGHT1, GL_SPECULAR, lightColor1[2]);

  /* Spotlight-Eigenschaften der zweiten Lichtquelle setzen */
  glLightf (GL_LIGHT1, GL_SPOT_CUTOFF, (GLfloat)lightCutoff1);
  glLightf (GL_LIGHT1, GL_SPOT_EXPONENT, (GLfloat)lightExponent1);
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
	/* Z-Buffer-Test aktivieren */
  glEnable (GL_DEPTH_TEST);
	/* Polygonrueckseiten nicht anzeigen */
  glCullFace (GL_BACK);
  glEnable (GL_CULL_FACE);
	/* Normalen nach Transformationen wieder auf die
   * Einheitslaenge bringen */
  glEnable (GL_NORMALIZE);
	/* Blending aktivieren */
  glEnable (GL_BLEND);
	/* Blending Funktion setzen
   * Die Blending-Funktion legt fest, wie die Farbwerte
   * der Quelle und des Ziels berechnet werden.
   * (Hier nur die Alphawerte).
   *
   * der erste Parameter legt dies für die Quelle
   * der zweite Parameter für das Ziel fest.
   *
   * So würde z.B. GL_SRC_ALPHA, GL_ONE als Parameterpaar
   * additiver Lichtmischung entsprechen.  */
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	/* Lichtquellen Aktivieren */
	initLight ();

  /* Alles in Ordnung? */
  return (GLGETERROR == GL_NO_ERROR);
}
