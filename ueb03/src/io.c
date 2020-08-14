/**
 * @file
 * Ein-/Ausgabe-Modul.
 * Das Modul kapselt die Ein- und Ausgabe-Funktionalitaet (insbesondere die GLUT-
 * Callbacks) des Programms.
 *
 * Bestandteil eines Beispielprogramms fuer Animationen mit OpenGL & GLUT.
 *
 * @author copyright (C) Fachhochschule Wedel 1999-2011. All rights reserved.
 */

/* ---- System Header einbinden ---- */
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

#ifdef MACOSX
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

/* ---- Eigene Header einbinden ---- */
#include "io.h"
#include "types.h"
#include "logic.h"
#include "scene.h"
#include "debugGL.h"

/* ---- Konstanten ---- */
/** Anzahl der Aufrufe der Timer-Funktion pro Sekunde */
#define TIMER_CALLS_PS 60
/** Anzahl Sekunden, nach der das Wasser sich ausbreitet */
#define TIMER_WATER_SREAD 2

#include "stringOutput.h"

/**
 * Wann sich das Wasser das letzte mal ausgebreitet hat.
 */
static float lastWaterSpread = 0;
/**
 * Zoom
 */
static float zoom = 100.0f;
/**
 * ungleich 0 wenn Spiel pausiert wurde.
 * -1 => gestorben
 * 1  => gewonnen
 * 2  => Hilfe anzeigen
 */
static int paused = 0;

/**
 * Zum anzeigen ob der Spieler sich gerade bewegt.
 */
static int isMoving = GL_FALSE;

/**
 * Rotation der Ansicht.
 */
static int g_rotation = 0;

/**
 * Der Hilfetext
 */
char* helpTextLine1 = "Tastenbelegung";
char* helpTextLine2 = "Pfeiltasten: Bewegen den Spieler.";
char* helpTextLine3 = "q/Q/ESC: Spiel schliessen.";
char* helpTextLine4 = "F1: Zum hin- und herwechseln";
char* helpTextLine5 = "      zwischen Wireframe- und Flaechendarstellung.";
char* helpTextLine6 = "1/2/3:Starten das gewaehlte Level.";
char* helpTextLine7 = "h/H: Blendet diese Hilfe ein.";
char* helpTextLine8 = "Press any Key to continue";

/**
 * Setzt den gewünschten Wert für paused.
 * @param isPaused Der gewünschte Wert für paused
 */
void
setPaused (int isPaused)
{
  paused = isPaused;
}

/**
 * Gibt den aktuellen Wert von paused zurück.
 * @return Der aktuelle Wert von paused
 */
int
getPaused ()
{
  return paused;
}

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

/**
 * Zeigt die Hilfe in der Standardausgabe an.
 */
static void
showHelp ()
{
  setPaused (2);
}

/**
 * Verarbeitung eines Tasturereignisses.
 * Pfeiltasten steuern die Position des angezeigten Rechtecks.
 * F1-Taste (de-)aktiviert Wireframemodus.
 * F2-Taste schaltet zwischen Fenster und Vollbilddarstellung um.
 * ESC-Taste und q, Q beenden das Programm.
 * Falls Debugging aktiviert ist, wird jedes Tastaturereignis auf stdout
 * ausgegeben.
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
  /** Keycode der ESC */
