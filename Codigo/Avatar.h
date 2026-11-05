#pragma once
#include <cmath>
#include <cstdio>
#include <algorithm>
#include <glew.h>
#include <glfw3.h>
#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>
#include "Model.h"
#include "Camera.h"
#include "EscenarioDebate.h"

// Avatar jerárquico: Torso (raíz) -> Cola, Brazos, Cinturón -> Barras (hijos del cinturón).
// Misma estructura que ValleHelado: iniciar() una vez, entrarEn() al cambiar de escenario,
// actualizar() cada frame y dibujar() dentro del escenario. Se usa en el escenario 2 (Valle) y 3 (Erid).
namespace Avatar {

	// false : cada .obj tiene su origen en su Location de Blender (por eso 'barra.obj' se repite 3 veces
	//         con distinto offset). Se acomoda con los offsets de Blender. (valor para tu modelo)
	// true  : cada .obj conserva la posición que tenía en Blender (el torso ya trae su altura).
	//         Cada parte rota alrededor de su propio pivote.
	// Prueba: mira el avatar quieto. Si sale armado, está bien. Si sale hundido en el piso o las piezas
	// salen amontonadas, cambia este valor.
	inline constexpr bool PARTES_EN_MUNDO = false;

	inline constexpr float ESCALA = 5.0f;        // unidades de Blender -> unidades de la escena
	inline constexpr float VELOCIDAD = 12.0f;    // unidades por segundo
	inline constexpr float PISO_ERID = -1.0f;    // altura del piso de Erid (escenario 3)
	inline constexpr float VELOCIDAD_VALLE = 20.0f;
	inline constexpr float RADIO_VALLE = 140.0f; // la cámara del valle se limita a 170; se deja margen para seguirlo

	inline Model torso, cola, brazoIzq, brazoDer, cinturon, barra;

	// Ubicaciones (Location) tomadas de Blender: X, Y, Z
	inline const glm::vec3 bTorso(-0.025f, -0.148f, 1.651f);
	inline const glm::vec3 bCola(-0.015f, 0.254f, 1.213f);
	inline const glm::vec3 bBrazoIzq(-0.252f, -0.214f, 1.771f);
	inline const glm::vec3 bBrazoDer(0.144f, -0.351f, 1.737f);
	inline const glm::vec3 bCinturon(-0.008f, 0.239f, 1.246f);
	inline const glm::vec3 bBarras[3] = {
		glm::vec3(-0.210f, 0.181f, 1.239f), // Barra 1
		glm::vec3(-0.258f, 0.236f, 1.258f), // Barra 2
		glm::vec3(0.213f, 0.264f, 1.268f)   // Barra 3
	};

	// Convierte la resta de Blender (X, Y, Z) a ejes de OpenGL (X, Z, -Y)
	inline glm::vec3 OffsetGL(glm::vec3 hijo, glm::vec3 padre) {
		glm::vec3 d = hijo - padre;
		return glm::vec3(d.x, d.z, -d.y);
	}

	// Estado del avatar
	inline glm::vec3 pos(0.0f, PISO_ERID, 40.0f);   // posición en el suelo (los pies)
	inline float yaw = 0.0f;                         // grados; 0 = mira hacia -Z
	inline float fase = 0.0f;                        // avanza al caminar (balanceo de brazos)
	inline float mov = 0.0f;                         // 0 = quieto, 1 = caminando (suavizado)
	inline float tiempo = 0.0f;
	inline int modo = 3;                             // escenario donde está: 2 = Valle helado, 3 = Erid + Debate

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
		return ok;
	}

	// teclas: el arreglo de teclas de la ventana (mainWindow.getsKeys()); dt en segundos
	template <class T>
	inline void actualizar(const T* teclas, float dt) {
		if (!(dt > 0.0f) || !std::isfinite(dt)) return;
		tiempo += dt;

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

	// Matriz de una parte hija. 'rot' es su rotación local (animación).
	inline glm::mat4 parte(const glm::mat4& padre, const glm::vec3& hijoB, const glm::vec3& padreB, const glm::mat4& rot) {
		if (PARTES_EN_MUNDO) {
			glm::vec3 p = aGL(hijoB);   // pivote de la parte
			return padre * glm::translate(glm::mat4(1.0f), p) * rot * glm::translate(glm::mat4(1.0f), -p);
		}
		return padre * glm::translate(glm::mat4(1.0f), aGL(hijoB - padreB)) * rot;
	}

	inline void pintar(Model& m, const glm::mat4& matriz, GLuint uModel) {
		glUniformMatrix4fv(uModel, 1, GL_FALSE, glm::value_ptr(matriz));
		m.RenderModel();
	}

	inline void dibujar(GLuint uModel, GLuint uColor) {
		glUniform3f(uColor, 1.0f, 1.0f, 1.0f);

		const glm::mat4 I(1.0f);
		const float serpenteo = 10.0f * std::sin(tiempo * 3.0f) * mov;     // balanceo al caminar
		const float flotacion = 0.15f * std::sin(tiempo * 2.0f) * ESCALA;  // flotación suave

		// --- NIVEL 0: TORSO (raíz) ---
		glm::vec3 base = pos + glm::vec3(0.0f, flotacion, 0.0f);
		if (!PARTES_EN_MUNDO) base.y += aGL(bTorso).y * ESCALA;  // el origen del torso está a media altura
		glm::mat4 mTorso = glm::translate(I, base);
		mTorso = glm::rotate(mTorso, glm::radians(yaw + serpenteo), glm::vec3(0.0f, 1.0f, 0.0f));
		mTorso = glm::scale(mTorso, glm::vec3(ESCALA));
		pintar(torso, mTorso, uModel);

		// --- NIVEL 1: COLA ---
		float aleteo = glm::radians(-15.0f * std::cos(tiempo * 3.0f) * (0.4f + 0.6f * mov));
		pintar(cola, parte(mTorso, bCola, bTorso, glm::rotate(I, aleteo, glm::vec3(0.0f, 1.0f, 0.0f))), uModel);

		// --- NIVEL 1: BRAZOS (se balancean en sentidos opuestos al caminar) ---
		float swing = glm::radians(20.0f * std::cos(fase)) * mov;
		pintar(brazoIzq, parte(mTorso, bBrazoIzq, bTorso, glm::rotate(I, swing, glm::vec3(1.0f, 0.0f, 0.0f))), uModel);
		pintar(brazoDer, parte(mTorso, bBrazoDer, bTorso, glm::rotate(I, -swing, glm::vec3(1.0f, 0.0f, 0.0f))), uModel);

		// --- NIVEL 1: CINTURÓN ---
		glm::mat4 mCinturon = parte(mTorso, bCinturon, bTorso, I);
		pintar(cinturon, mCinturon, uModel);

		// --- NIVEL 2: 3 BARRAS (hijos del cinturón) ---
		for (int i = 0; i < 3; i++)
			pintar(barra, parte(mCinturon, bBarras[i], bCinturon, I), uModel);
	}
}