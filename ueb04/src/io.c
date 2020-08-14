/**
 * @file
 * Ein-/Ausgabe-Modul.
 * Das Modul kapselt die Ein- und Ausgabe-Funktionalitaet (insbesondere die GLUT-
 * Callbacks) des Programms.
 *
 * Bestandteil eines Beispielprogramms fuer Picking mit OpenGL & GLUT.
 *
 * @author copyright (C) Fachhochschule Wedel 1999-2011. All rights reserved.
 */

/* ---- System Header einbinden ---- */
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

/* ---- Eigene Header einbinden ---- */
#include "io.h"
#include "scene.h"
#include "debugGL.h"
#include "types.h"
#include "vertex.h"
#include "texture.h"
#include "stringOutput.h"


#define ESC 27
#define PLUS 43
#define MINUS 45
#define TIMER_CALLS_PS 60

/** Vollbild toggle */
static GLint g_fullscreen = 0;

/** Help toggle*/
static GLint g_helpToggle = 0;
/**
 * Der Hilfetext
 */
char* helpTextLine1 = " Hileausgabe:";
char* helpTextLine2 = "--------------";
char* helpTextLine3 = "Tastatur-Steuerungs:";
char* helpTextLine4 = "ESCAPE/q/Q:	beenden des Programms";
char* helpTextLine5 = "F1:	Toggle Wireframe-Modus";
char* helpTextLine6 = "F2:	Toggle Normals";
char* helpTextLine7 = "F11:	Toggle Fullscreen-Modus";
char* helpTextLine8 = "S/s: Kugeln ein-/ausblenden";
char* helpTextLine9 = "T/t:	Umschalten Textur";
char* helpTextLine10 = "+:	Grid-Auflösung erhöhen";
char* helpTextLine11 = "-:	Grid-Auflösung verringern";
char* helpTextLine12 = "--------------";
char* helpTextLine13 = "Maus-Steuerung:";
char* helpTextLine14 = "LMB:	Click auf Kugeln Y-Koordinante erhöhen";
char* helpTextLine15 = "RMB:	Click auf Kugeln Y-Koordinante verringern";

/**
 * Zeichgt die Hilfe an
 */
/**
 * Verarbeitung der Picking-Ergebnisse.
 * Findet den Treffer, der dem Betrachter am naechsten liegt und gibt die
 * Namen dieses Treffers aus.
 * @param numHits Anzahl der getroffenen Objekte (In).
 * @param buffer Buffer, in dem die Treffer gespeichert sind (In).
 * @param left 1 wenn linksklick, sonst 0.
 */
static void
processHits (GLint numHits, GLuint buffer[], int left)
{
  /* Anzahl der Namen des Treffers, der am naechsten zum Betrachter ist */
  GLint numOfClosestNames = 0;
  /* Anzahl der Namen des Treffers, der am naechsten zum Betrachter ist */
  GLuint *ptrClosestNames=NULL;

  /* Laufvariable - Nummer des akt. verarbeiteten Treffers */
  GLint i;
  /* Laufvariable - Zeiger auf den Beginn des Hit-Records */
  GLuint *ptr = (GLuint *) buffer;

  /* DepthBuffer - Wert */
  GLuint minZ = 0xffffffff;

  if (numHits > 0)
    {
      /* Alle Treffer verarbeiten */
      for (i = 0; i < numHits; i++)
        {
          /* Ist der Treffer naeher dran, als der bisher naechste? */
          if (*(ptr + 1) < minZ)
            {
              /* Anzahl der Namen merken */
              numOfClosestNames = *ptr;
              /* Zeiger auf den ersten Namen merken */
              ptrClosestNames = ptr + 3;
              /* als neuen minimalen Tiefenwert merken */
              minZ = *(ptr + 1);
            }
          /* zum Beginn des naechsten Hit-Records springen */
          /* Schrittweite: Anzahl-Feld + minZ-Feld + maxZ-Feld + Namen-Felder */
          ptr += 3 + numOfClosestNames;
        }

      if (numOfClosestNames > 0)
      {
        if (left == 1)
        {
          increaseHeight (*ptrClosestNames);
        } else {
          decreaseHeight (*ptrClosestNames);
        }
        glutPostRedisplay ();
      }
    }
}

/**
 * Setzen der Projektionsmatrix.
 * Setzt die Projektionsmatrix unter Beruecksichtigung des Seitenverhaeltnisses
 * des Anzeigefensters, sodass das Seitenverhaeltnisse der Szene unveraendert
 * bleibt und gleichzeitig entweder in x- oder y-Richtung der Bereich von -1
 * bis +1 zu sehen ist.
 * @param aspect Seitenverhaeltnis des Anzeigefensters (In).
 */
