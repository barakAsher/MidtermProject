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
uniform vec3 lightColor;
uniform vec3 lookAtDir;
uniform int lightType;
uniform float alpha;

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
//    vec3 lightDirectionReversed = normalize(-lightDirection);
    // transform normal vector

    vec3 normalTransformed = normalMatrix * vNormal;

    if(lightType == 0){
        vec4 rand = modelMtx*vec4(vPos,1);
        vec4 lightDirection = vec4(pointLight.pos,1)-rand;
        float distance = length(lightDirection);
        lightDirection = normalize(lightDirection);
        //    vec3 lightDirection = normalize(-dirLight.direction);
        // perform diffuse calculation

        vec3 ambient = pointLight.color*.1*materialColor;
        vec3 diffuse = pointLight.color*materialColor*max(dot(lightDirection,vec4(normalTransformed,1)),0);
        vec4 reflectanceVec = -lightDirection + 2 *(dot(lightDirection,vec4(normalTransformed,1)))*vec4(normalTransformed,1);
        //    vec3 reflect = normalize(reflect(lightDirection,vNormal));
        //    vec3 specular = pointLight.color * materialColor * (max(dot(normalTransformed, lightDirection), 0.0));


        // assign the color for this vertex
        //    color = dirLight.color * materialColor * (max(dot(normalTransformed, lightDirection), 0.0));
        float attenuation  = pointLight.atten.lin + pointLight.atten.quad * distance + pointLight.atten.exp*distance*distance;
        //    vec3 reflectanceVec = lightDirectionReversed + 2 *(dot(normalTransformed,-lightDirectionReversed))*normalTransformed;
        vec3 reflectance = pointLight.color * materialColor * pow(max(dot(vec4(lookAtDir,1), reflectanceVec), 0),alpha);

        color = diffuse + ambient + reflectance;
        color = color/attenuation;
    }else if(lightType == 1){
        vec3 lightDirection = normalize(-dirLight.direction);
        vec3 diffuse = dirLight.color * materialColor * (max(dot(normalTransformed, lightDirection), 0));

        vec3 reflectanceVec = lightDirection + 2 *dot(-lightDirection,normalTransformed)*normalTransformed;
        vec3 reflectance = dirLight.color * materialColor * pow(max(dot(lookAtDir, reflectanceVec), 0),alpha);

        color = diffuse;
    }

    // assign the color for this vertex
    //color = lightColor * materialColor * diffuse;
}