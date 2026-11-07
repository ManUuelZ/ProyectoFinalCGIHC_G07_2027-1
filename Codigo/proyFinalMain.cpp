/*
Proyecto final
Escenarios: 1 Espacio | 2 Valle helado | 3 Erid (Rocky + escenario del Debate) | 4 Camara Libre | 5 Conducir Nave | 9 Recorrido 4
*/
//para cargar imagen
#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <string>
#include <cmath>
#include <vector>
#include <math.h>

#include <glew.h>
#include <glfw3.h>

#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>

#include "Window.h"
#include "Mesh_tn.h"
#include "Shader_light.h"
#include "Camera.h"
#include "Texture.h"
#include "Model.h"
#include "Skybox.h"

//para iluminación
#include "CommonValues.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Material.h"
#include "PlanetaryRaceTrack.h"

// ===== Integracion de CrepperDatos.h =====
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <array>

namespace CrepperDatos {
	struct Nodo { const char* nombre; const char* archivo; int padre; glm::vec3 traslacion; };
	inline const Nodo nodos[] = {
		{ "Crepper_Raiz", nullptr, -1, glm::vec3(0.0f, 0.0f, 0.0f) },
		{ "cuerpo_inferior", "Models/CrepperJerarquizado/cuerpo_inferior.obj", 0, glm::vec3(0.0f, 0.67f, 0.05f) },
		{ "cabeza", "Models/CrepperJerarquizado/cabeza.obj", 1, glm::vec3(0.0f, 0.24f, -1.44f) },
		{ "borde_frontal", "Models/CrepperJerarquizado/borde_frontal.obj", 1, glm::vec3(0.000000000f, 0.000000000f, -1.975000000f) },
		{ "placa_01", "Models/CrepperJerarquizado/placa_01.obj", 1, glm::vec3(0.0f, 0.000000000f, -1.660000000f) },
		{ "placa_02", "Models/CrepperJerarquizado/placa_02.obj", 1, glm::vec3(0.0f, 0.000000000f, -1.010000000f) },
		{ "placa_03", "Models/CrepperJerarquizado/placa_03.obj", 1, glm::vec3(0.000000000f, 0.000000000f, -0.360000000f) },
		{ "placa_04", "Models/CrepperJerarquizado/placa_04.obj", 1, glm::vec3(0.000000000f, 0.000000000f, 0.275000000f) },
		{ "placa_05", "Models/CrepperJerarquizado/placa_05.obj", 1, glm::vec3(0.000000000f, 0.000000000f, 0.875000000f) },
		{ "placa_06", "Models/CrepperJerarquizado/placa_06.obj", 1, glm::vec3(0.000000000f, 0.000000000f, 1.430000000f) },
		{ "placa_07", "Models/CrepperJerarquizado/placa_07.obj", 1, glm::vec3(0.000000000f, 0.000000000f, 1.915000000f) },
		{ "cola", "Models/CrepperJerarquizado/cola.obj", 1, glm::vec3(0.000000000f, 0.000000000f, 1.780000000f) },
		{ "pata_izquierda_1", "Models/CrepperJerarquizado/pata_izquierda_1.obj", 1, glm::vec3(-0.860000000f, -0.099394742f, -1.510781433f) },
		{ "pata_izquierda_2", "Models/CrepperJerarquizado/pata_izquierda_2.obj", 1, glm::vec3(-0.980000000f, -0.099394742f, -1.010781433f) },
		{ "pata_izquierda_3", "Models/CrepperJerarquizado/pata_izquierda_3.obj", 1, glm::vec3(-1.020000000f, -0.099394742f, -0.420781433f) },
		{ "pata_izquierda_4", "Models/CrepperJerarquizado/pata_izquierda_4.obj", 1, glm::vec3(-1.000000000f, -0.099394742f, 0.179218567f) },
		{ "pata_izquierda_5", "Models/CrepperJerarquizado/pata_izquierda_5.obj", 1, glm::vec3(-0.940000000f, -0.099394742f, 0.779218567f) },
		{ "pata_izquierda_6", "Models/CrepperJerarquizado/pata_izquierda_6.obj", 1, glm::vec3(-0.820000000f, -0.099394742f, 1.329218567f) },
		{ "pata_izquierda_7", "Models/CrepperJerarquizado/pata_izquierda_7.obj", 1, glm::vec3(-0.650000000f, -0.099394742f, 1.799218567f) },
		{ "apendice_exterior_izquierda", "Models/CrepperJerarquizado/apendice_exterior_izquierda.obj", 2, glm::vec3(-0.560000000f, -0.435932314f, -0.492195686f) },
		{ "apendice_central_izquierda", "Models/CrepperJerarquizado/apendice_central_izquierda.obj", 2, glm::vec3(-0.230000000f, -0.405409379f, -0.711704545f) },
		{ "ojo_izquierda", "Models/CrepperJerarquizado/ojo_izquierda.obj", 2, glm::vec3(-0.580000000f, 0.280000000f, -0.836000000f) },
		{ "pata_derecha_1", "Models/CrepperJerarquizado/pata_derecha_1.obj", 1, glm::vec3(0.860000000f, -0.099394742f, -1.510781433f) },
		{ "pata_derecha_2", "Models/CrepperJerarquizado/pata_derecha_2.obj", 1, glm::vec3(0.980000000f, -0.099394742f, -1.010781433f) },
		{ "pata_derecha_3", "Models/CrepperJerarquizado/pata_derecha_3.obj", 1, glm::vec3(1.020000000f, -0.099394742f, -0.420781433f) },
		{ "pata_derecha_4", "Models/CrepperJerarquizado/pata_derecha_4.obj", 1, glm::vec3(1.000000000f, -0.099394742f, 0.179218567f) },
		{ "pata_derecha_5", "Models/CrepperJerarquizado/pata_derecha_5.obj", 1, glm::vec3(0.940000000f, -0.099394742f, 0.779218567f) },
		{ "pata_derecha_6", "Models/CrepperJerarquizado/pata_derecha_6.obj", 1, glm::vec3(0.820000000f, -0.099394742f, 1.329218567f) },
		{ "pata_derecha_7", "Models/CrepperJerarquizado/pata_derecha_7.obj", 1, glm::vec3(0.650000000f, -0.099394742f, 1.799218567f) },
		{ "apendice_exterior_derecha", "Models/CrepperJerarquizado/apendice_exterior_derecha.obj", 2, glm::vec3(0.560000000f, -0.424067686f, -0.492195686f) },
		{ "apendice_central_derecha", "Models/CrepperJerarquizado/apendice_central_derecha.obj", 2, glm::vec3(0.230000000f, -0.405409379f, -0.711704545f) },
		{ "ojo_derecha", "Models/CrepperJerarquizado/ojo_derecha.obj", 2, glm::vec3(0.580000000f, 0.280000000f, -0.836000000f) },
	};
	inline constexpr int cantidad = sizeof(nodos) / sizeof(nodos[0]);
	using Matrices = std::array<glm::mat4, cantidad>;
	inline Matrices calcularMatrices(const glm::mat4& mundo, const Matrices& rotaciones) {
		Matrices resultado;
		for (int i = 0; i < cantidad; ++i) {
			const glm::mat4 padre = nodos[i].padre < 0 ? mundo : resultado[nodos[i].padre];
			resultado[i] = padre * glm::translate(glm::mat4(1.0f), nodos[i].traslacion) * rotaciones[i];
		}
		return resultado;
	}
} // namespace CrepperDatos

// ===== Integracion de CrepperNPC.h =====
#include <cmath>
#include <cstring>
#include <algorithm>

namespace CrepperNPC {
	constexpr double pi = 3.14159265358979323846;
	constexpr float radioRecorrido = 40, velocidad = 6, escala = 3;
	constexpr float limiteCamara = 170;
	struct Pose { glm::vec3 posicion, frente; CrepperDatos::Matrices matrices; };
	inline Pose evaluar(double tiempo) {
		double angulo = std::fmod(tiempo * velocidad / radioRecorrido, 2 * pi);
		double paso = tiempo * 2 * pi * 1.3;
		glm::vec3 posicion(radioRecorrido * std::sin(angulo), .22f + .035f * std::sin(2 * paso),
			-40 + radioRecorrido * std::cos(angulo));
		glm::vec3 frente(std::cos(angulo), 0, -std::sin(angulo));
		auto mundo = glm::translate(glm::mat4(1), posicion);
		mundo = glm::rotate(mundo, std::atan2(-frente.x, -frente.z), glm::vec3(0, 1, 0));
		mundo = glm::scale(mundo, glm::vec3(escala));
		CrepperDatos::Matrices rotaciones;
		for (int i = 0; i < CrepperDatos::cantidad; ++i) {
			rotaciones[i] = glm::mat4(1);
			const char* nombre = CrepperDatos::nodos[i].nombre;
			if (std::strncmp(nombre, "pata_", 5) == 0) {
				int numero = nombre[std::strlen(nombre) - 1] - '1';
				bool derecha = std::strstr(nombre, "derecha") != nullptr;
				float fase = static_cast<float>(paso - numero * .72 + (derecha ? pi : 0));
				rotaciones[i] = glm::rotate(glm::mat4(1), .18f * std::sin(fase), glm::vec3(1, 0, 0));
				rotaciones[i] = glm::rotate(rotaciones[i], (derecha ? -1 : 1) * .10f * (std::max)(0.f, std::sin(fase)), glm::vec3(0, 0, 1));
			}
			else if (std::strcmp(nombre, "cabeza") == 0)
				rotaciones[i] = glm::rotate(glm::mat4(1), .045f * std::sin(static_cast<float>(tiempo)), glm::vec3(0, 1, 0));
			else if (std::strncmp(nombre, "apendice_", 9) == 0)
				rotaciones[i] = glm::rotate(glm::mat4(1), .07f * std::sin(static_cast<float>(paso) * .5f + i), glm::vec3(1, 0, 0));
			else if (std::strcmp(nombre, "cola") == 0)
				rotaciones[i] = glm::rotate(glm::mat4(1), .04f * std::sin(static_cast<float>(paso)), glm::vec3(0, 1, 0));
		}
		return { posicion,frente,CrepperDatos::calcularMatrices(mundo,rotaciones) };
	}
	inline glm::vec3 limitar(glm::vec3 p) {
		float radio = glm::length(glm::vec2(p.x, p.z));
		if (radio > limiteCamara) { p.x *= limiteCamara / radio; p.z *= limiteCamara / radio; }
		p.y = std::clamp(p.y, 2.f, 65.f);
		return p;
	}
}

