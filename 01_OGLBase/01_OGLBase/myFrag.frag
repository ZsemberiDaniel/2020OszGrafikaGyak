#version 140

// pipeline-b�l bej�v� per-fragment attrib�tumok
in vec3 vs_out_pos;
in vec3 vs_out_norm;
in vec2 vs_out_tex;

// kimen� �rt�k - a fragment sz�ne
out vec4 fs_out_col;

uniform vec3 eye_pos;

// ir�ny f�nyforr�s: f�ny ir�nya
uniform vec3 light_dir = vec3(0,-1,0);
uniform vec3 light_pos = vec3(0, 5, 0);
uniform float spotAngle = 3.14152f / 8.0f;

// f�nytulajdons�gok: ambiens, diff�z, ...
uniform vec3 La = vec3(1.0, 0.1, 0.1);
uniform vec3 Ld = vec3(1.0, 0.2, 0.2);
uniform vec3 Ls = vec3(1.0, 1.0, 1.0);

// anyagtulajdons�gok: ambiens, diff�z, ...
uniform vec3 Ka = vec3(0.2, 0.4, 0.6);
uniform vec3 Kd = vec3(0.2, 0.4, 0.6);
uniform vec3 Ks = vec3(0.2, 0.4, 0.6);

uniform sampler2D texImage;
uniform samplerCube skyboxTexture;

vec3 directionalLight(vec3 La, vec3 Ld, vec3 Ls, vec3 lightDir) 
{
	// ambient
	vec3 ambient = La * Ka;

	// diffuse
	vec3 toLight = normalize(-lightDir);
	
	vec3 normal = normalize(vs_out_norm);
	float di = clamp(dot(toLight, normal), 0.0f, 1.0f);
	vec3 diffuse = Ld * Kd * di;

	// specular
	vec3 specular = vec3(0);

	if (di > 0)
	{
		vec3 e = normalize(eye_pos - vs_out_pos);
		vec3 r = reflect(-toLight, normal);
		float si = pow(clamp(dot(e, r), 0.0f, 1.0f), 25);
		specular = Ls * Ks * si;
	}

	return ambient + diffuse + specular;
}

vec3 pointLight(vec3 La, vec3 Ld, vec3 Ls, vec3 lightPos, float brightness) 
{
	// ambient
	vec3 ambient = La * Ka;

	// diffuse
	vec3 toLight = normalize(lightPos - vs_out_pos);
	
	vec3 normal = normalize(vs_out_norm);
	float di = clamp(dot(toLight, normal), 0.0f, 1.0f);
	vec3 diffuse = Ld * Kd * di;

	// specular
	vec3 specular = vec3(0);

	if (di > 0)
	{
		vec3 e = normalize(eye_pos - vs_out_pos);
		vec3 r = reflect(-toLight, normal);
		float si = pow(clamp(dot(e, r), 0.0f, 1.0f), 25);
		specular = Ls * Ks * si;
	}

	// falloff for point light
	float dist = distance(light_pos, vs_out_pos);
	float falloff = brightness / dist / dist;

	return ambient + falloff * (diffuse + specular);
}

vec3 spotLight(vec3 La, vec3 Ld, vec3 Ls, vec3 lightPos, vec3 lightDir, float spotAngle)
{
	// ambient
	vec3 ambient = La * Ka;

	// diffuse
	vec3 toLight = normalize(lightPos - vs_out_pos);
	
	vec3 normal = normalize(vs_out_norm);
	float di = clamp(dot(toLight, normal), 0.0f, 1.0f);
	vec3 diffuse = Ld * Kd * di;

	// specular
	vec3 specular = vec3(0);

	if (di > 0)
	{
		vec3 e = normalize(eye_pos - vs_out_pos);
		vec3 r = reflect(-toLight, normal);
		float si = pow(clamp(dot(e, r), 0.0f, 1.0f), 25);
		specular = Ls * Ks * si;
	}

	// falloff for spotlight
	float angle = acos(dot(-toLight, normalize(lightDir)));
	float ex = exp(64 * (angle - spotAngle));
	float spoti = 1 / (ex + 1);

	return ambient + spoti * (diffuse + specular);
}

vec3 reflectionColor()
{
	vec3 I = normalize(vs_out_pos - eye_pos);
    vec3 R = reflect(I, vs_out_norm);
    return texture(skyboxTexture, R).rgb;
}

vec3 refractionColor(float ratio)
{
	vec3 I = normalize(vs_out_pos - eye_pos);
    vec3 R = refract(I, vs_out_norm, ratio);
    return texture(skyboxTexture, R).rgb;
}


void main()
{	
	/*fs_out_col = vec4(vs_out_norm, 1);
	return;*/

	vec3 color = vec3(0);
	color += directionalLight(La, Ld, Ls, light_dir);
	color += reflectionColor() * 0.2f;

	color = clamp(color, vec3(0), vec3(1));

	fs_out_col = vec4(color, 1);
}
