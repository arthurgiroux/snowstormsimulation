uniform samplerCube cubeMap;
varying vec3 position;

void main()
{
    gl_FragColor = textureCube(cubeMap, position);
    //gl_FragColor = vec4(1.0,1.0,1.0,1.0);
}