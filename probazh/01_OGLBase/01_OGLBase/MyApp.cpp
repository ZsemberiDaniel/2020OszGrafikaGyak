#include "MyApp.h"

#include <math.h>
#include <vector>

#include <array>
#include <list>
#include <tuple>
#include <imgui/imgui.h>
#include "includes/GLUtils.hpp"

CMyApp::CMyApp(void)
{
	m_camera.SetView(glm::vec3(5, 5, 5), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	m_mesh = nullptr;
}

CMyApp::~CMyApp(void)
{
	std::cout << "dtor!\n";
}


glm::vec3 CMyApp::GetPosSphere(float u, float v)
{
	u *= 2 * M_PI;
	v *= M_PI;
	float cu = cosf(u), su = sinf(u);
	float cv = cosf(v), sv = sinf(v);

	return { cu * sv, su * sv, cv };
}

glm::vec3 CMyApp::GetNormSphere(float u, float v)
{
	u *= 2 * M_PI;
	v *= M_PI;
	float cu = cosf(u), su = sinf(u);
	float cv = cosf(v), sv = sinf(v);

	return glm::normalize(glm::vec3(cu * sv, su * sv, cv));
}

void CMyApp::InitSphere()
{
	const int N = 20, M = 20;
	Vertex vert[(N + 1) * (M + 1)];
	for (int i = 0; i <= N; ++i)
		for (int j = 0; j <= M; ++j)
		{
			float u = i / (float)N;
			float v = j / (float)M;

			vert[i + j * (N + 1)].p = GetPosSphere(u, v);
			vert[i + j * (N + 1)].n = GetNormSphere(u, v);
			vert[i + j * (N + 1)].t = { u, v };
		}

	GLushort indices[3 * 2 * (N) * (M)];
	for (int i = 0; i < N; ++i)
		for (int j = 0; j < M; ++j)
		{
			indices[6 * i + j * 3 * 2 * (N)+0] = (i)+(j) * (N + 1);
			indices[6 * i + j * 3 * 2 * (N)+2] = (i + 1) + (j) * (N + 1);
			indices[6 * i + j * 3 * 2 * (N)+1] = (i)+(j + 1) * (N + 1);
			indices[6 * i + j * 3 * 2 * (N)+3] = (i + 1) + (j) * (N + 1);
			indices[6 * i + j * 3 * 2 * (N)+5] = (i + 1) + (j + 1) * (N + 1);
			indices[6 * i + j * 3 * 2 * (N)+4] = (i)+(j + 1) * (N + 1);
		}

	m_sphereArrayBuffer.BufferData(vert);

	// ?s a primit?veket alkot? cs?cspontok indexei (az el?z? t?mb?kb?l) - triangle list-el val? kirajzol?sra felk?sz?lve
	m_sphereIndexBuffer.BufferData(indices);

	// geometria VAO-ban val? regisztr?l?sa
	m_sphereVao.Init(
		{
			// 0-?s attrib?tum "l?nyeg?ben" glm::vec3-ak sorozata ?s az adatok az m_CubeVertexBuffer GPU pufferben vannak
			{ CreateAttribute<		0,						// attrib?tum: 0
									glm::vec3,				// CPU oldali adatt?pus amit a 0-?s attrib?tum meghat?roz?s?ra haszn?ltunk <- az elj?r?s a glm::vec3-b?l kik?vetkezteti, hogy 3 darab float-b?l ?ll a 0-?s attrib?tum
									0,						// offset: az attrib?tum t?rol? elej?t?l vett offset-je, byte-ban
									sizeof(Vertex)			// stride: a k?vetkez? cs?cspont ezen attrib?tuma h?ny byte-ra van az aktu?list?l
								>, m_sphereArrayBuffer },
			{ CreateAttribute<1, glm::vec3, (sizeof(glm::vec3)), sizeof(Vertex)>, m_sphereArrayBuffer },
			{ CreateAttribute<2, glm::vec2, (2 * sizeof(glm::vec3)), sizeof(Vertex)>, m_sphereArrayBuffer },
		},
		m_sphereIndexBuffer
	);
}


glm::vec3 CMyApp::GetPosPlane(float u, float v)
{
	return { u - 0.5f, glm::sin(u * 10.0f), v - 0.5f };
}

glm::vec3 CMyApp::GetNormPlane(float u, float v)
{
	glm::vec3 du = GetPosPlane(u + 0.01f, v) - GetPosPlane(u - 0.01f, v);
	glm::vec3 dv = GetPosPlane(u, v + 0.01f) - GetPosPlane(u, v - 0.01f);

	return glm::normalize(glm::cross(du, dv));
}

void CMyApp::InitPlane()
{
	const int N = 19, M = 19;
	Vertex vert[(N + 1) * (M + 1)];
	for (int i = 0; i <= N; ++i)
		for (int j = 0; j <= M; ++j)
		{
			float u = i / (float)N;
			float v = j / (float)M;

			vert[i + j * (N + 1)].p = GetPosPlane(u, v);
			vert[i + j * (N + 1)].n = GetNormPlane(u, v);
			vert[i + j * (N + 1)].t = { u, v };
		}

	GLushort indices[3 * 2 * (N) * (M)];
	for (int i = 0; i < N; ++i)
		for (int j = 0; j < M; ++j)
		{
			indices[6 * i + j * 3 * 2 * (N)+0] = (i)+(j) * (N + 1);
			indices[6 * i + j * 3 * 2 * (N)+2] = (i + 1) + (j) * (N + 1);
			indices[6 * i + j * 3 * 2 * (N)+1] = (i)+(j + 1) * (N + 1);
			indices[6 * i + j * 3 * 2 * (N)+3] = (i + 1) + (j) * (N + 1);
			indices[6 * i + j * 3 * 2 * (N)+5] = (i + 1) + (j + 1) * (N + 1);
			indices[6 * i + j * 3 * 2 * (N)+4] = (i)+(j + 1) * (N + 1);
		}

	m_planeArrayBuffer.BufferData(vert);

	// ?s a primit?veket alkot? cs?cspontok indexei (az el?z? t?mb?kb?l) - triangle list-el val? kirajzol?sra felk?sz?lve
	m_planeIndexBuffer.BufferData(indices);

	// geometria VAO-ban val? regisztr?l?sa
	m_planeVao.Init(
		{
			// 0-?s attrib?tum "l?nyeg?ben" glm::vec3-ak sorozata ?s az adatok az m_CubeVertexBuffer GPU pufferben vannak
			{ CreateAttribute<		0,						// attrib?tum: 0
									glm::vec3,				// CPU oldali adatt?pus amit a 0-?s attrib?tum meghat?roz?s?ra haszn?ltunk <- az elj?r?s a glm::vec3-b?l kik?vetkezteti, hogy 3 darab float-b?l ?ll a 0-?s attrib?tum
									0,						// offset: az attrib?tum t?rol? elej?t?l vett offset-je, byte-ban
									sizeof(Vertex)			// stride: a k?vetkez? cs?cspont ezen attrib?tuma h?ny byte-ra van az aktu?list?l
								>, m_planeArrayBuffer },
			{ CreateAttribute<1, glm::vec3, (sizeof(glm::vec3)), sizeof(Vertex)>, m_planeArrayBuffer },
			{ CreateAttribute<2, glm::vec2, (2 * sizeof(glm::vec3)), sizeof(Vertex)>, m_planeArrayBuffer },
		},
		m_planeIndexBuffer
	);
}

void CMyApp::InitCube()
{
	//struct Vertex{ glm::vec3 position; glm::vec3 normals; glm::vec2 texture; };
	std::vector<Vertex>vertices;
	
	//front									 
	vertices.push_back({ glm::vec3(-0.5, -0.5, +0.5), glm::vec3(0, 0, 1), glm::vec2(0, 0) });
	vertices.push_back({ glm::vec3(+0.5, -0.5, +0.5), glm::vec3(0, 0, 1), glm::vec2(1, 0) });
	vertices.push_back({ glm::vec3(-0.5, +0.5, +0.5), glm::vec3(0, 0, 1), glm::vec2(0, 1) });
	vertices.push_back({ glm::vec3(+0.5, +0.5, +0.5), glm::vec3(0, 0, 1), glm::vec2(1, 1) });
	//back
	vertices.push_back({ glm::vec3(+0.5, -0.5, -0.5), glm::vec3(0, 0, -1), glm::vec2(0, 0) });
	vertices.push_back({ glm::vec3(-0.5, -0.5, -0.5), glm::vec3(0, 0, -1), glm::vec2(1, 0) });
	vertices.push_back({ glm::vec3(+0.5, +0.5, -0.5), glm::vec3(0, 0, -1), glm::vec2(0, 1) });
	vertices.push_back({ glm::vec3(-0.5, +0.5, -0.5), glm::vec3(0, 0, -1), glm::vec2(1, 1) });
	//right									 
	vertices.push_back({ glm::vec3(+0.5, -0.5, +0.5), glm::vec3(1, 0, 0), glm::vec2(0, 0) });
	vertices.push_back({ glm::vec3(+0.5, -0.5, -0.5), glm::vec3(1, 0, 0), glm::vec2(1, 0) });
	vertices.push_back({ glm::vec3(+0.5, +0.5, +0.5), glm::vec3(1, 0, 0), glm::vec2(0, 1) });
	vertices.push_back({ glm::vec3(+0.5, +0.5, -0.5), glm::vec3(1, 0, 0), glm::vec2(1, 1) });
	//left									 
	vertices.push_back({ glm::vec3(-0.5, -0.5, -0.5), glm::vec3(-1, 0, 0), glm::vec2(0, 0) });
	vertices.push_back({ glm::vec3(-0.5, -0.5, +0.5), glm::vec3(-1, 0, 0), glm::vec2(1, 0) });
	vertices.push_back({ glm::vec3(-0.5, +0.5, -0.5), glm::vec3(-1, 0, 0), glm::vec2(0, 1) });
	vertices.push_back({ glm::vec3(-0.5, +0.5, +0.5), glm::vec3(-1, 0, 0), glm::vec2(1, 1) });
	//top									 
	vertices.push_back({ glm::vec3(-0.5, +0.5, +0.5), glm::vec3(0, 1, 0), glm::vec2(0, 0) });
	vertices.push_back({ glm::vec3(+0.5, +0.5, +0.5), glm::vec3(0, 1, 0), glm::vec2(1, 0) });
	vertices.push_back({ glm::vec3(-0.5, +0.5, -0.5), glm::vec3(0, 1, 0), glm::vec2(0, 1) });
	vertices.push_back({ glm::vec3(+0.5, +0.5, -0.5), glm::vec3(0, 1, 0), glm::vec2(1, 1) });
	//bottom								 
	vertices.push_back({ glm::vec3(-0.5, -0.5, -0.5), glm::vec3(0, -1, 0), glm::vec2(0, 0) });
	vertices.push_back({ glm::vec3(+0.5, -0.5, -0.5), glm::vec3(0, -1, 0), glm::vec2(1, 0) });
	vertices.push_back({ glm::vec3(-0.5, -0.5, +0.5), glm::vec3(0, -1, 0), glm::vec2(0, 1) });
	vertices.push_back({ glm::vec3(+0.5, -0.5, +0.5), glm::vec3(0, -1, 0), glm::vec2(1, 1) });

	std::vector<int> indices(36);
	int index = 0;
	for (int i = 0; i < 4 * 4; i += 4)
	{
		indices[index + 0] = i + 0;
		indices[index + 1] = i + 2;
		indices[index + 2] = i + 1;
		indices[index + 3] = i + 1;
		indices[index + 4] = i + 2;
		indices[index + 5] = i + 3;
		index += 6;
	}

	//
	// geometria defini�l�sa (std::vector<...>) �s GPU pufferekbe val� felt�lt�se BufferData-val
	//

	// vertexek poz�ci�i:
	/*
	Az m_CubeVertexBuffer konstruktora m�r l�trehozott egy GPU puffer azonos�t�t �s a most k�vetkez� BufferData h�v�s ezt
	1. bind-olni fogja GL_ARRAY_BUFFER target-re (hiszen m_CubeVertexBuffer t�pusa ArrayBuffer) �s
	2. glBufferData seg�ts�g�vel �tt�lti a GPU-ra az argumentumban adott t�rol� �rt�keit

	*/

	m_CubeVertexBuffer.BufferData(vertices);

	// �s a primit�veket alkot� cs�cspontok indexei (az el�z� t�mb�kb�l) - triangle list-el val� kirajzol�sra felk�sz�lve
	m_CubeIndices.BufferData(indices);

	// geometria VAO-ban val� regisztr�l�sa
	m_CubeVao.Init(
		{
			// 0-�s attrib�tum "l�nyeg�ben" glm::vec3-ak sorozata �s az adatok az m_CubeVertexBuffer GPU pufferben vannak
			{ CreateAttribute<		0,						// attrib�tum: 0
									glm::vec3,				// CPU oldali adatt�pus amit a 0-�s attrib�tum meghat�roz�s�ra haszn�ltunk <- az elj�r�s a glm::vec3-b�l kik�vetkezteti, hogy 3 darab float-b�l �ll a 0-�s attrib�tum
									0,						// offset: az attrib�tum t�rol� elej�t�l vett offset-je, byte-ban
									sizeof(Vertex)			// stride: a k�vetkez� cs�cspont ezen attrib�tuma h�ny byte-ra van az aktu�list�l
								>, m_CubeVertexBuffer },
			{ CreateAttribute<1, glm::vec3, (sizeof(glm::vec3)), sizeof(Vertex)>, m_CubeVertexBuffer },
			{ CreateAttribute<2, glm::vec2, (2 * sizeof(glm::vec3)), sizeof(Vertex)>, m_CubeVertexBuffer },
		},
		m_CubeIndices
	);
}

void CMyApp::InitSkyBox()
{
	m_SkyboxPos.BufferData(
		std::vector<glm::vec3>{
		// h�ts� lap
		glm::vec3(-1, -1, -1),
		glm::vec3(1, -1, -1),
		glm::vec3(1, 1, -1),
		glm::vec3(-1, 1, -1),
		// el�ls� lap
		glm::vec3(-1, -1, 1),
		glm::vec3(1, -1, 1),
		glm::vec3(1, 1, 1),
		glm::vec3(-1, 1, 1),
	}
	);

	// �s a primit�veket alkot� cs�cspontok indexei (az el�z� t�mb�kb�l) - triangle list-el val� kirajzol�sra felk�sz�lve
	m_SkyboxIndices.BufferData(
		std::vector<int>{
			// h�ts� lap
			0, 1, 2,
			2, 3, 0,
			// el�ls� lap
			4, 6, 5,
			6, 4, 7,
			// bal
			0, 3, 4,
			4, 3, 7,
			// jobb
			1, 5, 2,
			5, 6, 2,
			// als�
			1, 0, 4,
			1, 4, 5,
			// fels�
			3, 2, 6,
			3, 6, 7,
	}
	);

	// geometria VAO-ban val� regisztr�l�sa
	m_SkyboxVao.Init(
		{
			{ CreateAttribute<0, glm::vec3, 0, sizeof(glm::vec3)>, m_SkyboxPos },
		}, m_SkyboxIndices
	);

	// skybox texture
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

	glGenTextures(1, &m_skyboxTexture);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_skyboxTexture);

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	TextureFromFileAttach("assets/xpos.png", GL_TEXTURE_CUBE_MAP_POSITIVE_X);
	TextureFromFileAttach("assets/xneg.png", GL_TEXTURE_CUBE_MAP_NEGATIVE_X);
	TextureFromFileAttach("assets/ypos.png", GL_TEXTURE_CUBE_MAP_POSITIVE_Y);
	TextureFromFileAttach("assets/yneg.png", GL_TEXTURE_CUBE_MAP_NEGATIVE_Y);
	TextureFromFileAttach("assets/zpos.png", GL_TEXTURE_CUBE_MAP_POSITIVE_Z);
	TextureFromFileAttach("assets/zneg.png", GL_TEXTURE_CUBE_MAP_NEGATIVE_Z);

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

}

