#version 410 core

struct DirectionalLight{
    vec3 color;
    vec3 direction;
};
struct Attenuation{
    float lin;
    float quad;
    float exp;
};
struct PointLight{
    Attenuation atten;
    vec3 color;
    vec3 pos;
};

// uniform inputs
uniform mat4 mvpMatrix;                 // the precomputed Model-View-Projection Matrix
uniform mat4 modelMtx;
uniform mat3 normalMatrix;
uniform DirectionalLight dirLight;
uniform PointLight pointLight;
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
    // transform normal vector
    vec3 normalTransformed = normalMatrix * vNormal;
    vec4 rand = modelMtx*vec4(vPos,1);
    vec4 lightDirection = vec4(pointLight.pos,1)-rand;
    float distance = length(lightDirection);
    lightDirection = normalize(lightDirection);
//    vec3 lightDirection = normalize(-dirLight.direction);
    // perform diffuse calculation

    vec3 ambient = pointLight.color*.1*materialColor;
    vec3 diffuse = pointLight.color*materialColor*max(dot(lightDirection,vec4(normalTransformed,1)),0);
//    vec3 reflect = normalize(reflect(lightDirection,vNormal));
//    vec3 specular = pointLight.color * materialColor * (max(dot(normalTransformed, lightDirection), 0.0));


    // assign the color for this vertex
//    color = dirLight.color * materialColor * (max(dot(normalTransformed, lightDirection), 0.0));
    float attenuation  = pointLight.atten.lin + pointLight.atten.quad * distance + pointLight.atten.exp*distance*distance;
    color = diffuse + ambient;
    color = color/attenuation;
}