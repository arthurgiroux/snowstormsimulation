uniform mat4 worldcamera;
uniform vec4 cameraorigin;
uniform mat4 projection;
void main() {
    gl_Position = projection * worldcamera * gl_Vertex;
    float dist = length(gl_Position - cameraorigin);
    if (dist < 0.0) dist = -dist;
    gl_PointSize = 10.0/dist;
}