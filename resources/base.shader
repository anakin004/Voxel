#shader vertex
#version 330 core

// Positions/Coordinates
layout (location = 0) in vec3 aPos;

// texCoord
layout (location = 1) in vec2 aTex;

layout (location = 2) in int aNormalIndex;


// Outputs the texture coordinates to the fragment shader
out vec2 v_texCord;

out vec3 v_normal;

uniform vec3 u_lightPos;

// so we dont have to define the normals for every face
uniform vec3 u_faceNorms[6];

// to take in matrix model view projection matrix 
uniform mat4 u_camMatrix;

out vec3 v_lightVec;



void main()
{
	// transforms all coordinates using the mvp / camMatrix
	gl_Position = u_camMatrix * vec4(aPos.x, aPos.y, aPos.z, 1.0f); 

	vec3 tempVec = vec3(u_lightPos.x - aPos.x ,  u_lightPos.y - aPos.y, u_lightPos.z - aPos.z );
	tempVec = normalize(tempVec);
	v_lightVec = tempVec;
	
	v_normal = u_faceNorms[aNormalIndex];

	vec2 temp = aTex;
	
	v_texCord = temp;
}


//-----------------------------------------


#shader fragment

#version 330 core
out vec4 texColor;

in vec2 v_texCord;

in vec3 v_normal;

in vec3 v_lightVec;

uniform vec3 u_lightPos;

uniform vec3 u_lightNorm;

uniform vec3 u_lightColor;

uniform vec3 u_camPos;



uniform sampler2D u_Texture;

void main()
{
	

	// take vector from light to block, and normalize light veftor and take dot product of face and normalize the light vector


	float ambient = 1.1;
	float diffuseStrength = max(dot(v_normal, v_lightVec), 0.0);
	//vec3 reflectSource = normalize(reflect(-u_lightNorm, v_normal));
	//float specularStrength = max(0.0, dot(u_camPos, reflectSource));
	//specularStrength = pow(specularStrength, 2.0);


	vec4 calculatedLight = vec4(( ambient + diffuseStrength ) * u_lightColor, 1.0);



	vec4 textureColors = texture(u_Texture, v_texCord) * calculatedLight;

	texColor = textureColors;

}