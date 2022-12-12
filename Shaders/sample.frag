// Direction Light
#version 330 core

uniform sampler2D tex0;
uniform sampler2D norm_tex;
uniform sampler2D tex2;

uniform vec3 lightPos;
uniform vec3 lightColor;

uniform float ambientStr;
uniform vec3 ambientColor;

uniform vec3 cameraPos;
uniform float specStr;
uniform float specPhog;

in vec2 texCoord;
in vec3 normCoord;
in vec3 fragPos;

in mat3 TBN;

out vec4 FragColor;

void main()
{
	float op = 1.0f;
	vec4 pixelColor = texture(tex0, texCoord);
	if(pixelColor.a < 0.1){
		discard;
	}

	//vec3 normal = normalize(normCoord);
	vec3 normal = texture(norm_tex, texCoord).rgb;
	normal = normalize(normal * 2.0 - 1.0);
	normal = normalize(TBN * normal);

	vec3 lightDir = normalize(lightPos - fragPos);

	float diff = max(dot(normal, lightDir), 0.0f);
	vec3 diffuse = diff * lightColor;

	vec3 ambientCol = ambientStr * ambientColor;
	
	vec3 viewDir = normalize(cameraPos - fragPos);
	vec3 reflectDir = reflect(-lightDir, normal);

	float spec = pow(max(dot(reflectDir, viewDir), 0.1f),specPhog);
	vec3 specCol = spec * specStr * lightColor;

	FragColor = vec4(specCol + diffuse + ambientCol, 1.0) * texture(tex0, texCoord) * texture(tex2, texCoord);
}