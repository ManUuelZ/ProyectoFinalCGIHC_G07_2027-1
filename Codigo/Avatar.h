#pragma once
#include <cmath>
#include <cstdio>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <string>
#include <glew.h>
#include <glfw3.h>
#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>
#include "Model.h"
#include "Camera.h"
#include "EscenarioDebate.h"

// Avatar jerárquico: Torso (raíz) -> Cola, Brazos, Cinturón -> Barras (hijas del cinturón).
// Misma estructura que ValleHelado: iniciar() una vez, entrarEn() al cambiar de escenario,
// actualizar() cada frame y dibujar() dentro del escenario. Se usa en el escenario 2 (Valle) y 3 (Erid).
//
// Torso, cola, brazos y barra están centrados en su propio origen (LOCAL); el cinturón conserva su
// posición de Blender (EN MUNDO). Cada pieza se acomoda con su Location + un ajuste fino (AJUSTE).
//
// MODO CALIBRACIÓN (para ajustar piezas sin recompilar):
//   F12 = activar/desactivar     F1..F6 = elegir pieza (torso, cola, brazo izq, brazo der, cinturón, barras)
//   J / L = adelante / atrás     I / K = derecha / izquierda del avatar     U / O = arriba / abajo
//   Shift = movimiento fino      F11 = imprime en consola los valores de AJUSTE para pegarlos aquí
namespace Avatar {

	inline constexpr float ROT_FRENTE = -90.0f;  
	inline constexpr float ESCALA = 5.0f;        // unidades de Blender -> unidades de la escena
	inline constexpr float VELOCIDAD = 12.0f;    // unidades por segundo
	inline constexpr float PISO_ERID = -1.0f;    // altura del piso de Erid (escenario 3)
	inline constexpr float VELOCIDAD_VALLE = 20.0f;
	inline constexpr float RADIO_VALLE = 140.0f; 
	inline constexpr float ALTURA_VUELO = 3.5f;  // sube al dragón por encima de la arena de Erid

	// Ejes de animación en el espacio del modelo (con ROT_FRENTE = -90 el frente del dragón es -X del modelo)
	inline const glm::vec3 EJE_BRAZOS(0.0f, 0.0f, 1.0f);    // eje lateral: los brazos se balancean adelante/atrás
	inline const glm::vec3 EJE_COLA(0.0f, 1.0f, 0.0f);      // la cola se mueve de lado a lado
	inline const glm::vec3 EJE_CINTURON(0.0f, 1.0f, 0.0f);  // el cinturón gira sobre su eje vertical
	inline constexpr float GIRO_CINTURON = 0.0f;           // grados por segundo (0 = sin giro)
	inline constexpr float RADIO_BARRAS = 0.17f;            // radio del círculo de barras alrededor del cinturón

	inline Model torso, cola, brazoIzq, brazoDer, cinturon, barra;

	// Índices de las piezas
	enum { P_TORSO = 0, P_COLA, P_BI, P_BD, P_CINT, P_BARRA };
	inline const char* NOMBRES[6] = { "Torso", "Cola", "BrazoIzq", "BrazoDer", "Cinturon", "Barras" };

	// Ubicaciones (Location) tomadas de Blender: X, Y, Z
	inline const glm::vec3 bTorso(0.81334f, -0.078945f, 0.5f);
	inline const glm::vec3 bCola(0.813844f, -0.078945f, 0.5f);
	inline const glm::vec3 bBrazoIzq(-0.407789f, -0.240608f, 0.883164f);
	inline const glm::vec3 bBrazoDer(-0.468425f, 0.433187f, 0.959671f);
	inline const glm::vec3 bCinturon(0.107795f, -0.026743f, 0.731601f);

	// ¿El .obj conserva su posición de Blender (true) o está centrado en su origen (false)?
	inline constexpr bool MUNDO_TORSO = false, MUNDO_COLA = false, MUNDO_BI = false,
		MUNDO_BD = false, MUNDO_CINT = true;

	// Ajuste fino por pieza (unidades del modelo; X = adelante(-)/atrás(+), Y = arriba, Z = derecha(-)/izquierda(+)).
	// Valores iniciales estimados con las medidas de tus .obj; afínalos con el modo calibración (F12).
	inline glm::vec3 AJUSTE[6] = {
		glm::vec3(0.0f, 0.0f, 0.0f),     // Torso
		glm::vec3(-0.6f, 0.25f, -0.1f),  // Cola: cierra el hueco con el torso
		glm::vec3(0.0f, 0.0f, -0.7f),    // BrazoIzq: lo acerca al costado izquierdo del torso
		glm::vec3(0.0f, 0.0f, 0.0f),     // BrazoDer
		glm::vec3(0.0f, 0.0f, 0.0f),     // Cinturon
		glm::vec3(0.0f, 0.0f, 0.0f)      // Barras
	};

