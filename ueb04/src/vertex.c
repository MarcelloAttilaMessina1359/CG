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
#include <math.h>

#ifdef MACOSX
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

/* ---- Eigene Header einbinden ---- */
#include "vertex.h"
#include "types.h"

/* ---- Konstanten ---- */
//Konstanten und Typen zur besseren Lesbarkeit
#define CX (0)
#define CY (1)
#define CZ (2)
#define CR (3)
#define CG (4)
#define CB (5)
#define CA (6)

static GLint g_width = 2;

/** Anzahl der Wuerfeldrehungen pro Sekunde. */
static GLint g_numberOfIndices;

static GLint* g_indices;

static Vertex* g_vertices;

static CGVector3f* g_normals;

static CGVector2f* g_texCoords;

static GLint enableNormal = 0;

static CGColor3f g_colors[3] = { {1.0f, 0.0f, 1.0f}, {0.0f, 0.2f, 1.0f}, {0.0f, 1.0f, 1.0f} };

GLint
calcNumberOfIndices (GLint width)
{
  return (width - 1) * (width - 1) * 6;
}

void
calcIndices ()
{
  g_numberOfIndices = calcNumberOfIndices (g_width);
  g_indices = realloc (g_indices, sizeof(GLint) * g_numberOfIndices);
  GLint currentIndex = 0;

  for (GLint row = 0; row < g_width - 1; row++)
  {
    for (GLint column = 1; column < g_width; column++)
    {
      g_indices[currentIndex] = row * g_width + column;
      g_indices[++currentIndex] = row * g_width + column - 1;
      g_indices[++currentIndex] = (row + 1) * g_width + column - 1;

      g_indices[++currentIndex] = row * g_width + column;
      g_indices[++currentIndex] = (row + 1) * g_width + column - 1;
      g_indices[++currentIndex] = (row + 1) * g_width + column;
      currentIndex++;
    }
  }
}

void
setColor (GLint index)
{
  GLint colorIndex = 1;
  if (g_vertices[index][CY] > 0 && g_vertices[index][CY] > 1e-15)
  {
    colorIndex = 2;
  } else if (g_vertices[index][CY] < 0 && g_vertices[index][CY] < -1e-15) {
    colorIndex = 0;
  }

  g_vertices[index][CR] = g_colors[colorIndex][0];
  g_vertices[index][CG] = g_colors[colorIndex][1];
  g_vertices[index][CB] = g_colors[colorIndex][2];
  g_vertices[index][CA] = 1.0f;
}

void
setColors ()
{
  for (GLint i = 0; i < g_width * g_width; i++)
  {
    setColor (i);
  }
}

void
setNormal (GLint index)
{
  CGVector3f vertexX = { 0.0f, 0.0f, 0.0f };
  CGVector3f vertexZ = { 0.0f, 0.0f, 0.0f };
  // letzte Spalte
  if ((index + 1) % g_width == 0)
  {
    vertexX[CX] = g_vertices[index - 1][CX] - g_vertices[index][CX];
    vertexX[CY] = g_vertices[index - 1][CY] - g_vertices[index][CY];
    vertexX[CZ] = g_vertices[index - 1][CZ] - g_vertices[index][CZ];
  } else {
    vertexX[CX] = g_vertices[index][CX] - g_vertices[index + 1][CX];
    vertexX[CY] = g_vertices[index][CY] - g_vertices[index + 1][CY];
    vertexX[CZ] = g_vertices[index][CZ] - g_vertices[index + 1][CZ];
  }
  // letzte Reihe
  if (index >= g_width * g_width - g_width)
  {
    vertexZ[CX] = g_vertices[index][CX] - g_vertices[index - g_width][CX];
    vertexZ[CY] = g_vertices[index][CY] - g_vertices[index - g_width][CY];
    vertexZ[CZ] = g_vertices[index][CZ] - g_vertices[index - g_width][CZ];

  } else {
    vertexZ[CX] = g_vertices[index + g_width][CX] - g_vertices[index][CX];
    vertexZ[CY] = g_vertices[index + g_width][CY] - g_vertices[index][CY];
    vertexZ[CZ] = g_vertices[index + g_width][CZ] - g_vertices[index][CZ];
  }
  // Kreuzprodukt
  g_normals[index][CX] = 10 * (vertexX[CY] * vertexZ[CZ] - vertexX[CZ] * vertexZ[CY]);
  g_normals[index][CY] = 10 * (vertexX[CZ] * vertexZ[CX] - vertexX[CX] * vertexZ[CZ]);
  g_normals[index][CZ] = 10 * (vertexX[CX] * vertexZ[CY] - vertexX[CY] * vertexZ[CX]);

  GLfloat length = sqrt (g_normals[index][CX] * g_normals[index][CX]
                         + g_normals[index][CY] * g_normals[index][CY]
                         + g_normals[index][CZ] * g_normals[index][CZ]);
  g_normals[index][CX] /= length / 10;
  g_normals[index][CY] /= length / 10;
  g_normals[index][CZ] /= length / 10;
}