// ===== Integracion de ValleHelado.h =====
#include <vector>
#include <fstream>
#include <algorithm>
#include <initializer_list>
#include "Texture.h"

// Escenario 2: Valle helado con Crepper NPC y modelos de Planeta 1.
namespace ValleHelado {
	inline Model piezas[CrepperDatos::cantidad];
	inline Model aurelion;
	inline Model explorador;
	inline MeshModel terreno;
	inline Texture piso;
	inline Skybox cielo;
	inline Shader shader;
	inline double tiempo = 0;
	inline bool estabaActivo = false;
	inline GLint programa = 0;
	inline const char* vert = R"GLSL(#version 330 core
layout(location=0) in vec3 pos;
layout(location=1) in vec2 tex;
layout(location=2) in vec3 normal;
uniform mat4 model,view,projection;
out vec2 uv;
out vec3 n;
void main(){vec4 p=model*vec4(pos,1); uv=tex;
n=mat3(transpose(inverse(model)))*normal; gl_Position=projection*view*p;})GLSL";
	inline const char* frag = R"GLSL(#version 330 core
in vec2 uv;
in vec3 n;
out vec4 salida;
uniform sampler2D texturaPiso;
uniform vec3 tinte;
uniform bool usarTextura;
void main(){
vec3 norm=normalize(n);
float luz=.57+.43*abs(dot(norm,normalize(vec3(.4,1,.25))));
vec3 base=(usarTextura?texture(texturaPiso,uv).rgb:vec3(1))*tinte;
salida=vec4(base*luz,1);
})GLSL";
	inline bool cargarModelo(Model& destino, std::initializer_list<const char*> rutas) {
		for (const char* ruta : rutas) {
			std::ifstream prueba(ruta, std::ios::binary);
			if (prueba.good()) {
				destino.LoadModel(ruta);
				return destino.cargado();
			}
		}
		printf("No se encontro el modelo requerido: %s\n", *rutas.begin());
		return false;
	}
	inline glm::mat4 colocar(const Model& m, glm::vec3 posicion, float altura, float giro = 0.f) {
		const auto& a = m.minimo; const auto& b = m.maximo;
		float escala = altura / (std::max)(.001f, b.y - a.y);
		auto matriz = glm::translate(glm::mat4(1), posicion);
		matriz = glm::rotate(matriz, giro, glm::vec3(0, 1, 0));
		matriz = glm::scale(matriz, glm::vec3(escala));
		return glm::translate(matriz, glm::vec3(-(a.x + b.x) * .5f, -a.y, -(a.z + b.z) * .5f));
	}
	inline bool iniciar() {
		for (int i = 1; i < CrepperDatos::cantidad; ++i) {
			piezas[i].LoadModel(CrepperDatos::nodos[i].archivo);
			if (!piezas[i].cargado()) return false;
		}
		// Nota: Se ha quitado la carga de la "torre" para ser reemplazada por la plataforma Hextech
		if (!cargarModelo(aurelion, { "Models/aurelion sol.glb","Models/Equipo/aurelion sol.glb" })) return false;
		if (!cargarModelo(explorador, { "Models/carroexplorador.obj","Models/Equipo/carroexplorador.obj" })) return false;
		const char* rutaPiso = nullptr;
		for (const char* candidata : { "Textures/pisoNieve.jpg","Textures/pisoNieve.jpeg","Textures/Equipo/pisoNieve.jpg" }) {
			std::ifstream prueba(candidata, std::ios::binary);
			if (prueba.good()) { rutaPiso = candidata; break; }
		}
		if (!rutaPiso) {
			printf("No se encontro la textura del piso: Textures/pisoNieve.jpg\n");
			return false;
		}
		piso = Texture(rutaPiso);
		if (!piso.LoadTextureA()) return false;
		piso.UseTexture();
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
#ifdef GL_TEXTURE_MAX_ANISOTROPY_EXT
		if (GLEW_EXT_texture_filter_anisotropic) {
			GLfloat maxAniso = 1.f;
			glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAniso);
			if (maxAniso > 1.f) glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, (std::min)(maxAniso, 8.f));
		}
