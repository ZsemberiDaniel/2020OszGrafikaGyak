#version 140

// pipeline-ból bejövõ per-fragment attribútumok
in vec3 vs_out_pos;
in vec3 vs_out_norm;
in vec2 vs_out_tex;

// kimenõ érték - a fragment színe
out vec4 fs_out_col;

uniform vec3 eye_pos;

// irány fényforrás: fény iránya
uniform vec3 light_dir = vec3(0,-1,0);
uniform vec3 light_pos = vec3(0, 5, 0);
uniform float spotAngle = 3.14152f / 8.0f;

// fénytulajdonságok: ambiens, diffúz, ...
uniform vec3 La = vec3(1.0, 0.1, 0.1);
uniform vec3 Ld = vec3(1.0, 0.2, 0.2);
uniform vec3 Ls = vec3(1.0, 1.0, 1.0);

// anyagtulajdonságok: ambiens, diffúz, ...
uniform vec3 Ka = vec3(0.2, 0.4, 0.6);
uniform vec3 Kd = vec3(0.2, 0.4, 0.6);
uniform vec3 Ks = vec3(0.2, 0.4, 0.6);

uniform sampler2D texImage;

void main()
{	
	//
	// ambiens szín számítása
	//
	vec3 ambient = La * Ka;

	// directional light
	vec3 toLight = normalize(-light_dir);
	// spotlight, point light
	toLight = normalize(light_pos - vs_out_pos);

	//
	// diffúz szín számítása
	//	
	/* segítség:
	    - normalizálás: http://www.opengl.org/sdk/docs/manglsl/xhtml/normalize.xml
	    - skaláris szorzat: http://www.opengl.org/sdk/docs/manglsl/xhtml/dot.xml
	    - clamp: http://www.opengl.org/sdk/docs/manglsl/xhtml/clamp.xml
	*/

	vec3 normal = normalize(vs_out_norm);
	float di = clamp(dot(toLight, normal), 0.0f, 1.0f);
	vec3 diffuse = Ld * Kd * di;

	//
	// fényfoltképzõ szín
	//
	/* segítség:
		- reflect: http://www.opengl.org/sdk/docs/manglsl/xhtml/reflect.xml
		- power: http://www.opengl.org/sdk/docs/manglsl/xhtml/pow.xml
	*/
	vec3 specular = vec3(0);

	if (di > 0)
	{
		vec3 e = normalize(eye_pos - vs_out_pos);
		vec3 r = reflect(-toLight, normal);
		float si = pow(clamp(dot(e, r), 0.0f, 1.0f), 25);
		specular = Ls * Ks * si;
	}
	
	//
	// a fragment végsõ színének meghatározása
	//

	//fs_out_col = vec4(ambient + diffuse + specular, 1);

	// fs_out_col = vec4(ambient + falloff * (diffuse + specular), 1);
	// felületi normális
	//fs_out_col = vec4(vs_out_norm, 1);

	// falloff for point light
	float dist = distance(light_pos, vs_out_pos);
	float falloff = 30.0 / dist / dist;

	// falloff for spotlight
	float angle = acos(dot(-toLight, normalize(light_dir)));
	float ex = exp(64 * (angle - spotAngle));
	float spoti = 1 / (ex + 1);

	// textúrával
	vec4 textureColor = texture(texImage, vs_out_tex);
	fs_out_col = vec4(ambient + /*falloff*/ spoti * (diffuse + specular), 1) * textureColor;
}

// Feladatok

// 1) Fényszámítás
// - ambiens
// - diffúz
// - spekuláris

// 2) Textúra
