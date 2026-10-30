/*
Proyecto final
*/
//para cargar imagen
#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>
#include <math.h>

#include <glew.h>
#include <glfw3.h>

#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>
//para probar el importer
//#include<assimp/Importer.hpp>

#include "Window.h"
#include "Mesh_tn.h"
#include "Shader_light.h"
#include "Camera.h"
#include "Texture.h"
//#include "Sphere.h"
#include"Model.h"
#include "Skybox.h"

//para iluminación
#include "CommonValues.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Material.h"
const float toRadians = 3.14159265f / 180.0f;

Window mainWindow;
std::vector<MeshModel*> meshListModel; // recibe xyz uv nx ny nz
std::vector<Shader> shaderList;

Camera camera;

Texture brickTexture;
Texture dirtTexture;
Texture plainTexture;
Texture pisoTexture;

// Añadir Texturas a usar
float rotaPlanetas = 0.0f;
float rotaPlanetasOffset = 0.005f;

float rotaAnillo = 0.0f;
float rotaAnilloOffset = 0.0025f;

float rotaLuna = 0.0f;
float rotaLunaOffset = 0.025f;
// ----- Lista de Modelos a importar -----

// Rocky 
Model rocky;

Model cuerpoRocky; 
Model brazoDer; 
Model brazoIzq; 
Model piernaIzq; 
Model piernaDer;
Model piernaCen;

//Añadir modelo del avión. 
Model Avion_M; 

// Modelos del espacio
Model sol;
Model luna;
Model anilloAst;
Model planeta1;
Model planeta2;

// Skybox de nuestros escenarios
Skybox skybox1;		// Skybox del espacio
Skybox skybox2;		// Skybox del planeta 1
Skybox skybox3;		// Skybox del planeta 2

int escenarioActual = 1;	// Escenario actual a mostrar (1, 2 o 3)

// Variables para animación


//materiales
Material Material_brillante;
Material Material_opaco;


//Sphere cabeza = Sphere(0.5, 20, 20);
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;

// luz direccional
DirectionalLight mainLight;
//para declarar varias luces de tipo pointlight
PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];

// Vertex Shader
static const char* vShader = "shaders/shader_light.vert";

// Fragment Shader
static const char* fShader = "shaders/shader_light.frag";


//función de calculo de normales por promedio de vértices 
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
		//	x      y      z			u	  v			nx	  ny    nz
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
	   4,5,6,
	   4,6,7
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
	
	MeshModel *obj1 = new MeshModel();
	obj1->CreateMeshModel(vertices, indices, 32, 12);
	meshListModel.push_back(obj1);

	MeshModel *obj2 = new MeshModel();
	obj2->CreateMeshModel(vertices, indices, 32, 12);
	meshListModel.push_back(obj2);

	MeshModel *obj3 = new MeshModel();
	obj3->CreateMeshModel(floorVertices, floorIndices, 32, 6);
	meshListModel.push_back(obj3);

	MeshModel *obj4 = new MeshModel();
	obj4->CreateMeshModel(vegetacionVertices, vegetacionIndices, 64, 12);
	meshListModel.push_back(obj4);

	calcAverageNormals(indices, 12, vertices, 32, 8, 5);

	calcAverageNormals(vegetacionIndices, 12, vegetacionVertices, 64, 8, 5);

}


void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}





