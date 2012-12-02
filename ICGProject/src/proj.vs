uniform mat4 worldcamera;
uniform mat4 projection;
uniform mat4 modelworld;
void main() {
    gl_Position = projection * worldcamera * modelworld * gl_Vertex;
}