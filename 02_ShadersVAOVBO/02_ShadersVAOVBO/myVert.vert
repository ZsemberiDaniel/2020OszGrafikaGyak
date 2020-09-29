#version 130

// VBO-ból érkezõ változók
in vec3 vs_in_pos;
in vec3 vs_in_col;

// a pipeline-ban tovább adandó értékek
out vec3 vs_out_pos;
out vec3 vs_out_col;

uniform vec2 mousePos;

void main()
{
	vec3 p = vs_in_pos;

	float polar = atan(p.y, p.x);
	float mousePolar = atan(mousePos.y, mousePos.x);

	if (abs(mousePolar - polar) < 0.2f) {
		vs_out_col = vec3(1, 1, 0);
		p.xy *= 1.0f + 0.2f - abs(mousePolar - polar);
	} else {
		vs_out_col = vs_in_col;
	}

	gl_Position = vec4( p, 1 );
	vs_out_pos = p;
}