static void
setProjection (GLdouble aspect)
{
  /* perspektivische Projektion */
  gluPerspective (100.0,         /* Oeffnungswinkel */
                  aspect,       /* Seitenverhaeltnis */
                  0.1,          /* nahe Clipping-Ebene */
                  100.0 /* ferne Clipping-Ebene */ );
}

/**
 * Setzen der Kameraposition.
 */
static void
setCamera (void)
{
  /* Kameraposition */
  gluLookAt (0.0, 1.75, 1.0,   /* Augpunkt */
             0.0, 0.0, 0.0,     /* Zentrum */
             0.0, 1.0, 0.0);    /* Up-Vektor */
}

/**
 * Picking. Auswahl von Szenenobjekten durch Klicken mit der Maus.
 * @param left 1 wenn linksklick, sonst 0.
 */
static void
pick (int x, int y, int left)
{
  /** Groesse des Buffers fuer Picking Ergebnisse */
#define SELECTBUFSIZE 512

  /* Viewport (Darstellungsbereich des Fensters) */
  GLint viewport[4];

  /* Puffer fuer Picking-Ergebnis */
  GLuint buffer[SELECTBUFSIZE];

  /* Anzahl der getroffenen Namen beim Picking */
  GLint numHits;

  /* aktuellen Viewport ermitteln */
  glGetIntegerv (GL_VIEWPORT, viewport);

  /* Puffer festlegen, Name-Stack leeren und Rendermode wechseln */
  glSelectBuffer (SELECTBUFSIZE, buffer);
  glInitNames ();
  glRenderMode (GL_SELECT);

  /* Nachfolgende Operationen beeinflussen Projektionsmatrix */
  glMatrixMode (GL_PROJECTION);
  /* Matrix zuruecksetzen - Einheitsmatrix laden */
  glLoadIdentity ();

  gluPickMatrix (x, glutGet (GLUT_WINDOW_HEIGHT) - y, 5, 5, viewport);

  setProjection ((double) glutGet (GLUT_WINDOW_WIDTH) /
                 (double) glutGet (GLUT_WINDOW_HEIGHT));

  /* Nachfolgende Operationen beeinflussen Modelviewmatrix */
  glMatrixMode (GL_MODELVIEW);
  /* Matrix zuruecksetzen - Einheitsmatrix laden */
  glLoadIdentity ();

  setCamera ();

  /* Zeichnen */
  drawScene ();

  /* Zeichnen beenden und auswerten */
  glFlush ();

  /* Nachfolgende Operationen beeinflussen Projektionsmatrix */
  glMatrixMode (GL_PROJECTION);
  /* Matrix zuruecksetzen - Einheitsmatrix laden */
  glLoadIdentity ();

  setProjection ((double) glutGet (GLUT_WINDOW_WIDTH) /
                 (double) glutGet (GLUT_WINDOW_HEIGHT));

  numHits = glRenderMode (GL_RENDER);

  processHits (numHits, buffer, left);
}

/**
 * Zeigt die Hilfe im  Fenster an
 */
 static void
 showHelp (void)
 {
   CGColor3f textColor = { 1.0f, 1.0f, 1.0f };
   drawString (0.0f, 0.05f, textColor, helpTextLine1);
   drawString (0.0f, 0.10f, textColor, helpTextLine2);
   drawString (0.0f, 0.15f, textColor, helpTextLine3);
   drawString (0.0f, 0.20f, textColor, helpTextLine4);
   drawString (0.0f, 0.25f, textColor, helpTextLine5);
   drawString (0.0f, 0.30f, textColor, helpTextLine6);
   drawString (0.0f, 0.35f, textColor, helpTextLine7);
   drawString (0.0f, 0.40f, textColor, helpTextLine8);
   drawString (0.0f, 0.45f, textColor, helpTextLine9);
   drawString (0.0f, 0.50f, textColor, helpTextLine11);
   drawString (0.0f, 0.55f, textColor, helpTextLine12);
   drawString (0.0f, 0.60f, textColor, helpTextLine13);
   drawString (0.0f, 0.65f, textColor, helpTextLine14);
   drawString (0.0f, 0.70f, textColor, helpTextLine15);
 }

/**
 * Verarbeitung eines Tasturereignisses.
 * ESC-Taste und q, Q beenden das Programm.
 * @param key Taste, die das Ereignis ausgeloest hat. (ASCII-Wert oder WERT des
 *        GLUT_KEY_<SPECIAL>.
 * @param status Status der Taste, GL_TRUE=gedrueckt, GL_FALSE=losgelassen.
 * @param isSpecialKey ist die Taste eine Spezialtaste?
 * @param x x-Position des Mauszeigers zum Zeitpunkt der Ereignisausloesung.
 * @param y y-Position des Mauszeigers zum Zeitpunkt der Ereignisausloesung.
 */
