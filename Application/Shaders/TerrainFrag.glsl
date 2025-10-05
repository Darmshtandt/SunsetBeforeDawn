#version 420

in vec3 VertexPosition;
in vec3 VertexNormal;
in vec3 VertexTexCoords;
in vec4 VertexColor;
in vec3 FragPosition;

out vec4 Color;

uniform vec4 RenderColor;
uniform int IsTerrain = 0;
uniform vec2 TerrainSize;
uniform vec3 lightDirection;
uniform vec3 cameraPosition;

layout(binding = 0) uniform sampler2D Texture;
layout(binding = 1) uniform sampler2D BiomeMap;
layout(binding = 2) uniform sampler2D BiomeTextures[5];

float noise(vec3 p) {
	return fract(sin(dot(p, vec3(12.9898, 78.233, 45.164))) * 43758.5453);
}

float traceFog(vec3 rayOrigin, vec3 rayDirection) {
	float fogAmount = 0.0;
	float fogRadius = 1000.0;
	float fogDensity = 10.0;
	float stepSize = 0.1;
	vec3 position = -rayOrigin;
	
	float distToFog = length(position - cameraPosition);
	float fogFactor = 1.0 - smoothstep(0.0, fogRadius, distToFog);
		
	fogAmount += fogFactor * fogDensity * stepSize;        
	position += rayDirection * stepSize;

	return clamp(fogAmount, 0.1, 1.0);
}

vec4 ComputeTextureBlending(vec4 textureColor, float distanceInPercent, int nextXBiomeID, int nextYBiomeID) {
	vec4 nextTextureColor = texture(BiomeTextures[nextXBiomeID], VertexTexCoords.xy);
	if (nextXBiomeID == nextYBiomeID)
		nextTextureColor = texture(BiomeTextures[nextXBiomeID], VertexTexCoords.xy);

	textureColor = vec4(1, 0, 0, 1);
	nextTextureColor = vec4(0, 1, 0, 1);

	textureColor *= 1.0 - distanceInPercent;
	nextTextureColor *= distanceInPercent;
	textureColor = textureColor + nextTextureColor;
	return textureColor;
}

vec4 GetTexture() {
	if (IsTerrain == 0)
		return texture(Texture, VertexTexCoords.xy);
		
	vec2 terrainSize = vec2(16384.0, 16384.0);
	vec2 pixelPosition = vec2(FragPosition.z, FragPosition.x);
	vec2 biomeTexCoords = pixelPosition / terrainSize + 0.5;
	int biomeID = int(float(texture(BiomeMap, biomeTexCoords).r) * 256.0);
	
	vec4 textureColor = texture(BiomeTextures[biomeID], VertexTexCoords.xy);
	return textureColor;

	vec2 vertexDensity = vec2(64, 64); 
	vec2 vertexDistance = terrainSize / vertexDensity;

	vec2 terrainPoint = vec2(VertexPosition.x, VertexPosition.z);

	vec2 nextVertexPosition = terrainPoint + vertexDistance;

	int nextXBiomeID = -1;
	int nextYBiomeID = -1;
	if (nextVertexPosition.x <= terrainSize.x) {
		vec2 nextBiomeTexCoords = vec2(VertexPosition.z, nextVertexPosition.x) / terrainSize + 0.5;
		nextXBiomeID = int(float(texture(BiomeMap, nextBiomeTexCoords).r) * 256.0);
		if (nextXBiomeID == biomeID)
			nextXBiomeID = -1;
	}
	if (nextVertexPosition.y <= terrainSize.y) {
		vec2 nextBiomeTexCoords = vec2(nextVertexPosition.y, VertexPosition.x) / terrainSize + 0.5;
		nextYBiomeID = int(float(texture(BiomeMap, nextBiomeTexCoords).r) * 256.0);
		if (nextYBiomeID == biomeID)
			nextYBiomeID = -1;
	}
	
	if (nextXBiomeID != -1 && nextYBiomeID != -1) {
		float distanceInPercent = length(fract(vec2(FragPosition.x, FragPosition.z) / vertexDistance));
		textureColor = vec4(0, 0, 0, 1);
	}
	else {
		if (nextXBiomeID != -1) {
			float distanceInPercent = fract(FragPosition.x / vertexDistance.x);
			textureColor = ComputeTextureBlending(textureColor, distanceInPercent, nextXBiomeID, nextYBiomeID);
		}
		else if (nextYBiomeID != -1 && false) {
			float distanceInPercent = fract(FragPosition.z / vertexDistance.y);
			textureColor = ComputeTextureBlending(textureColor, distanceInPercent, nextXBiomeID, nextYBiomeID);
		}
	}

	return textureColor;
}

vec4 ComputeDirectionLight(vec4 color) {
	vec3 lightColor = vec3(1.0, 1.0, 0.8);
	vec3 ambientColor = vec3(0.25, 0.25, 0.25);
	float shininess = 32.0;
	
	vec3 ambient = ambientColor * lightColor;
	vec3 lightDirectiona = normalize(vec3(-1.0, 1.0, 1.0));

	float diffusion = max(dot(VertexNormal, lightDirectiona), 0.0);
	vec3 diffuse = diffusion * lightColor;
	
	vec3 lightToFrag = normalize(vec3(10.0, 100.0, 100.0) - FragPosition);
	vec3 reflectDir = reflect(-lightDirectiona, VertexNormal);

	float spec = pow(max(dot(lightToFrag, reflectDir), 0.0), shininess);
	vec3 specular = spec * lightColor;

	vec3 lighting = ambient + diffuse + specular;
	return clamp(color * vec4(lighting, 1.0), 0.0, 1.0);
}

vec4 ComputeFog(vec4 color) {
	vec3 viewDirirection = normalize(cameraPosition - FragPosition);
	float fogFactor = traceFog(FragPosition, normalize(viewDirirection));
	
	vec4 fogColor = vec4(0.5, 0.5, 1.0, 1.0);
	return mix(fogColor, color, fogFactor);
}

void main() {
	vec4 TextureColor = GetTexture();
	Color = TextureColor * RenderColor * VertexColor;

	Color = ComputeDirectionLight(Color);
	//Color = ComputeFog(Color);
}	