#define ESC 27
#define ONE 49
#define TWO 50
#define THREE 51
  /* Taste gedrueckt */
  if (status == GLUT_DOWN)
  {
    if (!paused)
    {
    /* Spezialtaste gedrueckt */
      if (isSpecialKey)
      {
          switch (key)
          {
              /* Bewegung des Rechtecks in entsprechende Richtung starten */
            case GLUT_KEY_LEFT:
              if (!isMoving)
              {
                isMoving = GL_TRUE;
                setMovement (LEFT);
              }
              break;
            case GLUT_KEY_RIGHT:
              if (!isMoving)
              {
                isMoving = GL_TRUE;
                setMovement (RIGHT);
              }
              break;
            case GLUT_KEY_UP:
              if (!isMoving)
              {
                isMoving = GL_TRUE;
                setMovement (UP);
              }
              break;
            case GLUT_KEY_DOWN:
              if (!isMoving)
              {
                isMoving = GL_TRUE;
                setMovement (DOWN);
              }
              break;
            case GLUT_KEY_F1:
              toggleWireFrame();
              break;
            case GLUT_KEY_F2:
              setShowNormals (!getShowNormals ());
              break;
            case GLUT_KEY_F3:
              setLight0Status(!getLight0Status());
              glutPostRedisplay ();
              break;
            case GLUT_KEY_F4:
              setLight1Status(!getLight1Status());
              glutPostRedisplay ();
              break;
          }
      }
      /* normale Taste gedrueckt */
      else
      {
        switch (key)
        {
          case 'a':
          case 'A':
            if (g_rotation == 0)
            {
              g_rotation = 7;
            } else {
              g_rotation -= 1;
            }
            break;
          case 'd':
          case 'D':
            if (g_rotation == 7)
            {
              g_rotation = 0;
            } else {
              g_rotation += 1;
            }
            break;
            /* Programm beenden */
          case 'q':
          case 'Q':
          case ESC:
            exit (0);
            break;
          case 'h':
          case 'H':
            showHelp ();
            break;
          case 'c' :
          case 'C' :
            setCameraStatus(!getCameraStatus());
            break;
          case ONE:
            setLevel(LEVEL_1);
            break;
          case TWO :
            setLevel(LEVEL_2);
            break;
          case THREE :
            setLevel(LEVEL_3);
            break;
        }
      }
    } else if (paused == 1) {
      // paused
      int currLevel = getLevel ();
      if (currLevel == 3)
      {
        setLevel (LEVEL_1);
      } else {
        setLevel (currLevel + 1);
      }
      setPaused (0);
    } else {
      setLevel (getLevel ());
      setPaused (0);
    }
  } 
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
 * Callback fuer Loslassen von Spezialtasten.
 * Ruft Ereignisbehandlung fuer Tastaturereignis auf.
 * @param key betroffene Taste (In).
 * @param x x-Position der Maus zur Zeit des Loslassens (In).
 * @param y y-Position der Maus zur Zeit des Loslassens (In).
 */
