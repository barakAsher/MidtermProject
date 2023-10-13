#version 410 core

struct DirectionalLight{
    vec3 color;
    vec3 direction;
    float intensity;
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
struct SpotLight{
    Attenuation atten;
    vec3 color;
    vec3 pos;
    vec3 dir;
    float angle;
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
uniform SpotLight spotLights[10];
uniform vec3 materialColor;             // the material color for our vertex (& whole object)
uniform vec3 lookAtDir;
uniform float alpha;

// attribute inputs
layout(location = 0) in vec3 vPos;      // the position of this specific vertex in object space
layout(location = 1) in vec3 vNormal;

// varying outputs
layout(location = 0) out vec3 color;    // color to apply to this vertex

vec3 calcPointLight(PointLight pLight, vec4 vPosTrans, vec4 vNormTrans){
    vec4 lightDirection = vec4(pLight.pos,1)-vPosTrans; //get the light direction to point light
    float distance = length(lightDirection);
    lightDirection = normalize(lightDirection);

    vec3 diffuse = pLight.color*materialColor*max(dot(lightDirection,vNormTrans),0);
    vec3 ambient = pLight.color*.1*materialColor;
    vec4 reflectanceVec = lightDirection + 2 *dot(-lightDirection,vNormTrans)*vNormTrans;
    vec3 reflectance = pLight.color * materialColor * pow(max(dot(vec4(lookAtDir,1), reflectanceVec), 0),alpha);

    float attenuation  = pLight.atten.lin + pLight.atten.quad * distance + pLight.atten.exp*distance*distance;
    vec3 color = diffuse + ambient + reflectance;
    color = color/attenuation;
    return color;
}

vec3 calcDirLight(DirectionalLight dirLight, vec3 vNormTrans){
    vec3 lightDirection = normalize(-dirLight.direction);
    vec3 diffuse = dirLight.color * materialColor * (max(dot(vNormTrans, lightDirection), 0));
    vec3 ambient = dirLight.color*.1*materialColor;

    vec3 reflectanceVec = lightDirection + 2 *dot(-lightDirection,vNormTrans)*vNormTrans;
    vec3 reflectance = dirLight.color * materialColor * pow(max(dot(lookAtDir, reflectanceVec), 0),alpha);

    vec3 retColor = (diffuse +ambient + reflectance)* dirLight.intensity;
    return retColor;
}

vec3 calcSpotLight(SpotLight spotLight, vec4 vPosTrans, vec4 vNormTrans){
    vec4 lightDirection = -(vec4(spotLight.pos,1)-vPosTrans); //get the light direction to point light
    float distance = length(lightDirection);
    lightDirection = normalize(lightDirection);
    float checkDot = (dot(lightDirection,vec4(spotLight.dir,1)));
    if(checkDot < 0){
        return vec3(0,0,0);
    }
    float checkAngle = acos(checkDot);
    if (checkAngle > spotLight.angle){
        return vec3(0,0,0);
    }

    vec3 ambient = spotLight.color*.1*materialColor;
    vec3 diffuse = spotLight.color*materialColor*max(dot(lightDirection,vNormTrans),0);
    vec4 reflectanceVec = lightDirection + 2 *dot(-lightDirection,vNormTrans)*vNormTrans;
    vec3 reflectance = spotLight.color * materialColor * pow(max(dot(vec4(lookAtDir,1), reflectanceVec), 0),alpha);

    float attenuation  = spotLight.atten.lin + spotLight.atten.quad * distance + spotLight.atten.exp*distance*distance;
    vec3 retColor = diffuse + ambient + reflectance;
    retColor = retColor/attenuation;
    return retColor;
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
    for(int i=0;i<numDirLights;i++){
        color += calcDirLight(dirLights[i], normalTransformed);
    }
    for(int i=0;i<numPointLights;i++){
        color += calcPointLight(pointLights[i], vertexTransformed, vec4(normalTransformed,1));
    }
    for(int i=0;i<numSpotLights;i++){
        color += calcSpotLight(spotLights[i], vertexTransformed, vec4(normalTransformed, 1));
    }

}