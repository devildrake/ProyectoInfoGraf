#version 330 core

#define NUM_MAX_PLIGHTS 2
#define NUM_MAX_SLIGHTS 3

struct Material{
sampler2D TexturaDifusa;
sampler2D TexturaEspecular;
float shininess;
};


struct PLight {
 vec3 LAmbiental;
 vec3 Ldiffuse;
 vec3 Lspecular;
 vec3 pos;

};

struct DLight {
 vec3 LAmbiental;
 vec3 Ldiffuse;
 vec3 Lspecular;
 vec3 pos;
 vec3 dir;

};

struct SLight {
 vec3 LAmbiental;
 vec3 Ldiffuse;
 vec3 Lspecular;
 vec3 pos;
 vec3 dir;
 float aperturaMax;
 float aperturaMin;
};

in vec3 FragPos;  
in vec3 Normal;  
in vec2 TexCoords;
  
out vec4 color;
  
uniform vec3 viewPos;
uniform Material material;
uniform DLight dlight;
uniform PLight plight[NUM_MAX_PLIGHTS];
uniform SLight slight[NUM_MAX_SLIGHTS];

vec3 DirectionalLight(DLight light, vec3 Normal, vec3 viewDirection);
vec3 PointLight(PLight light, vec3 Normal, vec3 viewDirection);
vec3 SpotLight(SLight light, vec3 Normal, vec3 viewDirection);

void main(){
vec3 viewPortDir = normalize(viewPos - FragPos);




color = vec4(DirectionalLight(dlight,Normal,viewPortDir),1);

 
for(int i=0;i<NUM_MAX_PLIGHTS;i++){

color+=vec4(PointLight(plight[i],Normal,viewPortDir),1);
}
for(int i=0;i<NUM_MAX_SLIGHTS;i++){
color+=vec4(SpotLight(slight[i],Normal,viewPortDir),1);
}
} 

vec3 DirectionalLight(DLight light, vec3 Normal, vec3 viewDirection){
vec3 outColor;
vec3 ambient = vec3(texture(material.TexturaDifusa, TexCoords))*light.LAmbiental;

vec3 incidenciaLuzNorm = normalize(-light.dir);

    float diff = max(dot(normalize(Normal.xyz), incidenciaLuzNorm), 0.0);

    vec3 reflectDir = reflect(-incidenciaLuzNorm, normalize(Normal.xyz));
	
	
	//vec3 directionToViewPoint = normalize(viewPos - position.xyz);
	
	
    float spec = pow(max(dot(viewDirection, reflectDir), 0.0), material.shininess);

    vec3 diffuse  = light.Ldiffuse * diff * vec3(texture(material.TexturaDifusa, TexCoords));
	
    vec3 specular = 0.00001 * 0.00001 * spec * vec3(texture(material.TexturaEspecular, TexCoords));
	
outColor = ambient + diffuse + specular;

return outColor;
}

vec3 PointLight(PLight light, vec3 Normal, vec3 viewDirection){
vec3 outColor;
vec3 ambient  =  light.LAmbiental * vec3(texture(material.TexturaDifusa, TexCoords));
 
	vec3 lightDir = normalize (light.pos - FragPos);
	
   float diff = max(dot(normalize(Normal),lightDir),0); 
	
	vec3 diffuse = light.Ldiffuse*diff*vec3(texture(material.TexturaDifusa, TexCoords));
	
	//viewDirection se calcula con la linea de aqui abajo
	//vec3 viewPortDir = normalize(viewPos - FragPos);
	vec3 reflectionDir = reflect(-lightDir,normalize(Normal));
	
float esp = pow(max(dot(reflectionDir,viewDirection),0.0),material.shininess);
   
   vec3 specular = light.Lspecular*esp*vec3(texture(material.TexturaEspecular, TexCoords));
   
   float d = length(light.pos - FragPos);
   
   float fAtt = 1/(1+0.045*d + 0.0075*d*d);
   
	outColor = (ambient+diffuse+specular)*fAtt;
	return outColor;
}


vec3 SpotLight(SLight light, vec3 Normal, vec3 viewDirection){
vec3 outColor;
vec4 iluminacionDifusa;
vec4 iluminacionEspecular;
vec3 iluminacionAmbientalL = vec3(texture(material.TexturaDifusa, TexCoords))*light.LAmbiental;

   float d = length(light.pos - FragPos.xyz);
   float fAtt = 1/(1+0.045*d + 0.0075*d*d);
   

vec3 incidenciaLuzNorm = normalize(light.pos - FragPos.xyz);


float theta = dot(incidenciaLuzNorm,normalize(-light.dir));


		float diff = max(dot(normalize(Normal.xyz),incidenciaLuzNorm),0.0);
		vec3 diffuse = light.Ldiffuse*diff*vec3(texture(material.TexturaDifusa, TexCoords));

		//vec3 viewPortDir = normalize(camPos - position.xyz);
		vec3 reflectionDir = reflect(-incidenciaLuzNorm,normalize(Normal.xyz));
		float esp = pow(max(dot(reflectionDir,viewDirection),0.0),material.shininess);

		vec3 specular = light.Lspecular*esp*vec3(texture(material.TexturaEspecular, TexCoords)); 

		float epsilon = light.aperturaMin - light.aperturaMax;
		float inte = clamp((theta-light.aperturaMax)/epsilon,0,1);
   
   vec3 sumaIluminaciones = specular+iluminacionAmbientalL + diffuse;

		outColor = inte*fAtt*sumaIluminaciones;

	return outColor; 
}