#pragma once

#include <vector>
#include <cmath>
#include <glew.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include "Mesh_tn.h"
#include "Texture.h"
#include "Material.h"
#include "SpotLight.h"
#include "Model.h"

class EscenarioDebate {
public:
	// Escala aumentada al doble para proporción monumental
	static constexpr float Y_BASE = -2.0f;
	static constexpr float Z_INICIO_ESCALERAS = -120.0f;
	static constexpr float ANCHO_ESCENARIO = 100.0f;  // Antes 50.0f
	static constexpr float ALTO_ESCALON = 1.2f;       // Antes 0.6f
	static constexpr float PROF_ESCALON = 3.0f;       // Antes 1.5f
	static constexpr int NUM_ESCALONES = 8;

	// Guarda el índice del SpotLight del rayo abductor para moverlo con la nave
	static int& IndiceSpotRayo() {
		static int idx = -1;
		return idx;
	}

	static MeshModel* ObtenerCubo() {
		static MeshModel* cubo = nullptr;
		if (!cubo) {
			GLfloat v[] = {
				// Frente (+Z)
				-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  0.0f,  0.0f,  1.0f,
				 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  0.0f,  0.0f,  1.0f,
				 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,  0.0f,  0.0f,  1.0f,
				-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,  0.0f,  0.0f,  1.0f,
				// Atrás (-Z)
				 0.5f, -0.5f, -0.5f,  0.0f, 0.0f,  0.0f,  0.0f, -1.0f,
				-0.5f, -0.5f, -0.5f,  1.0f, 0.0f,  0.0f,  0.0f, -1.0f,
				-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  0.0f,  0.0f, -1.0f,
				 0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  0.0f,  0.0f, -1.0f,
				 // Derecha (+X)
				  0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  1.0f,  0.0f,  0.0f,
				  0.5f, -0.5f, -0.5f,  1.0f, 0.0f,  1.0f,  0.0f,  0.0f,
				  0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  1.0f,  0.0f,  0.0f,
				  0.5f,  0.5f,  0.5f,  0.0f, 1.0f,  1.0f,  0.0f,  0.0f,
				  // Izquierda (-X)
				  -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, -1.0f,  0.0f,  0.0f,
				  -0.5f, -0.5f,  0.5f,  1.0f, 0.0f, -1.0f,  0.0f,  0.0f,
				  -0.5f,  0.5f,  0.5f,  1.0f, 1.0f, -1.0f,  0.0f,  0.0f,
				  -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, -1.0f,  0.0f,  0.0f,
				  // Arriba (+Y)
				  -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,  0.0f,  1.0f,  0.0f,
				   0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  0.0f,  1.0f,  0.0f,
				   0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  0.0f,  1.0f,  0.0f,
				  -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  0.0f,  1.0f,  0.0f,
				  // Abajo (-Y)
				  -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,  0.0f, -1.0f,  0.0f,
				   0.5f, -0.5f, -0.5f,  1.0f, 0.0f,  0.0f, -1.0f,  0.0f,
				   0.5f, -0.5f,  0.5f,  1.0f, 1.0f,  0.0f, -1.0f,  0.0f,
				  -0.5f, -0.5f,  0.5f,  0.0f, 1.0f,  0.0f, -1.0f,  0.0f
			};
			unsigned int idx[] = {
				0,  1,  2,   0,  2,  3,
				4,  5,  6,   4,  6,  7,
				8,  9, 10,   8, 10, 11,
				12, 13, 14,  12, 14, 15,
				16, 17, 18,  16, 18, 19,
				20, 21, 22,  20, 22, 23
			};
			cubo = new MeshModel();
			cubo->CreateMeshModel(v, idx, 24 * 8, 36);
		}
		return cubo;
	}

