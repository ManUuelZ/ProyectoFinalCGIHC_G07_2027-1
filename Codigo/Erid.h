#pragma once
#include <vector>
#include <string>
#include <cmath>
#include <cstdio>
#include <glew.h>
#include <glfw3.h>
#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>
#include "CommonValues.h"
#include "Mesh_tn.h"
#include "Shader_light.h"
#include "Camera.h"
#include "Texture.h"
#include "Model.h"
#include "Skybox.h"
#include "Material.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "EscenarioDebate.h"

// Escenario 3: Planeta 2 (Erid) con Rocky, rocas y el escenario del Debate.
// Misma estructura que ValleHelado: iniciar() una vez, entrar() al cambiar de escenario, dibujar() cada frame.
namespace Erid {
    // Modelos de Erid
    inline Model rocky;
    inline Model brazoRocky;
    inline Model rocas1;
    // Modelos del Debate
    inline Model podio;
    inline Model kang;
    inline Model kodos;
    inline Model nave;

    inline MeshModel piso;
    inline Texture texPiso;
    inline Texture texPlain;
    inline Material matOpaco;
    inline Material matBrillante;
    inline Skybox cielo;
    inline Shader shader;

    // Luces propias del escenario
    inline DirectionalLight luzDireccional;
    inline PointLight puntuales[MAX_POINT_LIGHTS];
    inline SpotLight spots[MAX_SPOT_LIGHTS];
    inline unsigned int spotCount = 0;

    // Animación de Rocky saludando
    inline float rotBrazoRocky = 0.0f;
    inline float rotBrazoRockyOffset = 0.1f;   // grados por frame a 60 fps
    inline bool saludo = true;

    inline double tiempo = 0;
    inline bool estabaActivo = false;
    inline const float aRadianes = 3.14159265f / 180.0f;

    inline bool cargar(Model& destino, const char* ruta) {
        destino.LoadModel(ruta);
        if (!destino.cargado()) {
            printf("No se encontro el modelo requerido: %s\n", ruta);
            return false;
        }
        return true;
    }

    inline bool iniciar() {
        // Modelos (ajusta las rutas si tus archivos se llaman distinto)
        if (!cargar(rocky, "Models/rockyPersonaje.obj")) return false;
        if (!cargar(brazoRocky, "Models/rockyBrazo.obj")) return false;
        if (!cargar(rocas1, "Models/rocas1.obj")) return false;
        if (!cargar(podio, "Models/podio.obj")) return false;
        if (!cargar(kang, "Models/kang.obj")) return false;
        if (!cargar(kodos, "Models/kodos.obj")) return false;
        if (!cargar(nave, "Models/naveAlien.obj")) return false;

        // Texturas
        texPiso = Texture("Textures/pisoPlaneta2.jpg");
        if (!texPiso.LoadTextureA()) return false;
        texPlain = Texture("Textures/plain.png");
        if (!texPlain.LoadTextureA()) return false;

        // Skybox de Erid
        std::vector<std::string> caras;
        caras.push_back("Textures/Skybox/erid_right.jpg");
        caras.push_back("Textures/Skybox/erid_left.jpg");
        caras.push_back("Textures/Skybox/erid_bottom.jpg");
        caras.push_back("Textures/Skybox/erid_top.jpg");
        caras.push_back("Textures/Skybox/erid_back.jpg");
        caras.push_back("Textures/Skybox/erid_front.jpg");
        cielo = Skybox(caras);

        // Shader de iluminación propio del escenario
        shader.CreateFromFiles("shaders/shader_light.vert", "shaders/shader_light.frag");

        matOpaco = Material(0.3f, 4);
        matBrillante = Material(4.0f, 256);

        // Piso (20 x 20, se escala al dibujar)
        unsigned int indicesPiso[] = { 0, 2, 1,  1, 2, 3 };
        GLfloat verticesPiso[] = {
            -10.0f, 0.0f, -10.0f,   0.0f,  0.0f,   0.0f, -1.0f, 0.0f,
             10.0f, 0.0f, -10.0f,  10.0f,  0.0f,   0.0f, -1.0f, 0.0f,
            -10.0f, 0.0f,  10.0f,   0.0f, 10.0f,   0.0f, -1.0f, 0.0f,
             10.0f, 0.0f,  10.0f,  10.0f, 10.0f,   0.0f, -1.0f, 0.0f
        };
        piso.CreateMeshModel(verticesPiso, indicesPiso, 32, 6);

        // Luces: direccional + linterna (0) + 2 reflectores y rayo abductor (1, 2, 3)
        luzDireccional = DirectionalLight(1.0f, 1.0f, 1.0f,
            0.25f, 0.25f,
            1.0f, -1.0f, 0.0f);

        spots[0] = SpotLight(1.0f, 1.0f, 1.0f,
            0.0f, 2.0f,
            0.0f, 0.0f, 0.0f,
            0.0f, -1.0f, 0.0f,
            1.0f, 0.0f, 0.0f,
            5.0f);
        spotCount = 1;
        EscenarioDebate::AgregarSpotLights(spots, spotCount);
        return true;
    }

