#version 330 core

in vec3 inPos;	
in vec3 inColor;
in vec2 inTexCoord;
in vec3 inNormal;
in vec3 inTangent;

uniform mat4 modelViewProj;
uniform mat4 modelView;
uniform mat4 normal;

out vec3 color;
out vec3 pos;
out vec3 norm;
out vec2 texCoord;
out mat3 TBN;

void main()
{

	//Calcular bitangente etc
	vec3 bitangent = cross(inTangent, inNormal);

	vec3 n = normalize(normal * vec4(inNormal, 0.0)).xyz;
	vec3 t = normalize(normal * vec4(inTangent, 0.0)).xyz;
	vec3 b = normalize(normal * vec4(bitangent, 0.0)).xyz;

	TBN = transpose(mat3(t,b,n));

	color = inColor;
	texCoord = inTexCoord;
	norm = (normal * vec4(inNormal, 0.0)).xyz;
	pos = (modelView * vec4(inPos, 1.0)).xyz;
	
	gl_Position =  modelViewProj * vec4 (inPos,1.0);
}
