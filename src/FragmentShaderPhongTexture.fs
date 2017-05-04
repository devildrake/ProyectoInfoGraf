#version 330 core

#define NUM_MAX_PLIGHTS 4
#define NUM_MAX_SLIGHTS 4

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
vec3 viewPortDir = normalize(camPos - position.xyz);	

color = DirectionalLight(dlight,Normal,viewPortDir)+PointLight(plight[0],Normal,viewPortDir)+SpotLight(slight[0],Normal,viewPortDir);
} 

vec3 DirectionalLight(DLight light, vec3 Normal, vec3 viewDirection){
vec3 outColor;
vec3 ambient = vec3(texture(material.TexturaDifusa, TexCoord))*light.LAmbiental;

vec3 incidenciaLuzNorm = normalize(-light.dir);

    float diff = max(dot(normalize(Normal.xyz), incidenciaLuzNorm), 0.0);

    vec3 reflectDir = reflect(-incidenciaLuzNorm, normalize(Normal.xyz));
	
	
	//vec3 directionToViewPoint = normalize(viewPos - position.xyz);
	
	
    float spec = pow(max(dot(viewDirection, reflectDir), 0.0), material.shininess);

    vec3 diffuse  = Ldiffuse * diff * vec3(texture(material.TexturaDifusa, TexCoord));
	
    vec3 specular = 0.00001 * 0.00001 * spec * vec3(texture(material.TexturaEspecular, TexCoord));
	
outColor = vec4(iluminacionAmbientalL + diffuse + specular,1 );

return outColor;
}

vec3 PointLight(PLight light, vec3 Normal, vec3 viewDirection){
vec3 outColor;
vec3 ambient  =  iAmbiental * vec3(texture(material.TexturaDifusa, TexCoords));
 
	vec3 lightDir = normalize (light.pos - FragPos);
	
   float diff = max(dot(normalize(Normal),lightDir),0); 
	
	vec3 diffuse = light.intensidadFuenteDifusa*light.coeficienteDifuso*diff*vec3(texture(material.TexturaDifusa, TexCoords));
	
	//viewDirection se calcula con la linea de aqui abajo
	//vec3 viewPortDir = normalize(viewPos - FragPos);
	vec3 reflectionDir = reflect(-lightDir,normalize(Normal));
	
float esp = pow(max(dot(reflectionDir,viewDirection),0.0),material.shininess);
   
   vec3 specular = light.intensidadFuenteEspecular*light.coeficienteEspecular*esp*vec3(texture(material.TexturaEspecular, TexCoords));
   
   float d = length(light.pos - FragPos);
   
   float fAtt = 1/(1+0.045*d + 0.0075*d*d);
   
	outColor = (ambient+diffuse+specular)*fAtt;
	return outColor;
}


vec3 SpotLight(SLight light, vec3 Normal, vec3 viewDirection){

vec4 iluminacionDifusa;
vec4 iluminacionEspecular;
vec3 iluminacionAmbientalL = vec3(texture(material.TexturaDifusa, TexCoord))*light.iAmbiental;

   float d = length(light.pos - FragPos.xyz);
   float fAtt = 1/(1+0.045*d + 0.0075*d*d);
   

vec3 incidenciaLuzNorm = normalize(light.pos - FragPos.xyz);


float theta = dot(incidenciaLuzNorm,normalize(-light.dir));

	if(theta>light.aperturaMax){
		float diff = max(dot(normalize(Normal.xyz),incidenciaLuzNorm),0.0);
		vec3 diffuse = light.intensidadFuenteDifusa*light.coeficienteDifuso*diff*vec3(texture(material.TexturaDifusa, TexCoord));

		//vec3 viewPortDir = normalize(camPos - position.xyz);
		vec3 reflectionDir = reflect(-incidenciaLuzNorm,normalize(Normal.xyz));
		float esp = pow(max(dot(reflectionDir,viewDirection),0.0),material.shininess);

		vec3 specular = light.intensidadFuenteEspecular*light.coeficienteEspecular*esp*vec3(texture(material.TexturaEspecular, TexCoord)); 

		float epsilon = light.aperturaMin - light.aperturaMax;
		float inte = clamp((theta-light.aperturaMax)/epsilon,0,1);
   
   vec3 sumaIluminaciones = specular+iluminacionAmbientalL + diffuse;

		color = vec4(inte*fAtt*sumaIluminaciones,1);
   
	}else{
	color = vec4(fAtt * iluminacionAmbientalL,1);
	}
	
}