void CMyApp::InitShaders()
{
	// a shadereket t�rol� program l�trehoz�sa az OpenGL-hez hasonl� m�don:
	m_program.AttachShaders({
		{ GL_VERTEX_SHADER, "myVert.vert"},
		{ GL_FRAGMENT_SHADER, "myFrag.frag"}
	});

	// attributomok osszerendelese a VAO es shader kozt
	m_program.BindAttribLocations({
		{ 0, "vs_in_pos" },				// VAO 0-as csatorna menjen a vs_in_pos-ba
		{ 1, "vs_in_norm" },			// VAO 1-es csatorna menjen a vs_in_norm-ba
		{ 2, "vs_in_tex" },				// VAO 2-es csatorna menjen a vs_in_tex-be
	});

	m_program.LinkProgram();

	// shader program r�vid l�trehoz�sa, egyetlen f�ggv�nyh�v�ssal a fenti h�rom:
	m_programSkybox.Init(
		{
			{ GL_VERTEX_SHADER, "skybox.vert" },
			{ GL_FRAGMENT_SHADER, "skybox.frag" }
		},
		{
			{ 0, "vs_in_pos" },				// VAO 0-as csatorna menjen a vs_in_pos-ba
		}
	);
}

bool CMyApp::Init()
{
	// t�rl�si sz�n legyen k�kes
	glClearColor(0.125f, 0.25f, 0.5f, 1.0f);

	glEnable(GL_CULL_FACE); // kapcsoljuk be a hatrafele nezo lapok eldobasat
	glEnable(GL_DEPTH_TEST); // m�lys�gi teszt bekapcsol�sa (takar�s)

	InitPlane();
	InitSphere();
	InitShaders();
	InitCube();
	InitSkyBox();

	// egy�b text�r�k bet�lt�se
	m_woodTexture.FromFile("assets/wood.jpg");
	m_suzanneTexture.FromFile("assets/marron.jpg");
	m_grassTexture.FromFile("assets/grass.jpg");
	m_forestTexture.FromFile("assets/forest.jpg");

	// mesh betoltese
	m_mesh = ObjParser::parse("assets/henger.obj");
	m_mesh->initBuffers();
	
	// kamera
	m_camera.SetProj(45.0f, 640.0f / 480.0f, 0.01f, 1000.0f);

	treeHeights.resize(20);
	for (int i = 0; i < treeHeights.size(); i++)
	{
		treeHeights[i].resize(20);
		for (int k = 0; k < treeHeights[i].size(); k++)
		{
			treeHeights[i][k] = 0.0f;
		}
	}

	int count = rand() % 50 + 50;
	for (int i = 0; i < count; i++)
	{
		int posX, posZ;
		do {
			posX = rand() % 18 + 1;
			posZ = rand() % 18 + 1;
		} while (treeHeights[posX][posZ] > 0.0f);

		treeHeights[posX][posZ] = (rand() % 90) / 100.0f + 0.05f;
	}

	return true;
}

