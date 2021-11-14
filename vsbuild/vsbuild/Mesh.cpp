#include "Mesh.h"

Mesh::Mesh(vector<vertexAddr> vertices, vector<textureAddr> textures, vector<GLuint> indices)
{
	this->vertices = vertices;
	this->textures = textures;
	this->indices = indices;

	// ��ȡ��������Ҫ�������Ժ󣬾Ϳ��Դ��͵���ʾ�б������Ⱦ�ļ�����
	// --------------------------------------------------------------
	setupMesh();
}

// ��Ⱦ����
// ----------------------
GLvoid Mesh::draw(Shader &shader)
{
	// ************************* ������ ************************* //
	// ************************************************************ //
	GLuint diffuseNr = 1;
	GLuint specularNr = 1;
	GLuint normalNr = 1;
	GLuint heightNr = 1;

	for (unsigned int i = 0; i < textures.size(); i++)
	{
		// �ڰ�ǰ������Ӧ��ŵ�����
		// ------------------------------
		glActiveTexture(GL_TEXTURE0 + i);

		string number;
		string name = textures[i].type;
		if (name == "texture_diffuse")
			number = to_string(diffuseNr++);
		else if (name == "texture_specular")
			number = to_string(specularNr++);
		else if (name == "texture_normal")
			number = to_string(normalNr++);
		else if (name == "texture_height")
			number = to_string(heightNr++);

		glUniform1i(glGetUniformLocation(shader.ID, (name + number).c_str()), i);
		glBindTexture(GL_TEXTURE_2D, textures[i].id);
	}

	// ************************* ���� mesh ************************* //
	// ************************************************************* //
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	// 
	//
	glActiveTexture(GL_TEXTURE0);
}

GLvoid Mesh::setupMesh()
{
	// ���� buffers / arrays
	// ------------------------ 
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	// ��������ݴ洢�� buffer ��
	// --------------------------
	glBindVertexArray(VAO);

	// ��������ݴ洢�� buffer ��
	// --------------------------------
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertexAddr), &vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);

	// positions
	// --------------------------
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertexAddr), (GLvoid*)offsetof(vertexAddr, position));

	// normals
	// --------------------------
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertexAddr), (GLvoid*)offsetof(vertexAddr, normal));

	// texture coords
	// --------------------------
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(vertexAddr), (GLvoid*)offsetof(vertexAddr, texCoords));

	// tangent
	// --------------------------
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(vertexAddr), (GLvoid*)offsetof(vertexAddr, tangent));

	// bitangent
	// --------------------------
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(vertexAddr), (GLvoid*)offsetof(vertexAddr, bitangent));

	glBindVertexArray(0);
}

GLuint loadTexture(GLchar const * path)
{
	// ������� ID
	// --------------
	GLuint textureID;
	glGenTextures(1, &textureID);

	// �� stbi_load ����ȥ����������ͼ
	// -------------------------------
	GLint width, height, nrComponents;
	stbi_uc * data = stbi_load(path, &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		// ������󶨵� buffer ��
		// -------------------------------------
		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		
		// Ϊ��ǰ�󶨵������Զ�����������Ҫ�Ķ༶��Զ����
		// -----------------------------
		glGenerateMipmap(GL_TEXTURE_2D);

		// Set the behavior when scaling
		// ----------------------------------------------------------
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// �ͷ����ݲ�����������
		// -------------------
		stbi_image_free(data);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	else
	{
		cout << "TEXTURE::LOAD::FAILED" << endl;
		cout << "PATH::" << path << endl;
		stbi_image_free(data);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	return textureID;
}