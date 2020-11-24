#version 330 core

// VBO-b�l �rkez� v�ltoz�k
in vec3 vs_in_pos;
in vec3 vs_in_norm;
in vec2 vs_in_tex;

// a pipeline-ban tov�bb adand� �rt�kek
out vec3 vs_out_pos;
out vec3 vs_out_norm;
out vec2 vs_out_tex;

// shader k�ls� param�terei
uniform mat4 MVP;
uniform mat4 world;
uniform mat4 worldIT;

uniform sampler2D heightTexture;
uniform float time;

void main()
{
	/*vec2 uv = vs_in_pos.xz;
	uv.x *= 2 * 3.1415;
	vec3 newPos = vec3(cos(uv.x), uv.y, sin(uv.x));
	vec3 newNorm = normalize(vec3(cos(uv.x), 0, sin(uv.x)));*/

	vec3 newPos = vs_in_pos + vs_in_norm * texture(heightTexture, vs_in_tex + time).g;

	gl_Position = MVP * vec4(newPos, 1 );
	
	vs_out_pos = (world * vec4(newPos, 1)).xyz;
	vs_out_norm = (worldIT * vec4(vs_in_norm, 0)).xyz;
	vs_out_tex = vs_in_tex;
}