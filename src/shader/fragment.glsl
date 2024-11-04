
#version 330 core
out vec4 FragColor;
in vec2 TexCoord;
in vec3 Normal;
in vec3 crntPos;


uniform vec3 viewPos;
struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
}; 
uniform Material material;
struct DirectionalLight {
	vec3 direction;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};
uniform DirectionalLight dirLight;

struct PointLight {
	vec3 Position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	vec4 Color;
	float constant;
	float linear;
	float quadratic;
};
uniform PointLight pointLight[4];

struct Light {
    vec3 Position;
    vec3 Direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    vec4 Color;
    float constant;
    float linear;
    float quadratic;
    float cutOff;
    float outerCutOff;
};
uniform Light light;  
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.Position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // attenuation
    float distance    = length(light.Position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + 
  			     light.quadratic * (distance * distance));    
    // combine results
    vec3 ambient  = light.ambient  * vec3(texture(material.diffuse, TexCoord));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.diffuse, TexCoord));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoord));
    ambient *=attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular)*light.Color.xyz;
}  

vec3 CalcDirLight(DirectionalLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // combine results
    vec3 ambient  = light.ambient  * vec3(texture(material.diffuse, TexCoord));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.diffuse, TexCoord));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoord));
    return (ambient + diffuse + specular);
} 

void main()
{ 
float distance    = length(light.Position - crntPos);
float attenuation = 1.0 / (light.constant + light.linear * distance + 
    		    light.quadratic * (distance * distance)); 
vec3 lightDir = normalize(light.Position - crntPos);
float theta = dot(lightDir, normalize(-light.Direction));
float epsilon = light.cutOff - light.outerCutOff;
float intensity= clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

    // diffuse 
    float diff = max(dot(Normal, lightDir), 0.0);
    // specular
    vec3 viewDir = normalize(viewPos - crntPos);
    vec3 reflectDir = reflect(-lightDir, Normal);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoord));
vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoord));  ;
vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoord));  
ambient  *= attenuation; 
diffuse  *= attenuation;
specular *= attenuation; 
diffuse  *= intensity;
specular *= intensity;
vec3 result = (ambient + diffuse + specular)*light.Color.xyz;
for(int i = 0; i < 4; i++){
	result += CalcPointLight(pointLight[i], Normal, crntPos, viewDir);
}
result+= CalcDirLight(dirLight, Normal, viewDir);
   FragColor = vec4(result, 1.0);
   
   }