    inline void entrar(Camera& camera) {
        // Yaw -90 mira hacia -Z, donde está el estrado del debate
        camera = Camera(glm::vec3(0.0f, 5.0f, 50.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f, 1.5f, 0.5f);
        glfwSetWindowTitle(glfwGetCurrentContext(), "Erid - Debate | 1 Espacio | 2 Valle | 3 Erid");
    }

    inline void dibujar(Camera& camera, const glm::mat4& projection, double dt) {
        if (!estabaActivo) entrar(camera);
        estabaActivo = true;
        if (std::isfinite(dt) && dt > 0) tiempo += dt;
        const float t = static_cast<float>(tiempo);

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 view = camera.calculateViewMatrix();
        cielo.DrawSkybox(view, projection);

        shader.UseShader();
        GLuint uModel = shader.GetModelLocation();
        GLuint uProjection = shader.GetProjectionLocation();
        GLuint uView = shader.GetViewLocation();
        GLuint uEye = shader.GetEyePositionLocation();
        GLuint uColor = shader.getColorLocation();
        GLuint uSpecular = shader.GetSpecularIntensityLocation();
        GLuint uShininess = shader.GetShininessLocation();

        glUniformMatrix4fv(uProjection, 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(uView, 1, GL_FALSE, glm::value_ptr(view));
        glUniform3f(uEye, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

        // Linterna pegada a la cámara y rayo abductor siguiendo a la nave
        glm::vec3 bajo = camera.getCameraPosition();
        bajo.y -= 0.3f;
        spots[0].SetFlash(bajo, camera.getCameraDirection());
        EscenarioDebate::ActualizarLuzRayo(spots, t);

        shader.SetDirectionalLight(&luzDireccional);
        shader.SetPointLights(puntuales, 0);
        shader.SetSpotLights(spots, spotCount);

        // Animación de Rocky saludando (independiente de los FPS)
        float paso = rotBrazoRockyOffset * 60.0f * ((std::isfinite(dt) && dt > 0) ? static_cast<float>(dt) : 0.0f);
        if (saludo) {
            rotBrazoRocky += paso;
            if (rotBrazoRocky >= 30.0f) saludo = false;
        }
        else {
            rotBrazoRocky -= paso;
            if (rotBrazoRocky <= -30.0f) saludo = true;
        }

        glm::mat4 model(1.0f);
        glm::mat4 modelaux(1.0f);
        glm::vec3 color(1.0f, 1.0f, 1.0f);

        // Piso
        model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(100.0f, 1.0f, 100.0f));
        glUniformMatrix4fv(uModel, 1, GL_FALSE, glm::value_ptr(model));
        glUniform3fv(uColor, 1, glm::value_ptr(color));
        texPiso.UseTexture();
        matOpaco.UseMaterial(uSpecular, uShininess);
        piso.RenderMeshModel();

        // Rocas alrededor
        model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -5.0f, 0.0f));
        model = glm::rotate(model, 90.0f * aRadianes, glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(150.0f, 150.0f, 150.0f));
        glUniformMatrix4fv(uModel, 1, GL_FALSE, glm::value_ptr(model));
        glUniform3fv(uColor, 1, glm::value_ptr(color));
        rocas1.RenderModel();

        // Rocky
        model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 1.25f, 0.0f));
        modelaux = model;
        model = glm::rotate(model, 90.0f * aRadianes, glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(25.0f, 25.0f, 25.0f));
        glUniformMatrix4fv(uModel, 1, GL_FALSE, glm::value_ptr(model));
        glUniform3fv(uColor, 1, glm::value_ptr(color));
        rocky.RenderModel();

        // Brazo de Rocky (jerarquía)
        model = modelaux;
        model = glm::translate(model, glm::vec3(-0.6f, 1.35f, 2.4f));
        model = glm::rotate(model, rotBrazoRocky * aRadianes, glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::rotate(model, 180.0f * aRadianes, glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::rotate(model, 90.0f * aRadianes, glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(25.0f, 25.0f, 25.0f));
        glUniformMatrix4fv(uModel, 1, GL_FALSE, glm::value_ptr(model));
        glUniform3fv(uColor, 1, glm::value_ptr(color));
        brazoRocky.RenderModel();

        // Escenario del Debate: estructura, podios, Kang, Kodos, nave y rayo (el cono translúcido va al final)
        EscenarioDebate::DibujarEscenario(uModel, uColor, uSpecular, uShininess, texPlain, matOpaco);
        EscenarioDebate::DibujarModelosDebate(uModel, uColor, uSpecular, uShininess,
            podio, kang, kodos, nave, texPlain, matBrillante, t);

        glUseProgram(0);
    }
}