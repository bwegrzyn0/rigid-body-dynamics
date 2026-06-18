#version 330 core

in vec3 normal;
in vec3 fragPos;

out vec4 FragColor;

uniform vec3 defColor;
uniform vec3 lightPos;
uniform samplerCube skybox;
uniform vec3 cameraPos;

void main() {
	float ambientBrightness = 0.3f;
	vec3 lightUnitVec = normalize(lightPos - fragPos);
	float cos_theta = max(dot(normalize(normal), lightUnitVec), 0.0f);
	vec3 diffuse = cos_theta * vec3(1.0f);
	vec3 reflectionDir = reflect(normalize(fragPos - cameraPos), normal);
	vec4 reflection = texture(skybox, reflectionDir);
	FragColor = vec4(defColor * ambientBrightness + defColor * diffuse + reflection.xyz * 0.4f, 1.0f);
	// FragColor = vec4(reflection.xyz, 1.0f);
}