void
enableNormals (GLint enableNormals)
{
  enableNormal = enableNormals;
}

int
normalsEnabled (void)
{
  return enableNormal;
}

void
setNormals ()
{
  g_normals = realloc (g_normals, sizeof(CGVector3f) * g_width * g_width);
  if (g_normals != NULL)
  {
    for (GLint i = 0; i < g_width * g_width; i++)
    {
      setNormal (i);
    }
  }
}

void
setTexCoords ()
{
  g_texCoords = realloc(g_texCoords, sizeof(CGVector2f) * g_width * g_width);
  for (GLint i = 0; i < g_width * g_width; i++)
  {
    g_texCoords[i][CX] = (g_vertices[i][CX] + 1) / 2;
    g_texCoords[i][CY] = (g_vertices[i][CZ] + 1) / 2;
  }
}

void
increaseWidth ()
{
  if (g_width < 20)
  {
    Vertex* oldVertices = malloc (sizeof(Vertex) * g_width * g_width);
    if (oldVertices != NULL)
    {
      oldVertices = memcpy (oldVertices, g_vertices, sizeof(Vertex) * g_width * g_width);
      g_width++;
      g_vertices = realloc (g_vertices, sizeof(Vertex) * g_width * g_width);
      if (g_vertices != NULL)
      {
        for (GLint i = 0; i < g_width * g_width; ++i)
        {
          g_vertices[i][CX] = (i % g_width) * (2.0f / (g_width - 1)) - 1;
          g_vertices[i][CZ] = (i / g_width) * (2.0f / (g_width - 1)) - 1;
          if (i == 0 || ((i + 1) % g_width != 0 && i < g_width * g_width - g_width))
          {
            g_vertices[i][CY] = oldVertices[i - i / g_width][CY];
          } else {
            g_vertices[i][CY] = 0;
          }
        }
        calcIndices ();
        setTexCoords ();
        setColors ();
        setNormals ();
      }
      free (oldVertices);
    }
  }
}

void
decreaseWidth ()
{
  if (g_width > 2)
  {
    Vertex* oldVertices = malloc (sizeof(Vertex) * g_width * g_width);
    if (oldVertices != NULL)
    {
      memcpy (oldVertices, g_vertices, sizeof(Vertex) * g_width * g_width);
      g_width--;
      g_vertices = realloc (g_vertices, sizeof(Vertex) * g_width * g_width);
      if ( g_vertices != NULL)
      {
        for (GLint i = 0; i < g_width * g_width; ++i)
        {
          g_vertices[i][CX] = (i % g_width) * (2.0f / (g_width - 1)) - 1;
          g_vertices[i][CY] = oldVertices[i + i / g_width][CY];
          g_vertices[i][CZ] = (i / g_width) * (2.0f / (g_width - 1)) - 1;
        }
        calcIndices ();
        setColors ();
        setTexCoords ();
        setNormals ();
      }
      free (oldVertices);
    }
  }
}

void
increaseHeight (GLint index)
{
  if (g_vertices[index][CY] < 1.0f)
  {
    g_vertices[index][CY] += 0.1f;
    setColor (index);
    setNormal (index);
  }
}

void
decreaseHeight (GLint index)
{
  if (g_vertices[index][CY] > -1.0f)
  {
    g_vertices[index][CY] -= 0.1f;
    setColor (index);
    setNormal (index);
  }
}

GLint
getWidth ()
{
  return g_width;
}

GLint
getNumberOfIndices ()
{
  return g_numberOfIndices;
}

GLint*
getIndices ()
{
  return g_indices;
}

Vertex*
getVertices ()
{
  return g_vertices;
}

CGVector3f*
getNormals ()
{
  return g_normals;
}

CGVector2f*
getTexCoords ()
{
  return g_texCoords;
}

void
initVertices ()
{
  g_numberOfIndices = calcNumberOfIndices(g_width);
  g_indices = malloc (sizeof(GLint) * g_numberOfIndices);
  calcIndices ();
  g_vertices = malloc (sizeof(Vertex) * g_width * g_width);
  g_normals = malloc (sizeof(CGVector3f) * g_width * g_width);
  g_texCoords = malloc (sizeof(CGVector2f) * g_width * g_width);

  g_vertices[0][CX] = -1;
  g_vertices[0][CY] = 0;
  g_vertices[0][CZ] = -1;

  g_vertices[1][CX] = 1;
  g_vertices[1][CY] = 0;
  g_vertices[1][CZ] = -1;

  g_vertices[2][CX] = -1;
  g_vertices[2][CY] = 0;
  g_vertices[2][CZ] = 1;

  g_vertices[3][CX] = 1;
  g_vertices[3][CY] = 0;
  g_vertices[3][CZ] = 1;
  setColors ();
  setNormals ();
  setTexCoords ();
}
