#version 330 core

in vec3 normal;
in vec3 fragPos;

out vec4 FragColor;

uniform vec3 defColor;
uniform vec3 lightPos;

void main() {
	float ambientBrightness = 0.5f;
	vec3 lightUnitVec = normalize(lightPos - fragPos);
	float cos_theta = max(dot(normalize(normal), lightUnitVec), 0.0f);
	vec3 diffuse = cos_theta * vec3(1.0f);
	FragColor = vec4(defColor * ambientBrightness + defColor * diffuse, 1.0f);
}
