/**
 * @file
 * Darstellungs-Modul.
 * Das Modul kapselt die Rendering-Funktionalitaet (insbesondere der OpenGL-
 * Aufrufe) des Programms.
 *
 * Bestandteil eines Beispielprogramms fuer Picking mit OpenGL & GLUT.
 *
 * @author copyright (C) Fachhochschule Wedel 1999-2011. All rights reserved.
 */

/* ---- System Header einbinden ---- */
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <stdlib.h>
#include <math.h>

/* ---- Eigene Header einbinden ---- */
#include "scene.h"
#include "types.h"
#include "debugGL.h"
#include "vertex.h"
#include "texture.h"

#define CX (0)
#define CY (1)
#define CZ (2)
#define CR (3)
#define CG (4)
#define CB (5)
#define CA (6)

/**
 * 0 => Kugeln werden nicht sichtbar.
 * 1 => Kugeln werden sichtbar.
 */
static GLint g_showBalls = 0;

/**
 * Zeichnet eine Kugel mit dem Durchmesser 1 und dem Mittelpunkt (0/0/0).
 */
void
drawSphere (void)
{
  /* Quadric erzuegen */
  GLUquadricObj *qobj = gluNewQuadric ();
  if (qobj != 0)
    {
      /* Normalen fuer Quadrics berechnen lassen */
      gluQuadricNormals (qobj, GLU_SMOOTH);

      if (normalsEnabled())
      {
        GLint normal[3];
        glGetIntegerv(GL_CURRENT_NORMAL, normal);
        glBegin(GL_LINES);
        glVertex3f(0, 0, 0);
        glVertex3f(normal[0] * 2, normal[1] * 2, normal[2] * 2);
        glEnd();
      }

      /* Kugel zeichen */
      gluSphere (qobj, 1.0, 20, 20);

      /* Loeschen des Quadrics nicht vergessen */
      gluDeleteQuadric (qobj);
    }
  else
    {
      CG_ERROR (("Could not create Quadric\n"));
    }
}

void
drawBalls (Vertex* vertices)
{
  GLint width = getWidth ();
  CGVector3f* normals = getNormals ();
  for (GLint i = 0; i < width * width; i++)
  {
    glPushName (i);
    {

      glPushMatrix ();
      {
        glTranslatef (vertices[i][CX], vertices[i][CY], vertices[i][CZ]);

        /* Material des Balls */
        float matAmbient[] = { vertices[i][CR], vertices[i][CG], vertices[i][CB], vertices[i][CA]};
        float matSpecular[] = { 1.54f, 1.54f, 1.54f, 1.0f };
        float matShininess[] = { 50.0f };

        if (!g_showBalls)
        {
          glDepthMask(GL_FALSE);
          matAmbient[0] = 0.0f;
          matAmbient[1] = 0.0f;
          matAmbient[2] = 0.0f;
          matAmbient[3] = 0.0f;
          
          matSpecular[0] = 0.0f;
          matSpecular[1] = 0.0f;
          matSpecular[2] = 0.0f;
          
          matShininess[0] = 0.0f;
        }

        /* Setzen der Material-Parameter */
        glMaterialfv (GL_FRONT, GL_AMBIENT, matAmbient);
        glMaterialfv (GL_FRONT, GL_DIFFUSE, matAmbient);
        glMaterialfv (GL_FRONT, GL_SPECULAR, matSpecular);
        glMaterialfv (GL_FRONT, GL_SHININESS, matShininess);

        glScalef (0.025f, 0.025f, 0.025f);
        drawSphere ();
        if (!g_showBalls)
        {
          glDepthMask(GL_TRUE);
        }
      }
      glPopMatrix ();
    }
    glPopName ();

    if (normalsEnabled())
    {
      glDepthMask(GL_TRUE);
      /* Material des Balls */
      float matAmbient[] = { vertices[i][CR], vertices[i][CG], vertices[i][CB], vertices[i][CA]};
      float matSpecular[] = { 1.54f, 1.54f, 1.54f, 1.0f };
      float matShininess[] = { 50.0f };
      
      /* Setzen der Material-Parameter */
      glMaterialfv (GL_FRONT, GL_AMBIENT, matAmbient);
      glMaterialfv (GL_FRONT, GL_DIFFUSE, matAmbient);
      glMaterialfv (GL_FRONT, GL_SPECULAR, matSpecular);
      glMaterialfv (GL_FRONT, GL_SHININESS, matShininess);
        
      glBegin(GL_LINES);
      {
        glVertex3f(vertices[i][CX], vertices[i][CY], vertices[i][CZ]);
        glVertex3f(vertices[i][CX] + normals[i][CX] * 0.2, vertices[i][CY] + normals[i][CY] * 0.2, vertices[i][CZ] + vertices[i][CZ] * 0.2);
      }
      glEnd();
    }
  }
}

