#version 410 core

struct DirectionalLight{
    vec3 color;
    vec3 direction;
};

// uniform inputs
uniform mat4 mvpMatrix;                 // the precomputed Model-View-Projection Matrix
uniform mat3 normalMatrix;
//uniform vec3 lightColor;
//uniform vec3 lightDirection;
uniform DirectionalLight dirLight;
uniform vec3 materialColor;             // the material color for our vertex (& whole object)

// attribute inputs
layout(location = 0) in vec3 vPos;      // the position of this specific vertex in object space
layout(location = 1) in vec3 vNormal;

// varying outputs
layout(location = 0) out vec3 color;    // color to apply to this vertex

void main() {
    // transform & output the vertex in clip space
    gl_Position = mvpMatrix * vec4(vPos, 1.0);

    // compute Light vector
    //vec3 lightDirectionReversed = -1 * lightDirection;
    vec3 lightDirectionReversed = normalize(-dirLight.direction);
    vec3 lightVector = normalize(lightDirectionReversed);

    // transform normal vector
    vec3 normalTransformed = normalMatrix * vNormal;

    // perform diffuse calculation
    float diffuse = max(dot(normalTransformed, lightVector), 0.0);

    // assign the color for this vertex
    //color = lightColor * materialColor * diffuse;
    color = dirLight.color * materialColor * (max(dot(normalTransformed, lightDirectionReversed), 1.0));
}