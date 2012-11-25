varying vec3 normal, lightDir;

uniform vec3 diffuseColor;
uniform vec3 lightcolor;
		
void main()
{	
	vec4 finalcolor = vec4(0.0);
    
    vec3 N = normalize(normal);
    vec3 Ldir = normalize(lightDir);
    float lambertTermDir = dot(N, Ldir);
    
    if (lambertTermDir > 0.0) {
        finalcolor += vec4(lightcolor, 1.0)*vec4(diffuseColor, 1.0)*lambertTermDir;
    }

	
	finalcolor += vec4(diffuseColor,1.0)*0.1;

    gl_FragColor = finalcolor;
	
}