	// Genera una malla cónica truncada para el haz de luz verde del rayo abductor
	static MeshModel* ObtenerConoAbductor() {
		static MeshModel* cono = nullptr;
		if (!cono) {
			const int SEG = 24;
			const float rSup = 0.25f; // Radio arriba (bajo la nave)
			const float rInf = 1.0f;  // Radio abajo (en el estrado)
			std::vector<GLfloat> v;
			std::vector<unsigned int> idx;

			for (int i = 0; i <= SEG; i++) {
				float ang = (2.0f * 3.14159265f * i) / SEG;
				float c = cos(ang), s = sin(ang);

				// Vértice superior (Y = +0.5)
				v.push_back(rSup * c); v.push_back(0.5f); v.push_back(rSup * s);
				v.push_back((float)i / SEG); v.push_back(1.0f);
				v.push_back(c); v.push_back(0.2f); v.push_back(s);

				// Vértice inferior (Y = -0.5)
				v.push_back(rInf * c); v.push_back(-0.5f); v.push_back(rInf * s);
				v.push_back((float)i / SEG); v.push_back(0.0f);
				v.push_back(c); v.push_back(0.2f); v.push_back(s);
			}
			for (int i = 0; i < SEG; i++) {
				int b = i * 2;
				idx.push_back(b);     idx.push_back(b + 1); idx.push_back(b + 2);
				idx.push_back(b + 1); idx.push_back(b + 3); idx.push_back(b + 2);
			}
			cono = new MeshModel();
			cono->CreateMeshModel(v.data(), idx.data(), (unsigned int)v.size(), (unsigned int)idx.size());
		}
		return cono;
	}

	// 1. Altura del piso escalada a las nuevas dimensiones
	static float ObtenerAlturaPiso(float x, float z) {
		if (x >= -50.0f && x <= 50.0f) {
			float zFinEscaleras = Z_INICIO_ESCALERAS - (NUM_ESCALONES * PROF_ESCALON); // -144.0f
			if (z <= Z_INICIO_ESCALERAS && z > zFinEscaleras) {
				int escalon = (int)((Z_INICIO_ESCALERAS - z) / PROF_ESCALON) + 1;
				if (escalon > NUM_ESCALONES) escalon = NUM_ESCALONES;
				return Y_BASE + (escalon * ALTO_ESCALON);
			}
			if (z <= zFinEscaleras && z >= -210.0f) {
				return Y_BASE + (NUM_ESCALONES * ALTO_ESCALON); // Y_ESTRADO = 7.6f
			}
		}
		return Y_BASE;
	}

	// 2. Registro de los 2 reflectores blancos + el SpotLight verde del Rayo Abductor
	static void AgregarSpotLights(SpotLight* arregloSpotLights, unsigned int& spotLightCount) {
		const float Y_ESTRADO = Y_BASE + (NUM_ESCALONES * ALTO_ESCALON); // 7.6f

		// Reflector Izquierdo (apunta a columnas izquierdas y podio de Kang)
		if (spotLightCount < MAX_SPOT_LIGHTS) {
			arregloSpotLights[spotLightCount] = SpotLight(
				0.85f, 0.90f, 1.0f,
				0.5f, 3.0f,
				-18.0f, Y_ESTRADO + 0.5f, -146.0f,
				0.0f, 0.65f, -0.75f,
				1.0f, 0.004f, 0.0001f,
				32.0f
			);
			spotLightCount++;
		}
		// Reflector Derecho (apunta a columnas derechas y podio de Kodos)
		if (spotLightCount < MAX_SPOT_LIGHTS) {
			arregloSpotLights[spotLightCount] = SpotLight(
				0.85f, 0.90f, 1.0f,
				0.5f, 3.0f,
				18.0f, Y_ESTRADO + 0.5f, -146.0f,
				0.0f, 0.65f, -0.75f,
				1.0f, 0.004f, 0.0001f,
				32.0f
			);
			spotLightCount++;
		}
		// SpotLight 3: RAYO ABDUCTOR VERDE (Apunta verticalmente hacia abajo desde la nave)
		if (spotLightCount < MAX_SPOT_LIGHTS) {
			IndiceSpotRayo() = (int)spotLightCount;
			arregloSpotLights[spotLightCount] = SpotLight(
				0.15f, 1.0f, 0.25f,             // Color verde alienígena intenso
				1.0f, 4.5f,                     // Alta intensidad para bañar el centro del estrado
				0.0f, Y_BASE + 78.0f, -158.0f,  // Posición inicial bajo la nave
				0.0f, -1.0f, 0.0f,              // Dirección: totalmente hacia abajo (-Y)
				1.0f, 0.002f, 0.00005f,         // Alcance largo hasta el piso del estrado
				22.0f                           // Ángulo del cono abductor
			);
			spotLightCount++;
		}
	}