	// Se calculan en iniciar() leyendo los .obj
	inline glm::vec3 anclaje(0.0f);          // centro del torso (espacio local) para centrar el avatar en 'pos'
	inline glm::vec3 jCint = glm::vec3(0.107795f, 0.731601f, 0.026743f); // pivote del cinturón (centro de su malla)
	inline glm::vec3 centroBarra(0.0f);      // centro de la malla de la barra

	// Estado del avatar
	inline glm::vec3 pos(0.0f, PISO_ERID, 40.0f);   // posición en el suelo (los pies)
	inline float yaw = 0.0f;                         // grados; 0 = mira hacia -Z
	inline float fase = 0.0f;                        // avanza al caminar (balanceo de brazos)
	inline float mov = 0.0f;                         // 0 = quieto, 1 = caminando (suavizado)
	inline float tiempo = 0.0f;
	inline int modo = 3;                             // escenario donde está: 2 = Valle helado, 3 = Erid + Debate

	// Calibración
	inline bool calibrar = false;
	inline int seleccion = P_COLA;
	inline bool previo[GLFW_KEY_LAST + 1] = {};

	// Blender (X, Y, Z) -> OpenGL (X, Z, -Y)
	inline glm::vec3 aGL(const glm::vec3& v) { return glm::vec3(v.x, v.z, -v.y); }

	// Llamar una vez al entrar a un escenario (2 o 3): lo coloca en su punto de inicio
	inline void entrarEn(int escenario) {
		modo = escenario;
		mov = 0.0f;
		yaw = 0.0f;
		if (modo == 2) pos = glm::vec3(0.0f, 0.0f, 30.0f);        // Valle helado: piso en y = 0
		else           pos = glm::vec3(0.0f, PISO_ERID, 40.0f);   // Erid: frente al estrado
	}

	// Caja que envuelve todos los vértices de un .obj (en las coordenadas del archivo)
	inline bool leerCaja(const char* ruta, glm::vec3& mn, glm::vec3& mx) {
		std::ifstream f(ruta);
		if (!f) return false;
		mn = glm::vec3(1e30f);
		mx = glm::vec3(-1e30f);
		bool hay = false;
		std::string linea;
		while (std::getline(f, linea)) {
			if (linea.size() > 2 && linea[0] == 'v' && linea[1] == ' ') {
				std::istringstream s(linea.substr(2));
				float x, y, z;
				if (s >> x >> y >> z) {
					const glm::vec3 v(x, y, z);
					mn = glm::min(mn, v);
					mx = glm::max(mx, v);
					hay = true;
				}
			}
		}
		return hay;
	}

	inline bool cargar(Model& m, const char* ruta) {
		m.LoadModel(ruta);
		if (!m.cargado()) {
			printf("Avatar: no se encontro el modelo %s\n", ruta);
			return false;
		}
		return true;
	}

	inline bool iniciar() {
		bool ok = true;
		ok &= cargar(torso, "Models/TorsoAvatar.obj");
		ok &= cargar(cola, "Models/ColaAvatar.obj");
		ok &= cargar(brazoIzq, "Models/BIAvatar.obj");
		ok &= cargar(brazoDer, "Models/BDAvatar.obj");
		ok &= cargar(cinturon, "Models/Torus.obj");
		ok &= cargar(barra, "Models/Barra.obj");

		glm::vec3 mn, mx;
		// Centro del torso: el avatar queda centrado sobre 'pos'
		anclaje = glm::vec3(0.0f);
		if (leerCaja("Models/TorsoAvatar.obj", mn, mx)) {
			glm::vec3 c = (mn + mx) * 0.5f;
			if (MUNDO_TORSO) c -= aGL(bTorso);
			anclaje = glm::vec3(c.x, 0.0f, c.z);
		}
		// El cinturón gira sobre el centro de su malla (su Location está fuera de centro)
		jCint = aGL(bCinturon);
		if (MUNDO_CINT && leerCaja("Models/Torus.obj", mn, mx)) jCint = (mn + mx) * 0.5f;
		// Centro de la barra, para repartirlas alrededor del cinturón
		centroBarra = glm::vec3(0.0f);
		if (leerCaja("Models/Barra.obj", mn, mx)) centroBarra = (mn + mx) * 0.5f;
		return ok;
	}