void
drawWater (Vertex* vertices)
{
  GLint* indices = getIndices ();
  CGVector3f* normals = getNormals ();

  float matAmbient[] = { 1.0f, 1.0f, 1.0f, 1.0f};
  glMaterialfv (GL_FRONT, GL_AMBIENT, matAmbient);
  glMaterialfv (GL_FRONT, GL_DIFFUSE, matAmbient);

  glVertexPointer (3,
                   GL_DOUBLE,
                   sizeof(Vertex),
                   &(vertices[0][CX]));

  if (getTexturingStatus ())
  {
    glDisable (GL_COLOR_MATERIAL);
    glEnable (GL_TEXTURE_2D);
    bindTexture (texWater);
    CGVector2f* texCoords = getTexCoords ();
    glTexCoordPointer (2,
                       GL_FLOAT,
                       sizeof(CGVector2f),
                       &(texCoords[0][CX]));
  } else {
    glDisable (GL_TEXTURE_2D);
    glEnable (GL_COLOR_MATERIAL);
    glColorPointer (4,
                    GL_DOUBLE,
                    sizeof(Vertex),
                    &(vertices[0][CR]));
  }

  glNormalPointer (GL_FLOAT,
                   sizeof(CGVector3f),
                   &(normals[0][CX]));

  glDrawElements (GL_TRIANGLES,
                  getNumberOfIndices (),
                  GL_INT,
                  indices);
  glDisable (GL_TEXTURE_2D);
  glDisable (GL_COLOR_MATERIAL);
}

void
setShowBalls (GLint show)
{
  g_showBalls = show;
}

GLint
getShowBalls ()
{
  return g_showBalls;
}

/**
 * Zeichen-Funktion.
 * Stellt die Szene dar. Eine Spielfigur, angeleuchtet von einer festen
 * Lichtquelle.
 */
void
drawScene (void)
{
  /* erste (feste) Lichtquelle */
  CGPoint4f lightPos0 = { 0.0f, 100.0f, 0.0f, 0.0f };

  /* Lichtberechnung aktivieren */
  glEnable (GL_LIGHTING);

  /* Position der ersten Lichtquelle setzen */
  glLightfv (GL_LIGHT0, GL_POSITION, lightPos0);

  Vertex* vertices = getVertices ();

  drawWater (vertices);

  drawBalls (vertices);



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
  CGColor4f lightColor0[3] =
    { {0.5f, 0.5f, 0.5f, 1.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {1.5f, 1.5f, 1.5f,
                                                           1.0f}
  };

  /* Farbe der zweiten Lichtquelle */
  CGColor4f lightColor1[3] =
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

  /* Erste Lichtquelle aktivieren */
  glEnable (GL_LIGHT0);
}

/**
 * Initialisierung der Szene (inbesondere der OpenGL-Statusmaschine).
 * Setzt Hintergrund- und Zeichenfarbe, aktiviert Tiefentest und
 * Backface-Culling.
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

  /* Lichtquellen aktivieren */
  initLight ();

  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_COLOR_ARRAY);
  glEnableClientState(GL_NORMAL_ARRAY);
  glEnableClientState(GL_TEXTURE_COORD_ARRAY);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

  /* Vertices initialisiern */
  initVertices ();

  /* Alles in Ordnung? */
  return (GLGETERROR == GL_NO_ERROR);
}

/**
 * (De-)aktiviert den Wireframe-Modus.
 */
void
toggleWireframeMode (void)
{
  /* Flag: Wireframe: ja/nein */
  static GLboolean wireframe = GL_FALSE;

  /* Modus wechseln */
  wireframe = !wireframe;

  if (wireframe)
    {
      glPolygonMode (GL_FRONT_AND_BACK, GL_LINE);
    }
  else
    {
      glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);
    }
}
