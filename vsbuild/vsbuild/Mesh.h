#ifndef _MESH_H_
#define _MESH_H_

#include "Const.h"
#include "Shader.h"

// ��ʾһ����������
// ------------------------------
struct vertexAddr
{
	vec3 position;
	vec3 normal;
	vec2 texCoords;
	vec3 tangent;
	vec3 bitangent;
};

// ���ڱ�ʾÿһ����������ݽṹ
// --------------------------------
struct textureAddr {
	GLuint  id;
	string type;
	string path;
};

GLuint loadTexture(GLchar const * path);

class Mesh {
public:
	vector<vertexAddr>   vertices;
	vector<textureAddr>  textures;
	vector<GLuint>       indices;
	GLuint               VAO;

	Mesh(vector<vertexAddr> vertices, vector<textureAddr> textures, vector<GLuint> indices);

	// ��Ⱦ����
	// --------------------------
	GLvoid draw(Shader & shader);

private:
	// ��Ⱦ����
	// ------------
	GLuint VBO, EBO;

	GLvoid setupMesh();
};

#endif // !_MESH_H_