	// Actualiza la posición de la luz del rayo abductor en cada frame según la levitación
	static void ActualizarLuzRayo(SpotLight* arregloSpotLights, float tiempoSegundos) {
		int idx = IndiceSpotRayo();
		if (idx >= 0) {
			float offsetLevitacion = 3.5f * sin(tiempoSegundos * 2.2f);
			glm::vec3 posLuzNave(0.0f, Y_BASE + 78.0f + offsetLevitacion, -158.0f);
			arregloSpotLights[idx].SetFlash(posLuzNave, glm::vec3(0.0f, -1.0f, 0.0f));
		}
	}

	// 3. Dibujado de la estructura al doble de tamaño
	static void DibujarEscenario(
		GLuint uniformModel,
		GLuint uniformColor,
		GLuint uniformSpecularIntensity,
		GLuint uniformShininess,
		Texture& texBlancaBase,
		Material& matOpaco
	) {
		MeshModel* cuboMesh = ObtenerCubo();
		glm::mat4 model(1.0f);
		matOpaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		texBlancaBase.UseTexture();

		const float Y_ESTRADO = Y_BASE + (NUM_ESCALONES * ALTO_ESCALON); // 7.6f

		// A. Escalinata monumental de 8 peldaños (Ancho = 100, Alto = 9.6 total, Prof = 24 total)
		glm::vec3 colorPiedra(0.68f, 0.68f, 0.72f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(colorPiedra));
		for (int i = 0; i < NUM_ESCALONES; i++) {
			float yPos = Y_BASE + (i * ALTO_ESCALON) + (ALTO_ESCALON * 0.5f);
			float zPos = Z_INICIO_ESCALERAS - (i * PROF_ESCALON) - (PROF_ESCALON * 0.5f);

			model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(0.0f, yPos, zPos));
			model = glm::scale(model, glm::vec3(ANCHO_ESCENARIO, ALTO_ESCALON, PROF_ESCALON));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			cuboMesh->RenderMeshModel();
		}

