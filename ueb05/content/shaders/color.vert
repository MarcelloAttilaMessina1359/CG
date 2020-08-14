#version 330 core

/**
 * Position des Vertex.
 * Wird von der Host-Anwendung über den Attribut-Pointer 0
 * bereitgestellt.
 */
layout (location = 0) in vec4 vPosition;
/**
 * Textur-Koordinate des Vertex.
 * Wird von der Host-Anwendung über den Attribut-Pointer 1
 * bereitgestellt.
 */
layout (location = 1) in vec2 vTexCoord;

/**
 * Hightmap um die höhe des jewiligen vertices zu bestimmen
 */
uniform sampler2D Heightmap;

/**
 * Ausgabe des Vertex-Shaders.
 * Die Textur-Koordinate wird interpoliert vom Fragment-Shader
 * entgegengenommen.
 */
out vec2 fTexCoord;

/**
 * Die Normale des Vertices
 */
out vec3 vNormal;

/**
 * Die Position des Vertex
 */
out vec3 v;

/**
 * Die höhe des Vertices, abhängig von der Height map
 */
out float height;

out vec3 view;

/**
 * Projektions-Matrix.
 * Wird dem Shader mit einem Aufruf von glUniformMatrix4fv übergeben.
 */
uniform mat4 Projection;
/**
 * View-Matrix.
 * Wird dem Shader mit einem Aufruf von glUniformMatrix4fv übergeben.
 */
uniform mat4 ModelView;

/**
 * Vergangene Zeit
 */
uniform float Time;



/**
 * Methode um die Normale eines Vertices zu berechnen
 */
void setNormal()
{
  /* Off set zur nächsten Koordinate */
  float offSetX = 0.001f;
  float offSetZ = offSetX;

  /* Neben liegende punkte festlegen, in x und z */
  if (vPosition.x > 0.99f)
  {
    offSetX *= -1;
  }
  if (vPosition.z > 0.99f)
  {
    offSetZ *= -1;
  }
  vec3 neighbour1 = vec3(vPosition.x, vPosition.y, vPosition.z + offSetZ);
  vec3 neighbour2 = vec3(vPosition.x + offSetX, vPosition.y, vPosition.z);

  /* Textur Koordinaten der benachtparten Punkte berechnen */
  vec2 texCoordNeighbour1 = vec2(neighbour1.x + 1 * 2, neighbour1.z + 1 * 2);
  vec2 texCoordNeighbour2 = vec2(neighbour2.x + 1 * 2, neighbour2.z + 1 * 2);
  neighbour1.y = texture(Heightmap, texCoordNeighbour1).r;
  neighbour2.y = texture(Heightmap, texCoordNeighbour2).r;

  vNormal = normalize(cross(neighbour1, neighbour2));
}

/**
 * Hauptprogramm des Vertex-Shaders.
 * Diese Funktion wird für jeden Vertex ausgeführt!
 */
void main(void)
{
    /* Neue Vertex-Position berechnen auf Grundlage der übergebenen
     * Anhebung. vPosition ist die Vertex-Position, die von der Host-
     * Anwendung im Buffer gespeichert wurde. Die w-Komponente wird
     * von der OpenGL-Implementierung automatisch auf 1 gesetzt. */

    vec4 elevatedPosition;
    float newHeight = texture(Heightmap, vTexCoord).r * 1.0f;
    elevatedPosition.x = vPosition.x;
    elevatedPosition.y = newHeight;
    elevatedPosition.z = vPosition.z;
    elevatedPosition.w = vPosition.w;

    view = vec3(ModelView[0][0], ModelView[1][0], ModelView[2][0]);
    /*
     * Die höhe weiter Geben
     */
    height = newHeight;

    v = vec3(ModelView * vPosition);

    /* Die Textur-Koordinate wird untransformiert an den Fragment-
     * Shader weitergereicht. Bei der Rasterization wird dieser Wert
     * jedoch interpoliert. */
    fTexCoord = vTexCoord;
    setNormal();
    /* Setzen der Vertex-Position im Device-Koordinatensystem.
     * Nachfolgend findet das Clipping und die Rasterization statt. */
    gl_Position = Projection * ModelView * elevatedPosition;
}
