#version 440

// lighting
struct Material {
    vec3 ambient; // intensity and other metadata
    vec3 diffuse;
    vec3 specular;
    sampler2D diffuseTex; // color data
    sampler2D specularTex;// color data
};



in vec3 vs_position;
in vec3 vs_color;
in vec2 vs_texcoord;
in vec3 vs_normal;

out vec4 fs_color;

uniform Material material;


uniform vec3 lightPos0;
uniform vec3 cameraPos;

void main(){

    // Ambient Light
    vec3 ambientLight=material.ambient;
    
    // Diffuse Light
    vec3 posToLightDirVec=normalize(vs_position-lightPos0);
    vec3 diffuseColor=vec3(.8f,.8f,.8f);
    float diffuse=clamp(dot(posToLightDirVec,vs_normal),0,1);
    vec3 diffuseFinal=(material.diffuse)*diffuse;
    
    // Specular Light
    vec3 lightToPosDirVec=normalize(lightPos0-vs_position);
    vec3 reflectDirVec=normalize(reflect(lightToPosDirVec,normalize(vs_normal)));
    vec3 posToViewDirVec=normalize(vs_position-cameraPos);
    float specularConstant=pow(max(dot(posToViewDirVec,reflectDirVec),0),10);
    vec3 specularFinal = (material.specular) * specularConstant;
    
    // Attenuation
    
    fs_color=texture(material.diffuseTex,vs_texcoord)*vec4(vs_color,1.f) * texture(material.specularTex, vs_texcoord)
    *(vec4(ambientLight,1.f)+vec4(diffuseFinal,1.f) + vec4(specularFinal, 1.f));
    
}