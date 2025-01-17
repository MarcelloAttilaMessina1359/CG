#version 330 core

/**
 * Interpolierte Textur-Koordinate des Fragments.
 */
in vec2 fTexCoord;

/**
 * Die Position des Lichtes
 */
 uniform vec3 LightPos;
/**
 * Die Normale des Vertices
 */
in vec3 vNormal;

/**
 * Die Position des Vertex
 */
in vec3 v;

/**
 * Höhe des Vertices, von ihr hängt die Textur ab
 */
in float height;

/**
 * Farbe des Fragments, die auf den Framebuffer geschrieben wird.
 */
out vec4 FragColor;

/**
 * Texture-Sampler, um auf die Textur zuzugreifen. Die Einstellungen
 * des Samplers (Interpolation und Randbehandlung) werden beim Erstellen
 * der Textur gesetzt.
 */
uniform sampler2D Snow;

/**
 * Texture-Sampler, um auf die Textur zuzugreifen. Die Einstellungen
 * des Samplers (Interpolation und Randbehandlung) werden beim Erstellen
 * der Textur gesetzt.
 */
uniform sampler2D Rock;


const vec4 ambientSnow = vec4(0.3f, 0.3f, 0.3f, 1.0f);
const vec4 diffuseSnow = vec4(0.3f, 0.3f, 0.3f, 1.0f);
const vec4 specularSnow = vec4(0.3f, 0.3f, 0.3f, 1.0f);

const vec4 ambientRock = vec4(0.1f, 0.1f, 0.1f, 1.0f);
const vec4 diffuseRock = vec4(0.3f, 0.3f, 0.3f, 1.0f);
const vec4 specularRock = vec4(0.3f, 0.3f, 0.3f, 1.0f);
/**
 * Hauptprogramm des Fragment-Shaders.
 * Diese Funktion wird für jedes Fragment ausgeführt!
 */
void main(void)
{
  //FragColor = vec4(vNormal.x, vNormal.y, vNormal.z, 1.0f);
  /* Der richtungs Vektor zur licht Quelle */
  vec3 L = normalize(LightPos - v);
  /* der Richtungs vektor zur Eye position */
  vec3 E = normalize(-v);
  /* Die richtung der Reflektion */
  vec3 R = normalize(-reflect(L,vNormal));
  vec4 Iamb;
  vec4 Idiff;
  vec4 Ispec;
  if (height < 0.3f)
  {
    FragColor = texture(Rock, fTexCoord);
    /* Ambiente wert berechnen */
    Iamb = ambientRock;
    /* Difffuse wert berechnen */
    Idiff = diffuseRock  * max(dot(vNormal,L), 0.0);
    Idiff = clamp(Idiff, 0.0, 1.0);
    /* Specular wert berechnen */
    Ispec = specularRock * pow(max(dot(R,E),0.0),0.3);
    Ispec = clamp(Ispec, 0.0, 1.0);
  }
  else if (height > 0.6f)
  {
    FragColor = texture(Snow, fTexCoord);
    /* Ambiente wert berechnen */
    Iamb = ambientSnow;
    /* Difffuse wert berechnen */
    Idiff = diffuseSnow  * max(dot(vNormal,L), 0.0);
    Idiff = clamp(Idiff, 0.0, 1.0);
    /* Specular wert berechnen */
    Ispec = specularSnow * pow(max(dot(R,E),0.0),0.3);
    Ispec = clamp(Ispec, 0.0, 1.0);
  }
  else
  {
    float weight = smoothstep(0.33f, 0.57f, height);
    FragColor = mix (texture(Rock, fTexCoord), texture(Snow, fTexCoord), weight);
    Iamb = mix(ambientRock, ambientSnow, weight);
    /* diffuse berechnen*/
    Idiff = mix(diffuseRock, diffuseSnow, weight) * max(dot(vNormal,L), 0.0);
    Idiff = clamp(Idiff, 0.0, 1.0);
    /* Specular wert berechnen */
    Ispec = mix(specularRock, specularSnow, weight) * pow(max(dot(R,E),0.0),0.3);
    Ispec = clamp(Ispec, 0.0, 1.0);
  }

  FragColor = FragColor + Iamb + Idiff + Ispec;
}
