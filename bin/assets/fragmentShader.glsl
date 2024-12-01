in vec3 vCol;
in vec2 vTexCoord;

uniform sampler2D uTexture;

void main() {
	// gl_FragColor = vec4(vCol + textureColor, 1.0);
	gl_FragColor = mix(mix(vec4(vCol, 1.0), vec4(1.0, 1.0, 1.0, 1.0), 0.5), texture2D(uTexture, vTexCoord), 0.002);
}