uniform mat4 worldcamera;
uniform mat4 projection;
uniform mat4 billboard;
attribute vec3 pos;
void main() {
    mat4 trans = mat4(1, 0, 0, 0,
    0, 1, 0, 0,
    0, 0, 1, 0,
    -gl_Vertex.x, -gl_Vertex.y, -gl_Vertex.z, 1);
    
    mat4 transback = mat4(1, 0, 0, 0,
    0, 1, 0, 0,
    0, 0, 1, 0,
    gl_Vertex.x, gl_Vertex.y, gl_Vertex.z, 1);
    
    float angle = 3.14 / 2.0;
    mat4 rotation = mat4(1, 0, 0, 0,
                         0, cos(angle), sin(angle), 0,
                         0, -sin(angle), cos(angle), 0,
    0, 0, 0, 1);
    
    mat4 trans2 = mat4(1, 0, 0, 0,
    0, 1, 0, 0,
    0, 0, 1, 0,
    pos.x, pos.y, pos.z, 1);
    gl_Position = projection * worldcamera * trans2 * billboard * gl_Vertex;
    gl_TexCoord[0]  = gl_MultiTexCoord0;
    billboard;
}