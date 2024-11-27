in vec3 vCol;
in vec2 vTexCoord;

uniform sampler2D uTexture;

void main() {
	// gl_FragColor = vec4(vCol + textureColor, 1.0);
	gl_FragColor = mix(vec4(vCol, 1.0), texture2D(uTexture, vTexCoord), 0.2);
}