uniform mat4 worldcamera;
uniform mat4 projection;
uniform mat4 billboard;
attribute vec3 pos;
void main() {
    mat4 trans = mat4(1, 0, 0, 0,
                      0, 1, 0, -1,
                      0, 0, 1, 0,
    0, 0, 0, 1);
    mat4 scaling = mat4(0.5, 0, 0, 0,
                        0, 0.5, 0, 0,
                        0, 0, 0.5, 0,
    0, 0, 0, 1);
    gl_Position = projection * worldcamera * gl_Vertex;
    gl_TexCoord[0]  = gl_MultiTexCoord0;
    billboard;
    pos;
    worldcamera;

}