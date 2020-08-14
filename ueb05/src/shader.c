/**
 * @file
 * Einfaches Beispielprogramm fuer OpenGL, GLUT & GLEW
 * sowie einen Shader in GLSL.
 * Vier texturierte Quadrate werden gezeigt.
 * @author copyright (C) Fachhochschule Wedel 1999-2018. All rights reserved.
 */

/* ---- System Header einbinden ---- */
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>

/* ---- Eigene Header einbinden ---- */
#include "utility.h"
#include "debugGL.h"
#include "types.h"

/* Bibliothek um Bilddateien zu laden. Es handelt sich um eine
 * Bibliothek, die sowohl den Header als auch die Quelle in einer Datei
 * bereitstellt. Die Quelle kann durch die Definition des Makros
 * STB_IMAGE_IMPLEMENTeATION eingebunden werden. Genauere Informationen
 * sind der "stb_image.h" zu entnehmen.
 *
 * Quelle: https://github.com/nothings/stb */
 // define global functions as static inline
#define STB_IMAGE_STATIC
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>  // Bibliothek um Bilder zu laden
							// Quelle: https://github.com/nothings/stb

/* ---- Konstanten ---- */

/** Anzahl der Aufrufe der Timer-Funktion pro Sekunde */
#define TIMER_CALLS_PS 60
#define NUM_OF_VERTICES (100)
#define MESH_WIDTH (GLint)(sqrt(NUM_OF_VERTICES))


/* ---- Globale Variablen ---- */

/** Buffer-Objekt, um die Vertizes zu speichern. */
static GLuint g_arrayBuffer;

/** Vertex-Array-Objekt, um die Attribut-Pointer zu speichern. */
static GLuint g_vertexArrayObject;

/** Programm-Objekt zum Rendern */
static GLuint g_program;

/** Textur */
static GLuint g_textureHeightmap;

/** Textur  2 */
static GLuint g_textureSnow;

/** Textur  3 */
static GLuint g_textureRocks;

/** Location der uniform-Variable "ModelView" */
static GLint g_locationModelViewMatrix;

/** Location der uniform-Variable "Elevation" */
static GLint g_locationTime;

/** Location der uniform-Variable "Texture" */
static GLuint g_locationTextureHeightmap;

/** Location der uniform-Variable "Texture" */
static GLuint g_locationTextureSnow;
/** Location der uniform-Variable "Texture" */
static GLuint g_locationTextureRocks;
/** Location der uniform-Variable "Texture" */
static GLuint g_locationLightPos;
/** gibt an, ob die Animation pausiert ist */
static GLint g_paused = 0;

static float g_lastPaused = 0.0f;

#define VERTICES (NUM_OF_VERTICES * 2 * 3)

#define BEFORE_LAST_ROW ((MESH_WIDTH + 1) * 6)

#define VERTICES_TO_DRAW (VERTICES - ((MESH_WIDTH) * 6))
/* Die Vertices des Mesh */
Vertex *vertices; // SOLLTE VERTICES TO DRAW SEIN

/* ---- Funktionen ---- */