static void
handleKeyboardEvent (int key, int status, GLboolean isSpecialKey, int x,
                     int y)
{
	(void)x;
	(void)y;
  /** Keycode der ESC-Taste */
  /* Taste gedrueckt */
  if (status == GLUT_DOWN)
    {
      /* Spezialtaste gedrueckt */
      if (isSpecialKey)
        {
          switch (key) {
              /* (De-)Aktivieren des Wireframemode */
            case GLUT_KEY_F1:
              toggleWireframeMode ();
              glutPostRedisplay ();
              break;
            case GLUT_KEY_F11:
              if (g_fullscreen)
              {
                g_fullscreen = 0;
                glutReshapeWindow(500, 500);
                glutPositionWindow(0,100);
              } else {
                g_fullscreen = 1;
                glutFullScreen ();
              }
              break;
              case GLUT_KEY_F2:
              enableNormals (!normalsEnabled());
              break;
          }
        }
      /* normale Taste gedrueckt */
      else
        {
          switch (key)
            {
            case PLUS:
              increaseWidth ();
              break;
            case MINUS:
              decreaseWidth ();
              break;
            case 't':
            case 'T':
              if (getTexturingStatus ())
              {
                setTexturingStatus (0);
              } else {
                setTexturingStatus (1);
              }
              break;
            case 's':
            case 'S':
              setShowBalls (!getShowBalls ());
              break;
            case 'h':
            case 'H':
              g_helpToggle = !g_helpToggle;
              break;
              /* Programm beenden */
            case 'q':
            case 'Q':
            case ESC:
              free (getNormals ());
              free (getVertices ());
              free (getIndices ());
              exit (0);
              break;
            }
        }
    }
}



/**
 * Verarbeitung eines Mausereignisses.
 * Durch Bewegung der Maus bei gedrueckter Maustaste kann die aktuelle
 * Zeichenfarbe beeinflusst werden.
 * Falls Debugging aktiviert ist, wird jedes Mausereignis auf stdout
 * ausgegeben.
 * @param x x-Position des Mauszeigers zum Zeitpunkt der Ereignisausloesung.
 * @param y y-Position des Mauszeigers zum Zeitpunkt der Ereignisausloesung.
 * @param eventType Typ des Ereignisses.
 * @param button ausloesende Maustaste (nur bei Ereignissen vom Typ mouseButton).
 * @param buttonState Status der Maustaste (nur bei Ereignissen vom Typ mouseButton).
 */
static void
handleMouseEvent (int x, int y, CGMouseEventType eventType, int button,
                  int buttonState)
{
  switch (eventType)
    {
    case mouseButton:
      switch (button)
        {
        case GLUT_LEFT_BUTTON:
          if (buttonState == GLUT_UP)
          {
            pick (x, y, 1);
          }
          break;
        case GLUT_RIGHT_BUTTON:
          if (buttonState == GLUT_UP)
          {
            pick (x, y, 0);
          }
        default:
          break;
        }
      break;
    default:
      break;
    }
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
  handleKeyboardEvent (key, GLUT_DOWN, GL_FALSE, x, y);
}

/**
 * Callback fuer Druck auf Spezialtasten.
 * Ruft Ereignisbehandlung fuer Tastaturereignis auf.
 * @param key betroffene Taste (In).
 * @param x x-Position der Maus zur Zeit des Tastendrucks (In).
 * @param y y-Position der Maus zur Zeit des Tastendrucks (In).
 */
static void
cbSpecial (int key, int x, int y)
{
  handleKeyboardEvent (key, GLUT_DOWN, GL_TRUE, x, y);
}

/**
 * Mouse-Button-Callback.
 * @param button Taste, die den Callback ausgeloest hat.
 * @param state Status der Taste, die den Callback ausgeloest hat.
 * @param x X-Position des Mauszeigers beim Ausloesen des Callbacks.
 * @param y Y-Position des Mauszeigers beim Ausloesen des Callbacks.
 */
static void
cbMouseButton (int button, int state, int x, int y)
{
  handleMouseEvent (x, y, mouseButton, button, state);
}

/**
 * Callback fuer Aenderungen der Fenstergroesse.
 * Initiiert Anpassung der Projektionsmatrix an veränderte Fenstergroesse.
 * @param w Fensterbreite (In).
 * @param h Fensterhoehe (In).
 */
static void
cbReshape (int w, int h)
{
  /* Das ganze Fenster ist GL-Anzeigebereich */
  glViewport (0, 0, (GLsizei) w, (GLsizei) h);

  /* Nachfolgende Operationen beeinflussen Projektionsmatrix */
  glMatrixMode (GL_PROJECTION);
  /* Matrix zuruecksetzen - Einheitsmatrix laden */
  glLoadIdentity ();

  /* Anpassen der Projektionsmatrix an das Seitenverhältnis des Fensters */
  setProjection ((GLdouble) w / (GLdouble) h);
}

