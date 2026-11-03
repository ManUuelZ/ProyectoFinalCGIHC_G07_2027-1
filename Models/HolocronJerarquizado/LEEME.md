# Holocron jerarquizado

El original `Models/Holocron.obj` se conserva. `preparar_holocron.py` agrupa
componentes conectados completos: cuerpo y ocho esquinas con sus adornos.
Se conservan las 70209 caras, las UV y las normales. No hay cortes nuevos.
`jerarquia.json` registra el centro original, pivotes y numero de caras.

Los vertices de cada esquina son locales a su pivote. La reconstruccion es
`M_raiz * T(pivote)`. Durante la animacion se aplica
`M_raiz * R_orbita * T(pivote + desplazamiento) * R_local`.
Las piezas vuelven exactamente a su transformacion de reposo al cerrar.

La malla original contiene pequenos detalles y un ligero desnivel propios
del archivo; cerrar reconstruye ese mismo cubo, sin reemplazar su geometria.

El MTL original referencia texturas ausentes de otra computadora. Se usa
color azul/dorado provisional, no las texturas originales. El archivo
`HolocronJerarquizado.blend` permite editar las piezas con un padre comun.
`vista_cerrado.png` y `vista_abierto.png` son vistas de inspeccion en Blender.

## Animacion en practica9full.cpp

Las ecuaciones y los pivotes estan integrados en `practica9full.cpp`.
Tambien se integro el vuelo y humo del avion, sin depender de sus
encabezados de animacion. Tiempo real en segundos, independiente de los FPS.
Para entregar esta actualizacion, incluir el main y esta carpeta; se
requieren las bibliotecas, clases, shaders, texturas y otros modelos del
proyecto base (incluido `Models/cohete.obj` y su MTL).

- 0 a 2 s: cubo cerrado.
- 2 a 4 s: apertura radial suave.
- 4 a 8 s: las esquinas orbitan el cuerpo y giran sobre su propio eje.
  Las superiores e inferiores orbitan en sentidos contrarios.
- 8 a 10 s: cierre suave.
- 10 a 12 s: pausa cerrado, seguida del siguiente ciclo.

La interpolacion es `s(u)=6u^5-15u^4+10u^3`, con velocidad y aceleracion
nulas en sus extremos. Como no se proporciono el GIF, este movimiento
interpreta la descripcion del ejercicio.

## Cohete

Se usa `Models/cohete.obj`, cuyo eje longitudinal es +Y. Un ciclo de 32 s:
2 s de espera, 6 s de ascenso, 16 s de trayectoria, 6 s de descenso y 2 s
de espera. El punto de salida es (-45,-2,15). En el tramo curvo, respecto a
ese punto: `x=65(cos(a)-1)`, `y=60+20(1-cos(a))`, `z=40 sin(a)`;
`a=2*pi*s(u)`. La inclinacion sigue la tangente con transiciones suaves.
El regreso permite repetir la demostracion sin teletransportar el modelo.

Ejecutar desde la raiz `compilar_practica9.cmd`, `ejecutar_practica9.cmd`.
`probar_holocron_cohete.cmd` verifica cierre, continuidad, periodicidad,
importacion de piezas y conservacion de triangulos.