static void
cbSpecialUp (int key, int x, int y)
{
  handleKeyboardEvent (key, GLUT_UP, GL_TRUE, x, y);
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
 * Setzen der Projektionsmatrix.
 * Setzt die Projektionsmatrix unter Berücksichtigung des Seitenverhaeltnisses
 * des Anzeigefensters, sodass das Seitenverhaeltnisse der Szene unveraendert
 * bleibt und gleichzeitig entweder in x- oder y-Richtung der Bereich von -1
 * bis +1 zu sehen ist.
 * @param aspect Seitenverhaeltnis des Anzeigefensters (In).
 */
static void
setProjection (GLdouble aspect)
{
  /* Nachfolgende Operationen beeinflussen Projektionsmatrix */
  glMatrixMode (GL_PROJECTION);
  /* Matrix zuruecksetzen - Einheitsmatrix laden */
  glLoadIdentity ();

  /* Koordinatensystem bleibt quadratisch */
  if (aspect <= 1)
    {
      gluOrtho2D (-1.0, 1.0,    /* links, rechts */
                  -1.0 / aspect, 1.0 / aspect); /* unten, oben */
    }
  else
    {
      gluOrtho2D (-1.0 * aspect, 1.0 * aspect,  /* links, rechts */
                  -1.0, 1.0);   /* unten, oben */
    }
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

  /* Seit dem letzten Funktionsaufruf vergangene Zeit in Sekunden */
  double interval = (double) (thisCallTime - lastCallTime) / 1000.0f;

  if (!paused && (thisCallTime - lastWaterSpread) / 1000.0f >= TIMER_WATER_SREAD) {
    spreadWater ();
    lastWaterSpread = thisCallTime;
  }
  if (isMoving)
  {
    /* Neue Spieler Position berechnen */
    isMoving = movePlayer(interval);
  }
  /* Wieder als Timer-Funktion registrieren */
  glutTimerFunc (1000 / TIMER_CALLS_PS, cbTimer, thisCallTime);

  /* Neuzeichnen anstossen */
  glutPostRedisplay ();
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

  /* Anpassen der Projektionsmatrix an das Seitenverhältnis des Fensters */
  setProjection ((GLdouble) w / (GLdouble) h);
}

/**
 * Setzt einen Viewport für 2-dimensionale Darstellung.
 * Ruft das zeichnen der Szene in diesem Viewport auf.
 *
 * @param x, y Position des Viewports im Fenster - (0, 0) ist die untere linke Ecke
 * @param width, height Breite und Höhe des Viewports
 */
static void
set2DViewport (GLint x, GLint y, GLint width, GLint height) {
  /* Seitenverhältnis bestimmen */
  double aspect = (double) width / height;

  /* Folge Operationen beeinflussen die Projektionsmatrix */
  glMatrixMode (GL_PROJECTION);

  /* Einheitsmatrix laden */
  glLoadIdentity ();

  /* Viewport-Position und -Ausdehnung bestimmen */
  glViewport (x, y, width, height);

  /* Das Koordinatensystem bleibt immer quadratisch */
  if (aspect <= 1) {
    gluOrtho2D (-1, 1,                      /* left, right */
                -1 / aspect, 1 / aspect);   /* bottom, top */
  } else {
    gluOrtho2D (-1 * aspect, 1 * aspect,    /* left, right */
                -1, 1);                     /* bottom, top */
  }

  /* Folge Operationen beeinflussen die Modelviewmatrix */
  glMatrixMode (GL_MODELVIEW);

  /* Einheitsmatrix laden */
  glLoadIdentity ();

  /* Szene zeichnen lassen */
  drawScene2D ();
}

/**
 * Setzt einen Viewport für 3-dimensionale Darstellung
 * mit perspektivischer Projektion und legt eine Kamera fest.
 * Ruft das zeichnen der Szene in diesem Viewport auf.
 *
 * @param x, y Position des Viewports im Fenster - (0, 0) ist die untere linke Ecke
 * @param width, height Breite und Höhe des Viewports
 * @param lookAt enthält die für glLookAt benötigten Daten zur Kamera (Augpunkt, Zentrum, Up-Vektor)
 */
static void
set3DViewport (GLint x, GLint y, GLint width, GLint height, GLdouble lookAt[9]) {
  /* Seitenverhältnis bestimmen */
  double aspect = (double) width / height;

  /* Folge Operationen beeinflussen die Projektionsmatrix */
  glMatrixMode (GL_PROJECTION);

  /* Einheitsmatrix laden */
  glLoadIdentity ();

  /* Viewport-Position und -Ausdehnung bestimmen */
  glViewport (x, y, width, height);

  /* Perspektivische Darstellung */
  gluPerspective (zoom,       /* Öffnungswinkel */
                  aspect,   /* Seitenverhältnis */
                  0.1,      /* nahe Clipping-Ebene */
                  100);     /* ferne Clipping-Ebene */

  /* Folge Operationen beeinflussen die Modelviewmatrix */
  glMatrixMode (GL_MODELVIEW);

  /* Einheitsmatrix laden */
  glLoadIdentity ();

  /* Kameraposition */
  gluLookAt (lookAt[0], lookAt[1], lookAt[2],   /* Augpunkt */
             lookAt[3], lookAt[4], lookAt[5],     /* Zentrum */
             lookAt[6], lookAt[7], lookAt[8]);    /* Up-Vektor */

  /* Szene zeichnen lassen */
  drawScene3D ();
}

/**
 * Zeichen-Callback.
 * Loescht die Buffer, ruft das Zeichnen der Szene auf und tauscht den Front-
 * und Backbuffer.
 */
static void
cbDisplay (void)
{
  /* Die verschiebung des at Position Punkts, abhängig von der Rotation
     des Spielers */
  GLint xShift = 0,
        zShift = 0;
  /* Die Position des Spielers */
  CGPoint2f * playerPosition = getPlayerPosition();
  /* Die Rotation des Spielers*/
  GLint playerRotation = getPlayerRotation();
  /* Die umgerechnete Position des Spielers */
  GLfloat playerPositionX = -9.0f  + 2.0f * (*playerPosition)[0] + 1;
  GLfloat playerPositionZ = -9.0f  + 2.0f * (*playerPosition)[1] + 1;

 switch (playerRotation) {
   case 0:
        zShift = 6;
   break;
   case 90:
        xShift = 6;
   break;
   case 180:
        zShift = -6;
   break;
   case 270:
        xShift = -6;
   break;
 }

  //Direction playerRotation = getPlayerRotation();
  /* glTranslatef(-9.0f  + 2.0f * (*playerPosition)[0] + 1,
               0.0f,
               -9.0f + 2.0f * (*playerPosition)[1] + 1); */
  /* Farbe des textes */
  CGColor3f textColor = { 1.0f, 1.0f, 1.0f };
  /* Kamera von schräg oben */
  GLfloat rotatedX = 0.0;
  GLfloat rotatedZ = 10.0;
  switch (g_rotation) {
    case 1:
      rotatedX = 10.0;
      rotatedZ = 10.0;
      break;
    case 2:
      rotatedX = 10.0;
      rotatedZ = 0.0;
      break;
    case 3:
      rotatedX = 10.0;
      rotatedZ = -10.0;
      break;
    case 4:
      rotatedX = 0.0;
      rotatedZ = -10.0;
      break;
    case 5:
      rotatedX = -10.0;
      rotatedZ = -10.0;
      break;
    case 6:
      rotatedX = -10.0;
      rotatedZ = 0.0;
      break;
    case 7:
      rotatedX = -10.0;
      rotatedZ = 10.0;
      break;
    default:
      break;
  }
  GLdouble lookAt1[9] = {rotatedX, 10.0, rotatedZ, /* Augpunkt */
                         0.0, 0.0, 0.0,   /* Zentrum */
                         0.0, 1.0, 0.0};   /* Up-Vektor */
  /* Kamera Position des Spielers */
  GLdouble lookAt2[9] = { playerPositionX, .7, playerPositionZ, /* Augpunkt */
                         playerPositionX + xShift, .7, playerPositionZ + zShift,  /* Player Position + x */
                         0.0, 1.0, 0.0};   /* Up-Vektor */


  /* Fensterdimensionen auslesen */
  int width = glutGet (GLUT_WINDOW_WIDTH);
  int height = glutGet (GLUT_WINDOW_HEIGHT);

  /* Die ID des aktuellen Levels */
  Level id = getLevel();

  /* Buffer zuruecksetzen */
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  /* Nachfolgende Operationen beeinflussen Modelviewmatrix */
  glMatrixMode (GL_MODELVIEW);

  /* Matrix zuruecksetzen - Einheitsmatrix laden */
  glLoadIdentity ();

  /* Szene zeichnen */
  if (paused != 2)
  {

  /* 2D-Viewport setzen */
    set2DViewport (width * 2 / 3, height * 2 / 3,   /* x, y */
                  width / 3, height / 3);  /* breite, hoehe */
    /* 3D-Viewports setzen */
    set3DViewport (0, 0,                    /* x, y */
                  width, height,   /* breite, hoehe */
                  getCameraStatus() ? lookAt2 : lookAt1);                /* Kamera */


  }
  if (!paused)
  {
    //Das aktuelle Level anzeigen
    drawString (0.4f, 0.99f, textColor, "LEVEL%d", id);
  } else if (paused == 1) {
    CGColor3f textColor = { 0, 1.0f, 0 };
    drawString (0.4f, 0.99f, textColor, "Gewonnen!");
  } else if (paused == -1) {
    CGColor3f textColor = { 1.0f, 0, 0 };
    drawString (0.4f, 0.99f, textColor, "You Died");
  } else if (paused == 2) {
    drawString (0.0f, 0.25f, textColor, helpTextLine1);
    drawString (0.0f, 0.35f, textColor, helpTextLine2);
    drawString (0.0f, 0.4f, textColor, helpTextLine3);
    drawString (0.0f, 0.45f, textColor, helpTextLine4);
    drawString (0.0f, 0.5f, textColor, helpTextLine5);
    drawString (0.0f, 0.55f, textColor, helpTextLine6);
    drawString (0.0f, 0.6f, textColor, helpTextLine7);
    drawString (0.0f, 0.7f, textColor, helpTextLine8);
  }
  /* Objekt anzeigen */
  glutSwapBuffers ();           /* fuer DoubleBuffering */
}

/**
 * Methode die das Zoomen mit dem Mausrad hÄndelt
 */
void mouse(int button, int state, int x, int y)
{
   // Wheel reports as button 3(scroll up) and button 4(scroll down)
   if (state == GLUT_DOWN)
   {
     //Ran zoomen
     if (button == 3)
     {
       if (zoom > 5)
       {
         zoom *= 0.9;
       }
     }
     //Raus zoomen
     if (button == 4)
     {
       if (zoom < 150)
       {
         zoom *= 1.10;
       }
     }
   }

}

/**
 * Registrierung der GLUT-Callback-Routinen.
 */
static void
registerCallbacks (void)
{

  /* Tasten-Druck-Callback - wird ausgefuehrt, wenn eine Taste gedrueckt wird */
  glutKeyboardFunc (cbKeyboard);
  /* Spezialtasten-Druck-Callback - wird ausgefuehrt, wenn Spezialtaste
   * (F1 - F12, Links, Rechts, Oben, Unten, Bild-Auf, Bild-Ab, Pos1, Ende oder
   * Einfuegen) gedrueckt wird */
  glutSpecialFunc (cbSpecial);

  /* Spezialtasten-Loslass-Callback - wird ausgefuehrt, wenn eine Spezialtaste
   * losgelassen wird */
  glutSpecialUpFunc (cbSpecialUp);

  /* Zoomen und drehen der Maus bearbeiten*/
  glutMouseFunc(mouse);
  /* Automat. Tastendruckwiederholung ignorieren */
  glutIgnoreKeyRepeat (1);


  /* Timer-Callback - wird einmalig nach msescs Millisekunden ausgefuehrt */
  glutTimerFunc (1000 / TIMER_CALLS_PS, /* msecs - bis Aufruf von func */
                 cbTimer,       /* func  - wird aufgerufen    */
                 glutGet (GLUT_ELAPSED_TIME));  /* value - Parameter, mit dem
      //                                             func aufgerufen wird */

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

	srand(time(0));

  /* DEBUG-Ausgabe */
  INFO (("Erzeuge Fenster...\n"));

  /* Initialisieren des Fensters */
  /* RGB-Framewbuffer, Double-Buffering und z-Buffer anfordern */
  glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH); /* fuer DoubleBuffering */
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

          setLevel (LEVEL_1);

          /* DEBUG-Ausgabe */
          INFO (("Registriere Callbacks...\n"));

          registerCallbacks ();

          /* DEBUG-Ausgabe */
          INFO (("...fertig.\n\n"));

          /* DEBUG-Ausgabe */
          INFO (("Trete in Schleife der Ereignisbehandlung ein...\n"));

          glutMainLoop ();
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