	// ---------- Modo calibración ----------
	inline bool teclaPulsada(int k) {
		GLFWwindow* w = glfwGetCurrentContext();
		return w && glfwGetKey(w, k) == GLFW_PRESS;
	}
	inline bool flanco(int k) {          // true solo en el frame en que se presiona
		const bool ahora = teclaPulsada(k);
		const bool r = ahora && !previo[k];
		previo[k] = ahora;
		return r;
	}
	inline void imprimirAjustes() {
		printf("---- Pega esto en Avatar.h ----\n");
		printf("inline glm::vec3 AJUSTE[6] = {\n");
		for (int i = 0; i < 6; i++)
			printf("\tglm::vec3(%.3ff, %.3ff, %.3ff)%s // %s\n",
				AJUSTE[i].x, AJUSTE[i].y, AJUSTE[i].z, i < 5 ? "," : "", NOMBRES[i]);
		printf("};\n-------------------------------\n");
	}
	inline void calibracion(float dt) {
		if (flanco(GLFW_KEY_F12)) {
			calibrar = !calibrar;
			printf("Avatar: calibracion %s (pieza: %s)\n", calibrar ? "ACTIVADA" : "desactivada", NOMBRES[seleccion]);
		}
		if (!calibrar) return;

		const int fk[6] = { GLFW_KEY_F1, GLFW_KEY_F2, GLFW_KEY_F3, GLFW_KEY_F4, GLFW_KEY_F5, GLFW_KEY_F6 };
		for (int i = 0; i < 6; i++)
			if (flanco(fk[i])) { seleccion = i; printf("Avatar: editando %s\n", NOMBRES[i]); }

		const float paso = (teclaPulsada(GLFW_KEY_LEFT_SHIFT) ? 0.05f : 0.4f) * dt;
		glm::vec3& a = AJUSTE[seleccion];
		if (teclaPulsada(GLFW_KEY_J)) a.x -= paso;   // adelante
		if (teclaPulsada(GLFW_KEY_L)) a.x += paso;   // atrás
		if (teclaPulsada(GLFW_KEY_I)) a.z -= paso;   // derecha del avatar
		if (teclaPulsada(GLFW_KEY_K)) a.z += paso;   // izquierda del avatar
		if (teclaPulsada(GLFW_KEY_U)) a.y += paso;   // arriba
		if (teclaPulsada(GLFW_KEY_O)) a.y -= paso;   // abajo
		if (flanco(GLFW_KEY_F11)) imprimirAjustes();
	}

	// teclas: el arreglo de teclas de la ventana (mainWindow.getsKeys()); dt en segundos
	template <class T>
	inline void actualizar(const T* teclas, float dt) {
		if (!(dt > 0.0f) || !std::isfinite(dt)) return;
		tiempo += dt;
		calibracion(dt);

		glm::vec3 dir(0.0f);
		if (teclas[GLFW_KEY_W]) dir.z -= 1.0f;
		if (teclas[GLFW_KEY_S]) dir.z += 1.0f;
		if (teclas[GLFW_KEY_A]) dir.x -= 1.0f;
		if (teclas[GLFW_KEY_D]) dir.x += 1.0f;

		bool caminando = glm::length(dir) > 0.0f;
		if (caminando) {
			dir = glm::normalize(dir);
			const float vel = (modo == 2) ? VELOCIDAD_VALLE : VELOCIDAD;
			pos.x += dir.x * vel * dt;
			pos.z += dir.z * vel * dt;
			// El avatar mira hacia -Z con yaw = 0
			yaw = glm::degrees(std::atan2(-dir.x, -dir.z));
			fase += 9.0f * dt;
		}
		// Suaviza la transición entre quieto y caminando
		float objetivo = caminando ? 1.0f : 0.0f;
		mov += (objetivo - mov) * (std::min)(1.0f, 8.0f * dt);

		if (modo == 2) {
			// Valle helado: piso plano, dentro del radio que permite la cámara del valle
			float r = glm::length(glm::vec2(pos.x, pos.z));
			if (r > RADIO_VALLE) { pos.x *= RADIO_VALLE / r; pos.z *= RADIO_VALLE / r; }
			pos.y = 0.0f;
		}
		else {
			// Erid: límites del escenario
			pos.x = std::clamp(pos.x, -95.0f, 95.0f);
			pos.z = std::clamp(pos.z, -205.0f, 95.0f);
			// Sube las escaleras del estrado y no se hunde bajo el piso de Erid
			pos.y = (std::max)(PISO_ERID, EscenarioDebate::ObtenerAlturaPiso(pos.x, pos.z));
		}
	}

