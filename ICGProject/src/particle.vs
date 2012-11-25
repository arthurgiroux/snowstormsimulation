#version 110
uniform mat4 worldcamera;
uniform mat4 projection;
uniform vec3 v;
uniform float tZero;
attribute vec4 initPos;
uniform float T;
  
void main() {
    float t = T - tZero;
    vec3 p;
 
    gl_Position = projection * worldcamera * initPos;
}