#version 330 core
out vec4 FragColor;

struct Material {
	sampler2D diffuse;
    sampler2D specular;    
    float shininess;
};

struct DirLight {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};  
struct PointLight {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

	float constant;
    float linear;
    float quadratic;
};

#define NR_POINT_LIGHTS 4

uniform vec3 viewPos;
uniform Material material;
uniform DirLight dirLight;
uniform PointLight pointLights[NR_POINT_LIGHTS];

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);


void main()
{
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    vec3 result = CalcDirLight(dirLight, norm, viewDir);
    
    for(int i = 0; i < NR_POINT_LIGHTS; i++)
        result += CalcPointLight(pointLights[i] , norm , FragPos , viewDir );

    FragColor = vec4(result, 1.0);
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    //ambient
    vec3 ambient = light.ambient * texture(material.diffuse,TexCoord).rgb;

    //diffuse
    vec3 lightDir = normalize(-light.direction);
    float diff = max(dot(normal,lightDir),0.0);
    vec3 diffuse = light.diffuse * diff * texture(material.diffuse,TexCoord).rgb;

    //specular
    vec3 reflectDir=reflect(-lightDir,normal);
    float spec=pow(max(dot(reflectDir,viewDir),0.0),material.shininess);
    vec3 specular=light.specular * spec * texture(material.specular,TexCoord).rgb;

    return (ambient + diffuse + specular);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    //attenuation
    float Distance= length(light.position - fragPos);
    float attenuation= 1.0/(light.constant + light.linear * Distance + light.quadratic * (Distance * Distance));

    //ambient
    vec3 ambient = light.ambient * texture(material.diffuse,TexCoord).rgb;

    //diffuse
    vec3 lightDir = normalize(light.position - fragPos);
    float diff = max(dot(lightDir,normal),0.0);
    vec3 diffuse = light.diffuse * diff * texture(material.diffuse,TexCoord).rgb;

    //specular
    vec3 reflectDir = reflect(-lightDir,normal);
    float spec = pow(max(dot(reflectDir,viewDir),0.0),material.shininess);
    vec3 specular = light.specular * spec * texture(material.specular,TexCoord).rgb;

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    return (ambient + diffuse + specular);
}