void CMyApp::Clean()
{
	glDeleteTextures(1, &m_skyboxTexture);

	delete m_mesh;
}

void CMyApp::Update()
{
	static Uint32 last_time = SDL_GetTicks();
	float delta_time = (SDL_GetTicks() - last_time) / 1000.0f;

	m_camera.Update(delta_time);

	last_time = SDL_GetTicks();

	static int currentTime = 0;
	static float timeSinceStartOfTime = 0.0f;
	if (timeSinceStartOfTime >= 5.0f)
	{
		currentTime = (currentTime + 1) % 4;
		timeSinceStartOfTime = 0.0f;
	}
	lightColor = glm::mix(colors[currentTime], colors[(currentTime + 1) % 4], timeSinceStartOfTime / 5.0f);


	timeSinceStartOfTime += delta_time;

	for (int i = 0; i < treeHeights.size(); i++)
	{
		for (int k = 0; k < treeHeights[i].size(); k++)
		{
			if (treeHeights[i][k] != 0.0f)
			{
				treeHeights[i][k] += 0.95f / 10.0f * delta_time;
				// treeHeights[i][k] = glm::clamp(treeHeights[i][k], 0.0f, 1.0f);

				if (treeHeights[i][k] >= 1.0f)
				{
					treeHeights[i][k] = 0.0f;
				}
			}
		}
	}


	static float timeSinceLastPlant = 0.0f;

	if (timeSinceLastPlant >= 2.0f)
	{
		for (int i = 0; i < treeHeights.size(); i++)
		{
			for (int k = 0; k < treeHeights[i].size(); k++)
			{
				bool doPlant = (rand() % 100) <= 4;
				if (doPlant && treeHeights[i][k] == 0.0f)
				{
					treeHeights[i][k] = 0.05f;
				}
			}
		}

		timeSinceLastPlant = 0.0f;
	}

	timeSinceLastPlant += delta_time;
}