/**
 * Methode um das Mesh zu initialisiern
 */
 void
 initVertices ()
 {
   vertices = malloc(sizeof(Vertex) * VERTICES_TO_DRAW);
   //Falls ungleich NULL befüllen
   if (vertices != NULL)
   {
      //Auf x,y-Koordinaten Projetzieren und texturen Berechenen
      for (GLint vertex = 0; vertex < VERTICES_TO_DRAW ; vertex += 6)
      {
        /*vertex auf vertice Index skalieren (da pro vertice, 6 vertices für zwei dreiecke berechnet werden)*/
        GLint sVI = vertex / 6;

        if ((sVI + 1) % MESH_WIDTH != 0)
        {
          /* Erster Vertex (oben links)*/
          vertices[vertex].x = -1 + (sVI % MESH_WIDTH) * (2.0f / (MESH_WIDTH - 1));
          vertices[vertex].y = .0f;
          vertices[vertex].z = -1 + (sVI / MESH_WIDTH) * (2.0f / (MESH_WIDTH - 1));
          vertices[vertex].s = (vertices[vertex].x + 1) / 2;
          vertices[vertex].t = (vertices[vertex].z + 1) / 2;
          /* Zweiter vertex Vertex (oben rechts) */
          vertices[vertex + 1].x = -1 + ((sVI + 1) % MESH_WIDTH) * (2.0f / (MESH_WIDTH - 1));
          vertices[vertex + 1].y = .0f;
          vertices[vertex + 1].z = -1 + ((sVI + 1) / MESH_WIDTH) * (2.0f / (MESH_WIDTH - 1));
          vertices[vertex + 1].s = (vertices[vertex + 1].x + 1) / 2;
          vertices[vertex + 1].t = (vertices[vertex + 1].z + 1) / 2;
          /* Dritter vertex Vertex (unten rechts) */
          vertices[vertex + 2].x = -1 + ((sVI + MESH_WIDTH + 1) % MESH_WIDTH) * (2.0f / (MESH_WIDTH - 1));
          vertices[vertex + 2].y = .0f;
          vertices[vertex + 2].z = -1 + ((sVI + MESH_WIDTH + 1) / MESH_WIDTH) * (2.0f / (MESH_WIDTH - 1));
          vertices[vertex + 2].s = (vertices[vertex + 2].x + 1) / 2;
          vertices[vertex + 2].t = (vertices[vertex + 2].z + 1) / 2;
          /* Erster vertex zweites Dreieck (oben links) */
          vertices[vertex + 3].x = vertices[vertex].x;
          vertices[vertex + 3].y = vertices[vertex].y;
          vertices[vertex + 3].z = vertices[vertex].z;
          vertices[vertex + 3].s = vertices[vertex].s;
          vertices[vertex + 3].t = vertices[vertex].t;
          /* Dritter vertex zweites Dreieck (unten links) */
          vertices[vertex + 4].x = -1 + ((sVI + MESH_WIDTH) % MESH_WIDTH) * (2.0f / (MESH_WIDTH - 1));
          vertices[vertex + 4].y = .0f;
          vertices[vertex + 4].z = -1 + ((sVI + MESH_WIDTH) / MESH_WIDTH) * (2.0f / (MESH_WIDTH - 1));
          vertices[vertex + 4].s = (vertices[vertex + 4].x + 1) / 2;
          vertices[vertex + 4].t = (vertices[vertex + 4].z + 1) / 2;
          /* Zweiter vertex zweites Dreieck (unten rechts) */
          vertices[vertex + 5].x = vertices[vertex + 2].x;
          vertices[vertex + 5].y = vertices[vertex + 2].y;
          vertices[vertex + 5].z = vertices[vertex + 2].z;
          vertices[vertex + 5].s = vertices[vertex + 2].s;
          vertices[vertex + 5].t = vertices[vertex + 2].t;
        }
      }
   }
   else
   {
     exit(1);
   }
 }
static float t;
static float g_lastTime;
/**
 * Zeichen-Funktion.
 * Stellt die Szene dar.
 * Ausgabe eines Rechtecks.
 */
