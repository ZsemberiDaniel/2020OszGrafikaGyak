#pragma once

// C++ includes
#include <memory>

// GLEW
#include <GL/glew.h>

// SDL
#include <SDL.h>
#include <SDL_opengl.h>

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>

#include "includes/gCamera.h"

#include "includes/ProgramObject.h"
#include "includes/BufferObject.h"
#include "includes/VertexArrayObject.h"
#include "includes/TextureObject.h"

// mesh
#include "includes/ObjParser_OGL3.h"

class CMyApp
{
public:
	CMyApp(void);
	~CMyApp(void);

	bool Init();
	void Clean();

	void Update();
	void Render();

	void KeyboardDown(SDL_KeyboardEvent&);
	void KeyboardUp(SDL_KeyboardEvent&);
	void MouseMove(SDL_MouseMotionEvent&);
	void MouseDown(SDL_MouseButtonEvent&);
	void MouseUp(SDL_MouseButtonEvent&);
	void MouseWheel(SDL_MouseWheelEvent&);
	void Resize(int, int);

protected:
	void DrawTree(glm::mat4 world);

	// shaderekhez szükséges változók
	ProgramObject		m_program;			// mesh shader
	ProgramObject		m_programSkybox;	// skybox shader

	VertexArrayObject	m_CubeVao;			// VAO
	IndexBuffer			m_CubeIndices;		// index buffer
	ArrayBuffer			m_CubeVertexBuffer;	// VBO
	VertexArrayObject	m_SkyboxVao;
	IndexBuffer			m_SkyboxIndices;	
	ArrayBuffer			m_SkyboxPos;		


	VertexArrayObject	m_planeVao;
	IndexBuffer			m_planeIndexBuffer;
	ArrayBuffer			m_planeArrayBuffer;

	VertexArrayObject	m_sphereVao;
	IndexBuffer			m_sphereIndexBuffer;
	ArrayBuffer			m_sphereArrayBuffer;

	gCamera				m_camera;

	Texture2D			m_woodTexture;
	Texture2D			m_suzanneTexture;
	Texture2D			m_grassTexture;
	Texture2D			m_forestTexture;

	// nyers OGL azonosítók
	GLuint				m_skyboxTexture;

	glm::vec3 colors[4] = {
		glm::vec3(0.7, 0.7, 0.6),
		glm::vec3(0.6, 0.4, 0.3),
		glm::vec3(0.3, 0.6, 0.7),
		glm::vec3(0.5, 0.7, 0.6)
	};
	glm::vec3 lightColor = glm::vec3(1);

	glm::vec3 light_dir = glm::vec3(0, -1, 0);
	std::vector<std::vector<float>> treeHeights;

	struct Vertex
	{
		glm::vec3 p;
		glm::vec3 n;
		glm::vec2 t;
	};

	// mesh adatok
	Mesh *m_mesh;

	// a jobb olvashatóság kedvéért
	void InitShaders();
	void InitCube();
	void InitSkyBox();

	void InitPlane();
	glm::vec3 GetPosPlane(float u, float v);
	glm::vec3 GetNormPlane(float u, float v);

	void InitSphere();
	glm::vec3 GetPosSphere(float u, float v);
	glm::vec3 GetNormSphere(float u, float v);
};