void CMyApp::DrawTree(glm::mat4 world)
{
	glm::mat4 trunkWorld = world * glm::translate(glm::vec3(0, 2.0f, 0));
	m_program.SetTexture("texImage", 0, m_suzanneTexture);
	m_program.SetUniform("MVP", m_camera.GetViewProj() * trunkWorld);
	m_program.SetUniform("world", trunkWorld);
	m_program.SetUniform("worldIT", glm::inverse(glm::transpose(trunkWorld)));
	m_mesh->draw();

	m_sphereVao.Bind();
	glm::mat4 leavesWorld = world * glm::translate(glm::vec3(0, 4.0f, 0));
	m_program.SetTexture("texImage", 0, m_suzanneTexture);
	m_program.SetUniform("MVP", m_camera.GetViewProj() * leavesWorld);
	m_program.SetUniform("world", leavesWorld);
	m_program.SetUniform("worldIT", glm::inverse(glm::transpose(leavesWorld)));
	glDrawElements(GL_TRIANGLES, 2400, GL_UNSIGNED_SHORT, nullptr);
	m_sphereVao.Unbind();
}

void CMyApp::Render()
{
	// t�r�lj�k a frampuffert (GL_COLOR_BUFFER_BIT) �s a m�lys�gi Z puffert (GL_DEPTH_BUFFER_BIT)
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 viewProj = m_camera.GetViewProj();

	m_program.Use();
	m_program.SetUniform("eye_pos", m_camera.GetEye());
	m_program.SetUniform("light_dir", light_dir);
	m_program.SetUniform("La", lightColor);
	m_program.SetUniform("Ld", lightColor);
	m_program.SetUniform("Ls", lightColor);
	for (int i = 0; i < treeHeights.size(); i++)
	{
		for (int k = 0; k < treeHeights[i].size(); k++)
		{
			glm::vec3 pos = glm::vec3(i - 10, GetPosPlane((float)i / 20.0f, (float)k / 20.0f).y, k - 10);
			DrawTree(glm::translate(pos) * glm::scale(glm::vec3(treeHeights[i][k])));
		}
	}

	// kock�k
	//m_program.Use(); nem h�vjuk meg �jra, hisz ugyanazt a shadert haszn�lj�k
	m_CubeVao.Bind();
	m_program.SetTexture("texImage", 0, m_woodTexture);
	glm::mat4 cubeWorld;

	cubeWorld = glm::translate(glm::vec3(0, 2, 0)) * glm::scale(glm::vec3(20, 6, 20));
	m_program.SetUniform("MVP", viewProj * cubeWorld);
	m_program.SetUniform("world", cubeWorld);
	m_program.SetUniform("worldIT", glm::inverse(glm::transpose(cubeWorld)));
	m_program.SetTexture("texImage", 0, m_forestTexture);
	glDrawElements(GL_TRIANGLES, 24, GL_UNSIGNED_INT, nullptr);

	glm::mat4 planeWorld = glm::scale(glm::vec3(20, 1, 20));
	m_program.SetUniform("MVP", viewProj * planeWorld);
	m_program.SetUniform("world", planeWorld);
	m_program.SetUniform("worldIT", glm::inverse(glm::transpose(planeWorld)));
	m_program.SetTexture("texImage", 0, m_grassTexture);
	m_planeVao.Bind();
	glDrawElements(GL_TRIANGLES, 2166, GL_UNSIGNED_SHORT, nullptr);

	m_program.Unuse();

	// skybox
	// ments�k el az el�z� Z-test eredm�nyt, azaz azt a rel�ci�t, ami alapj�n update-elj�k a pixelt.
	GLint prevDepthFnc;
	glGetIntegerv(GL_DEPTH_FUNC, &prevDepthFnc);

	// most kisebb-egyenl�t haszn�ljunk, mert mindent kitolunk a t�voli v�g�s�kokra
	glDepthFunc(GL_LEQUAL);

	m_SkyboxVao.Bind();
	m_programSkybox.Use();
	m_programSkybox.SetUniform("MVP", viewProj * glm::translate( m_camera.GetEye()) );
	
	// cube map text�ra be�ll�t�sa 0-�s mintav�telez�re �s annak a shaderre be�ll�t�sa
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_skyboxTexture);
	glUniform1i(m_programSkybox.GetLocation("skyboxTexture"), 0);
	// az el�z� h�rom sor <=> m_programSkybox.SetCubeTexture("skyboxTexture", 0, m_skyboxTexture);

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);
	m_programSkybox.Unuse();

	// v�g�l �ll�tsuk vissza
	glDepthFunc(prevDepthFnc);


	// 1. feladat: k�sz�ts egy vertex shader-fragment shader p�rt, ami t�rolt geometria _n�lk�l_ kirajzol egy tetsz�leges poz�ci�ba egy XYZ tengely-h�rmast,
	//			   ahol az X piros, az Y z�ld a Z pedig k�k!

	//ImGui Testwindow
	ImGui::ShowTestWindow();
	if (ImGui::Begin("Settings"))
	{
		ImGui::SliderFloat3("Light direction", &(light_dir[0]), 0.0f, 1.0f);
	}
	ImGui::End();
}

void CMyApp::KeyboardDown(SDL_KeyboardEvent& key)
{
	m_camera.KeyboardDown(key);
}

void CMyApp::KeyboardUp(SDL_KeyboardEvent& key)
{
	m_camera.KeyboardUp(key);
}

void CMyApp::MouseMove(SDL_MouseMotionEvent& mouse)
{
	m_camera.MouseMove(mouse);
}

void CMyApp::MouseDown(SDL_MouseButtonEvent& mouse)
{
}

void CMyApp::MouseUp(SDL_MouseButtonEvent& mouse)
{
}

void CMyApp::MouseWheel(SDL_MouseWheelEvent& wheel)
{
}

// a k�t param�terbe az �j ablakm�ret sz�less�ge (_w) �s magass�ga (_h) tal�lhat�
void CMyApp::Resize(int _w, int _h)
{
	glViewport(0, 0, _w, _h );

	m_camera.Resize(_w, _h);
}