static void
drawScene (void)
{
  float viewMatrix[16];
  float lightPos[] = {0.0f, 100.0f, 0.0f};
  if (!g_paused)
  {
     t = (float) glutGet(GLUT_ELAPSED_TIME) / 1000 - g_lastPaused;
  }
  const float distance = 2;

lookAt(distance*sinf(t), 1, distance*cosf(t), 0, 0, 0, 0, 1, 0, viewMatrix);

  /*if (!g_paused)
  {
    lookAt(distance*sinf(t), 1, distance*cosf(g_lastPaused), 0, 0, 0, 0, 1, 0, viewMatrix);
  } else {
    lookAt(distance*sinf(g_lastPaused), 1, distance*cosf(g_lastPaused), 0, 0, 0, 0, 1, 0, viewMatrix);
  }*/

  /* Aktivieren des Programms. Ab jetzt ist die Fixed-Function-Pipeline
   * inaktiv und die Shader des Programms aktiv. */
  glUseProgram(g_program);

  /* Übermitteln der View-Matrix an den Shader.
   * OpenGL arbeitet intern mit Matrizen in column-major-layout. D.h.
   * nicht die Reihen, sondern die Spalten liegen hintereinander im
   * Speicher. Die Funktionen zur Erzeugen von Matrizen in diesem
   * Programm berücksichtigen dies. Deswegen müssen die Matrizen nicht
   * transponiert werden und es kann GL_FALSE übergeben werden. Beim
   * Schreiben eigener Matrix-Funktionen oder beim Verwenden von
   * Mathematik-Bibliotheken muss dies jedoch berücksichtigt werden. */
  glUniformMatrix4fv(g_locationModelViewMatrix, 1, GL_FALSE, viewMatrix);

  /* Übermitteln der aktuellen Anhebung */
  glUniform1f(g_locationTime, t);

  /* Uebermittulung der Kamera Position */
  glUniform1fv(g_locationLightPos, 1, lightPos);

  /* Übergeben der Textur an den Shader.
   * Texturen werden nicht direkt an den Shader übergeben, sondern
   * zuerst an eine Textureinheit gebunden. Anschließend wird dem
   * Programm nur der Index der Textureinheit übergeben. */
   glActiveTexture(GL_TEXTURE0);
   glBindTexture(GL_TEXTURE_2D, g_textureHeightmap);
   glUniform1i(g_locationTextureHeightmap, 0);

   glActiveTexture(GL_TEXTURE1);
   glBindTexture(GL_TEXTURE_2D, g_textureSnow);
   glUniform1i(g_locationTextureSnow, 1);

   glActiveTexture(GL_TEXTURE2);
   glBindTexture(GL_TEXTURE_2D, g_textureRocks);
   glUniform1i(g_locationTextureRocks, 2);

  /* Aktivieren des Vertex-Array-Objekts (VAO).
   * Hiermit werden alle Attribut-Pointer aktiv, die auf diesem VAO
   * gesetzt wurden. */
  glBindVertexArray(g_vertexArrayObject);

  /* Rendern der Dreiecke.
   * Ab dem ersten Dreieck im Buffer werden alle 8 Dreiecke gerendert.
   * Dem Draw-Command wird jedoch die Anzahl der Vertizes übergeben, die
   * gezeichnet werden sollen. */
  glDrawArrays(GL_TRIANGLES, 0, VERTICES_TO_DRAW);

  /* Zurücksetzen des OpenGL-Zustands, um Seiteneffekte zu verhindern */
  glBindVertexArray(0);
  glUseProgram(0);
}


/**
 * Bereitet die Szene vor.
 */
