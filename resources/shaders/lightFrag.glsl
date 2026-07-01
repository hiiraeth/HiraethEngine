#version 330 core

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;

    sampler2D texture_diffuse1;
    sampler2D texture_diffuse2;
    sampler2D texture_diffuse3;

    sampler2D texture_specular1;
    sampler2D texture_specular2;
    sampler2D texture_specular3;
};

struct Light {
    int type; // 0 = default point | 1 = directional
              // 2 = custom point  | 3 = spotlight
    vec3 position;
    vec3 direction;
    float cutoff; // for spotlight

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    // for attenuation, light type 2
    float constant;
    float linear;
    float quadratic;
};

#define NR_LIGHTS 4

in vec3 FragPos;
in vec3 Normal;
in vec2 TextureCoord;

out vec4 FragColor;

uniform bool bUseSpecularMap = false;
uniform bool bUseDiffuseMap = false;
uniform vec2 UVscale = vec2(1.0f, 1.0f);
uniform vec3 viewPos;
uniform Material material;
uniform Light lights[NR_LIGHTS];

// Function to calculate lighting for one light
vec3 CalculateLight(Light light, vec3 normal, vec3 fragPos, vec3 viewDir,
                    vec3 matDiffuse, vec3 matSpecular)
{
    if (light.type == 1) // directional
    {
        // Ambient
        vec3 ambient = light.ambient * matDiffuse;

        // Diffuse
        vec3 lightDir = normalize(-light.direction);
        float diff = max(dot(normal, lightDir), 0.0);
        vec3 diffuse = light.diffuse * diff * matDiffuse;  // Use passed-in diffuse

        // Specular
        vec3 reflectDir = reflect(-lightDir, normal);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
        vec3 specular = light.specular * spec * matSpecular; // use passed in specular

        return (ambient + diffuse + specular);
    }
    else if (light.type == 2) // custom point light
    {
        float distance = length(light.position - FragPos);
        float attenuation = 1.0 /
                            (light.constant + light.linear * distance + light.quadratic * pow(distance, 2));
        // Ambient
        vec3 ambient = light.ambient * matDiffuse;

        // Diffuse
        vec3 lightDir = normalize(light.position - fragPos);
        float diff = max(dot(normal, lightDir), 0.0);
        vec3 diffuse = light.diffuse * diff * matDiffuse;  // Use passed-in diffuse

        // Specular
        vec3 reflectDir = reflect(-lightDir, normal);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
        vec3 specular = light.specular * spec * matSpecular; // use passed in specular

        return (ambient + diffuse + specular) * attenuation;
    }
    else if (light.type == 3) // Spotlight
    {
        // 1. Geometry Data
        vec3 lightDir = normalize(light.position - fragPos);
        float distance = length(light.position - fragPos);

        // 2. Soft Edge Intensity
        float theta = dot(lightDir, normalize(-light.direction));
        float outerCutoff = light.cutoff - 0.05; // Outer edge of spotlight cone
        float epsilon = light.cutoff - outerCutoff; // Fade range (0.05)
        float intensity = clamp((theta - outerCutoff) / epsilon, 0.0, 1.0);

        // 3. Attenuation
        float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

        // 4. Calculate Color
        // Ambient (minimal, attenuated)
        vec3 ambient = light.ambient * matDiffuse;

        // Diffuse
        float diff = max(dot(normal, lightDir), 0.0);
        vec3 diffuse = light.diffuse * diff * matDiffuse;

        // Specular
        vec3 reflectDir = reflect(-lightDir, normal);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
        vec3 specular = light.specular * spec * matSpecular;

        // 5. Apply Modifiers - Apply intensity and attenuation to ALL components
        ambient  *= attenuation * intensity;
        diffuse  *= attenuation * intensity;
        specular *= attenuation * intensity;

        return (ambient + diffuse + specular);
    }
    else // default point light
    {
        // Ambient
        vec3 ambient = light.ambient * matDiffuse;

        // Diffuse
        vec3 lightDir = normalize(light.position - fragPos);
        float diff = max(dot(normal, lightDir), 0.0);
        vec3 diffuse = light.diffuse * diff * matDiffuse;  // Use passed-in diffuse

        // Specular
        vec3 reflectDir = reflect(-lightDir, normal);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
        vec3 specular = light.specular * spec * matSpecular; // use passed in specular

        return (ambient + diffuse + specular);
    }
}

void main()
{
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    // Determine what to use for diffuse color
    vec3 diffuseColor;
    vec3 specularColor;

    if (bUseDiffuseMap == true)
    {
        diffuseColor = vec3(texture(material.texture_diffuse1, TextureCoord * UVscale));
    }
    else
    {
        diffuseColor = material.diffuse;
    }

    if (bUseSpecularMap == true)
    {
        specularColor = vec3(texture(material.texture_specular1, TextureCoord * UVscale));
    }
    else
    {
        specularColor = material.specular;
    }

    // Calculate lighting from all lights
    vec3 result = vec3(0.0);
    for(int i = 0; i < NR_LIGHTS; i++)
    {
        result += CalculateLight(lights[i], norm, FragPos,
                                 viewDir, diffuseColor, specularColor);
    }

    FragColor = vec4(result, 1.0);
}
