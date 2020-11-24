#version 330 core

// pipeline-ból bejövõ per-fragment attribútumok
in vec3 vs_out_pos;

out vec4 fs_out_col;

uniform samplerCube skyboxTexture;

uniform vec3 sunDir = normalize(vec3(0,1,1));
uniform vec3 sunCol = 2*vec3(1.0,0.9,0.2);

void main()
{
	//fs_out_col = vec4( normalize(vs_out_pos), 1);
	fs_out_col = texture( skyboxTexture, (vs_out_pos) );

	vec3 dir = normalize(vs_out_pos);
}
