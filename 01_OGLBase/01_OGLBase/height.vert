#version 330 core

// VBO-ból érkezõ változók
in vec3 vs_in_pos;
in vec3 vs_in_norm;
in vec2 vs_in_tex;

// a pipeline-ban tovább adandó értékek
out vec3 vs_out_pos;
out vec3 vs_out_norm;
out vec2 vs_out_tex;

// shader külsõ paraméterei
uniform mat4 MVP;
uniform mat4 world;
uniform mat4 worldIT;

uniform sampler2D texImage;
uniform float time;

void main()
{
	vec3 newPos = vs_in_pos + vs_in_norm * texture(texImage, vs_in_tex + time).r;
	// newPos = vs_in_pos;
	/*vec2 uv = vs_in_pos.xz * 2 * 3.1415;
	newPos = vec3(cos(uv.x), uv.y, sin(uv.x));*/
	newPos = vs_in_pos;

	gl_Position = MVP * vec4( newPos, 1 );
	
	vs_out_pos = (world * vec4(newPos, 1)).xyz;
	vs_out_norm = (worldIT * vec4(vs_in_norm, 0)).xyz;
	vs_out_tex = vs_in_tex;
}