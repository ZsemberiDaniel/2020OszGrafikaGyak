#version 140

// pipeline-ból bejövõ per-fragment attribútumok
in vec3 vs_out_pos;
in vec2 vs_out_tex0;

// kimenõ érték - a fragment színe
out vec4 fs_out_col;

uniform sampler2D texImage;
uniform sampler2D texImage2;

void main()
{
	fs_out_col = mix(texture(texImage, vs_out_tex0), texture(texImage2, vs_out_tex0), 0.5);

	//fs_out_col = texture(texImage, vs_out_tex0) * 0.5 + texture(texImage2, vs_out_tex0) * 0.5;
}

// procedurális textúra...
