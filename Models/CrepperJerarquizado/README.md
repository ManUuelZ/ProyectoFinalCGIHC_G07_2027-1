# Crepper: jerarquia para animacion

Derivado de `../crepper.obj`, sin recortar ni simplificar geometria: 31 piezas
y 47 952 caras. Se conservan normales, UV, materiales y orden de las caras.
El original permanece ensamblado; cada OBJ de esta carpeta esta centrado en
su propio pivote. Cargar las piezas sin sus transformaciones las desarma.

## Arbol

```text
Crepper_Raiz                         posicion, orientacion y escala global
└── cuerpo_inferior                  balanceo del cuerpo
    ├── cabeza                      giro desde la union posterior
    │   ├── ojo_izquierda / ojo_derecha
    │   ├── apendice_exterior_izquierda / derecha
    │   └── apendice_central_izquierda / derecha
    ├── borde_frontal
    ├── placa_01 ... placa_07        placas independientes
    ├── cola                        pivote anterior
    ├── pata_izquierda_1 ... 7       pivotes superiores
    └── pata_derecha_1 ... 7
```

Los nombres izquierda/derecha se conservan del archivo fuente. En OBJ:
Y arriba, frente -Z, las piezas llamadas izquierda estan en -X. En Blender
la conversion es `(x, y, z) OBJ -> (x, -z, y)`; Z queda arriba.
La raiz esta en el origen original, cerca del suelo.

## Uso desde C++

`LabCGEHC/CrepperDatos.h` contiene rutas, indices de padres y desplazamientos
locales. Los padres siempre aparecen antes que sus hijos.

1. Cargar un `Model` por cada nodo con `archivo != nullptr`.
2. Inicializar **cada** matriz de `CrepperDatos::Matrices rotaciones` con
   `glm::mat4(1.0f)` (una matriz por pieza; la inicializacion por defecto no basta).
3. Llamar `CrepperDatos::calcularMatrices(mundo, rotaciones)` cada frame.
4. Enviar la matriz resultante de cada nodo al shader y dibujar su modelo.

La formula es `M_hijo = M_padre * T(desplazamiento_local) * R_hijo`.
Las coordenadas del OBJ ya tienen restado el pivote: no restarlo de nuevo.
La escala global se aplica en `mundo`, una sola vez.
Para empezar a caminar se puede balancear cada pata alrededor de X y alternar
su fase; Z permite levantarla lateralmente. Los ojos heredan la cabeza.

Es una jerarquia de piezas rigidas, sin esqueleto deformable. Los pivotes son
aproximaciones geometricas a las uniones, editables en `preparar_crepper.py`.
Las patas son piezas completas: no tienen rodillas separadas. Giros grandes
pueden abrir huecos entre piezas; empezar con angulos pequenos. Las placas
cuelgan del cuerpo y no forman una cadena de deformacion.

El MTL conserva los colores originales. El cargador `Model.cpp` aplica ahora
`Kd` mediante una textura de color cuando el material no aporta una imagen.
Los colores estan disponibles tanto en OpenGL como en Blender.

## Archivos y regeneracion

- `CrepperJerarquizado.blend`: jerarquia editable, pivotes y materiales.
- `jerarquia.json`: pivotes absolutos OBJ y traslaciones relativas al padre.
- `vista_previa.png`: ensamblaje en reposo.
- `*.obj` y `crepper.mtl`: piezas para Assimp.

Desde la raiz, ejecutar `python preparar_crepper.py`; despues
`blender --background --python crear_escena_crepper.py`.
Los scripts verifican caras conservadas, reconstruccion de vertices y limites
de cada pieza reimportada. La integracion del equipo coloca la criatura en el
escenario helado, en reposo y sin animacion de marcha.
