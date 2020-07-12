#version 440

// lighting
struct Material{
    vec3 ambient;// intensity and other metadata
    vec3 diffuse;
    vec3 specular;
    sampler2D diffuseTex;// color data
    sampler2D specularTex;// color data
};

in vec3 vs_position;
in vec3 vs_color;
in vec2 vs_texcoord;
in vec3 vs_normal;

out vec4 fs_color;

// uniforms
uniform Material material;

uniform vec3 lightPos0;
uniform vec3 cameraPos;

// functions

vec3 calculateAmbient(Material material){
    return material.ambient;
}

vec3 calculateDiffuse(Material material,vec3 vs_position,vec3 vs_normal,vec3 lightPos0){
    vec3 posToLightDirVec=normalize(lightPos0-vs_position);
    float diffuse=clamp(dot(posToLightDirVec,vs_normal),0,1);
    return(material.diffuse)*diffuse;
}

vec3 calculateSpecular(Material material,vec3 vs_position,vec3 vs_normal,vec3 lightPos0, vec3 cameraPos){
    vec3 lightToPosDirVec=normalize(vs_position-lightPos0);
    vec3 reflectDirVec=normalize(reflect(lightToPosDirVec,normalize(vs_normal)));
    vec3 posToViewDirVec=normalize(cameraPos-vs_position);
    float specularConstant=pow(max(dot(posToViewDirVec,reflectDirVec),0),10);
    return (material.specular)*specularConstant;
    
}

void main(){
    
    // Ambient Light
    vec3 ambientFinal=calculateAmbient(material);
    
    // Diffuse Light
    vec3 diffuseFinal=calculateDiffuse(material,vs_position,vs_normal,lightPos0);
    
    // Specular Light
    vec3 specularFinal= calculateSpecular(material, vs_position,vs_normal,lightPos0, cameraPos);
    // Attenuation
    
    fs_color=texture(material.diffuseTex,vs_texcoord)*vec4(vs_color,1.f)*texture(material.specularTex,vs_texcoord)
    *(vec4(ambientFinal,1.f)+vec4(diffuseFinal,1.f)+vec4(specularFinal,1.f));
    
}