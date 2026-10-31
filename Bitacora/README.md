# Bitacora para el proyecto final

## Integrantes:
-----------------------------------------------------
- **NOMBRE COMPLETO:** Calzada Lozada Sergio Armando
-
  28 de octubre de 2026: Definición Arquitectónica de los Dos Planetas y Alcance del UniversoRevisión de la propuesta oficial del Grupo 07: Se estableció la división del sistema planetario del proyecto para integrar los cuatro universos propuestos (League of Legends, Project Hail Mary, Los Simpson y Mickey 17):Planeta 1 (Runaterra / Monte Targon): Escenario de despegue con la plataforma Hextech del cohete Hail Mary, la entrada al Vacío, una luna en órbita y la Singularidad de Aurelion Sol en el cielo.Planeta 2 (Erid intervenido por Rigel VII): Escenario de aterrizaje donde se desarrolla la carrera espacial y se concentran los elementos de Los Simpson.Inventario de requerimientos de Los Simpson en el Planeta 2:Podio Presidencial de Kang y Kodos: Modelado jerárquico (base como nodo padre; micrófonos, pancartas y reflectores como hijos) con doble función: escenario principal iluminado por Spotlights nocturnos y arcos de checkpoint sobre la pista que activan el turbo de la nave al atravesarlos.NPCs de Kang y Kodos: Animación jerárquica con rotación cíclica de tentáculos dependiente del torso principal.Platillo Volador Rigelciano: Nave con cúpula semitransparente (Alpha Blending), anillo rotatorio en el eje $Y$ y cilindro inferior verde emisivo como rayo abductor.Inanimadas Barras de Carbono: Objetos emisivos de color verde fluorescente distribuidos por el terreno y los bordes del circuito, configurados como Point Lights en GLSL para guiar al jugador durante la noche.
  30 de octubre de 2026 : Diseño Geométrico del Planeta 2 y Trazado de la Pista

Geometría híbrida del Planeta 2 (PlanetaErid.h): Se diseñó una malla procedural en C++ y OpenGL basada en una esfera de gran escala (radio = 1200.0f) truncada en su polo superior (alturaMeseta = 900.0f) para formar una gran explanada plana rodeada de relieves rocosos. Esto permite visualizar el astro como un cuerpo esférico con su cinturón de asteroides desde el espacio exterior y, al mismo tiempo, contar con una superficie horizontal amplia al aterrizar.

Efecto de horizonte "casi infinito": Se definió la combinación de cuatro técnicas gráficas para ocultar los límites de la meseta al nivel del suelo:

Ampliación del plano de recorte lejano (zFar = 8000.0f) en la matriz de proyección en perspectiva (glm::perspective).

Repetición de coordenadas de texturizado (Texture Tiling de 100.0f) con GL_REPEAT para mantener detalle de roca a corta distancia sin estirar la textura.

Implementación de neblina densa (Fog) por distancia en el Fragment Shader (shader.frag) con color vec3(0.18, 0.15, 0.22), difuminando el borde de la meseta antes de los 600.0f metros.

Activación condicional de la neblina al entrar a la atmósfera del Planeta 2.

Trazado del circuito de carreras: Se planteó la generación de la pista sobre la meseta (Y = 900.1f) utilizando curvas Splines de Catmull-Rom para conectar rectas, curvas cerradas y rampas elevadas, delimitando los bordes laterales con bloques de Xenonita, estructuras rocosas de Erid y Rocky como espectador.

30 de octubre de 2026 (Noche): Atmósfera Espacial y Generación del Skybox del Planeta 2

Selección cromática de la atmósfera: Se definió una identidad visual de nebulosa espacial en tonos púrpura profundo y verde radioactivo para empatar la atmósfera densa de Erid con la estética rigelciana de Kang y Kodos, contemplando el ciclo de día y noche de 2 minutos.

Procesamiento de las 6 caras del Cubemap: A partir de la textura base de nebulosa seleccionada, se estructuró un flujo automatizado en Python (Pillow + NumPy) para generar las 6 vistas cuadradas de 1024x1024 píxeles (erid_right, erid_left, erid_top, erid_bottom, erid_back, erid_front) sin costuras laterales (seamless).

Fusión entre Skybox y terreno: Se programó un degradado en la mitad inferior de las caras laterales y en erid_bottom hacia el color exacto de la neblina de Erid (RGB(46, 38, 56)), logrando una transición imperceptible entre la geometría del Planeta 2 y el fondo espacial.
-----------------------------------------------------
- **NOMBRE COMPLETO:** Paz Rosas Jesús Eduardo

23/sep/26: Busqué modelos específicos para Aurelion sol y los probé en Blender.

30/sep/26: Agregué a la lista modelos de cajas hextech.

7/oct/26: Diseñé la torre hextech (sin detalle) y agregué los modelos de tec Hextech para adornar

14/oct/26: Descargué una textura para planeta, la asigné a un modelo de esfera y presenté los modelos de Aurelion, la plataforma de lanzamiento y un planeta.

-----------------------------------------------------
- **NOMBRE COMPLETO:** López Vizcarra Ángel David

30/sep/2026: Creé el modelo de mi personaje elegido(Creeper).

5/oct/2026: Busqué elementos del universo y encontré el modelo aun no texturizado de un camion.

7/oct/2026: Busqué un skybox para el planeta que compartiré con el personaje de LoL

-----------------------------------------------------
- **NOMBRE COMPLETO:** Zamora Ayala Antonio Manuel

11/oct/26 Del código de la Práctica 7, dejé solo lo más elemental para utilizarlo como base para nuestro proyecto

11/oct/26 Me puse a buscar en Google algunos modelos de mi personaje (Rocky) y encontré el siguiente: https://sketchfab.com/3d-models/project-hail-mary-rocky-853f49d0b6a7416b8b7c48c619d01798

12/oct/26 Descargué el modelo (Rocky) y en Blender me puse a texturizarlo y a rellenar algunos agujeros con el 'shortcut M + Merge at Center'

15/oct/26 Creé el repositorio para trabajar en conjunto el proyecto final

15/oct/26 Terminé de texturizar el modelo y lo separé en 6 elementos para posteriormente implementarlo con jerarquía en el código

15/oct/26 Reutilicé el modelo del satélite (ya separado para la jerarquía) que se usó en prácticas pasadas: https://www.cgtrader.com/free-3d-models/space/other/satellite-3d

15/oct/26 Reutilicé el modelo de la nave espacial que texturicé para la Práctica 8: https://sketchfab.com/3d-models/multi-universe-space-ship-3d-model-42a2cbc97d9c489b80c6533c44294d55

-----------------------------------------------------