static void
initScene (void)
{
  initVertices();
  {
    /* Erstellen eines Buffer-Objektes.
     * In modernem OpenGL werden alle Vertex-Daten in Buffer-Objekten
     * gespeichert. Dabei handelt es sich um Speicherbereiche die von
     * der OpenGL-Implementierung verwaltet werden und typischerweise
     * auf der Grafikkarte angelegt werden.
     *
     * Mit der Funktion glGenBuffers können Namen für Buffer-Objekte
     * erzeugt werden. Mit glBindBuffer werden diese anschließend
     * erzeugt, ohne jedoch einen Speicherbereich für die Nutzdaten
     * anzulegen. Dies geschieht nachfolgend mit einem Aufruf der
     * Funktion glBufferData.
     *
     * Um mit Buffer-Objekten zu arbeiten, mussen diese an Targets
     * gebunden werden. Hier wird das Target GL_ARRAY_BUFFER verwendet.
     *
     * Der OpenGL-Implementierung wird zusätzlich ein Hinweis mitgegeben,
     * wie die Daten eingesetzt werden. Hier wird GL_STATIC_DRAW
     * übergeben. OpenGL kann diesen Hinweis nutzen, um Optimierungen
     * vorzunehmen. */
    glGenBuffers(1, &g_arrayBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, g_arrayBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices) * VERTICES_TO_DRAW * 5, vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
  }

  {
    const GLuint positionLocation = 0;
    const GLuint texCoordLocation = 1;
    /* Erstellen eines Vertex-Array-Objektes (VAO).
     * Damit die OpenGL-Implementierung weiß welche Daten der Pipeline
     * bereitgestellt werden müssen, werden Attribut-Pointer gesetzt.
     * Um mehrere Datenquellen (unterschiedliche Meshes) zu verwalten
     * können die Attribut-Pointer in VAOs gruppiert werden.
     *
     * Die Erzeugung von VAOs geschieht prinzipiell genauso wie bei
     * Buffern oder anderen OpenGL-Objekten. */
    glGenVertexArrays(1, &g_vertexArrayObject);
    glBindVertexArray(g_vertexArrayObject);

    /* Die Pointer werden immer in den Buffer gesetzt, der am
     * GL_ARRAY_BUFFER-Target gebunden ist! */
    glBindBuffer(GL_ARRAY_BUFFER, g_arrayBuffer);

    /* Im Vertex-Shader existieren folgende Zeilen:
     * > layout (location = 0) in vec4 vPosition;
     * > layout (location = 1) in vec2 vTexCoord;
     *
     * Beim Aufruf einen Draw-Command, müssen diesen beiden Attributen
     * Daten bereitgestellt werden. Diese sollen aus dem oben erstellten
     * Buffer gelesen werden. Dafür müssen zwei Attribut-Pointer aktiviert
     * und eingerichtet werden. */
    glEnableVertexAttribArray(positionLocation);
    glVertexAttribPointer(
        positionLocation,                           /* location (siehe Shader) */
        3,                           /* Dimensionalität */
        GL_FLOAT,                    /* Datentyp im Buffer */
        GL_FALSE,                    /* Keine Normierung notwendig */
        sizeof(Vertex),              /* Offset zum nächsten Vertex */
        (void*)offsetof(Vertex, x)); /* Offset zum ersten Vertex */

    /* Zweiter Attribut-Pointer für die Textur-Koordinate */
    glEnableVertexAttribArray(texCoordLocation);
    glVertexAttribPointer(texCoordLocation, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, s));

    /* Um Programmierfehler leichter zu finden, sollte der OpenGL-
     * Zustand wieder zurückgesetzt werden. Wird beispielweise das Binden
     * eines Vertex-Array-Objekts vergessen werden, arbeitet OpenGL
     * auf dem vorher gebundenen. Vor allem bei starker Modularisierung
     * sind diese Fehler fschwer zu finden. */
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
  }

  {
    /* Erstellen des Programms */
    g_program = createProgram("../content/shaders/color.vert", "../content/shaders/color.frag");

    /* Abfragen der uniform-locations.
     * Dies kann auch zur Laufzeit gemacht werden, es ist jedoch
     * performanter dies vorab zu tun, da String-Vergleiche gemacht
     * werden müssen. */
    g_locationModelViewMatrix = glGetUniformLocation(g_program, "ModelView");
    g_locationTime = glGetUniformLocation(g_program, "Time");
    g_locationTextureHeightmap = glGetUniformLocation(g_program, "Heightmap");
    g_locationTextureSnow = glGetUniformLocation(g_program, "Snow");
    g_locationTextureRocks = glGetUniformLocation(g_program, "Rock");
    g_locationLightPos = glGetUniformLocation(g_program, "LightPos");
  }

  {
    /* Erstellen der Projektions-Matrix.
     * Da die Projektions-Matrix für die Laufzeit des Programms konstant
     * ist, wird sie einmalig gesetzt. */
    float projectionMatrix[16];
    perspective(60, (float)glutGet(GLUT_WINDOW_WIDTH) / glutGet(GLUT_WINDOW_HEIGHT), 0.01f, 5.0f, projectionMatrix);
    glUseProgram(g_program);
    glUniformMatrix4fv(glGetUniformLocation(g_program, "Projection"), 1, GL_FALSE, projectionMatrix);
    glUseProgram(0);
  }

  {
  /* Laden der Textur. */
  int width, height, comp;
  GLubyte* data = stbi_load("../content/textures/heightmap.png", &width, &height, &comp, 4);

  /* Erstellen des ersten Textur-Objekts. */
  glGenTextures(1, &g_textureHeightmap);
  glBindTexture(GL_TEXTURE_2D, g_textureHeightmap);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

  glBindTexture(GL_TEXTURE_2D, 0);
  stbi_image_free(data);

  /* Erstellen des zweiten Textur-Objekts. */
  data =  stbi_load("../content/textures/eye.png", &width, &height, &comp, 4);

  glGenTextures(1, &g_textureSnow);
  glBindTexture(GL_TEXTURE_2D, g_textureSnow);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

  glBindTexture(GL_TEXTURE_2D, 0);
  stbi_image_free(data);

  /* Erstellen des dritten Textur-Objekts. */
  data =  stbi_load("../content/textures/rock.jpg", &width, &height, &comp, 4);

  glGenTextures(1, &g_textureRocks);
  glBindTexture(GL_TEXTURE_2D, g_textureRocks);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

  glBindTexture(GL_TEXTURE_2D, 0);
  stbi_image_free(data);
  }
}