int main()
{
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();
	CreateObjects();
	CreateShaders();

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 1.0f, 0.5f);

	brickTexture = Texture("Textures/brick.png");
	brickTexture.LoadTextureA();
	dirtTexture = Texture("Textures/dirt.png");
	dirtTexture.LoadTextureA();
	plainTexture = Texture("Textures/plain.png");
	plainTexture.LoadTextureA();
	pisoTexture = Texture("Textures/piso.tga");
	pisoTexture.LoadTextureA();

	// ----- Carga de modelos -----
	// Rocky 
	rocky = Model();
	rocky.LoadModel("Models/rockyPersonaje.obj");

	cuerpoRocky = Model();
	cuerpoRocky.LoadModel("Models/cuerpoRocky.obj");

	brazoIzq = Model();
	brazoIzq.LoadModel("Models/brazoIzqRocky.obj");

	brazoDer = Model();
	brazoDer.LoadModel("Models/brazoDerRocky.obj");

	piernaIzq = Model();
	piernaIzq.LoadModel("Models/piernaIzqRocky.obj");

	piernaDer = Model();
	piernaDer.LoadModel("Models/piernaDerRocky.obj");

	piernaCen = Model();
	piernaCen.LoadModel("Models/piernaCenRocky.obj");

	// Modelos del espacio
	sol = Model();
	sol.LoadModel("Models/sol.obj");

	luna = Model();
	luna.LoadModel("Models/luna.obj");

	anilloAst = Model();
	anilloAst.LoadModel("Models/anilloAst.obj");

	planeta1 = Model();
	planeta1.LoadModel("Models/planeta1.obj");

	planeta2 = Model();
	planeta2.LoadModel("Models/planeta2.obj");

	// ----- Configuración Skybox Escenario 1 -----
	std::vector<std::string> skyboxFaces;
	skyboxFaces.push_back("Textures/Skybox/espacio_rt.tga");
	skyboxFaces.push_back("Textures/Skybox/espacio_lf.tga");
	skyboxFaces.push_back("Textures/Skybox/espacio_dn.tga");
	skyboxFaces.push_back("Textures/Skybox/espacio_up.tga");
	skyboxFaces.push_back("Textures/Skybox/espacio_bk.tga");
	skyboxFaces.push_back("Textures/Skybox/espacio_ft.tga");

	skybox1 = Skybox(skyboxFaces);
	
	// ----- Configuración Skybox Escenario 2 (Planeta 1)-----
	std::vector<std::string> skyboxFaces2;
	skyboxFaces2.push_back("Textures/Skybox/planeta1_rt.tga"); // Cambia estas rutas por tus texturas
	skyboxFaces2.push_back("Textures/Skybox/planeta1_lf.tga");
	skyboxFaces2.push_back("Textures/Skybox/planeta1_dn.tga");
	skyboxFaces2.push_back("Textures/Skybox/planeta1_up.tga");
	skyboxFaces2.push_back("Textures/Skybox/planeta1_bk.tga");
	skyboxFaces2.push_back("Textures/Skybox/planeta1_ft.tga");
	skybox2 = Skybox(skyboxFaces2);
	/*
	// ----- Configuración Skybox Escenario 3 (Planeta 2) -----
	std::vector<std::string> skyboxFaces3;
	skyboxFaces3.push_back("Textures/Skybox/espacio_rt.tga"); // Cambia estas rutas por tus texturas
	skyboxFaces3.push_back("Textures/Skybox/espacio_lf.tga");
	skyboxFaces3.push_back("Textures/Skybox/espacio_dn.tga");
	skyboxFaces3.push_back("Textures/Skybox/espacio_up.tga");
	skyboxFaces3.push_back("Textures/Skybox/espacio_bk.tga");
	skyboxFaces3.push_back("Textures/Skybox/espacio_ft.tga");
	skybox3 = Skybox(skyboxFaces3);
	*/
	Material_brillante = Material(4.0f, 256);
	Material_opaco = Material(0.3f, 4);


	//luz direccional, sólo 1 y siempre debe de existir (Luz del Sol)
	mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,
		0.3f, 0.3f,
		0.0f, -1.0f, 0.0f); // intensidad ambiental y difusa (0.3 y 0.3)

	//contador de luces puntuales
	unsigned int pointLightCount = 0;

	//Declaración de primer luz puntual
	pointLights[0] = PointLight(1.0f, 0.5f, 0.0f,
		5.0f, 5.0f,				// radiacion y color (0.3f y 0.6f)
		150.0f, 25.0f, 0.0f,
		0.3f, 0.002f, 0.001f); 
	pointLightCount++;

	//Declaración de primer luz puntual
	pointLights[0] = PointLight(1.0f, 1.0f, 1.0f,
		0.8f, 0.8f,				// radiacion y color (0.3f y 0.6f)
		0.0f, 0.0f, 150.0f,
		0.3f, 0.2f, 0.01f);
	pointLightCount++;

	unsigned int spotLightCount = 0;
	
	//linterna
	spotLights[0] = SpotLight(1.0f, 1.0f, 1.0f,
		0.0f, 2.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		5.0f);
	spotLightCount++;

	// luz infinita -> por eso no vemos cambios en lo que ilumina la luz, ya que al estar tan cerca del piso, no cambia la iluminacion
	
	//se crean mas luces puntuales y spotlight 

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);
	
	// Matrices y posiciones de las luces para la jerarquia
	glm::mat4 roverMatrix(1.0f);
	glm::mat4 avionMatrix(1.0f);

	glm::vec3 roverPos;
	glm::vec3 roverDir;

	glm::vec3 avionFrentPos;
	glm::vec3 avionFrentDir;

	glm::vec3 avionAbajoPos;
	glm::vec3 avionAbajoDir;
	
	////Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;

		//Recibir eventos del usuario
		glfwPollEvents();
		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		// Después de glfwPollEvents(); y la actualización de cámara
		if (mainWindow.getsKeys()[GLFW_KEY_1]) escenarioActual = 1;
		if (mainWindow.getsKeys()[GLFW_KEY_2]) escenarioActual = 2;
		//if (mainWindow.getsKeys()[GLFW_KEY_3]) escenarioActual = 3;

		// Clear the window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Lógica de Renderizado por Escenario
		if (escenarioActual == 1) {
			skybox1.DrawSkybox(camera.calculateViewMatrix(), projection);
		}
		else if (escenarioActual == 2) {
			skybox2.DrawSkybox(camera.calculateViewMatrix(), projection);
		}
		//else if (escenarioActual == 3) {
		//	skybox3.DrawSkybox(camera.calculateViewMatrix(), projection);
		//}

		// Configuración de los Shaders de Iluminación 
		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformEyePosition = shaderList[0].GetEyePositionLocation();
		uniformColor = shaderList[0].getColorLocation();

		//información en el shader de intensidad especular y brillo
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		// luz ligada a la cámara de tipo flash
		//sirve para que en tiempo de ejecución (dentro del while) se cambien propiedades de la luz
		glm::vec3 lowerLight = camera.getCameraPosition();
		lowerLight.y -= 0.3f;
		spotLights[0].SetFlash(lowerLight, camera.getCameraDirection());

		
		//información al shader de fuentes de iluminación
		shaderList[0].SetDirectionalLight(&mainLight);
		shaderList[0].SetPointLights(pointLights, pointLightCount);
		shaderList[0].SetSpotLights(spotLights, spotLightCount);

		glm::mat4 model(1.0);
		glm::mat4 modelaux(1.0);
		glm::mat4 modelaux2(1.0); // para la jerarquía de la base del rover
		glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);

		// Dibujado de objetos según el escenario
		if (escenarioActual == 1) {

			// Animación básica de los planetas
			rotaPlanetas += rotaPlanetasOffset * deltaTime;
			if (rotaPlanetas >= 360.0f * toRadians) {
				rotaPlanetas = 0.0f;
			}

			// Animación básica de los anillos de asteroides
			rotaAnillo += rotaAnilloOffset * deltaTime;

			// Animación de la luna orbitando alrededor del planeta 1
			rotaLuna += rotaLunaOffset * deltaTime;

			if (rotaLuna >= 360.0f * toRadians) {
				rotaLuna = 0.0f;
			}

			// Sol
			model = glm::mat4(1.0);
			color = glm::vec3(1.0f, 1.0f, 1.0f);
			model = glm::translate(model, glm::vec3(150.0f, 25.0f, 0.0f));
			model = glm::rotate(model, rotaPlanetas, glm::vec3(0.0f, -1.0f, 0.0f));
			model = glm::scale(model, glm::vec3(75.0f, 75.0f, 75.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			glUniform3fv(uniformColor, 1, glm::value_ptr(color));
			sol.RenderModel();

			// Planeta 1
			model = glm::mat4(1.0);
			color = glm::vec3(1.0f, 1.0f, 1.0f);
			model = glm::translate(model, glm::vec3(0.0f, 0.0f, 175.0f));
			model = glm::rotate(model, rotaPlanetas, glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::scale(model, glm::vec3(30.0f, 30.0f, 30.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			glUniform3fv(uniformColor, 1, glm::value_ptr(color));
			planeta1.RenderModel();

			// Luna
			model = glm::mat4(1.0f);
			color = glm::vec3(1.0f, 1.0f, 1.0f);
			model = glm::translate(model, glm::vec3(0.0f, 0.0f, 175.0f));
			model = glm::rotate(model, rotaLuna, glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::translate(model, glm::vec3(50.0f, 0.0f, 0.0f));
			model = glm::rotate(model, rotaLuna * 2.0f, glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			glUniform3fv(uniformColor, 1, glm::value_ptr(color));
			luna.RenderModel();

			// Planeta 2
			model = glm::mat4(1.0);
			color = glm::vec3(1.0f, 1.0f, 1.0f);
			model = glm::translate(model, glm::vec3(0.0f, 0.0f, -175.0f));
			model = glm::rotate(model, rotaPlanetas, glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::scale(model, glm::vec3(30.0f, 30.0f, 30.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			glUniform3fv(uniformColor, 1, glm::value_ptr(color));
			planeta2.RenderModel();

			// Cinturón de asteroides 1
			model = glm::mat4(1.0);
			color = glm::vec3(1.0f, 1.0f, 1.0f);
			model = glm::translate(model, glm::vec3(0.0f, 0.0f, -175.0f));
			model = glm::rotate(model, 90.0f * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::rotate(model, rotaAnillo, glm::vec3(0.0f, 0.0f, 1.0f));
			model = glm::scale(model, glm::vec3(1.05f, 1.05f, 1.05f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			glUniform3fv(uniformColor, 1, glm::value_ptr(color));
			anilloAst.RenderModel();

			// Cinturón de asteroides 2
			model = glm::mat4(1.0);
			color = glm::vec3(1.0f, 1.0f, 1.0f);
			model = glm::translate(model, glm::vec3(0.0f, 0.0f, -175.0f));
			model = glm::rotate(model, 75.0f * toRadians, glm::vec3(-1.0f, 0.0f, 0.0f));
			model = glm::rotate(model, rotaAnillo, glm::vec3(0.0f, 0.0f, 1.0f));
			model = glm::scale(model, glm::vec3(1.15f, 1.15f, 1.15f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			glUniform3fv(uniformColor, 1, glm::value_ptr(color));
			anilloAst.RenderModel();

		}
		else if (escenarioActual == 2) {
			// Coloca aquí los modelos, texturas y posiciones para tu Escenario 2

		}
		else if (escenarioActual == 3) {
			// Dibujar Piso 
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
			model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			glUniform3fv(uniformColor, 1, glm::value_ptr(color));

			pisoTexture.UseTexture();
			Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
			meshListModel[2]->RenderMeshModel();

			// Dibujar a Rocky (Solo en escenario 1)
			model = glm::mat4(1.0);
			color = glm::vec3(1.0f, 1.0f, 1.0f);
			model = glm::translate(model, glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::rotate(model, 90.0f * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::scale(model, glm::vec3(20.0f, 20.0f, 20.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			glUniform3fv(uniformColor, 1, glm::value_ptr(color));
			rocky.RenderModel();
		}


	
		/*
		//blending: transparencia o traslucidez
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshListModel[3]->RenderMeshModel();
		glDisable(GL_BLEND);
		*/

		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}

/*
	Ia = intensidad ambiental (radiación)
	Id = intensidad difusa (color -> que tan fuerte se va a ver) -> multiplica color * Id

	Luz direccional -> luz solar
		dirección: dónde empieza iluminando y donde lo vemos

	Luz puntual -> podemos tener 1 o + luces
		atenuación (x = distancia) -> Ax2 + Bx + C -> que tan lejos va a iluminar la luz puntual -> debe ser distinto de cero para que 
			no haya alguna indeterminación

	Luz spotlight -> linterna de mano
		orientación -> 
		apretura -> que tanta área ilumina de acuerdo a la dirección 

	intensidad especular -> brillo y reflejo de la luz 
		depende la posición de la cámara (ej. el brillo del proyector reflejado en el pizarrón)
	
	Material -> intensidad especular (Is) y shininess SOLO SE APLICA PARA MODELOS Y OBJETOS

	variables uniform son procesadas por shader

	shader_light se encian todas las variables unifor y sus funciones get

	OpenGL limita a tener 8 o 16 luces en simultáneo dependiento el hardware (donde debe existir al menos una luz de cada tipo)
*/