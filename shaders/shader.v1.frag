#version 330 core
#extension GL_ARB_explicit_uniform_location : enable

struct LightSource
{
	vec3 Amb; // Intesidad ambiental
	vec3 Diff; // Intensidad difusa
	vec4 Pos; // Posición
	vec4 Dir; // Dirección
	vec3 C; // Atenuación
	float CosCutOff;
	float SpotExponent;
};

out vec4 outColor;

in vec3 color;
in vec3 pos;
in vec3 norm;
in vec2 texCoord;
in mat3 TBN;

layout(location = 0) uniform sampler2D colorTex;
uniform sampler2D emiTex;
uniform sampler2D specTex;
uniform sampler2D normalTex;

#define NUM_LIGHTS 2
uniform LightSource lights[NUM_LIGHTS]; // Luces

//Propiedades del objeto
vec3 Ka;
vec3 Kd;
vec3 Ks;
vec3 N;
float alpha = 5000.0;
vec3 Ke;

//Propiedades de la luz
vec3 Ia = vec3 (0.3);
vec3 Id = vec3 (1.0);
vec3 Is = vec3 (1.0);
vec3 lpos = vec3 (0.0); 

vec3 shade(LightSource light);

void main()
{

	Ka = texture(colorTex, texCoord).rgb;
	Kd = texture(colorTex, texCoord).rgb;
	Ke = texture(emiTex, texCoord).rgb;
	Ks = texture(specTex, texCoord).rgb;//vec3 (1.0);

	N = normalize (norm);
	N = normalize(texture( normalTex, texCoord).rgb*2.0 - 1.0);

	vec3 finalColor = vec3(0.0);

	for(int i = 0; i < NUM_LIGHTS; i++)
	{
		finalColor += shade(lights[i]);
	}
	
	finalColor += Ke;

	outColor = vec4(finalColor, 1.0);
	
	//outColor = vec4(0.0,1.0,0.0, 1.0);
}

vec3 shade(LightSource light)
{
	vec3 color = vec3(0);

	//Ambiental
	vec3 resi = clamp(light.Amb * Ka, 0.0, 1.0);

	//CosLD para si ha de ser iluminado por el ángulo de corte
	vec3 L = normalize(light.Pos.xyz - pos * light.Pos.w); L = normalize(TBN * L); 
	vec4 D = normalize(light.Dir);
	float cosLD = dot(-L, D.xyz);

	if(cosLD >= light.CosCutOff)
	{
		//Diffuse
		vec3 diff = light.Diff * Kd * dot(N, L);
		diff = clamp(diff, 0.0, 1.0);

		//Specular
		vec3 V = normalize(-pos);V = normalize(TBN * V);	
		vec3 R = reflect(-L, N); //-L porque representa la dirección de los fotones sobre la superficie
		float SFactor = max(dot(R,V), 0.01);
		SFactor = pow(SFactor, 5000.0);
		vec3 spec = light.Diff * Ks * SFactor;
		spec = clamp(spec, 0.0, 1.0);
		
		float spotAtt = 1.0;
		if(light.CosCutOff != -1.0) //Si es un foco
		{
			spotAtt = cosLD - light.CosCutOff;
			float denominador = 1 - light.CosCutOff;
			spotAtt = pow(spotAtt / denominador, light.SpotExponent);
			//spotAtt = cosLD * SpotExponent;
			
		}
		resi += (diff + spec) * spotAtt;
		
	}

	if(light.Pos.w == 1)
	{
		//Att.
		float dist = distance(light.Pos.xyz, pos);
		float att = max(light.C.x + light.C.y * dist + light.C.z * pow(dist, 2), 1.0);
		att = min(1 / att, 1.0);
		resi /= att;
	}
	
	color = resi;	

	return color;
}
