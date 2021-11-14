#include "Shader.h"

Shader::Shader(const GLchar * vertexPath, const GLchar * fragmentPath)
{
	// ************************ ���ļ�·���л�ȡ����/Ƭ����ɫ�� ************************** //
	// *********************************************************************************** //
	string vertexCode;
	string fragmentCode;
	ifstream vShaderFile;
	ifstream fShaderFile;

	// ��֤ifstream��������׳��쳣
	// ----------------------------
	vShaderFile.exceptions(ifstream::failbit | ifstream::badbit);
	fShaderFile.exceptions(ifstream::failbit | ifstream::badbit);

	try
	{
		// ���ļ�
		// --------------------------
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);
		stringstream vShaderStream, fShaderStream;

		// ��ȡ�ļ��Ļ������ݵ�����
		// ----------------------------------
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();

		// �ر��ļ�
		// -----------------
		vShaderFile.close();
		fShaderFile.close();

		// ת������ GLchar ����
		// --------------------------------
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
	}
	catch (ifstream::failure & e)
	{
		cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << endl;
	}
	const GLchar * vShaderCode = vertexCode.c_str();
	const GLchar * fShaderCode = fragmentCode.c_str();

	// ************************* ������ɫ�� ************************* //
	// ************************************************************** //

	GLuint vertex, fragment;
	GLint success;
	GLchar infoLog[512];

	// ������ɫ��
	// --------------------------------------
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);

	// ��ӡ�����������еĻ���
	// 
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertex, 512, NULL, infoLog);
		cout << "ERROR::SHADER::VERTEX::COMPLATION_FAILED" << endl;
		cout << infoLog << endl;
	}

	// Ƭ����ɫ��
	// ------------------------------------------- 
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);

	// ��ӡ�����������еĻ���
	// --------------------------------------------------
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragment, 512, NULL, infoLog);
		cout << "ERROR::SHADER::FRAGMENT::COMPLATION_FAILED" << endl;
		cout << fragmentPath << endl;
		cout << infoLog << endl;
	}

	// ************************* ��ɫ������ ************************* //
	// ************************************************************** //

	this->ID = glCreateProgram();
	glAttachShader(this->ID, vertex);
	glAttachShader(this->ID, fragment);
	glLinkProgram(this->ID);

	// ��ӡ���Ӵ�������еĻ���
	// ------------------------------------------ 
	glGetProgramiv(this->ID, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(this->ID, 512, NULL, infoLog);
		cout << "ERROR:SHADER::PROGRAM::LINKING_FAILED" << endl;
		cout << infoLog << endl;
	}

	// ɾ����ɫ���������Ѿ����ӵ����ǵĳ������ˣ��Ѿ�������Ҫ��
	// --------------------------------------------------------
	glDeleteShader(vertex);
	glDeleteShader(fragment);
}
