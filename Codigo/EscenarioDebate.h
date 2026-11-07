#pragma once

#include <vector>
#include <cmath>
#include <glew.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include "CommonValues.h"   // MAX_SPOT_LIGHTS
#include "Mesh_tn.h"
#include "Texture.h"
#include "Material.h"
#include "SpotLight.h"
#include "Model.h"

class EscenarioDebate {
public:
	static constexpr float Y_BASE = -2.0f;
	static constexpr float Z_INICIO_ESCALERAS = -120.0f;
	static constexpr float ANCHO_ESCENARIO = 100.0f;
	static constexpr float ALTO_ESCALON = 1.2f;
	static constexpr float PROF_ESCALON = 3.0f;
	static constexpr int NUM_ESCALONES = 8;

	static int& IndiceLuzIzq() { static int idx = -1; return idx; }
	static int& IndiceLuzDer() { static int idx = -1; return idx; }
	static int& IndiceSpotRayo() { static int idx = -1; return idx; }

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

	// Cono truncado para el haz del rayo abductor
	static MeshModel* ObtenerConoAbductor() {
		static MeshModel* cono = nullptr;
		if (!cono) {
			const int SEG = 24;
			const float rSup = 0.25f;
			const float rInf = 1.0f;
			std::vector<GLfloat> v;
			std::vector<unsigned int> idx;

			for (int i = 0; i <= SEG; i++) {
				float ang = (2.0f * 3.14159265f * i) / SEG;
				float c = cos(ang), s = sin(ang);

				v.push_back(rSup * c); v.push_back(0.5f); v.push_back(rSup * s);
				v.push_back((float)i / SEG); v.push_back(1.0f);
				v.push_back(c); v.push_back(0.2f); v.push_back(s);

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

	// Altura del piso (por si otro archivo, como Avatar.h, la consulta)
	static float ObtenerAlturaPiso(float x, float z) {
		if (x >= -50.0f && x <= 50.0f) {
			float zFinEscaleras = Z_INICIO_ESCALERAS - (NUM_ESCALONES * PROF_ESCALON); // -144.0f
			if (z <= Z_INICIO_ESCALERAS && z > zFinEscaleras) {
				int escalon = (int)((Z_INICIO_ESCALERAS - z) / PROF_ESCALON) + 1;
				if (escalon > NUM_ESCALONES) escalon = NUM_ESCALONES;
				return Y_BASE + (escalon * ALTO_ESCALON);
			}
			if (z <= zFinEscaleras && z >= -210.0f) {
				return Y_BASE + (NUM_ESCALONES * ALTO_ESCALON);
			}
		}
		return Y_BASE;
	}

	static void AgregarSpotLights(SpotLight* arregloSpotLights, unsigned int& spotLightCount) {
		const float Y_ESTRADO = Y_BASE + (NUM_ESCALONES * ALTO_ESCALON);

		if (spotLightCount < MAX_SPOT_LIGHTS) {
			IndiceLuzIzq() = spotLightCount;
			arregloSpotLights[spotLightCount++] = SpotLight(
				0.85f, 0.90f, 1.0f, 0.5f, 3.0f,
				-18.0f, Y_ESTRADO + 2.5f, -146.0f, 0.0f, 0.65f, -0.75f, 1.0f, 0.004f, 0.0001f, 32.0f);
		}
		if (spotLightCount < MAX_SPOT_LIGHTS) {
			IndiceLuzDer() = spotLightCount;
			arregloSpotLights[spotLightCount++] = SpotLight(
				0.85f, 0.90f, 1.0f, 0.5f, 3.0f,
				18.0f, Y_ESTRADO + 2.5f, -146.0f, 0.0f, 0.65f, -0.75f, 1.0f, 0.004f, 0.0001f, 32.0f);
		}
		if (spotLightCount < MAX_SPOT_LIGHTS) {
			IndiceSpotRayo() = spotLightCount;
			arregloSpotLights[spotLightCount++] = SpotLight(
				0.0f, 0.0f, 0.0f, 1.0f, 4.5f,   // inicia apagado
				0.0f, Y_BASE + 78.0f, -158.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.002f, 0.00005f, 22.0f);
		}
	}

	// Tecla L: enciende/apaga los reflectores (rayoOn se conserva solo por compatibilidad con main)
	static void ActualizarLucesDinamicas(SpotLight* arregloSpotLights, float tiempoSegundos, bool focosOn, bool rayoOn) {
		glm::vec3 colorFocos = focosOn ? glm::vec3(0.85f, 0.90f, 1.0f) : glm::vec3(0.0f, 0.0f, 0.0f);
		const float yFoco = (Y_BASE + (NUM_ESCALONES * ALTO_ESCALON)) + 2.5f;

		if (IndiceLuzIzq() >= 0) {
			arregloSpotLights[IndiceLuzIzq()] = SpotLight(
				colorFocos.x, colorFocos.y, colorFocos.z, 0.5f, 3.0f,
				-18.0f, yFoco, -146.0f, 0.0f, 0.65f, -0.75f, 1.0f, 0.004f, 0.0001f, 32.0f);
		}
		if (IndiceLuzDer() >= 0) {
			arregloSpotLights[IndiceLuzDer()] = SpotLight(
				colorFocos.x, colorFocos.y, colorFocos.z, 0.5f, 3.0f,
				18.0f, yFoco, -146.0f, 0.0f, 0.65f, -0.75f, 1.0f, 0.004f, 0.0001f, 32.0f);
		}

		// Luz verde del rayo, sigue la levitación de la nave
		if (IndiceSpotRayo() >= 0) {
			glm::vec3 colorRayo = rayoOn ? glm::vec3(0.15f, 1.0f, 0.25f) : glm::vec3(0.0f, 0.0f, 0.0f);
			float offset = 3.5f * sin(tiempoSegundos * 2.2f);
			arregloSpotLights[IndiceSpotRayo()] = SpotLight(
				colorRayo.x, colorRayo.y, colorRayo.z, 1.0f, 4.5f,
				0.0f, Y_BASE + 78.0f + offset, -158.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.002f, 0.00005f, 22.0f);
		}
	}

	// Módulo de valla metálica centrado en (centroX, Y_BASE, centroZ)
	static void DibujarModuloValla(
		MeshModel* cuboMesh,
		GLuint uniformModel,
		float centroX,
		float centroZ,
		float rotacionYGrados,
		float largo = 14.0f,
		float alto = 4.2f
	) {
		glm::mat4 base(1.0f);
		base = glm::translate(base, glm::vec3(centroX, Y_BASE, centroZ));
		base = glm::rotate(base, glm::radians(rotacionYGrados), glm::vec3(0.0f, 1.0f, 0.0f));

		glm::mat4 model(1.0f);
		const float grosor = 0.22f;

		// Travesaño superior
		model = glm::translate(base, glm::vec3(0.0f, alto, 0.0f));
		model = glm::scale(model, glm::vec3(largo, grosor, grosor));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		cuboMesh->RenderMeshModel();

		// Travesaño inferior
		model = glm::translate(base, glm::vec3(0.0f, 0.7f, 0.0f));
		model = glm::scale(model, glm::vec3(largo, grosor, grosor));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		cuboMesh->RenderMeshModel();

		// Postes laterales + patas de apoyo
		float extX[2] = { -largo * 0.5f, largo * 0.5f };
		for (int i = 0; i < 2; i++) {
			model = glm::translate(base, glm::vec3(extX[i], alto * 0.5f, 0.0f));
			model = glm::scale(model, glm::vec3(grosor * 1.3f, alto, grosor * 1.3f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			cuboMesh->RenderMeshModel();

			model = glm::translate(base, glm::vec3(extX[i], 0.12f, 0.0f));
			model = glm::scale(model, glm::vec3(0.5f, 0.24f, 2.2f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			cuboMesh->RenderMeshModel();
		}

		// Barrotes internos
		const int numBarrotes = 7;
		float paso = largo / (numBarrotes + 1);
		float altoBarrote = alto - 0.7f;
		for (int b = 1; b <= numBarrotes; b++) {
			float xLocal = (-largo * 0.5f) + (b * paso);
			model = glm::translate(base, glm::vec3(xLocal, 0.7f + (altoBarrote * 0.5f), 0.0f));
			model = glm::scale(model, glm::vec3(grosor * 0.65f, altoBarrote, grosor * 0.65f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			cuboMesh->RenderMeshModel();
		}
	}

	static void DibujarCarcasaFoco(
		MeshModel* cuboMesh, GLuint uniformModel, GLuint uniformColor,
		float x, float y, float z, float rotY
	) {
		glm::mat4 base(1.0f);
		base = glm::translate(base, glm::vec3(x, y, z));
		base = glm::rotate(base, glm::radians(rotY), glm::vec3(0.0f, 1.0f, 0.0f));

		glm::vec3 colorNegro(0.1f, 0.1f, 0.1f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(colorNegro));

		// Base
		glm::mat4 model = glm::translate(base, glm::vec3(0.0f, 0.2f, 0.0f));
		model = glm::scale(model, glm::vec3(2.0f, 0.4f, 2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		cuboMesh->RenderMeshModel();

		// Soporte vertical
		model = glm::translate(base, glm::vec3(0.0f, 1.2f, 0.0f));
		model = glm::scale(model, glm::vec3(0.4f, 2.0f, 0.4f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		cuboMesh->RenderMeshModel();

		// Caja del reflector
		model = glm::translate(base, glm::vec3(0.0f, 2.5f, 0.0f));
		model = glm::rotate(model, glm::radians(45.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.5f, 1.5f, 2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		cuboMesh->RenderMeshModel();
	}

	// Estructura del escenario: escalinata, explanada, pilares, pared, vallas y carcasas
	static void DibujarEscenario(
		GLuint uniformModel, GLuint uniformColor,
		GLuint uniformSpecularIntensity, GLuint uniformShininess,
		Texture& texBlancaBase,
		Texture& texEscalones,
		Material& matOpaco
	) {
		MeshModel* cuboMesh = ObtenerCubo();
		glm::mat4 model(1.0f);
		matOpaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		texEscalones.UseTexture();
		glm::vec3 colorPiedra(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(colorPiedra));
		const float Y_ESTRADO = Y_BASE + (NUM_ESCALONES * ALTO_ESCALON);

		// A. Escalinata

		for (int i = 0; i < NUM_ESCALONES; i++) {
			float yPos = Y_BASE + (i * ALTO_ESCALON) + (ALTO_ESCALON * 0.5f);
			float zPos = Z_INICIO_ESCALERAS - (i * PROF_ESCALON) - (PROF_ESCALON * 0.5f);

			model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(0.0f, yPos, zPos));
			model = glm::scale(model, glm::vec3(ANCHO_ESCENARIO, ALTO_ESCALON, PROF_ESCALON));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			cuboMesh->RenderMeshModel();
		}

		// B. Explanada superior
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, Y_BASE + 4.8f, -177.0f));
		model = glm::scale(model, glm::vec3(ANCHO_ESCENARIO, 9.6f, 66.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		cuboMesh->RenderMeshModel();
		texBlancaBase.UseTexture();

		// C. Pilares
		glm::vec3 colorMarmol(0.96f, 0.95f, 0.88f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(colorMarmol));
		float posX[6] = { -40.0f, -24.0f, -8.0f, 8.0f, 24.0f, 40.0f };
		for (int i = 0; i < 6; i++) {
			// Base
			model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(posX[i], Y_ESTRADO + 2.4f, -168.0f));
			model = glm::scale(model, glm::vec3(6.4f, 4.8f, 6.4f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			cuboMesh->RenderMeshModel();

			// Fuste
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

		// D. Cornisa
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, Y_ESTRADO + 46.0f, -168.0f));
		model = glm::scale(model, glm::vec3(ANCHO_ESCENARIO, 5.6f, 10.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		cuboMesh->RenderMeshModel();

		// E. Pared de fondo
		glm::vec3 colorAzulDebate(0.54f, 0.51f, 0.78f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(colorAzulDebate));
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, Y_ESTRADO + 24.0f, -198.0f));
		model = glm::scale(model, glm::vec3(ANCHO_ESCENARIO, 48.0f, 2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		cuboMesh->RenderMeshModel();

		// F. Vallas
		glm::vec3 colorAcero(0.56f, 0.59f, 0.64f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(colorAcero));

		DibujarModuloValla(cuboMesh, uniformModel, -42.0f, -102.0f, 0.0f);
		DibujarModuloValla(cuboMesh, uniformModel, -26.0f, -102.0f, 0.0f);
		DibujarModuloValla(cuboMesh, uniformModel, -52.0f, -110.0f, 35.0f);

		DibujarModuloValla(cuboMesh, uniformModel, 26.0f, -102.0f, 0.0f);
		DibujarModuloValla(cuboMesh, uniformModel, 42.0f, -102.0f, 0.0f);
		DibujarModuloValla(cuboMesh, uniformModel, 52.0f, -110.0f, -35.0f);

		DibujarModuloValla(cuboMesh, uniformModel, -56.0f, -124.0f, 90.0f);
		DibujarModuloValla(cuboMesh, uniformModel, 56.0f, -124.0f, 90.0f);

		// Carcasas de los reflectores
		DibujarCarcasaFoco(cuboMesh, uniformModel, uniformColor, -18.0f, Y_ESTRADO, -146.0f, -25.0f);
		DibujarCarcasaFoco(cuboMesh, uniformModel, uniformColor, 18.0f, Y_ESTRADO, -146.0f, 25.0f);

		// Deja el color en blanco para lo que se dibuje después
		glm::vec3 blanco(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(blanco));
	}

	// Podios, Kang, Kodos, nave nodriza y Homero (sin rayo abductor)
	static void DibujarModelosDebate(
		GLuint uniformModel,
		GLuint uniformColor,
		GLuint uniformSpecularIntensity,
		GLuint uniformShininess,
		Model& podioModel,
		Model& kangModel,
		Model& kodosModel,
		Model& naveModel,
		Model& homeroModel,
		Texture& texBlancaBase,
		Material& matBrillante,
		float tiempoSegundos,
		bool rayoOn, float alturaAbduccion, float giroAbduccion
	) {
		// Estado limpio por seguridad
		glDepthMask(GL_TRUE);
		glDisable(GL_BLEND);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


		glm::mat4 model(1.0f);
		glm::vec3 blanco(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(blanco));

		const float Y_ESTRADO = Y_BASE + (NUM_ESCALONES * ALTO_ESCALON);

		// Podio izquierdo
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-16.0f, Y_ESTRADO, -152.0f));
		model = glm::scale(model, glm::vec3(3.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		podioModel.RenderModel();

		// Podio derecho
		model = glm::translate(glm::mat4(1.0f), glm::vec3(16.0f, Y_ESTRADO, -152.0f));
		model = glm::scale(model, glm::vec3(3.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		podioModel.RenderModel();

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		// (Opcional, pero recomendado para cristales) Evita que el cristal oculte lo que hay dentro de sí mismo
		glDepthMask(GL_FALSE);

		// Kang
		model = glm::mat4(1.0f);
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-16.0f, Y_ESTRADO, -156.5f));
		model = glm::scale(model, glm::vec3(4.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		kangModel.RenderModel();

		// Kodos
		model = glm::mat4(1.0f);
		model = glm::translate(glm::mat4(1.0f), glm::vec3(16.0f, Y_ESTRADO, -156.5f));
		model = glm::scale(model, glm::vec3(4.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		kodosModel.RenderModel();
		glDepthMask(GL_TRUE);
		glDisable(GL_BLEND);
		// Nave nodriza levitando
		const float offsetLevitacion = 3.5f * sin(tiempoSegundos * 2.2f);
		const float giroNave = tiempoSegundos * 35.0f * 0.01745329f;
		const float yNaveActual = Y_BASE + 80.0f + offsetLevitacion;

		model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, yNaveActual, -158.0f));
		model = glm::rotate(model, giroNave, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, 0.08f * sin(tiempoSegundos * 1.5f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(7.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		naveModel.RenderModel();

		// --- Homero: sube poco a poco dentro del rayo ---
// Tope: base de la nave, para que no la atraviese
		const float alturaMax = (yNaveActual - 2.0f) - Y_ESTRADO - 3.0f;
		const float alturaHomero = (alturaAbduccion < alturaMax) ? alturaAbduccion : alturaMax;

		glm::mat4 modelAbducido = glm::translate(glm::mat4(1.0f),
			glm::vec3(0.0f, Y_ESTRADO + alturaHomero, -158.0f));
		if (rayoOn && alturaHomero > 0.1f) {
			// Gira e inclina un poco mientras flota
			modelAbducido = glm::rotate(modelAbducido, giroAbduccion, glm::vec3(0.0f, 1.0f, 0.0f));
			modelAbducido = glm::rotate(modelAbducido, glm::radians(10.0f) * sin(tiempoSegundos * 3.0f),
				glm::vec3(1.0f, 0.0f, 1.0f));
		}
		modelAbducido = glm::scale(modelAbducido, glm::vec3(1.8f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelAbducido));
		glUniform3fv(uniformColor, 1, glm::value_ptr(blanco));
		homeroModel.RenderModel();

		// --- Cono del rayo: translúcido, al final y solo si está encendido ---
		if (rayoOn) {
			const float alturaRayo = (yNaveActual - 2.0f) - Y_ESTRADO;
			const float yCentroRayo = Y_ESTRADO + (alturaRayo * 0.5f);
			const float pulsoRadio = 14.0f + 1.5f * sin(tiempoSegundos * 6.0f);

			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE);
			glDepthMask(GL_FALSE);

			texBlancaBase.UseTexture();
			matBrillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
			glm::vec3 colorRayoVerde(0.15f, 0.95f, 0.30f);
			glUniform3fv(uniformColor, 1, glm::value_ptr(colorRayoVerde));

			glm::mat4 modelRayo = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, yCentroRayo, -158.0f));
			modelRayo = glm::rotate(modelRayo, -giroNave * 1.5f, glm::vec3(0.0f, 1.0f, 0.0f));
			modelRayo = glm::scale(modelRayo, glm::vec3(pulsoRadio, alturaRayo, pulsoRadio));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelRayo));
			ObtenerConoAbductor()->RenderMeshModel();

			// Restaurar estado
			glDepthMask(GL_TRUE);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glDisable(GL_BLEND);
			glUniform3fv(uniformColor, 1, glm::value_ptr(blanco));
		}
	}
};