/**
 * Initialisierung des OpenGL-Systems.
 * Setzt Shading, Hintergrund- und Zeichenfarbe.
 * @return Rueckgabewert: im Fehlerfall 0, sonst 1.
 */
static int
initOpenGL (void)
{
  /* DEBUG-Ausgabe */
  INFO (("Initialisiere OpenGL...\n"));

  /* Beim Aufruf von glClear werden alle Pixel des Framebuffers auf
   * diesen Wert gesetzt. */
  glClearColor(0, 0, 0, 0);

  /* Beim Aufrus von glClear werden alle Pixel des Depthbuffer auf
   * diesen Wert gesetzt */
  glClearDepth(1);

  /* Aktivieren des Tiefentests */
  glEnable(GL_DEPTH_TEST);

  /* DEBUG-Ausgabe */
  INFO (("...fertig.\n\n"));

  /* Alles in Ordnung? */
  return 1;
}

/**
 * Timer-Callback.
 * Initiiert Berechnung der aktuellen Position und Farben und anschliessendes
 * Neuzeichnen, setzt sich selbst erneut als Timer-Callback.
 * @param lastCallTime Zeitpunkt, zu dem die Funktion als Timer-Funktion
 *   registriert wurde (In).
 */
static void
cbTimer (int lastCallTime)
{
	(void)lastCallTime;
  /* Seit dem Programmstart vergangene Zeit in Millisekunden */
  int thisCallTime = glutGet (GLUT_ELAPSED_TIME);

  /* Wieder als Timer-Funktion registrieren */
  glutTimerFunc (1000 / TIMER_CALLS_PS, cbTimer, thisCallTime);

  /* Neuzeichnen anstossen */
  glutPostRedisplay ();
}

/**
 * Zeichen-Callback.
 * Loescht die Buffer, ruft das Zeichnen der Szene auf und erzwingt die
 * Darstellung durch OpenGL.
 */
static void
cbDisplay (void)
{
  /* Buffer zuruecksetzen */
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  /* Szene zeichnen */
  drawScene ();

  /* Objekt anzeigen */
  glutSwapBuffers();
}

/**
 * Debug-Callback.
 * @param type Art der Debug-Information. Einer der folgenden Werte:
 *     DEBUG_TYPE_ERROR_ARB
 *     DEBUG_TYPE_DEPRECATED_BEHAVIOR_ARB
 *     DEBUG_TYPE_UNDEFINED_BEHAVIOR_ARB
 *     DEBUG_TYPE_PORTABILITY_ARB
 *     DEBUG_TYPE_PERFORMANCE_ARB
 *     DEBUG_TYPE_OTHER_ARB
 * @param message Debug-Nachricht.
 */
