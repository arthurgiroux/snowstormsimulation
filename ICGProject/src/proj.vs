uniform mat4 worldcamera;
uniform mat4 projection;
uniform mat4 modelworld;
varying vec4 color;
void main() {
    gl_Position = projection * worldcamera * modelworld * gl_Vertex;
    color = gl_Color;
}