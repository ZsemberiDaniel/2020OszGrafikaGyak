#version 130

// bemeneti változó - most a vertex shader-bõl (vagyis ottani out)
in vec4 vs_out_col;
in vec4 vs_out_pos;

// kimeneti változó - a fragment színe
out vec4 fs_out_col;

uniform float windowWidth;
uniform float windowHeight;

vec2 square(vec2 p) {
	return vec2(p.x * p.x - p.y * p.y, 2 * p.x * p.y);
}

void main()
{
	// x^2 + y^2 <= r^2
	vec2 p = vs_out_pos.xy;
	float scale = windowWidth / windowHeight;
	p.x *= scale;

	p *= 2;

	// TODO: mandelbrot
	// 100. elemet meghatarozni a sorozatnak
	// ha length(100. elem) > 10^6 akkor nem eleme a halmaznak
	// egyebkent igen
    vec2 c = p;
	int n = 30;
    for (int i = 0; i < n; i++) {
        c = square(c) + p;

		if (length(c) > 1000000) {
			fs_out_col = vec4(1) * float(i) / n;
			return;
		}
    }

    fs_out_col = vec4(1);
}