#ifdef GLEW_CORRECT
static void
cbDebugOutput(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
  if (type == GL_DEBUG_TYPE_ERROR_ARB)
    {
      printf("ARB_debug_output: %s\n", message);
      exit(1);
    }
}
#endif

/**
 * Wenn Momentan Wireframes gezeichnet werden,
 * werden nun gefuellte Objekte gezeichnet und umgekehrt.
 */
static void
toggleWireFrame ()
{
  GLint polygonMode[2];
  glGetIntegerv(GL_POLYGON_MODE, polygonMode);
  if (polygonMode[0] == GL_LINE)
  {
    // wireframe
    glPolygonMode(GL_FRONT, GL_FILL);
    glPolygonMode(GL_BACK, GL_FILL);
  } else {
    // normal
    glPolygonMode(GL_FRONT, GL_LINE);
    glPolygonMode(GL_BACK, GL_LINE);
  }
}

void
printHelp ()
{
  printf ("\nTastenbelegung:\n");
  printf ("Q/q/ESC: Programm schliessen\n");
  printf ("F1:      Wireframemodus an-/ausschalten\n");
  printf ("P/p:     Pausiert das drehen der zene\n");
  printf ("H/h:     Zeigt diese Hilfe an\n");
}

/**
 * Callback fuer Tastendruck.
 * Ruft Ereignisbehandlung fuer Tastaturereignis auf.
 * @param key betroffene Taste (In).
 * @param x x-Position der Maus zur Zeit des Tastendrucks (In).
 * @param y y-Position der Maus zur Zeit des Tastendrucks (In).
 */
static void
cbKeyboard (unsigned char key, int x, int y)
{
	(void)x;
	(void)y;
/** Keycode der ESC-Taste */
#define ESC 27

  switch (key)
  {
    /* Programm beenden */
    case 'q':
    case 'Q':
    case ESC:
      exit (0);
      break;
    case 'h':
    case 'H':
      printHelp ();
      break;
    case 'p':
    case 'P':
      if (g_paused)
      {
        g_paused = 0;
        g_lastPaused = (float) t + glutGet(GLUT_ELAPSED_TIME) / 1000;
        printf("Die zeit bei Pause Drücken %f, die aktuelle %f\n", t, g_lastPaused);
        g_lastTime += t;
      } else {
        g_paused = 1;
      }
      break;
    case GLUT_KEY_F1:
      toggleWireFrame ();
      break;
  }
}

/**
 * Registrierung der GLUT-Callback-Routinen.
 */
static void
registerCallbacks (void)
{
  /* DEBUG-Ausgabe */
  INFO (("Registriere Callbacks...\n"));

  /* Ob die extension ARB_debug_output bereit steht hängt vom Treiber
   * ab. Deswegen muss zur Laufzeit geprüft werden, ob das Callback
   * registriert werden kann. */
  if (GLEW_ARB_debug_output)
    {
      /* Verhindert, dass die OpenGL Implementierung das Callback
       * asynchron aufruft. Damit ist sichergestellt, dass der Callstack
       * im Falle eines Fehlers bereit steht.  */
      glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);

      /* In den Rechenzentren ist im Sommersemester 2016 eine veraltete
       * GLEW-Version installiert. Die dort erwartete Funktions-
       * signatur des Callback-Handlers stimmt nicht mit der hier
       * implementierten überein. Wenn ihr (Studenten) auf euren
       * Laptops arbeitet, ist es jedoch durchaus möglich, dass dieser
       * Code fehlerfrei kompiliert. Definiert einfach mal das Makro
       * und testet das. */
#ifdef GLEW_CORRECT
      /* Wenn dieses Callback gesetzt ist, wird die übergebene Funktion
       * immer dann aufgerufen, wenn Fehler auftreten oder weitere
       * nennenswerte Informationen ausgegeben werden müssen. Ihr
       * müsst dann nicht immer glGetError aufrufen und bekommt
       * zusätzlich auch noch einige Zusatzinformationen, die das
       * Debuggen vereinfachen. */
      glDebugMessageCallback(cbDebugOutput, NULL);
#endif

      /* Aktivieren der Extension */
      glEnable(GL_DEBUG_OUTPUT);
    }

  /* Display-Callback (wird ausgefuehrt, wenn neu gezeichnet wird
   * z.B. nach Erzeugen oder Groessenaenderungen des Fensters) */
  glutDisplayFunc (cbDisplay);

  /* Tasten-Druck-Callback - wird ausgefuehrt, wenn eine Taste gedrueckt wird */
  glutKeyboardFunc (cbKeyboard);

    /* Timer-Callback - wird einmalig nach msescs Millisekunden ausgefuehrt */
  glutTimerFunc (1000 / TIMER_CALLS_PS,         /* msecs - bis Aufruf von func */
                 cbTimer,                       /* func  - wird aufgerufen    */
                 glutGet (GLUT_ELAPSED_TIME));  /* value - Parameter, mit dem
                                                   func aufgerufen wird */

  /* DEBUG-Ausgabe */
  INFO (("...fertig.\n\n"));
}

