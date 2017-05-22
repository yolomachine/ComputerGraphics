#version 330 core
in vec3 texCoords;
in vec4 vEyeSpacePos;
out vec4 color;

uniform samplerCube skybox;
uniform float time;
uniform bool sepia = false;

float foo;

float getFogFactor(float fFogCoord, float iEquation);

void main() {   
	vec4 vFogColor = vec4(0.2f, 0.2f, 0.5f, 1.0f);
    float fFogCoord = abs(vEyeSpacePos.z / vEyeSpacePos.w);

    color = texture(skybox, texCoords);

    if (sepia) {
	    foo = max(max(color.x, color.y), color.z);
	    color = vec4(foo * 0.85, foo * 0.8, 0.55 * foo, 1.0);
	};

	//color = mix(color, vFogColor, getFogFactor(fFogCoord, 0.0f)); 
}

float getFogFactor(float fFogCoord, float iEquation) {
   float fResult = 0.0f;
   float fStart = 100.0f;
   float fEnd = 150.0f;
   float fDensity = 0.04f;
   fFogCoord = 140.0f;

   if(iEquation == 0)
      fResult = (fEnd - fFogCoord) / (fEnd - fStart);
   else if(iEquation == 1)
      fResult = exp(-fDensity * fFogCoord);
   else if(iEquation == 2)
      fResult = exp(-pow(fDensity * fFogCoord, 2.0));
      
   fResult = 1.0 - clamp(fResult, 0.0, 1.0);
   
   return fResult;
};