	// Cámara en tercera persona: detrás (+Z) y arriba del avatar, mirando hacia él
	inline void seguirConCamara(Camera& cam) {
		glm::vec3 objetivo = pos + glm::vec3(0.0f, 4.0f, 0.0f);
		glm::vec3 camPos = pos + glm::vec3(0.0f, 11.0f, 24.0f);
		glm::vec3 d = glm::normalize(objetivo - camPos);
		cam = Camera(camPos, glm::vec3(0.0f, 1.0f, 0.0f),
			glm::degrees(std::atan2(d.z, d.x)), glm::degrees(std::asin(d.y)), 1.0f, 0.5f);
	}

	// Matriz final de una pieza a partir de su marco (ya colocado y rotado en su pivote J)
	inline glm::mat4 pieza(const glm::mat4& marco, const glm::vec3& J, bool enMundo) {
		// En mundo: los vértices traen la posición de Blender, se restan para girar sobre el pivote.
		// Local: los vértices ya están centrados en el pivote.
		return enMundo ? marco * glm::translate(glm::mat4(1.0f), -J) : marco;
	}

	inline void pintar(Model& m, const glm::mat4& matriz, GLuint uModel) {
		glUniformMatrix4fv(uModel, 1, GL_FALSE, glm::value_ptr(matriz));
		m.RenderModel();
	}

	inline void dibujar(GLuint uModel, GLuint uColor) {
		glUniform3f(uColor, 1.0f, 1.0f, 1.0f);

		const float serpenteo = 10.0f * std::sin(tiempo * 3.0f) * mov;
		const float flotacion = 0.15f * std::sin(tiempo * 2.0f) * ESCALA;

		const glm::vec3 jT = aGL(bTorso);
		const glm::vec3 jCola = aGL(bCola);
		const glm::vec3 jBI = aGL(bBrazoIzq);
		const glm::vec3 jBD = aGL(bBrazoDer);

		// --- RAÍZ: posición, giro y escala del avatar ---
		const glm::vec3 base = pos + glm::vec3(0.0f, flotacion + ALTURA_VUELO, 0.0f);
		glm::mat4 raiz(1.0f);
		raiz = glm::translate(raiz, base);
		raiz = glm::rotate(raiz, glm::radians(yaw + ROT_FRENTE + serpenteo), glm::vec3(0.0f, 1.0f, 0.0f));
		raiz = glm::scale(raiz, glm::vec3(ESCALA));
		raiz = glm::translate(raiz, -anclaje);   // el centro del torso queda en 'base'

		// Marco de una articulación: se coloca en el pivote de la pieza (relativo al torso) y rota ahí.
		auto marco = [&](int idx, const glm::vec3& J, float ang, const glm::vec3& eje) {
			return raiz
				* glm::translate(glm::mat4(1.0f), J - jT + AJUSTE[idx])
				* glm::rotate(glm::mat4(1.0f), ang, eje);
			};

		// --- NIVEL 0: TORSO (raíz) ---
		pintar(torso, pieza(marco(P_TORSO, jT, 0.0f, glm::vec3(0, 1, 0)), jT, MUNDO_TORSO), uModel);

		// --- NIVEL 1: COLA (se mueve de lado a lado) ---
		const float aleteo = glm::radians(-15.0f * std::cos(tiempo * 3.0f) * (0.4f + 0.6f * mov));
		pintar(cola, pieza(marco(P_COLA, jCola, aleteo, EJE_COLA), jCola, MUNDO_COLA), uModel);

		// --- NIVEL 1: BRAZOS (balanceo opuesto, sobre el eje lateral) ---
		const float swing = glm::radians(20.0f * std::cos(fase)) * mov;
		pintar(brazoIzq, pieza(marco(P_BI, jBI, swing, EJE_BRAZOS), jBI, MUNDO_BI), uModel);
		pintar(brazoDer, pieza(marco(P_BD, jBD, -swing, EJE_BRAZOS), jBD, MUNDO_BD), uModel);

		// --- NIVEL 1: CINTURÓN (hijo del torso, gira sobre el centro de su malla) ---
		const glm::mat4 marcoCint = marco(P_CINT, jCint, glm::radians(GIRO_CINTURON * tiempo), EJE_CINTURON);
		pintar(cinturon, pieza(marcoCint, jCint, MUNDO_CINT), uModel);

		// --- NIVEL 2: BARRAS (hijas del cinturón: 3 repartidas en círculo, giran con él) ---
		for (int i = 0; i < 3; i++) {
			const float a = glm::radians(120.0f * i);
			const glm::vec3 anillo(RADIO_BARRAS * std::cos(a), 0.0f, RADIO_BARRAS * std::sin(a));
			const glm::mat4 mBarra = marcoCint
				* glm::translate(glm::mat4(1.0f), anillo + AJUSTE[P_BARRA])
				* glm::translate(glm::mat4(1.0f), -centroBarra);   // centra la malla en su posición
			pintar(barra, mBarra, uModel);
		}
	}
}