		// B. Explanada superior (Desde Z = -144 hasta Z = -210)
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, Y_BASE + 4.8f, -177.0f));
		model = glm::scale(model, glm::vec3(ANCHO_ESCENARIO, 9.6f, 66.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		cuboMesh->RenderMeshModel();

		// C. 6 Pilares gigantes (Altura de 36 unidades) + Cornisa
		glm::vec3 colorMarmol(0.96f, 0.95f, 0.88f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(colorMarmol));
		float posX[6] = { -40.0f, -24.0f, -8.0f, 8.0f, 24.0f, 40.0f };
		for (int i = 0; i < 6; i++) {
			// Base del pilar
			model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(posX[i], Y_ESTRADO + 2.4f, -168.0f));
			model = glm::scale(model, glm::vec3(6.4f, 4.8f, 6.4f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			cuboMesh->RenderMeshModel();

			// Fuste (Columna de 36 unidades de alto)
			model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(posX[i], Y_ESTRADO + 22.8f, -168.0f));
			model = glm::scale(model, glm::vec3(4.4f, 36.0f, 4.4f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			cuboMesh->RenderMeshModel();

			// Capitel
			model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(posX[i], Y_ESTRADO + 42.0f, -168.0f));
			model = glm::scale(model, glm::vec3(6.8f, 2.4f, 6.8f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			cuboMesh->RenderMeshModel();
		}

		// D. Cornisa superior
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, Y_ESTRADO + 46.0f, -168.0f));
		model = glm::scale(model, glm::vec3(ANCHO_ESCENARIO, 5.6f, 10.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		cuboMesh->RenderMeshModel();

		// E. Pared de fondo Azul Lavanda (100 de ancho x 48 de alto)
		glm::vec3 colorAzulDebate(0.54f, 0.51f, 0.78f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(colorAzulDebate));
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, Y_ESTRADO + 24.0f, -198.0f));
		model = glm::scale(model, glm::vec3(ANCHO_ESCENARIO, 48.0f, 2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		cuboMesh->RenderMeshModel();

		glm::vec3 blanco(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(blanco));
	}

	// 4. Dibujado de Podios, Kang, Kodos, Nave Gigante y Haz del Rayo Abductor
	static void DibujarModelosDebate(
		GLuint uniformModel,
		GLuint uniformColor,
		GLuint uniformSpecularIntensity,
		GLuint uniformShininess,
		Model& podioModel,
		Model& kangModel,
		Model& kodosModel,
		Model& naveModel,
		Texture& texBlancaBase,
		Material& matBrillante,
		float tiempoSegundos
	) {
		glm::mat4 model(1.0f);
		glm::vec3 blanco(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(blanco));

		const float Y_ESTRADO = Y_BASE + (NUM_ESCALONES * ALTO_ESCALON); // 7.6f

		// --- A. PODIO IZQUIERDO (KANG) - Escala aumentada ---
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-16.0f, Y_ESTRADO, -152.0f));
		model = glm::scale(model, glm::vec3(3.5f, 3.5f, 3.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		podioModel.RenderModel();

		// --- B. PODIO DERECHO (KODOS) ---
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(16.0f, Y_ESTRADO, -152.0f));
		model = glm::scale(model, glm::vec3(3.5f, 3.5f, 3.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		podioModel.RenderModel();

		// --- C. ALIEN 1: KANG ---
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-16.0f, Y_ESTRADO, -156.5f));
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		kangModel.RenderModel();

		// --- D. ALIEN 2: KODOS ---
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(16.0f, Y_ESTRADO, -156.5f));
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		kodosModel.RenderModel();

		// --- E. NAVE NODRIZA LEVITANDO ---
		float offsetLevitacion = 3.5f * sin(tiempoSegundos * 2.2f);
		float giroNave = tiempoSegundos * 35.0f * 0.01745329f;
		float yNaveActual = Y_BASE + 80.0f + offsetLevitacion;

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, yNaveActual, -158.0f));
		model = glm::rotate(model, giroNave, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, 0.08f * sin(tiempoSegundos * 1.5f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(7.5f, 7.5f, 7.5f)); // Escala gigante
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		naveModel.RenderModel();

		// --- F. CONO VOLUMÉTRICO DEL RAYO ABDUCTOR (Semitransparente) ---
		// Se extiende desde la base de la nave (yNaveActual - 2.0f) hasta el piso del estrado (Y_ESTRADO)
		float alturaRayo = (yNaveActual - 2.0f) - Y_ESTRADO;
		float yCentroRayo = Y_ESTRADO + (alturaRayo * 0.5f);
		float pulsoRadio = 14.0f + 1.5f * sin(tiempoSegundos * 6.0f); // Efecto pulsante

		glEnable(GL_BLEND);
		// Blending aditivo para que el rayo brille como energía láser verde
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);
		glDepthMask(GL_FALSE);

		texBlancaBase.UseTexture();
		matBrillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		glm::vec3 colorRayoVerde(0.15f, 0.95f, 0.30f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(colorRayoVerde));

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, yCentroRayo, -158.0f));
		model = glm::rotate(model, -giroNave * 1.5f, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(pulsoRadio, alturaRayo, pulsoRadio));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		ObtenerConoAbductor()->RenderMeshModel();

		// Restaurar estado de OpenGL
		glDepthMask(GL_TRUE);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDisable(GL_BLEND);
		glUniform3fv(uniformColor, 1, glm::value_ptr(blanco));
	}
};