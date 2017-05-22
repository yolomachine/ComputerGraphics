#version 330 core
#define NR_POINT_LIGHTS 4

struct Material {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

struct DirLight {
    vec3 direction;
	
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    vec3 position;
    
    float constant;
    float linear;
    float quadratic;
	
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct SpotLight {
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;
  
    float constant;
    float linear;
    float quadratic;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;       
};

in vec3 newColor;
in vec2 newTextureCoords;
in vec3 fragPos;
in vec3 newNormal;
in vec4 vEyeSpacePos;
out vec4 resultingColor;

uniform sampler2D texture;

uniform float time;
uniform bool sepia = false;
uniform bool axes = false;
uniform bool fog = true;
uniform bool flashlight = false;
uniform float fogType = 0.0f;

uniform vec3 viewPos;
uniform vec3 lightColor;
uniform DirLight dirLight;
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform SpotLight spotLight;
uniform Material material;

vec3 getDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 getPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 getSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

float getFogFactor(float fFogCoord, float iEquation);

void main() {
	if (axes)
		resultingColor = vec4(newColor, 1.0f);
	else {
		vec3 norm 	 = normalize(newNormal);
    	vec3 viewDir = normalize(viewPos - fragPos);
    	vec3 result  = getDirLight(dirLight, norm, viewDir);

	    for(int i = 0; i < NR_POINT_LIGHTS; ++i)
	        result += getPointLight(pointLights[i], norm, fragPos, viewDir); 
	    if (flashlight)
	    	result += getSpotLight(spotLight, norm, fragPos, viewDir);
		resultingColor = vec4(result, 1.0f) * texture(texture, newTextureCoords);
	}

	if (sepia) {
		float filterColor = max(max(resultingColor.x, resultingColor.y), resultingColor.z);
	    resultingColor = vec4(filterColor * 0.85, filterColor * 0.8, 0.55 * filterColor, 1.0);
	};

	if (fog) 
		resultingColor = mix(resultingColor, vec4(0.5f, 0.5f, 0.5f, 1.0f), getFogFactor(abs(vEyeSpacePos.z / vEyeSpacePos.w), fogType));
};

float getFogFactor(float fFogCoord, float iEquation) {
   float fResult = 0.0f;
   float fStart = 0.0f;
   float fEnd = 300.0f;
   float fDensity = 0.03f;

   if(iEquation == 0)
      fResult = (fEnd*fEnd - fFogCoord*fFogCoord) / (fEnd*fEnd - fStart*fStart);
   else if(iEquation == 1)
      fResult = exp(-fDensity * fFogCoord);
   else if(iEquation == 2)
      fResult = exp(-pow(fDensity * fFogCoord, 2.0));
      
   fResult = 1.0 - clamp(fResult, 0.0, 1.0);
   
   return fResult;
};

vec3 getDirLight(DirLight light, vec3 normal, vec3 viewDir) {
    vec3 lightDir = normalize(-light.direction);

    float diff = max(dot(normal, lightDir), 0.0);

    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    vec3 ambient = light.ambient * material.diffuse;
    vec3 diffuse = light.diffuse * (diff * material.diffuse);
    vec3 specular = light.specular * (spec * material.specular);
    return (ambient + diffuse + specular);
};


vec3 getPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
    vec3 lightDir = normalize(light.position - fragPos);

    float diff = max(dot(normal, lightDir), 0.0);

    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    float distance = length(light.position - fragPos);
    float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    

    vec3 ambient = light.ambient * material.diffuse;
    vec3 diffuse = light.diffuse * (diff * material.diffuse);
    vec3 specular = light.specular * (spec * material.specular);
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
};

vec3 getSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
    vec3 lightDir = normalize(light.position - fragPos);
    float diff = max(dot(normal, lightDir), 0.0);

    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    float distance = length(light.position - fragPos);
    float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    

    float theta = dot(lightDir, normalize(-light.direction)); 
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

    vec3 ambient = light.ambient * material.diffuse;
    vec3 diffuse = light.diffuse * (diff * material.diffuse);
    vec3 specular = light.specular * (spec * material.specular);
    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    return (ambient + diffuse + specular);
}