/**
 * Zeichen-Callback.
 * Loescht die Buffer, ruft das Zeichnen der Szene auf und tauscht den Front-
 * und Backbuffer.
 */
static void
cbDisplay (void)
{
  /* Framewbuffer und z-Buffer zuruecksetzen */
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  /* Nachfolgende Operationen beeinflussen Modelviewmatrix */
  glMatrixMode (GL_MODELVIEW);
  /* Matrix zuruecksetzen - Einheitsmatrix laden */
  glLoadIdentity ();

  setCamera ();

  /* Szene zeichnen */
  drawScene ();

  if (g_helpToggle) {
    showHelp();
  }
  /* Objekt anzeigen */
  glutSwapBuffers ();
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
  /* Seit dem Programmstart vergangene Zeit in Millisekunden */
  int thisCallTime = glutGet (GLUT_ELAPSED_TIME);

  /* Wieder als Timer-Funktion registrieren */
  glutTimerFunc (1000 / TIMER_CALLS_PS, cbTimer, thisCallTime);

  /* Neuzeichnen anstossen */
  glutPostRedisplay ();
}

/**
 * Registrierung der GLUT-Callback-Routinen.
 */
static void
registerCallbacks (void)
{
  /* Mouse-Button-Callback (wird ausgefuehrt, wenn eine Maustaste
   * gedrueckt oder losgelassen wird) */
  glutMouseFunc (cbMouseButton);

  /* Tasten-Druck-Callback - wird ausgefuehrt, wenn eine Taste gedrueckt wird */
  glutKeyboardFunc (cbKeyboard);

  /* Spezialtasten-Druck-Callback - wird ausgefuehrt, wenn Spezialtaste
   * (F1 - F12, Links, Rechts, Oben, Unten, Bild-Auf, Bild-Ab, Pos1, Ende oder
   * Einfuegen) gedrueckt wird */
  glutSpecialFunc (cbSpecial);

  /* Timer-Callback*/
  glutTimerFunc (1000 / TIMER_CALLS_PS,
                 cbTimer,
                 glutGet (GLUT_ELAPSED_TIME));

  /* Reshape-Callback - wird ausgefuehrt, wenn neu gezeichnet wird (z.B. nach
   * Erzeugen oder Groessenaenderungen des Fensters) */
  glutReshapeFunc (cbReshape);

  /* Display-Callback - wird an mehreren Stellen imlizit (z.B. im Anschluss an
   * Reshape-Callback) oder explizit (durch glutPostRedisplay) angestossen */
  glutDisplayFunc (cbDisplay);
}

/**
 * Initialisiert das Programm (inkl. I/O und OpenGL) und startet die
 * Ereignisbehandlung.
 * @param title Beschriftung des Fensters
 * @param width Breite des Fensters
 * @param height Hoehe des Fensters
 * @return ID des erzeugten Fensters, 0 im Fehlerfall
 */
int
initAndStartIO (char *title, int width, int height, int argc, char **argv)
{
  int windowID = 0;

  /* Glut initialisieren */
  glutInit (&argc, argv);

  /* DEBUG-Ausgabe */
  INFO (("Erzeuge Fenster...\n"));

  /* Initialisieren des Fensters */
  /* RGB-Framewbuffer, Double-Buffering und z-Buffer anfordern */
  glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize (width, height);
  glutInitWindowPosition (0, 0);

  /* Fenster erzeugen */
  windowID = glutCreateWindow (title);

  if (windowID)
    {

      /* DEBUG-Ausgabe */
      INFO (("...fertig.\n\n"));

      /* DEBUG-Ausgabe */
      INFO (("Initialisiere Szene...\n"));

      if (initScene ())
        {
          /* DEBUG-Ausgabe */
          INFO (("...fertig.\n\n"));

          INFO (("Lade und initialisiere Texturen...\n"));

          if (initTextures ())
          {
            /* DEBUG-Ausgabe */
            INFO (("...fertig.\n\n"));

            /* DEBUG-Ausgabe */
            INFO (("Registriere Callbacks...\n"));

            registerCallbacks ();

            /* DEBUG-Ausgabe */
            INFO (("...fertig.\n\n"));

            /* DEBUG-Ausgabe */
            INFO (("Trete in Schleife der Ereignisbehandlung ein...\n"));

            glutMainLoop ();
          } else {
            /* DEBUG-Ausgabe */
            INFO (("...fehlgeschlagen.\n\n"));

            glutDestroyWindow (windowID);
            windowID = 0;
          }
        }
      else
        {
          /* DEBUG-Ausgabe */
          INFO (("...fehlgeschlagen.\n\n"));

          glutDestroyWindow (windowID);
          windowID = 0;
        }
    }
  else
    {
      /* DEBUG-Ausgabe */
      INFO (("...fehlgeschlagen.\n\n"));
    }

  return windowID;
}
