/*
Práctica 7: Iluminación 1 
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
Texture AgaveTexture;

// Añadir Texturas a usar


// ----- Lista de Modelos a importar -----

// Rocky 
Model baseRover; 
Model pinza; 
Model brazo1; 
Model brazo2; 
Model baseBrazo; 
Model llantaDelanteraCerca; 
Model llantaDelanteraLejos; 
Model llantaMedioCerca; 
Model llantaMedioLejos; 
Model llantaAtrasCerca; 
Model llantaAtrasLejos; 
//Añadir modelo del avión. 
Model Avion_M; 


Skybox skybox;

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

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.3f, 0.5f);

	brickTexture = Texture("Textures/brick.png");
	brickTexture.LoadTextureA();
	dirtTexture = Texture("Textures/dirt.png");
	dirtTexture.LoadTextureA();
	plainTexture = Texture("Textures/plain.png");
	plainTexture.LoadTextureA();
	pisoTexture = Texture("Textures/piso.tga");
	pisoTexture.LoadTextureA();
	AgaveTexture = Texture("Textures/Agave.tga");
	AgaveTexture.LoadTextureA();

	// ----- Carga de modelos -----
	// Rocky 
	baseRover = Model();
	baseRover.LoadModel("Models/baseRover.fbx");

	baseBrazo = Model();
	baseBrazo.LoadModel("Models/baseBrazo.fbx");

	brazo1 = Model();
	brazo1.LoadModel("Models/brazo1.fbx");

	brazo2 = Model();
	brazo2.LoadModel("Models/brazo2.fbx");

	pinza = Model();
	pinza.LoadModel("Models/pinza.fbx");

	llantaDelanteraCerca = Model();
	llantaDelanteraCerca.LoadModel("Models/llantaDelanteraCerca.fbx");

	llantaDelanteraLejos = Model();
	llantaDelanteraLejos.LoadModel("Models/llantaDelanteraLejos.fbx");

	llantaMedioCerca = Model();
	llantaMedioCerca.LoadModel("Models/llantaMedioCerca.fbx");

	llantaMedioLejos = Model();
	llantaMedioLejos.LoadModel("Models/llantaMedioLejos.fbx");

	llantaAtrasCerca = Model();
	llantaAtrasCerca.LoadModel("Models/llantaAtrasCerca.fbx");

	llantaAtrasLejos = Model();
	llantaAtrasLejos.LoadModel("Models/llantaAtrasLejos.fbx");


	std::vector<std::string> skyboxFaces;
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_rt.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_lf.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_dn.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_up.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_bk.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_ft.tga");

	skybox = Skybox(skyboxFaces);

	Material_brillante = Material(4.0f, 256);
	Material_opaco = Material(0.3f, 4);


	//luz direccional, sólo 1 y siempre debe de existir (Luz del Sol)
	mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,
		0.3f, 0.6f,
		0.0f, -1.0f, 0.0f); // intensidad ambiental y difusa (0.3 y 0.3)

	//contador de luces puntuales
	unsigned int pointLightCount = 0;

	//Declaración de primer luz puntual
	pointLights[0] = PointLight(1.0f, 0.0f, 0.0f,
		0.3f, 0.6f,				// radiacion y color (0.3f y 0.6f)
		-6.0f, 1.5f, 1.5f,
		0.3f, 0.2f, 0.1f); 
	pointLightCount++;

	/*
	// luz amarilla amarilla arriba del helicoptero
	pointLights[1] = PointLight(1.0f, 1.0f, 0.0f,
		0.1f, 0.3f,
		0.0f, 6.0f, 6.0f,
		0.3f, 0.2f, 0.1f); // radiacion y color (0.3f y 0.6f)
	// tomamos la posicion del helicoptero y subimos un poco más en Y
	pointLightCount++;
	*/
	unsigned int spotLightCount = 0;
	
	//linterna
	spotLights[0] = SpotLight(1.0f, 1.0f, 1.0f,
		0.0f, 2.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		5.0f);
	spotLightCount++;
	
	/*/luz fija
	spotLights[0] = SpotLight(0.0f, 1.0f, 0.0f,
		0.3f, 0.3f,
		5.0f, 10.0f, 0.0f,
		0.0f, -5.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		15.0f); // Circunferencia
	spotLightCount++;
	/*
	//luz azul
	spotLights[2] = SpotLight(0.0f, 0.0f, 1.0f,
		0.3f, 0.3f,
		-6.0f, 8.0f, 6.0f,
		0.0f, -5.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		15.0f); // Circunferencia
	spotLightCount++;
	*/

	// 1. Faro Frontal Rover (Índice 1)
	spotLights[1] = SpotLight(1.0f, 1.0f, 1.0f,
		1.0f, 1.0f,
		0.0f, 2.8f, 0.0f, 
		1.0f, 0.0f, 0.0f, // Dirección hacia adelante (+X)
		1.0f, 0.0f, 0.0f, 
		10.0f); // Apertura del faro
	spotLightCount++;
	
	// 2. Faro hacia abajo del avioncito (Índice 2)
	spotLights[2] = SpotLight(1.0f, 1.0f, 0.8f, // Color Blanco un poco más cálido para diferenciar la tonalidad
		1.0f, 1.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f, // Dirección hacia abajo (-Y)
		1.0f, 0.0f, 0.0f,
		15.0f);
	spotLightCount++;

	// 3. Faro frontal del avioncito (Índice 3)
	spotLights[3] = SpotLight(1.0f, 1.0f, 1.0f,
		1.0f, 1.0f,
		0.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f, // Dirección hacia adelante (+X)
		1.0f, 0.0f, 0.0f,
		20.0f);
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

		// Clear the window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		skybox.DrawSkybox(camera.calculateViewMatrix(), projection);
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

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));

		pisoTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshListModel[2]->RenderMeshModel();
		
		// ----- Rocky -----
		// --- Cuerpo ---
		model = glm::mat4(1.0);
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		model = glm::translate(model, glm::vec3(1.0f + mainWindow.getmuevex(), -1.0f, 6.0));
		model = glm::translate(model, glm::vec3(0.0f, 2.8f, 10.0f));
		modelaux = model;
		modelaux2 = model;

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		baseRover.RenderModel();


		// --- Brazos ----
		// Base del brazo
		model = modelaux;
		model = glm::translate(model, glm::vec3(1.75f, 1.5f, -1.0f));
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		baseBrazo.RenderModel();

		// Brazo 1 (conectado a la base del brazo)
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::rotate(model, glm::radians(mainWindow.getarticulacion1()), glm::vec3(0.0f, 0.0f, 1.0f));
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		brazo1.RenderModel();

		// Brazo 2 (conectado al brazo 1)
		model = modelaux;
		model = glm::translate(model, glm::vec3(2.45f, 2.5f, -0.01f));
		//model = glm::rotate(model, glm::radians(mainWindow.getarticulacion2()), glm::vec3(0.0f, 0.0f, 1.0f));
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		brazo2.RenderModel();

		// Pinza (conectado al brazo 2)
		model = modelaux;
		model = glm::translate(model, glm::vec3(-3.4f, 3.3f, 0.125f));
		//model = glm::rotate(model, glm::radians(mainWindow.getarticulacion3()), glm::vec3(0.0f, 0.0f, 1.0f));
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		pinza.RenderModel();

		//	Llanta delantera derecha
		model = modelaux2;
		model = glm::translate(model, glm::vec3(2.075f, 1.15f, 3.07f));
		//model = glm::rotate(model, glm::radians(mainWindow.getarticulacion4()), glm::vec3(0.0f, 0.0f, 1.0f));
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		llantaDelanteraLejos.RenderModel();

		//	Llanta delantera izquierda
		model = modelaux2;
		model = glm::translate(model, glm::vec3(2.075f, 1.14f, -3.05f));
		//model = glm::rotate(model, glm::radians(mainWindow.getarticulacion5()), glm::vec3(0.0f, 0.0f, 1.0f));
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		llantaDelanteraCerca.RenderModel();

		//Llanta media derecha
		model = modelaux2;
		model = glm::translate(model, glm::vec3(-2.5f, 0.25f, 3.07f));
		//model = glm::rotate(model, glm::radians(mainWindow.getarticulacion6()), glm::vec3(0.0f, 0.0f, 1.0f));
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		llantaMedioLejos.RenderModel();

		// Llanta media izquierda
		model = modelaux2;
		model = glm::translate(model, glm::vec3(-2.45f, 0.25f, -3.07f));
		//model = glm::rotate(model, glm::radians(mainWindow.getarticulacion7()), glm::vec3(0.0f, 0.0f, 1.0f));
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		llantaMedioCerca.RenderModel();

		//Llanta trasera derecha
		model = modelaux2;
		model = glm::translate(model, glm::vec3(-3.5f, 0.25f, 3.07f));
		//model = glm::rotate(model, glm::radians(mainWindow.getarticulacion8()), glm::vec3(0.0f, 0.0f, 1.0f));
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		llantaAtrasLejos.RenderModel();

		//	Llanta trasera izquierda
		model = modelaux2;
		model = glm::translate(model, glm::vec3(-3.55f, 0.25f, -3.07f));
		//model = glm::rotate(model, glm::radians(mainWindow.getarticulacion9()), glm::vec3(0.0f, 0.0f, 1.0f));
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		llantaAtrasCerca.RenderModel();
		 

		//Agave ¿qué sucede si lo renderizan antes del helicóptero?
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		
		//blending: transparencia o traslucidez
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		AgaveTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshListModel[3]->RenderMeshModel();
		glDisable(GL_BLEND);

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