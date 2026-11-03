# Rover preparado para animacion

Abre `RoverJerarquizado.blend` en Blender. En el Outliner encontraras un objeto
vacio `Rover_Raiz` con siete hijos: cuerpo y seis ruedas. Cada rueda incluye
neumatico y rin; la suspension permanece en el cuerpo. El brazo permanece fijo.
Izquierda y derecha se definen mirando hacia el frente del rover (+X).

Una jerarquia hace que los hijos hereden la transformacion de su padre.
Mover o rotar `Rover_Raiz` mueve el conjunto. Rotar una rueda solo mueve esa rueda.
Separar geometria y definir padres son dos pasos distintos.

## En Blender

Selecciona una rueda y pulsa `R`, `Y`, `90`: girara sobre su propio centro.
Aqui Blender usa Z arriba y el eje de las ruedas es Y. Deshaz la prueba con Ctrl+Z.
Para animar, inserta claves de rotacion en distintos fotogramas.

## En tu programa OpenGL

OBJ no almacena padres, pivotes ni animaciones. Por eso hay un OBJ independiente
por pieza. Las ruedas estan centradas en (0,0,0); al cargarlas debes trasladarlas
al pivote indicado en `jerarquia.json`. El cuerpo conserva las coordenadas originales.
Los OBJ conservan la convencion original: X frente, Y arriba, Z derecha.
En estos archivos las ruedas giran alrededor de Z.

Ejemplo adaptado a `Model` y GLM (fragmentos para integrar en tu practica 9):

```cpp
// Declaraciones:
Model cuerpoRover;
Model ruedasRover[6];
const char* nombresRuedas[6] = {
    "rueda_delantera_izquierda", "rueda_delantera_derecha",
    "rueda_media_izquierda", "rueda_media_derecha",
    "rueda_trasera_izquierda", "rueda_trasera_derecha"
};
const glm::vec3 pivotesRuedas[6] = {
    { 5.709923f, 1.704993f, -3.070003f},
    { 5.712381f, 1.704992f,  3.070000f},
    {-0.947183f, 1.412098f, -4.158950f},
    {-0.944719f, 1.412099f,  4.158949f},
    {-5.113412f, 1.385755f, -4.158949f},
    {-5.130509f, 1.412099f,  4.158949f}
};
float anguloRuedas = 0.0f; // radianes

// Una sola vez, despues de crear el contexto OpenGL:
cuerpoRover.LoadModel("Models/RoverJerarquizado/cuerpo.obj");
for (int i = 0; i < 6; ++i)
    ruedasRover[i].LoadModel(std::string("Models/RoverJerarquizado/")
                            + nombresRuedas[i] + ".obj");

// En el render, usando la matriz padre existente de tu rover:
glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(matrizRover));
cuerpoRover.RenderModel();
for (int i = 0; i < 6; ++i) {
    glm::mat4 rueda = glm::translate(matrizRover, pivotesRuedas[i]);
    rueda = glm::rotate(rueda, anguloRuedas, glm::vec3(0, 0, 1));
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(rueda));
    ruedasRover[i].RenderModel();
}
```

La formula es `M_rueda = M_rover * T_pivote * R_giro`.
No hace falta trasladar de regreso por -pivote: los vertices exportados ya estan
centrados. Cada iteracion parte de la matriz del rover, no de la rueda anterior.
Reemplaza el dibujo del OBJ completo por estos dibujos para evitar duplicarlo.

Para rodar hacia +X, disminuye el angulo. Para ligar giro y avance:
`anguloRuedas -= distanciaAvanzada / radioMundial;`.
El radio local es aproximadamente 1.241; si mantienes escala uniforme 0.4,
el radio mundial es aproximadamente 0.4964. Usa distancia con signo y actualiza
una vez por fotograma. Las seis ruedas usan el mismo signo de giro.

## Comprobaciones y materiales

Se conservaron las 15921 caras: 8313 en el cuerpo y 1268 en cada rueda.
Los originales no fueron modificados. Se incluyo una vista previa del ensamblaje.
El modelo original tiene pequenas diferencias entre posiciones de las ruedas;
se conservaron en los pivotes.

Falta `SpacialRoverT301Texture.png`, referenciada por el material del brazo.
Los materiales y UV se conservaron, pero esa imagen no estaba disponible.
Ademas, el cargador actual `Model.cpp` usa texturas y no aplica el color Kd del
MTL a los materiales sin imagen. Por eso los colores de Blender pueden diferir
de OpenGL; esto ya sucede con el OBJ original.

La jerarquia de Blender y la geometria exportada fueron verificadas. El ejemplo
C++ es una guia de integracion; no se modifico ni compilo tu practica existente.
