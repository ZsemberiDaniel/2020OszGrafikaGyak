#version 130

uniform vec4 color = vec4(1);
in vec3 vs_out_vel;

out vec4 fs_out_color;


void main()
{
	fs_out_color = vec4(length(vs_out_vel) * vec3(1), 1);
}

// 1. feladat: sz�nezd a r�szecsk�ket a sebess�gvektoruk nagys�ga szerint!
// 2. feladat: k�dold el a sz�nben a sebess�gvektor X, Y �s Z tengely szerinti nagys�g�t!