#version 330 core

in vec2 texCoord;

out vec4 FragColor;

// uniform sampler2D aTexture;

void main() {
	// FragColor = texture(aTexture, texCoord);
	FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
}
