#pragma once

// GLEW
#include <GL/glew.h>

// SDL
#include <SDL.h>
#include <SDL_opengl.h>

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>

#include "gCamera.h"
#include "ObjParser_OGL3.h"

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
	struct Vertex
	{
		glm::vec3 p;
		glm::vec3 n;
		glm::vec2 t;
	};

	Mesh* m_mesh;

	// seg�df�ggv�nyek
	glm::vec3 GetPos(float u, float v);
	glm::vec3 GetPos2(float u, float v);
	glm::vec3 GetNorm(float u, float v);
	glm::vec3 GetNorm2(float u, float v);
	glm::vec2 GetTex(float u, float v);

	// shaderekhez sz�ks�ges v�ltoz�k
	GLuint m_programID; // shaderek programja

	// OpenGL-es dolgok
	GLuint m_vaoID; // vertex array object er�forr�s azonos�t�
	GLuint m_vboID; // vertex buffer object er�forr�s azonos�t�
	GLuint m_ibID;  // index buffer object er�forr�s azonos�t�
	GLuint m_loadedTextureID; // bet�lt�tt text�ra er�forr�s azonos�t�

	GLuint m_plane_vaoID;
	GLuint m_plane_vboID;
	GLuint m_plane_ibID;

	GLuint m_torus_vaoID;
	GLuint m_torus_vboID;
	GLuint m_torus_ibID;

	GLuint m_cube_vaoID;
	GLuint m_cube_vboID;
	GLuint m_cube_ibID;

	gCamera	m_camera;


	// uniform v�ltoz�k helye a shaderekben
	GLuint m_loc_mvp;
	GLuint m_loc_world;
	GLuint m_loc_worldIT;
	GLuint m_loc_tex;
	GLuint m_loc_eyePos;

	// NxM darab n�gysz�ggel k�zel�tj�k a parametrikus fel�let�nket => (N+1)x(M+1) pontban kell ki�rt�kelni
	static const int N = 80;
	static const int M = 40;

	//A jobb olvashat�s�g kedv��rt
	void InitCube();
	void InitSphere();
	void InitTorus();
	void InitShaders();
	void InitTextures();
	void InitPlane();
};

