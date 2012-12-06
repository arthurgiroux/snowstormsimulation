uniform sampler2D texture;
void main() {
    gl_FragColor = texture2D(texture, gl_TexCoord[0].xy);
    //gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);
    texture;
}