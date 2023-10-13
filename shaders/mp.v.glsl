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
uniform int numDirLights;
uniform int numPointLights;
uniform int numSpotLights;
uniform DirectionalLight dirLights[10];
uniform PointLight pointLights[10];
uniform vec3 materialColor;             // the material color for our vertex (& whole object)
uniform vec3 lightColor;
uniform vec3 lightDirection;
uniform vec3 lookAtDir;
float alpha = 0.1;

// attribute inputs
layout(location = 0) in vec3 vPos;      // the position of this specific vertex in object space
layout(location = 1) in vec3 vNormal;

// varying outputs
layout(location = 0) out vec3 color;    // color to apply to this vertex

vec3 calcPointLight(PointLight pLight, vec4 vPosTrans, vec4 vNormTrans){
    vec4 lightDirection = vec4(pLight.pos,1)-vPosTrans; //get the light direction to point light
    float distance = length(lightDirection);
    lightDirection = normalize(lightDirection);

    vec3 ambient = pLight.color*.1*materialColor;
    vec3 diffuse = pLight.color*materialColor*max(dot(lightDirection,vNormTrans),0);
    vec4 reflectanceVec = lightDirection + 2 *(dot(vNormTrans,-lightDirection))*vNormTrans;
    vec3 reflectance = lightColor * materialColor * max(dot(vec4(lookAtDir,1), reflectanceVec), 0.0);

    float attenuation  = pLight.atten.lin + pLight.atten.quad * distance + pLight.atten.exp*distance*distance;
    vec3 color = diffuse + ambient + reflectance;
    color = color/attenuation;
    return color;
}
void main() {
    // transform & output the vertex in clip space
    gl_Position = mvpMatrix * vec4(vPos, 1.0);

    // compute Light vector
    //vec3 lightDirectionReversed = -1 * lightDirection;
//    vec3 lightDirectionReversed = normalize(-lightDirection);
    // transform normal vector
    vec3 normalTransformed = normalMatrix * vNormal;
    vec4 vertexTransformed = modelMtx*vec4(vPos,1);
    color = vec3(0,0,0);
    for(int i=0;i<numPointLights;i++){
        color += calcPointLight(pointLights[i], vertexTransformed, vec4(normalTransformed,1));
    }

}