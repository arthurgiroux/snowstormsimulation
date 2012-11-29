uniform mat4 worldcamera;
uniform vec4 cameraorigin;
uniform mat4 projection;
void main() {
    gl_Position = projection * worldcamera * gl_Vertex;
    gl_PointSize = clamp(10.0/length(gl_Position - cameraorigin), 1., 5.);
}