#endif
		glBindTexture(GL_TEXTURE_2D, 0);
		const char* baseCubemap = "Textures/Skybox/valle_helado_cubemap/";
		if (!std::ifstream(std::string(baseCubemap) + "px.png", std::ios::binary).good())
			baseCubemap = "Textures/valle_helado_cubemap/";
		stbi_set_flip_vertically_on_load(false);
		std::vector<std::string> caras;
		for (const char* c : { "px","nx","py","ny","pz","nz" })
			caras.push_back(std::string(baseCubemap) + c + ".png");
		cielo = Skybox(caras);
		stbi_set_flip_vertically_on_load(true);
		shader.CreateFromString(vert, frag); shader.UseShader();
		glGetIntegerv(GL_CURRENT_PROGRAM, &programa);
		constexpr float mitadPiso = 1500.f, escalaU = mitadPiso / 40.f, escalaV = mitadPiso / 60.f;
		GLfloat vertices[] = {
			-mitadPiso,0.f,-mitadPiso, -escalaU,-escalaV, 0.f,1.f,0.f,
			 mitadPiso,0.f,-mitadPiso,  escalaU,-escalaV, 0.f,1.f,0.f,
			-mitadPiso,0.f, mitadPiso, -escalaU, escalaV, 0.f,1.f,0.f,
			 mitadPiso,0.f, mitadPiso,  escalaU, escalaV, 0.f,1.f,0.f,
		};
		unsigned indices[] = { 0,2,1, 1,2,3 };
		terreno.CreateMeshModel(vertices, indices, 32, 6);
		return programa != 0;
	}
	inline void entrar(Camera& camera) {
		camera = Camera({ 50,27,65 }, { 0,1,0 }, -125, -15, 24, .25f);
		glfwSetWindowTitle(glfwGetCurrentContext(), "Valle helado - Crepper NPC | 1 Espacio | 2 Valle | 3 Erid | 4 Libre | 5 Nave");
	}
	inline void dibujar(Camera& camera, const glm::mat4& projection, double dt) {
		if (!estabaActivo) entrar(camera);
		estabaActivo = true;
		if (std::isfinite(dt) && dt > 0) tiempo += dt;
		auto pos = camera.getCameraPosition(), limitado = CrepperNPC::limitar(pos);
		if (glm::length(pos - limitado) > .0001f) {
			auto f = camera.getCameraDirection();
			camera = Camera(limitado, { 0,1,0 }, glm::degrees(std::atan2(f.z, f.x)), glm::degrees(std::asin(f.y)), 24, .25f);
		}
		glClearColor(.65f, .78f, .87f, 1); glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		auto view = camera.calculateViewMatrix(); cielo.DrawSkybox(view, projection);
		shader.UseShader();
		GLint locModel = glGetUniformLocation(programa, "model");
		GLint locTinte = glGetUniformLocation(programa, "tinte");
		GLint locUsarTex = glGetUniformLocation(programa, "usarTextura");
		glUniformMatrix4fv(glGetUniformLocation(programa, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(programa, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		glUniform1i(glGetUniformLocation(programa, "texturaPiso"), 0);

		// Piso nevado
		auto ident = glm::mat4(1);
		glUniformMatrix4fv(locModel, 1, GL_FALSE, glm::value_ptr(ident));
		glUniform3f(locTinte, 1.f, 1.f, 1.f);
		glUniform1i(locUsarTex, 1);
		piso.UseTexture();
		terreno.RenderMeshModel();

		// Modelos de Planeta 1 (Aurelion y explorador)
		auto mAurelion = colocar(aurelion, { -42.f,24.f,-95.f }, 22.f, static_cast<float>(tiempo * .15));
		glUniformMatrix4fv(locModel, 1, GL_FALSE, glm::value_ptr(mAurelion));
		aurelion.RenderModel();

		auto mExplorador = colocar(explorador, { 40.f,0.f,-8.f }, 7.5f, -.65f);
		glUniform3f(locTinte, .76f, .82f, .88f);
		glUniformMatrix4fv(locModel, 1, GL_FALSE, glm::value_ptr(mExplorador));
		explorador.RenderModel();

		// Crepper NPC jerarquizado
		glUniform1i(locUsarTex, 0);
		auto pose = CrepperNPC::evaluar(tiempo);
		for (int i = 1; i < CrepperDatos::cantidad; ++i) {
			const char* nombre = CrepperDatos::nodos[i].nombre;
			glm::vec3 color(.53f, .59f, .58f);
			if (std::strncmp(nombre, "ojo_", 4) == 0) color = { .018f,.025f,.025f };
			else if (std::strcmp(nombre, "cabeza") == 0 || std::strncmp(nombre, "apendice", 8) == 0) color = { .65f,.69f,.67f };
			else if (std::strcmp(nombre, "borde_frontal") == 0) color = { .2f,.28f,.28f };
			glUniform3fv(locTinte, 1, glm::value_ptr(color));
			glUniformMatrix4fv(locModel, 1, GL_FALSE, glm::value_ptr(pose.matrices[i]));
			piezas[i].RenderModel();
		}
		glUseProgram(0);
	}
	inline void captura(const std::string& ruta) {
		int w, h; glfwGetFramebufferSize(glfwGetCurrentContext(), &w, &h);
		std::vector<unsigned char> datos(w * h * 3);
		glPixelStorei(GL_PACK_ALIGNMENT, 1); glReadPixels(0, 0, w, h, GL_RGB, GL_UNSIGNED_BYTE, datos.data());
		std::ofstream f(ruta, std::ios::binary);
		f << "P6\n" << w << " " << h << "\n255\n";
		for (int y = h - 1; y >= 0; --y)f.write(reinterpret_cast<char*>(datos.data() + y * w * 3), w * 3);
	}
	inline void captura(int numero) {
		captura("x64/Practica9/valle_" + std::to_string(numero) + ".ppm");
	}
}

// ===== Fin de la integracion =====
#include "EscenarioDebate.h"
#include "Avatar.h"

const float toRadians = 3.14159265f / 180.0f;

Window mainWindow;
std::vector<MeshModel*> meshListModel;
std::vector<Shader> shaderList;

Camera camera;

Texture brickTexture;
Texture dirtTexture;
Texture plainTexture;
Texture pisoPlaneta2;
Texture pisoPlaneta1;

// Variables para los planetas 
float rotaPlanetas = 0.0f;
float rotaPlanetasOffset = 0.005f;

float rotaAnillo = 0.0f;
float rotaAnilloOffset = 0.0025f;

float rotaLuna = 0.0f;
float rotaLunaOffset = 0.005f;

// Variables para el satelite
float movSatelite = 0.0f;
float movSateliteOffset = 0.002f;
int estadoSatelite = 0;
float rotacionSatelite = 0.0f;
float rotaAntena = 0.0f;
float rotaAntenaOffset = 0.025f;
float rotaPanelIzq = 0.0f;
float rotaPanelDer = 0.0f;
float rotaPanelIzqOffset = 0.01f;
float rotaPanelDerOffset = 0.01f;

// Variables para rocky
float rotBrazoRocky = 0.0f;
float rotBrazoRockyOffset = 0.1f;
bool saludo = true;

// Variables para el modo de cámara
bool camaraLigadaAvatar = true;
bool camaraEnNave = false;

// Lista de Modelos a importar
Model rocky;
Model brazoRocky;
Model xenonita;
Model rocas1;
Model roca2;
PlanetaryRaceTrack pistaCarreras;

Model Podio_M;
Model Kang_M;
Model Kodos_M;
Model NaveAlien_M;
Texture texEscalones;

Model naveEspacial;
bool focosEncendidos = true;
bool teclaL_presionada = false;
bool rayoEncendido = false;
bool teclaR_presionada = false;
float alturaAbduccion = 0.0f;
float giroAbduccion = 0.0f;
Model Homero_M;

// Configuración de los 4 vehículos
PlanetaryRaceTrack::RacerConfig corredores[4] = {
	{ -3.3f, 22.0f,  0.0f, 0.8f, 180.0f, 0.45f, glm::vec3(1.0f, 0.25f, 0.25f) },
	{ -1.1f, 25.5f,  8.0f, 0.8f, 180.0f, 0.45f, glm::vec3(0.25f, 0.6f, 1.0f) },
	{ 1.1f, 23.5f, 16.0f, 0.8f, 180.0f, 0.45f, glm::vec3(0.3f, 1.0f, 0.35f) },
	{ 3.3f, 27.0f, 24.0f, 0.8f, 180.0f, 0.45f, glm::vec3(1.0f, 0.85f, 0.2f) }
};

// Modelos del espacio
Model sol;
Model luna;
Model anilloAst;
Model planeta1;
Model planeta2;
Model cuerpoSat;
Model antenaSat;
Model panelDerSat;
Model panelIzqSat;
Model asteroides;

// PLATAFORMA HEXTECH
float anguloCirculoHex = 0.0f;
float anguloSemiAro = 0.0f;
Model BasePlataforma_M;
Model NucleoPlataforma_M;
Model CirculoHex_M;
Model TorrePlataforma_M;
Model EsferaPlataforma_M;
Model SemiaroPlataforma_M;
Model AndamioIzqPlataforma_M;
Model AndamioDerPlataforma_M;

// ESTACION ESPACIAL
Model EstacionCabina_M;
Model EstacionEstructura_M;
Model EstacionPanelDerecho_M;
Model EstacionPanelIzquierdo_M;
Model EstacionCapsulasDer_M;
Model EstacionCapsulasIzq_M;
Model EstacionPuertaIzq_M;
Model EstacionPuertaDer_M;

// RECORRIDO 4
Model Cohete_M;
float coheteAltura = 0.0f;
float coheteVelocidad = 0.0f;
bool  coheteLanzando = false;
float cicloCohete = 0.0f;
const float CICLO_COHETE = 5.0f;
const float ACELERACION_COHETE = 5.0f;
const float ALTURA_MAX_COHETE = 500.0f;

float tiempoRecorrido4 = 0.0f;
int faseRecorrido4 = 0; // 0 = Valle (cohete), 1 = Espacio (estación)
const float DURACION_FASE1 = 12.0f;
const float DURACION_FASE2 = 25.0f; 
float aperturaPuertas = 0.0f;
float progresoNaveNPC = 0.0f;
const float VELOCIDAD_RECORRIDO4 = 0.02f;

// Skybox
Skybox skybox1;
Skybox skybox2;
Skybox skybox3;

int escenarioActual = 1;

Material Material_brillante;
Material Material_opaco;

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;

DirectionalLight mainLight;
PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];

static const char* vShader = "shaders/shader_light.vert";
static const char* fShader = "shaders/shader_light.frag";
static const char* TITULO_VENTANA = "Proyecto final | 1 Espacio | 2 Valle | 3 Erid | 4 Camara Libre | 5 Nave | 9 Recorrido";

void calcAverageNormals(unsigned int* indices, unsigned int indiceCount, GLfloat* vertices, unsigned int verticeCount,
	unsigned int vLength, unsigned int normalOffset)
{
	for (size_t i = 0; i < indiceCount; i += 3)
	{
		unsigned int in0 = indices[i] * vLength;
		unsigned int in1 = indices[i + 1] * vLength;
		unsigned int in2 = indices[i + 2] * vLength;
		glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
		glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
		glm::vec3 normal = glm::cross(v1, v2);
		normal = glm::normalize(normal);

		in0 += normalOffset; in1 += normalOffset; in2 += normalOffset;
		vertices[in0] += normal.x; vertices[in0 + 1] += normal.y; vertices[in0 + 2] += normal.z;
		vertices[in1] += normal.x; vertices[in1 + 1] += normal.y; vertices[in1 + 2] += normal.z;
		vertices[in2] += normal.x; vertices[in2 + 1] += normal.y; vertices[in2 + 2] += normal.z;
	}

	for (size_t i = 0; i < verticeCount / vLength; i++)
	{
		unsigned int nOffset = i * vLength + normalOffset;
		glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
		vec = glm::normalize(vec);
		vertices[nOffset] = vec.x; vertices[nOffset + 1] = vec.y; vertices[nOffset + 2] = vec.z;
	}
}

void CreateObjects()
{
	unsigned int indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	GLfloat vertices[] = {
			-1.0f, -1.0f, -0.6f,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, -1.0f, 1.0f,		0.5f, 0.0f,		0.0f, 0.0f, 0.0f,
			1.0f, -1.0f, -0.6f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,		0.5f, 1.0f,		0.0f, 0.0f, 0.0f
	};

	unsigned int floorIndices[] = {
		0, 2, 1,
		1, 2, 3
	};

	GLfloat floorVertices[] = {
		-10.0f, 0.0f, -10.0f,	0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, -10.0f,	10.0f, 0.0f,	0.0f, -1.0f, 0.0f,
		-10.0f, 0.0f, 10.0f,	0.0f, 10.0f,	0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, 10.0f,		10.0f, 10.0f,	0.0f, -1.0f, 0.0f
	};

	unsigned int vegetacionIndices[] = {
		0, 1, 2,
		0, 2, 3,
		4, 5, 6,
		4, 6, 7
	};

	GLfloat vegetacionVertices[] = {
		-0.5f, -0.5f, 0.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.0f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.0f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.0f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,

		0.0f, -0.5f, -0.5f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.5f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.5f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, -0.5f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,
	};

	MeshModel* obj1 = new MeshModel();
	obj1->CreateMeshModel(vertices, indices, 32, 12);
	meshListModel.push_back(obj1);

	MeshModel* obj2 = new MeshModel();
	obj2->CreateMeshModel(vertices, indices, 32, 12);
	meshListModel.push_back(obj2);

	MeshModel* obj3 = new MeshModel();
	obj3->CreateMeshModel(floorVertices, floorIndices, 32, 6);
	meshListModel.push_back(obj3);

	MeshModel* obj4 = new MeshModel();
	obj4->CreateMeshModel(vegetacionVertices, vegetacionIndices, 64, 12);
	meshListModel.push_back(obj4);

	calcAverageNormals(indices, 12, vertices, 32, 8, 5);
	calcAverageNormals(vegetacionIndices, 12, vegetacionVertices, 64, 8, 5);
}

void CreateShaders()
{
	Shader* shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}

int main(int argc, char** argv)
{
	mainWindow = Window(1366, 768);
	const bool verificarValle = argc > 1 && std::string(argv[1]) == "--verificar-valle";
	const bool verificarTodo = argc > 1 && std::string(argv[1]) == "--verificar";
	const bool verificar = verificarValle || verificarTodo;
	if (mainWindow.Initialise() != 0) return 1;
	int framePrueba = 0;
	bool falloGL = false;
	CreateObjects();
	CreateShaders();
	if (!ValleHelado::iniciar()) return 2;

	camera = Camera(glm::vec3(0.0f, 0.0f, 375.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 1.0f, 0.5f);

	brickTexture = Texture("Textures/brick.png");
	brickTexture.LoadTextureA();
	dirtTexture = Texture("Textures/dirt.png");
	dirtTexture.LoadTextureA();
	plainTexture = Texture("Textures/plain.png");
	plainTexture.LoadTextureA();
	pisoPlaneta2 = Texture("Textures/pisoPlaneta2.jpg");
	pisoPlaneta2.LoadTextureA();
	pisoPlaneta1 = Texture("Textures/pisonieve.jpg");
	pisoPlaneta1.LoadTextureA();
	texEscalones = Texture("Textures/escalones.png");
	texEscalones.LoadTextureA();

	// ----- Carga de modelos -----
	rocky = Model(); rocky.LoadModel("Models/rockyPersonaje.obj");
	brazoRocky = Model(); brazoRocky.LoadModel("Models/rockyBrazo.obj");
	xenonita = Model(); xenonita.LoadModel("Models/xenonita.obj");
	rocas1 = Model(); rocas1.LoadModel("Models/rocas1.obj");
	roca2 = Model(); roca2.LoadModel("Models/roca2.obj");

	Podio_M = Model();     Podio_M.LoadModel("Models/podium.obj");
	Kang_M = Model();      Kang_M.LoadModel("Models/alien_1.obj");
	Kodos_M = Model();     Kodos_M.LoadModel("Models/alien_1.obj");
	NaveAlien_M = Model(); NaveAlien_M.LoadModel("Models/UFO.obj");
	Homero_M = Model();    Homero_M.LoadModel("Models/Milhouse.obj");

	naveEspacial = Model(); naveEspacial.LoadModel("Models/naveEspacial.obj");

	// Naves de carrera
	Model navesCarrera[4];
	const char* rutasNaves[4] = {
		"Models/naveEspacial.obj",
		"Models/Spacechip.obj",
		"Models/nave3.obj",
		"Models/nave4.obj"
	};
	float escalaNaves[4] = { 5.0f, 5.0f, 5.0f, 5.0f };
	float alturaNaves[4] = { 0.25f, 0.25f, 0.25f, 0.25f };
	for (int i = 0; i < 4; i++) {
		navesCarrera[i] = Model();
		navesCarrera[i].LoadModel(rutasNaves[i]);
	}

	Avatar::iniciar();
	pistaCarreras.invertNormals = true;
	pistaCarreras.buildMesh();

	// Espacio
	sol = Model(); sol.LoadModel("Models/sol.obj");
	luna = Model(); luna.LoadModel("Models/luna.obj");
	anilloAst = Model(); anilloAst.LoadModel("Models/anilloAst.obj");
	planeta1 = Model(); planeta1.LoadModel("Models/planeta1.obj");
	planeta2 = Model(); planeta2.LoadModel("Models/planeta2.obj");
	cuerpoSat = Model(); cuerpoSat.LoadModel("Models/cuerpoSat.obj");
	antenaSat = Model(); antenaSat.LoadModel("Models/antena.obj");
	panelIzqSat = Model(); panelIzqSat.LoadModel("Models/panelIzq.obj");
	panelDerSat = Model(); panelDerSat.LoadModel("Models/panelDer.obj");
	asteroides = Model(); asteroides.LoadModel("Models/asteroides.obj");

	// Plataforma Hextech
	BasePlataforma_M = Model(); BasePlataforma_M.LoadModel("Models/BasePlataforma.obj");
	TorrePlataforma_M = Model(); TorrePlataforma_M.LoadModel("Models/TorrePlataforma.obj");
	CirculoHex_M = Model(); CirculoHex_M.LoadModel("Models/CirculoPlataforma.obj");
	AndamioIzqPlataforma_M = Model(); AndamioIzqPlataforma_M.LoadModel("Models/AndamioIzqPlataforma.obj");
	AndamioDerPlataforma_M = Model(); AndamioDerPlataforma_M.LoadModel("Models/AndamioDerPlataforma.obj");
	EsferaPlataforma_M = Model(); EsferaPlataforma_M.LoadModel("Models/EsferaPlataforma.obj");
	SemiaroPlataforma_M = Model(); SemiaroPlataforma_M.LoadModel("Models/Semiaro.obj");
	NucleoPlataforma_M = Model(); NucleoPlataforma_M.LoadModel("Models/NucleoPlataforma.obj");

	// Estación
	EstacionCabina_M = Model(); EstacionCabina_M.LoadModel("Models/EstacionCabina.obj");
	EstacionEstructura_M = Model(); EstacionEstructura_M.LoadModel("Models/EstacionEstructura.obj");
	EstacionPanelDerecho_M = Model(); EstacionPanelDerecho_M.LoadModel("Models/EstacionPanelDerecho.obj");
	EstacionPanelIzquierdo_M = Model(); EstacionPanelIzquierdo_M.LoadModel("Models/EstacionPanelIzquierdo.obj");
	EstacionCapsulasDer_M = Model(); EstacionCapsulasDer_M.LoadModel("Models/EstacionCapsulasDer.obj");
	EstacionCapsulasIzq_M = Model(); EstacionCapsulasIzq_M.LoadModel("Models/EstacionCapsulasIzq.obj");
	EstacionPuertaIzq_M = Model(); EstacionPuertaIzq_M.LoadModel("Models/EstacionPuertaIzq.obj");
	EstacionPuertaDer_M = Model(); EstacionPuertaDer_M.LoadModel("Models/EstacionPuertaDer.obj");

	// Cohete
	Cohete_M = Model(); Cohete_M.LoadModel("Models/Cohete.obj");

	// Skyboxes
	std::vector<std::string> skyboxFaces;
	skyboxFaces.push_back("Textures/Skybox/espacio_rt.tga");
	skyboxFaces.push_back("Textures/Skybox/espacio_lf.tga");
	skyboxFaces.push_back("Textures/Skybox/espacio_dn.tga");
	skyboxFaces.push_back("Textures/Skybox/espacio_up.tga");
	skyboxFaces.push_back("Textures/Skybox/espacio_bk.tga");
	skyboxFaces.push_back("Textures/Skybox/espacio_ft.tga");
	skybox1 = Skybox(skyboxFaces);

	std::vector<std::string> skyboxFaces2;
	skyboxFaces2.push_back("Textures/Skybox/planeta1_rt.tga");
	skyboxFaces2.push_back("Textures/Skybox/planeta1_lf.tga");
	skyboxFaces2.push_back("Textures/Skybox/planeta1_dn.tga");
	skyboxFaces2.push_back("Textures/Skybox/planeta1_up.tga");
	skyboxFaces2.push_back("Textures/Skybox/planeta1_bk.tga");
	skyboxFaces2.push_back("Textures/Skybox/planeta1_ft.tga");
	skybox2 = Skybox(skyboxFaces2);

	std::vector<std::string> skyboxFaces3;
	skyboxFaces3.push_back("Textures/Skybox/erid_right.jpg");
	skyboxFaces3.push_back("Textures/Skybox/erid_left.jpg");
	skyboxFaces3.push_back("Textures/Skybox/erid_bottom.jpg");
	skyboxFaces3.push_back("Textures/Skybox/erid_top.jpg");
	skyboxFaces3.push_back("Textures/Skybox/erid_back.jpg");
	skyboxFaces3.push_back("Textures/Skybox/erid_front.jpg");
	skybox3 = Skybox(skyboxFaces3);

	Material_brillante = Material(4.0f, 256);
	Material_opaco = Material(0.3f, 4);

	unsigned int pointLightCount = 0;
	pointLights[0] = PointLight(1.0f, 0.75f, 0.0f, 5.0f, 5.0f, 0.0f, 75.0f, -150.0f, 0.3f, 0.0005f, 0.0003f);
	pointLightCount++;
	pointLights[1] = PointLight(1.0f, 1.0f, 1.0f, 0.8f, 0.8f, 0.0f, 0.0f, 150.0f, 0.3f, 0.2f, 0.01f);
	pointLightCount++;

	int indicePuntualNave = pointLightCount;
	pointLights[indicePuntualNave] = PointLight(1.0f, 0.0f, 1.0f, 0.8f, 1.0f, 0.0f, 0.0f, 0.0f, 0.3f, 0.2f, 0.1f);
	pointLightCount++;

	unsigned int spotLightCount = 0;
	spotLights[0] = SpotLight(1.0f, 1.0f, 1.0f, 0.0f, 2.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 5.0f);
	spotLightCount++;
	EscenarioDebate::AgregarSpotLights(spotLights, spotLightCount);

	int indiceFaroNave = spotLightCount;
	spotLights[spotLightCount] = SpotLight(1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 2.8f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0005f, 0.00019f, 25.0f);
	spotLightCount++;

	// Luz de la plataforma Hextech
	int indicePuntualHextech = pointLightCount;
	pointLights[indicePuntualHextech] = PointLight(0.1f, 0.65f, 1.0f, 0.5f, 1.8f, 10.5f, 20.5f, -30.0f, 1.0f, 0.045f, 0.0075f);
	pointLightCount++;

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0, uniformSpecularIntensity = 0, uniformShininess = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);

	while (!mainWindow.getShouldClose())
	{
		if (escenarioActual == 1) {
			mainLight = DirectionalLight(1.0f, 1.0f, 1.0f, 0.4f, 0.4f, 0.0f, -1.0f, 0.0f);
		}
		else if (escenarioActual == 2) {
			mainLight = DirectionalLight(1.0f, 1.0f, 1.0f, 0.3f, 0.3f, 1.0f, -1.0f, 0.0f);
		}
		else if (escenarioActual == 3) {
			mainLight = DirectionalLight(1.0f, 1.0f, 1.0f, 0.25f, 0.25f, 1.0f, -1.0f, 0.0f);
		}
		else if (escenarioActual == 4) {
			mainLight = DirectionalLight(1.0f, 1.0f, 1.0f, 0.4f, 0.4f, 0.0f, -1.0f, 0.0f);
		}

		GLfloat now = glfwGetTime();
		const double segundosValle = verificar ? 1.0 / 60.0 : now - lastTime;
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;

		const float tiempoVuelo = (float)glfwGetTime();

		glfwPollEvents();

		if (mainWindow.getsKeys()[GLFW_KEY_L]) {
			if (!teclaL_presionada) { focosEncendidos = !focosEncendidos; teclaL_presionada = true; }
		}
		else { teclaL_presionada = false; }

		if (mainWindow.getsKeys()[GLFW_KEY_R]) {
			if (!teclaR_presionada) { rayoEncendido = !rayoEncendido; teclaR_presionada = true; }
		}
		else { teclaR_presionada = false; }

		const float dtSeg = static_cast<float>(segundosValle);
		const float VEL_SUBIDA = 10.0f;
		const float VEL_BAJADA = 12.0f;
		const float VEL_GIRO = 1.2f;
		const float ALTURA_TOPE = 65.0f;

		if (rayoEncendido) {
			alturaAbduccion += VEL_SUBIDA * dtSeg;
			giroAbduccion += VEL_GIRO * dtSeg;
			if (alturaAbduccion > ALTURA_TOPE) alturaAbduccion = ALTURA_TOPE;
		}
		else {
			if (alturaAbduccion > 0.0f) {
				alturaAbduccion -= VEL_BAJADA * dtSeg;
				if (alturaAbduccion < 0.0f) alturaAbduccion = 0.0f;
			}
		}

		const GLfloat dxMouse = mainWindow.getXChange();
		const GLfloat dyMouse = mainWindow.getYChange();

		const bool camaraLibre = !camaraEnNave && (escenarioActual == 1 || (!camaraLigadaAvatar && escenarioActual != 4));
		if (camaraLibre) {
			camera.keyControl(mainWindow.getsKeys(), escenarioActual == 2 ? static_cast<float>(segundosValle) : deltaTime);
			camera.mouseControl(dxMouse, dyMouse);
		}

		if (mainWindow.getsKeys()[GLFW_KEY_1] && escenarioActual != 1) {
			escenarioActual = 1; camaraEnNave = false;
			camera = Camera(glm::vec3(0.0f, 0.0f, 375.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 2.5f, 0.5f);
			glfwSetWindowTitle(glfwGetCurrentContext(), TITULO_VENTANA);
		}
		if (mainWindow.getsKeys()[GLFW_KEY_2] && escenarioActual != 2) {
			escenarioActual = 2; camaraEnNave = false; camaraLigadaAvatar = true;
			camera = Camera(glm::vec3(0.0f, 0.0f, 50.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 1.5f, 0.5f);
		}
		if (mainWindow.getsKeys()[GLFW_KEY_3] && (escenarioActual != 3 || camaraEnNave)) {
			escenarioActual = 3; camaraEnNave = false; camaraLigadaAvatar = true;
			camera = Camera(glm::vec3(0.0f, 5.0f, 50.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f, 1.5f, 0.5f);
			glfwSetWindowTitle(glfwGetCurrentContext(), TITULO_VENTANA);
		}

		if (mainWindow.getsKeys()[GLFW_KEY_4]) {
			if (camaraEnNave || camaraLigadaAvatar) {
				glm::vec3 p = camera.getCameraPosition();
				glm::vec3 f = camera.getCameraDirection();
				camera = Camera(p, glm::vec3(0.0f, 1.0f, 0.0f), glm::degrees(std::atan2(f.z, f.x)), glm::degrees(std::asin(std::clamp(f.y, -0.99f, 0.99f))), 1.5f, 0.5f);
			}
			camaraLigadaAvatar = false; camaraEnNave = false;
			glfwSetWindowTitle(glfwGetCurrentContext(), "Camara Libre | 1 Espacio | 2 Valle | 3 Erid (Avatar) | 4 Libre | 5 Nave");
		}

		if (mainWindow.getsKeys()[GLFW_KEY_5]) {
			escenarioActual = 3; camaraLigadaAvatar = false; camaraEnNave = true;
			glfwSetWindowTitle(glfwGetCurrentContext(), "Modo Conduccion Nave 3ra Persona (A/D cambiar carril) | 3 Avatar | 4 Libre | 5 Nave");
		}

		//Tecla 9 para Recorrido 4 ----
		if (mainWindow.getsKeys()[GLFW_KEY_9] && escenarioActual != 4) {
			escenarioActual = 4;
			camaraLigadaAvatar = false;
			camaraEnNave = false;
			camera = Camera(glm::vec3(250.0f, 120.0f, 250.0f), glm::vec3(0.0f, 1.0f, 0.0f), -135.0f, -20.0f, 1.5f, 0.5f);
			glfwSetWindowTitle(glfwGetCurrentContext(), "Recorrido 4 - Cinemática | 1,2,3 Escenarios | 9 Recorrido");
		}

		// ---- Lógica de tiempo y cámara para el Recorrido 4 ----
		if (escenarioActual == 4) {
			tiempoRecorrido4 += deltaTime * VELOCIDAD_RECORRIDO4;

			if (tiempoRecorrido4 > (DURACION_FASE1 + DURACION_FASE2)) {
				tiempoRecorrido4 = 0.0f;
				cicloCohete = 0.0f;
				coheteAltura = 0.0f;
				coheteVelocidad = 0.0f;
			}

			float anguloOrbita = tiempoRecorrido4 * 0.08f;

			if (tiempoRecorrido4 < DURACION_FASE1) {
				faseRecorrido4 = 0;
				glm::vec3 centro(0.0f, 20.0f, 0.0f);
				float radio = 90.0f;
				glm::vec3 camPos = centro + glm::vec3(radio * cos(anguloOrbita), 25.0f + 15.0f * sin(anguloOrbita * 0.7f), radio * sin(anguloOrbita));
				glm::vec3 lookAt(0.0f, 40.0f, 0.0f);
				glm::vec3 dir = glm::normalize(lookAt - camPos);
				camera = Camera(camPos, glm::vec3(0.0f, 1.0f, 0.0f), glm::degrees(atan2(dir.z, dir.x)), glm::degrees(asin(dir.y)), 1.0f, 0.5f);
			}
			else {
				faseRecorrido4 = 1;
				float tLocal = tiempoRecorrido4 - DURACION_FASE1;

				// Cámara más cerca de la estación (radio = 60)
				glm::vec3 centro(200.0f, 80.0f, 150.0f);
				float radio = 10.0f;
				glm::vec3 camPos = centro + glm::vec3(radio * cos(anguloOrbita), 5.0f + 1.0f * sin(anguloOrbita * 0.6f), radio * sin(anguloOrbita));
				glm::vec3 lookAt(200.0f, 80.0f, 150.0f);
				glm::vec3 dir = glm::normalize(lookAt - camPos);
				camera = Camera(camPos, glm::vec3(0.0f, 1.0f, 0.0f), glm::degrees(atan2(dir.z, dir.x)), glm::degrees(asin(dir.y)), 1.0f, 0.5f);

				// Tiempos ralentizados
				if (tLocal < 3.0f) {
					aperturaPuertas = tLocal / 3.0f; progresoNaveNPC = 1.0f;
				}
				else if (tLocal < 9.0f) {
					aperturaPuertas = 1.0f; progresoNaveNPC = 1.0f - ((tLocal - 3.0f) / 6.0f); // Nave entra (6s)
				}
				else if (tLocal < 12.0f) {
					aperturaPuertas = 1.0f - ((tLocal - 9.0f) / 3.0f); progresoNaveNPC = 0.0f; // Cierra (3s)
				}
				else if (tLocal < 15.0f) {
					aperturaPuertas = (tLocal - 12.0f) / 3.0f; progresoNaveNPC = 0.0f; // Abre (3s)
				}
				else if (tLocal < 21.0f) {
					aperturaPuertas = 1.0f; progresoNaveNPC = (tLocal - 15.0f) / 6.0f; // Nave sale (6s)
				}
				else {
					aperturaPuertas = 1.0f - ((tLocal - 21.0f) / 4.0f); progresoNaveNPC = 1.0f; // Cierra (4s)
				}

				aperturaPuertas = std::clamp(aperturaPuertas, 0.0f, 1.0f);
				progresoNaveNPC = std::clamp(progresoNaveNPC, 0.0f, 1.0f);
			}
		}
		// -----------------------------------------

		if (verificarValle) escenarioActual = 2;
		else if (verificarTodo) {
			if (framePrueba == 1) escenarioActual = 2;
			else if (framePrueba == 61) {
				escenarioActual = 3;
				camera = Camera(glm::vec3(0.0f, 8.0f, 50.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, -5.0f, 0.5f, 0.5f);
			}
		}

		static int escenarioAvatar = 0;
		if (escenarioActual == 2 || escenarioActual == 3) {
			if (escenarioAvatar != escenarioActual) {
				Avatar::entrarEn(escenarioActual);
				escenarioAvatar = escenarioActual;
			}

			if (camaraLigadaAvatar && !camaraEnNave) {
				Avatar::actualizar(mainWindow.getsKeys(), static_cast<float>(segundosValle));
				Avatar::seguirConCamara(camera);
			}
		}
		else {
			escenarioAvatar = 0;
		}

		glm::mat4 matrizPista = glm::mat4(1.0f);
		glm::mat4 modelVehiculo = glm::mat4(1.0f);
		if (escenarioActual == 3) {
			if (camaraEnNave) {
				float dtSec = static_cast<float>(segundosValle);
				if (mainWindow.getsKeys()[GLFW_KEY_A] || mainWindow.getsKeys()[GLFW_KEY_LEFT])
					corredores[0].laneOffset -= 4.5f * dtSec;
				if (mainWindow.getsKeys()[GLFW_KEY_D] || mainWindow.getsKeys()[GLFW_KEY_RIGHT])
					corredores[0].laneOffset += 4.5f * dtSec;
				corredores[0].laneOffset = std::clamp(corredores[0].laneOffset, -4.0f, 4.0f);
			}

			matrizPista = glm::translate(glm::mat4(1.0f), glm::vec3(-100.0f, 2.0f, 150.0f));
			matrizPista = glm::scale(matrizPista, glm::vec3(2.5f));
			modelVehiculo = pistaCarreras.getVehicleTransform(corredores[0], tiempoVuelo, matrizPista);

			if (camaraEnNave) {
				glm::vec3 posNave = glm::vec3(modelVehiculo[3]);
				glm::vec3 upNave = glm::normalize(glm::vec3(modelVehiculo[1]));
				glm::vec3 fwdNave = glm::normalize(glm::vec3(modelVehiculo[2]));
				glm::vec3 camPos = posNave - fwdNave * 18.0f + upNave * 7.5f;
				glm::vec3 target = posNave + fwdNave * 25.0f + upNave * 2.0f;
				glm::vec3 lookDir = glm::normalize(target - camPos);
				float yawDeg = glm::degrees(std::atan2(lookDir.z, lookDir.x));
				float pitchDeg = glm::degrees(std::asin(std::clamp(lookDir.y, -0.99f, 0.99f)));
				camera = Camera(camPos, upNave, yawDeg, pitchDeg, 0.0f, 0.0f);
			}
		}

		if (escenarioActual == 2) {
			auto proyeccionValle = glm::perspective(glm::radians(45.0f), (float)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1200.0f);
			ValleHelado::dibujar(camera, proyeccionValle, segundosValle);

			// Avatar con shader del valle
			{
				GLint prog = ValleHelado::programa;
				glUseProgram(prog);
				glUniform1i(glGetUniformLocation(prog, "usarTextura"), 1);
				Avatar::dibujar(glGetUniformLocation(prog, "model"), glGetUniformLocation(prog, "tinte"));
				glUseProgram(0);
			}

			// ===== PLATAFORMA HEXTECH (con shader de iluminación principal) =====
			shaderList[0].UseShader();

			GLuint uModel = shaderList[0].GetModelLocation();
			GLuint uProjection = shaderList[0].GetProjectionLocation();
			GLuint uView = shaderList[0].GetViewLocation();
			GLuint uEyePos = shaderList[0].GetEyePositionLocation();
			GLuint uColor = shaderList[0].getColorLocation();

			glUniformMatrix4fv(uProjection, 1, GL_FALSE, glm::value_ptr(proyeccionValle));
			glUniformMatrix4fv(uView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
			glUniform3f(uEyePos, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

			shaderList[0].SetDirectionalLight(&mainLight);
			PointLight lucesHextech[1];
			lucesHextech[0] = pointLights[indicePuntualHextech];
			shaderList[0].SetPointLights(lucesHextech, 1);
			shaderList[0].SetSpotLights(spotLights, 0);

			anguloCirculoHex += 2.0f * deltaTime;
			anguloSemiAro += 1.0f * deltaTime;
			float flotacionEsfera = sin((float)glfwGetTime() * 0.5f) * 0.2f;

			glm::vec3 color = glm::vec3(1.0f);

			glm::mat4 modelBase = glm::translate(glm::mat4(1.0f), glm::vec3(10.5f, 0.0f, -30.0f));
			modelBase = glm::rotate(modelBase, 180.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
			modelBase = glm::scale(modelBase, glm::vec3(10.0f));
			glUniform3fv(uColor, 1, glm::value_ptr(color));
			glUniformMatrix4fv(uModel, 1, GL_FALSE, glm::value_ptr(modelBase));
			BasePlataforma_M.RenderModel();

			glm::mat4 modelNucleo = glm::translate(modelBase, glm::vec3(0.0f, 0.41f, 0.0f));
			modelNucleo = glm::rotate(modelNucleo, glm::radians(anguloCirculoHex), glm::vec3(0.0f, 1.0f, 0.0f));
			glUniformMatrix4fv(uModel, 1, GL_FALSE, glm::value_ptr(modelNucleo));
			NucleoPlataforma_M.RenderModel();

			glm::mat4 modelCirculo = glm::translate(modelBase, glm::vec3(0.0f, 0.85f, -0.41f));
			modelCirculo = glm::rotate(modelCirculo, glm::radians(anguloCirculoHex), glm::vec3(0.0f, 1.0f, 0.0f));
			glUniformMatrix4fv(uModel, 1, GL_FALSE, glm::value_ptr(modelCirculo));
			CirculoHex_M.RenderModel();

			glUniformMatrix4fv(uModel, 1, GL_FALSE, glm::value_ptr(modelBase));
			AndamioIzqPlataforma_M.RenderModel();

			glm::mat4 modelAndamioDer = glm::translate(modelBase, glm::vec3(-2.5f, 0.85f, 0.45f));
			glUniformMatrix4fv(uModel, 1, GL_FALSE, glm::value_ptr(modelAndamioDer));
			AndamioDerPlataforma_M.RenderModel();

			glm::mat4 modelTorre = glm::translate(modelBase, glm::vec3(0.0f, 3.15f, 0.95f));
			glUniformMatrix4fv(uModel, 1, GL_FALSE, glm::value_ptr(modelTorre));
			TorrePlataforma_M.RenderModel();

			glm::mat4 modelEsfera = glm::translate(modelTorre, glm::vec3(0.0f, 3.3f + flotacionEsfera, 0.0f));
			glUniformMatrix4fv(uModel, 1, GL_FALSE, glm::value_ptr(modelEsfera));
			EsferaPlataforma_M.RenderModel();

			glm::mat4 modelSemiAro = glm::rotate(modelEsfera, glm::radians(anguloSemiAro), glm::vec3(0.0f, 1.0f, 1.0f));
			glUniformMatrix4fv(uModel, 1, GL_FALSE, glm::value_ptr(modelSemiAro));
			SemiaroPlataforma_M.RenderModel();

			glUseProgram(0);

			if (verificarValle) {
				if (framePrueba == 0 || framePrueba == 120 || framePrueba == 240) ValleHelado::captura(framePrueba);
				GLenum error = glGetError();
				if (error != GL_NO_ERROR) { printf("GL ERROR: %u\n", error); falloGL = true; }
				if (++framePrueba > 240) break;
			}
			else if (verificarTodo) {
				if (framePrueba == 30) ValleHelado::captura("x64/Practica9/escena2.ppm");
				if (++framePrueba > 62) break;
			}
			mainWindow.swapBuffers();
			continue;
		}

		ValleHelado::estabaActivo = false;

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		if (escenarioActual == 1) {
			skybox1.DrawSkybox(camera.calculateViewMatrix(), projection);
		}
		else if (escenarioActual == 3) {
			skybox3.DrawSkybox(camera.calculateViewMatrix(), projection);
		}
		else if (escenarioActual == 4) {
			if (faseRecorrido4 == 0) skybox2.DrawSkybox(camera.calculateViewMatrix(), projection);
			else skybox1.DrawSkybox(camera.calculateViewMatrix(), projection);
		}

		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformEyePosition = shaderList[0].GetEyePositionLocation();
		uniformColor = shaderList[0].getColorLocation();
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		glm::vec3 lowerLight = camera.getCameraPosition();
		lowerLight.y -= 0.3f;
		spotLights[0].SetFlash(lowerLight, camera.getCameraDirection());

		if (escenarioActual == 1) {
			shaderList[0].SetDirectionalLight(&mainLight);
			shaderList[0].SetPointLights(pointLights, 1);
			shaderList[0].SetSpotLights(spotLights, 0);
		}
		else if (escenarioActual == 4) {
			shaderList[0].SetDirectionalLight(&mainLight);
			shaderList[0].SetPointLights(pointLights, 0);
			shaderList[0].SetSpotLights(spotLights, 0);
		}
		else if (escenarioActual == 3) {
			EscenarioDebate::ActualizarLucesDinamicas(spotLights, tiempoVuelo, focosEncendidos, rayoEncendido);
			shaderList[0].SetDirectionalLight(&mainLight);
			PointLight lucesErid[1];
			lucesErid[0] = pointLights[indicePuntualNave];
			shaderList[0].SetPointLights(lucesErid, 1);
			shaderList[0].SetSpotLights(spotLights, spotLightCount);
		}

		glm::mat4 model(1.0);
		glm::mat4 modelaux(1.0);
		glm::mat4 modelaux2(1.0);
		glm::mat4 modelaux3(1.0);
		glm::mat4 modelaux4(1.0);
		glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);

		if (escenarioActual == 1) {
			rotaPlanetas += rotaPlanetasOffset * deltaTime;
			if (rotaPlanetas >= 360.0f * toRadians) rotaPlanetas = 0.0f;
			rotaAnillo += rotaAnilloOffset * deltaTime;
			rotaLuna += rotaLunaOffset * deltaTime;
			if (rotaLuna >= 360.0f * toRadians) rotaLuna = 0.0f;
			rotaAntena += rotaAntenaOffset * deltaTime;
			rotaPanelIzq += rotaPanelIzqOffset * deltaTime;
			rotaPanelDer += rotaPanelDerOffset * deltaTime;
			movSatelite += movSateliteOffset * deltaTime;
			rotacionSatelite += 0.001f * deltaTime;

			model = glm::mat4(1.0); color = glm::vec3(1.0f);
			model = glm::translate(model, glm::vec3(0.0f, 75.0f, -150.0f));
			model = glm::rotate(model, rotaPlanetas, glm::vec3(0.0f, -1.0f, 0.0f));
			model = glm::scale(model, glm::vec3(100.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			glUniform3fv(uniformColor, 1, glm::value_ptr(color));
			sol.RenderModel();

			model = glm::mat4(1.0); color = glm::vec3(1.0f);
			model = glm::translate(model, glm::vec3(250.0f, 0.0f, 175.0f));
			model = glm::rotate(model, rotaPlanetas, glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::scale(model, glm::vec3(40.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			glUniform3fv(uniformColor, 1, glm::value_ptr(color));
			planeta1.RenderModel();

			model = glm::mat4(1.0f); color = glm::vec3(1.0f);
			model = glm::translate(model, glm::vec3(250.0f, 0.0f, 175.0f));
			model = glm::rotate(model, rotaLuna, glm::vec3(0.0f, -1.0f, 0.0f));
			model = glm::translate(model, glm::vec3(100.0f, 0.0f, 0.0f));
			model = glm::rotate(model, rotaLuna * 2.0f, glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::scale(model, glm::vec3(12.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			glUniform3fv(uniformColor, 1, glm::value_ptr(color));
			luna.RenderModel();

			model = modelaux2; color = glm::vec3(1.0f);
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(250.0f, 0.0f, 175.0f));
			model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
			model = glm::rotate(model, movSatelite, glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::translate(model, glm::vec3(60.0f, 0.0f, 0.0f));
			model = glm::rotate(model, rotacionSatelite, glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::scale(model, glm::vec3(0.75f));
			modelaux = model;
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			glUniform3fv(uniformColor, 1, glm::value_ptr(color));
			cuerpoSat.RenderModel();

			model = modelaux;
			model = glm::translate(model, glm::vec3(0.0f, 0.0f, -0.75f));
			model = glm::rotate(model, rotaAntena, glm::vec3(0.0f, 0.0f, 1.0f));
			modelaux3 = model;
			model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			glUniform3fv(uniformColor, 1, glm::value_ptr(color));
			antenaSat.RenderModel();

			model = modelaux;
			model = glm::translate(model, glm::vec3(0.0f, 1.7f, 0.0f));
			model = glm::rotate(model, rotaPanelIzq, glm::vec3(0.0f, 1.0f, 0.0f));
			modelaux3 = model;
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			glUniform3fv(uniformColor, 1, glm::value_ptr(color));
			panelIzqSat.RenderModel();

			model = modelaux;
			model = glm::translate(model, glm::vec3(0.0f, -1.65f, 0.0f));
			model = glm::rotate(model, rotaPanelDer, glm::vec3(0.0f, 1.0f, 0.0f));
			modelaux3 = model;
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			glUniform3fv(uniformColor, 1, glm::value_ptr(color));
			panelDerSat.RenderModel();

			model = glm::mat4(1.0); color = glm::vec3(1.0f);
			model = glm::translate(model, glm::vec3(-250.0f, 0.0f, 175.0f));
			model = glm::rotate(model, rotaPlanetas, glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::scale(model, glm::vec3(40.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			glUniform3fv(uniformColor, 1, glm::value_ptr(color));
			planeta2.RenderModel();

			model = glm::mat4(1.0); color = glm::vec3(1.0f);
			model = glm::translate(model, glm::vec3(-250.0f, 0.0f, 175.0f));
			model = glm::rotate(model, 90.0f * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::rotate(model, rotaAnillo, glm::vec3(0.0f, 0.0f, 1.0f));
			model = glm::scale(model, glm::vec3(1.55f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			glUniform3fv(uniformColor, 1, glm::value_ptr(color));
			anilloAst.RenderModel();

			model = glm::mat4(1.0); color = glm::vec3(1.0f);
			model = glm::translate(model, glm::vec3(-250.0f, 0.0f, 175.0f));
			model = glm::rotate(model, 75.0f * toRadians, glm::vec3(-1.0f, 0.0f, 0.0f));
			model = glm::rotate(model, rotaAnillo, glm::vec3(0.0f, 0.0f, 1.0f));
			model = glm::scale(model, glm::vec3(1.7f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			glUniform3fv(uniformColor, 1, glm::value_ptr(color));
			anilloAst.RenderModel();

			model = glm::mat4(1.0); color = glm::vec3(1.0f);
			model = glm::translate(model, glm::vec3(-100.0f, -50.0f, 750.0f));
			model = glm::rotate(model, 90.0f * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::rotate(model, 45.0f * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
			model = glm::scale(model, glm::vec3(50.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			glUniform3fv(uniformColor, 1, glm::value_ptr(color));
			asteroides.RenderModel();

			// ESTACION
			glm::mat4 modelEstacionCabina = glm::mat4(1.0);
			modelEstacionCabina = glm::translate(modelEstacionCabina, glm::vec3(220.0f, 40.0f, 150.0f));
			modelEstacionCabina = glm::rotate(modelEstacionCabina, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
			color = glm::vec3(1.0f);
			glUniform3fv(uniformColor, 1, glm::value_ptr(color));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelEstacionCabina));
			EstacionCabina_M.RenderModel();

			glm::mat4 modelEstacionPuertaIzq = modelEstacionCabina;
			modelEstacionPuertaIzq = glm::translate(modelEstacionPuertaIzq, glm::vec3(-2.6f, 0.0f, 0.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelEstacionPuertaIzq));
			EstacionPuertaIzq_M.RenderModel();

			glm::mat4 modelEstacionPuertaDer = modelEstacionCabina;
			modelEstacionPuertaDer = glm::translate(modelEstacionPuertaDer, glm::vec3(-2.6f, 0.0f, 0.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelEstacionPuertaDer));
			EstacionPuertaDer_M.RenderModel();

			glm::mat4 modelEstacionEstructura = modelEstacionCabina;
			modelEstacionEstructura = glm::translate(modelEstacionEstructura, glm::vec3(0.0f, 0.0f, 0.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelEstacionEstructura));
			EstacionEstructura_M.RenderModel();

			glm::mat4 modelEstacionPanelDerecho = modelEstacionEstructura;
			modelEstacionPanelDerecho = glm::translate(modelEstacionPanelDerecho, glm::vec3(0.0f, 0.0f, 8.5f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelEstacionPanelDerecho));
			EstacionPanelDerecho_M.RenderModel();

			glm::mat4 modelEstacionPanelIzquierdo = modelEstacionEstructura;
			modelEstacionPanelIzquierdo = glm::translate(modelEstacionPanelIzquierdo, glm::vec3(0.0f, 0.0f, -8.5f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelEstacionPanelIzquierdo));
			EstacionPanelIzquierdo_M.RenderModel();

			glm::mat4 modelEstacionCapsulasDer = modelEstacionEstructura;
			modelEstacionCapsulasDer = glm::translate(modelEstacionCapsulasDer, glm::vec3(0.0f, 0.0f, 5.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelEstacionCapsulasDer));
			EstacionCapsulasDer_M.RenderModel();

			glm::mat4 modelEstacionCapsulasIzq = modelEstacionEstructura;
			modelEstacionCapsulasIzq = glm::translate(modelEstacionCapsulasIzq, glm::vec3(0.0f, 0.0f, -5.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelEstacionCapsulasIzq));
			EstacionCapsulasIzq_M.RenderModel();
		}

		else if (escenarioActual == 3) {
			if (saludo) {
				rotBrazoRocky += rotBrazoRockyOffset;
				if (rotBrazoRocky >= 30.0f) saludo = false;
			}
			else {
				rotBrazoRocky -= rotBrazoRockyOffset;
				if (rotBrazoRocky <= -30.0f) saludo = true;
			}

			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
			model = glm::scale(model, glm::vec3(100.0f, 1.0f, 100.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			glUniform3fv(uniformColor, 1, glm::value_ptr(color));
			pisoPlaneta2.UseTexture();
			Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
			meshListModel[2]->RenderMeshModel();

			model = glm::mat4(1.0); color = glm::vec3(1.0f);
			model = glm::translate(model, glm::vec3(0.0f, -5.0f, 0.0f));
			model = glm::rotate(model, 90.0f * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::scale(model, glm::vec3(150.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			glUniform3fv(uniformColor, 1, glm::value_ptr(color));
			rocas1.RenderModel();

			model = glm::mat4(1.0); color = glm::vec3(1.0f);
			model = glm::translate(model, glm::vec3(-75.0f, -0.5f, 340.0f));
			modelaux = model;
			model = glm::rotate(model, 90.0f * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::rotate(model, -30.0f * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
			model = glm::scale(model, glm::vec3(90.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			glUniform3fv(uniformColor, 1, glm::value_ptr(color));
			roca2.RenderModel();

			color = glm::vec3(0.85f, 0.85f, 0.9f);
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(matrizPista));
			glUniform3fv(uniformColor, 1, glm::value_ptr(color));
			dirtTexture.UseTexture();
			Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
			pistaCarreras.draw();

			Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
			color = glm::vec3(1.0f);
			glUniform3fv(uniformColor, 1, glm::value_ptr(color));

			for (int i = 0; i < 4; i++) {
				glm::mat4 mv = (i == 0) ? modelVehiculo : pistaCarreras.getVehicleTransform(corredores[i], tiempoVuelo, matrizPista);
				if (i == 0) {
					glm::vec3 naveSpotPos = glm::vec3(mv * glm::vec4(0.0f, 0.0f, 5.0f, 1.0f));
					glm::vec3 naveSpotDir = glm::normalize(glm::vec3(mv * glm::vec4(0.0f, -0.4f, 1.0f, 0.0f)));
					spotLights[indiceFaroNave].SetFlash(naveSpotPos, naveSpotDir);
					glm::vec3 navePuntualPos = glm::vec3(mv * glm::vec4(-0.5f, 2.25f, 0.0f, 1.0f));
					pointLights[indicePuntualNave].SetPosition(navePuntualPos);
				}
				glm::mat4 modelNave = glm::scale(mv, glm::vec3(escalaNaves[i]));
				modelNave = glm::translate(modelNave, glm::vec3(0.0f, alturaNaves[i], 0.0f));
				glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelNave));
				navesCarrera[i].RenderModel();
			}

			Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);

			model = glm::mat4(1.0); color = glm::vec3(1.0f);
			model = glm::translate(model, glm::vec3(300.0f, 7.0f, 65.0f));
			modelaux4 = model;
			model = glm::rotate(model, 90.0f * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::scale(model, glm::vec3(35.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			glUniform3fv(uniformColor, 1, glm::value_ptr(color));
			rocky.RenderModel();

			model = modelaux4;
			model = glm::translate(model, glm::vec3(-0.75f, 1.9f, 3.3f));
			model = glm::rotate(model, rotBrazoRocky * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
			model = glm::rotate(model, 180.0f * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
			model = glm::rotate(model, 90.0f * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::scale(model, glm::vec3(35.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			glUniform3fv(uniformColor, 1, glm::value_ptr(color));
			brazoRocky.RenderModel();

			model = glm::mat4(1.0); color = glm::vec3(1.0f);
			model = glm::translate(model, glm::vec3(300.0f, 6.5f, 85.0f));
			modelaux4 = model;
			model = glm::rotate(model, 180.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::rotate(model, 90.0f * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::scale(model, glm::vec3(20.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			glUniform3fv(uniformColor, 1, glm::value_ptr(color));
			rocky.RenderModel();

			model = modelaux4;
			model = glm::translate(model, glm::vec3(0.575f, 1.15f, -1.85f));
			model = glm::rotate(model, rotBrazoRocky * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
			model = glm::rotate(model, 180.0f * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
			model = glm::rotate(model, 180.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::rotate(model, 90.0f * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
			glm::scale(model, glm::vec3(20.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			glUniform3fv(uniformColor, 1, glm::value_ptr(color));
			brazoRocky.RenderModel();

			model = glm::mat4(1.0); color = glm::vec3(1.0f);
			model = glm::translate(model, glm::vec3(300.0f, 1.25f, 0.0f));
			modelaux = model;
			model = glm::rotate(model, 90.0f * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::scale(model, glm::vec3(50.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			glUniform3fv(uniformColor, 1, glm::value_ptr(color));
			xenonita.RenderModel();

			model = glm::mat4(1.0); color = glm::vec3(1.0f);
			model = glm::translate(model, glm::vec3(375.0f, 4.0f, 75.0f));
			modelaux = model;
			model = glm::rotate(model, 50.0f * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::scale(model, glm::vec3(50.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			glUniform3fv(uniformColor, 1, glm::value_ptr(color));
			xenonita.RenderModel();

			model = glm::mat4(1.0); color = glm::vec3(1.0f);
			model = glm::translate(model, glm::vec3(300.0f, 1.25f, 150.0f));
			modelaux = model;
			model = glm::rotate(model, 20.0f * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::rotate(model, 105.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::scale(model, glm::vec3(50.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			glUniform3fv(uniformColor, 1, glm::value_ptr(color));
			xenonita.RenderModel();

			model = glm::mat4(1.0); color = glm::vec3(1.0f);
			model = glm::translate(model, glm::vec3(300.0f, -1.0f, 75.0f));
			modelaux = model;
			model = glm::rotate(model, 90.0f * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::rotate(model, 30.0f * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
			model = glm::scale(model, glm::vec3(30.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			glUniform3fv(uniformColor, 1, glm::value_ptr(color));
			roca2.RenderModel();

			EscenarioDebate::DibujarEscenario(uniformModel, uniformColor, uniformSpecularIntensity, uniformShininess, plainTexture, texEscalones, Material_opaco);
			Avatar::dibujar(uniformModel, uniformColor);
			EscenarioDebate::DibujarModelosDebate(uniformModel, uniformColor, uniformSpecularIntensity, uniformShininess, Podio_M, Kodos_M, Kang_M, NaveAlien_M, Homero_M, plainTexture, Material_brillante, tiempoVuelo, rayoEncendido, alturaAbduccion, giroAbduccion);
		}

		else if (escenarioActual == 4) {
			float dt4 = deltaTime * VELOCIDAD_RECORRIDO4;

			if (faseRecorrido4 == 0) {
				// FASE 1: VALLE (Piso + Plataforma + Cohete)
				model = glm::mat4(1.0);
				model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
				model = glm::scale(model, glm::vec3(200.0f, 1.0f, 200.0f));
				glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
				glUniform3fv(uniformColor, 1, glm::value_ptr(glm::vec3(1.0f)));
				pisoPlaneta1.UseTexture();
				Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
				meshListModel[2]->RenderMeshModel();

				cicloCohete += dt4;
				if (!coheteLanzando && cicloCohete >= CICLO_COHETE) { coheteLanzando = true; coheteVelocidad = 0.0f; }
				if (coheteLanzando) {
					coheteVelocidad += ACELERACION_COHETE * 3 * dt4;
					coheteAltura += coheteVelocidad * dt4;
					if (coheteAltura > ALTURA_MAX_COHETE) {
						coheteAltura = 0.0f; coheteVelocidad = 0.0f; coheteLanzando = false; cicloCohete = 0.0f;
					}
				}

				anguloCirculoHex += 16.0f * dt4;
				anguloSemiAro += 13.0f * dt4;
				float flotacionEsfera = sin((float)glfwGetTime() * 0.5f) * 0.2f;

				glm::mat4 modelBase = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
				modelBase = glm::scale(modelBase, glm::vec3(10.0f));
				glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelBase));
				BasePlataforma_M.RenderModel();

				glm::mat4 modelNucleo = glm::translate(modelBase, glm::vec3(0.0f, 0.41f, 0.0f));
				modelNucleo = glm::rotate(modelNucleo, glm::radians(anguloCirculoHex), glm::vec3(0.0f, 1.0f, 0.0f));
				glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelNucleo));
				NucleoPlataforma_M.RenderModel();

				glm::mat4 modelCirculo = glm::translate(modelBase, glm::vec3(0.0f, 0.85f, -0.41f));
				modelCirculo = glm::rotate(modelCirculo, glm::radians(anguloCirculoHex), glm::vec3(0.0f, 1.0f, 0.0f));
				glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelCirculo));
				CirculoHex_M.RenderModel();

				glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelBase));
				AndamioIzqPlataforma_M.RenderModel();

				glm::mat4 modelAndamioDer = glm::translate(modelBase, glm::vec3(-2.5f, 0.85f, 0.45f));
				glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelAndamioDer));
				AndamioDerPlataforma_M.RenderModel();

				glm::mat4 modelTorre = glm::translate(modelBase, glm::vec3(0.0f, 3.15f, 0.95f));
				glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelTorre));
				TorrePlataforma_M.RenderModel();

				glm::mat4 modelEsfera = glm::translate(modelTorre, glm::vec3(0.0f, 3.3f + flotacionEsfera, 0.0f));
				glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelEsfera));
				EsferaPlataforma_M.RenderModel();

				glm::mat4 modelSemiAro = glm::rotate(modelEsfera, glm::radians(anguloSemiAro), glm::vec3(0.0f, 1.0f, 1.0f));
				glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelSemiAro));
				SemiaroPlataforma_M.RenderModel();

				glm::mat4 modelCohete = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, coheteAltura + 15.0f, -7.0f));
				modelCohete = glm::scale(modelCohete, glm::vec3(5.0f));
				glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelCohete));
				if (Cohete_M.cargado()) Cohete_M.RenderModel(); else naveEspacial.RenderModel();
			}
			else {
				// FASE 2: ESPACIO (Estación + Naves + Celestes)
				glm::mat4 modelEstacionCabina = glm::translate(glm::mat4(1.0f), glm::vec3(200.0f, 80.0f, 150.0f));
				modelEstacionCabina = glm::rotate(modelEstacionCabina, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
				glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelEstacionCabina));
				EstacionCabina_M.RenderModel();

				glm::mat4 modelPuertaIzq = glm::translate(modelEstacionCabina, glm::vec3(-2.6f, 0.0f, -1.0f * aperturaPuertas));
				glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelPuertaIzq));
				EstacionPuertaIzq_M.RenderModel();

				glm::mat4 modelPuertaDer = glm::translate(modelEstacionCabina, glm::vec3(-2.6f, 0.0f, 1.0f * aperturaPuertas));
				glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelPuertaDer));
				EstacionPuertaDer_M.RenderModel();

				glm::mat4 modelEstructura = modelEstacionCabina;
				glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelEstructura));
				EstacionEstructura_M.RenderModel();

				glm::mat4 modelPanelDer = glm::translate(modelEstructura, glm::vec3(0.0f, 0.0f, 8.5f));
				glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelPanelDer));
				EstacionPanelDerecho_M.RenderModel();

				glm::mat4 modelPanelIzq = glm::translate(modelEstructura, glm::vec3(0.0f, 0.0f, -8.5f));
				glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelPanelIzq));
				EstacionPanelIzquierdo_M.RenderModel();

				glm::mat4 modelCapsDer = glm::translate(modelEstructura, glm::vec3(0.0f, 0.0f, 5.0f));
				glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelCapsDer));
				EstacionCapsulasDer_M.RenderModel();

				glm::mat4 modelCapsIzq = glm::translate(modelEstructura, glm::vec3(0.0f, 0.0f, -5.0f));
				glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelCapsIzq));
				EstacionCapsulasIzq_M.RenderModel();

				// Nave NPC Entrando/Saliendo
				float distanciaMax = 60.0f;
				float escalaNave = 2.0f;
				glm::vec3 posicionPuerta(-2.6f, 0.0f, 0.0f);
				float distancia = distanciaMax * progresoNaveNPC;
				glm::vec3 offsetNave = posicionPuerta + glm::vec3(-distancia, 0.0f, 0.0f);

				if (progresoNaveNPC > 0.02f) {
					float factorEscala = 1.0f;
					float angulonave = 0.0f;
					if (progresoNaveNPC < 0.15f) factorEscala = progresoNaveNPC / 0.15f;
					if (progresoNaveNPC > 0.85f) factorEscala = (1.0f - progresoNaveNPC) / 0.15f;

					if (tiempoRecorrido4 < 22.0f) angulonave = 90.0f;
					if (tiempoRecorrido4 > 22.0f) angulonave = 270.0f;

					glm::mat4 modelNave = glm::translate(modelEstacionCabina, offsetNave);
					modelNave = glm::rotate(modelNave, glm::radians(angulonave), glm::vec3(0.0f, 1.0f, 0.0f));
					modelNave = glm::scale(modelNave, glm::vec3(escalaNave * factorEscala));
					glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelNave));
					naveEspacial.RenderModel();
				}

				// Objetos celestes de fondo
				model = glm::translate(glm::mat4(1.0f), glm::vec3(500.0f, 150.0f, -400.0f));
				model = glm::rotate(model, rotaPlanetas, glm::vec3(0.0f, -1.0f, 0.0f));
				model = glm::scale(model, glm::vec3(80.0f));
				glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
				sol.RenderModel();

				model = glm::translate(glm::mat4(1.0f), glm::vec3(-400.0f, -50.0f, -500.0f));
				model = glm::rotate(model, rotaPlanetas, glm::vec3(0.0f, 1.0f, 0.0f));
				model = glm::scale(model, glm::vec3(60.0f));
				glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
				planeta1.RenderModel();

				model = glm::translate(glm::mat4(1.0f), glm::vec3(400.0f, -50.0f, -500.0f));
				model = glm::rotate(model, rotaPlanetas, glm::vec3(0.0f, 1.0f, 0.0f));
				model = glm::scale(model, glm::vec3(60.0f));
				glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
				planeta2.RenderModel();

				model = glm::translate(glm::mat4(1.0f), glm::vec3(400.0f, -50.0f, -500.0f));
				model = glm::rotate(model, 90.0f * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
				model = glm::rotate(model, rotaAnillo, glm::vec3(0.0f, 0.0f, 1.0f));
				model = glm::scale(model, glm::vec3(3.0f));
				glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
				anilloAst.RenderModel();

				model = glm::translate(glm::mat4(1.0f), glm::vec3(400.0f, -50.0f, -500.0f));
				model = glm::rotate(model, rotaLuna, glm::vec3(0.0f, -1.0f, 0.0f));
				model = glm::translate(model, glm::vec3(120.0f, 0.0f, 0.0f));
				model = glm::scale(model, glm::vec3(15.0f));
				glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
				luna.RenderModel();
			}
		}

		glUseProgram(0);

		if (verificarTodo) {
			if (framePrueba == 0) ValleHelado::captura("x64/Practica9/escena1.ppm");
			if (framePrueba == 61) ValleHelado::captura("x64/Practica9/escena3.ppm");
			if (++framePrueba > 62) break;
		}

		mainWindow.swapBuffers();
	}

	return falloGL ? 3 : 0;
}