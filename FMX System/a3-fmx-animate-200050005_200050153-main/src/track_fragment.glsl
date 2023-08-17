#version 330

in vec3 fWorldPos;
in vec3 fNormal;
in vec2 fTexcoord;


struct PointLight
{
	int on;
	vec3 position;
	float range;
};

struct SpotLight
{
	int on;
	vec3 position;
	vec3 direction;
	float innerCutoff;
	float outerCutoff;
	float range;
};

vec4 calculateShading(PointLight light, vec3 viewDir)
{
	float dist = length(light.position - fWorldPos);
	vec3 lightDir = (light.position - fWorldPos) / dist;

	float attentuation = clamp(1 - (dist * dist)/(light.range * light.range*10), 0.0 , 1.0);
	//diffuse
	float diff = max(dot(fNormal, lightDir), 0.0);
	vec4 diffuse = diff * vec4(1);

	//specular
	vec3 halfwayDir = normalize(lightDir - viewDir);
	float spec = pow(max(dot(fNormal, halfwayDir), 0.0), 64);
	vec4 specular = spec * vec4(0.2);
	diffuse = diffuse * attentuation;
	specular = specular * attentuation;
	vec4 result = (diffuse + specular);
	return result;
}

vec4 calculateShading(SpotLight light, vec3 viewDir, int atten)
{
 	float dist = length(light.position - fWorldPos);
	vec3 lightDir = normalize(light.position - fWorldPos);
	vec4 result;
//
//	
	float attentuation = clamp(1 - (dist * dist)/(light.range * light.range), 0.0 , 1.0);
	float theta = dot(lightDir, -light.direction);
	if (theta > light.outerCutoff)
	{	
		float intensity = clamp((theta - light.outerCutoff) / (light.innerCutoff - light.outerCutoff), 0.0, 1.0);
		float diff = max(dot(fNormal, lightDir), 0.0);
		vec4 diffuse = diff * vec4(1);
		vec3 halfwayDir = normalize(lightDir - viewDir);
		float spec = pow(max(dot(halfwayDir, fNormal), 0.0), 32);
		vec4 specular = spec * vec4(0.2);
		result = (diffuse*1.2 + specular) * intensity;
	}
	else
	{
		result = vec4(0.0 , 0.0 , 0.0 , 1.0);
	}
	if(atten==1)
		return result*attentuation*2;
	else
		return result;
}

uniform vec3 uCameraPos;
uniform PointLight uPointLights[2];
uniform SpotLight uSpotLight;
uniform SpotLight uHeadLight;

uniform sampler2D uColTex;

out vec4 frag_color;

void main () 
{
	vec3 viewDir = normalize(fWorldPos - uCameraPos);

	vec4 lighting=vec4(0,0,0,0);
	if(uPointLights[0].on==1)
		lighting += calculateShading(uPointLights[0], viewDir);
	if(uPointLights[1].on==1)
		lighting += calculateShading(uPointLights[1], viewDir);
	if(uSpotLight.on==1)
		lighting += calculateShading(uSpotLight, viewDir, 0);
	if(uHeadLight.on==1)
		lighting += calculateShading(uHeadLight, viewDir, 1);


  //frag_color = vec4(0.6, 0.2, 0.0, 1.0) * lighting;
  frag_color = texture(uColTex, 2*fTexcoord) * lighting;
}