/**
 * Erzeugt ein Fenster.
 * @param title Beschriftung des Fensters
 * @param width Breite des Fensters
 * @param height Hoehe des Fensters
 * @return ID des erzeugten Fensters, 0 im Fehlerfall
 */
static int
createWindow (char *title, int width, int height)
{
  int windowID = 0;

  /* DEBUG-Ausgabe */
  INFO (("Erzeuge Fenster...\n"));

  /* Initialisieren des Fensters */
  glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize (width, height);
  glutInitWindowPosition (0, 0);

  /* Es wird explizit ein Context mit der Version 3.3 und dem Kern-
   * Profil angefordert. */
  glutInitContextVersion(3, 3);
  glutInitContextProfile(GLUT_CORE_PROFILE);

  windowID = glutCreateWindow (title);

  /* DEBUG-Ausgabe */
  INFO (("...fertig..\n\n"));

  return windowID;
}

/**
 * Hauptprogramm.
 * Initialisiert Fenster, Anwendung und Callbacks, startet glutMainLoop.
 * @param argc Anzahl der Kommandozeilenparameter (In).
 * @param argv Kommandozeilenparameter (In).
 * @return Rueckgabewert im Fehlerfall ungleich Null.
 */
int
main (int argc, char **argv)
{
  GLenum error;

  /* Glut initialisieren */
  glutInit (&argc, argv);

  /* Erzeugen des Fensters */
  if (!createWindow ("Mein erstes OpenGL3.3-Programm", 500, 500))
    {
      fprintf (stderr, "Initialisierung des Fensters fehlgeschlagen!");
      exit (1);
    }

  /* Initialisieren von GLEW.
   * Wenn glewExperimental auf GL_TRUE gesetzt wird, läd GLEW
   * zusätzliche Extensions. In diesem konkreten Programm wird dies
   * beispielsweise benötigt, um glGenVertexArrays() aufzurufen. (Ohne
   * glewExperimental auf GL_TRUE zu setzen tritt ein Speicherzugriffs-
   * fehler auf.) */
  glewExperimental = GL_TRUE;
  error = glewInit();
  if (error != GLEW_OK)
    {
      fprintf (stderr, "Initialisierung von GLEW fehlgeschlagen!");
      exit (1);
    }

  /* Initialen OpenGL-Zustand setzen */
  if (!initOpenGL())
    {
      fprintf (stderr, "Initialisierung von OpenGL fehlgeschlagen!");
      exit (1);
    }

  /* registriere Callbacks */
  registerCallbacks ();

  /* Initialisiere Szene */
  initScene();

  /* Hauptschleife der Ereignisbehandlung starten */
  glutMainLoop ();

  return 0;
}
