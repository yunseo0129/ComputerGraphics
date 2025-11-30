#version 330 core

in vec3 FragPos;
in vec3 Normal;

out vec4 FragColor;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform float ambientStrength;
uniform bool lightOn;

void main()
{
    vec3 result = vec3(0.0);
    if (lightOn) {
        vec3 ambient = ambientStrength * lightColor;
        result = ambient * objectColor;
    }
    FragColor = vec4(result, 1.0);
}
