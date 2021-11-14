#ifndef _MODEL_H_
#define _MODEL_H_


#include "Const.h"
#include "Mesh.h"
#include "Shader.h"

GLuint textureFromFile(const GLchar * path, const string & directory, GLboolean gamma = false);

class Model
{
public:
	// ************************* model ���������� ************************* //
	// ******************************************************************** //

	// �洢��ǰ�����Ѿ����ص������Ż���ȷ����������س���һ��
	// ----------------------------------------------------------
	vector<textureAddr> texturesLoaded;	
	vector<Mesh>        meshes;
	string              directory;
	GLboolean           gammaCorrection;

	// ���캯��������һ�� 3D model ���ļ�·��
	// ------------------------------------------------------------------------
	Model(string const & path, GLboolean gamma = false) : gammaCorrection(gamma) { loadModel(path); };

	// ����ģ�͵���������
	// ---------------------
	void draw(Shader &shader);

private:
	// ���ļ��м���һ�� ASSIMP ��չ֧�ֵ�ģ�ͣ������ɵ�����洢������������
	// --------------------------------------------------------------------
	GLvoid loadModel(string const & path);

	// ѭ������ mesh
	// --------------------------------------------------
	GLvoid processNode(aiNode * node, const aiScene * scene);

	Mesh processMesh(aiMesh * mesh, const aiScene * scene);

	// ���������͵����в������������δ���أ����������
	// --------------------------------------------------------------------------------------------
	vector<textureAddr> loadMaterialTextures(aiMaterial * mat, aiTextureType type, string typeName);
};

#endif // !_MODEL_H_

