#include "ShaderApp.h"
#include "slog.h"
#include <cmath>
#include <fstream>
#include <sstream>
#include <vector>
#include <iostream>

using namespace Meta;
using namespace std;

ShaderApp::ShaderApp()
	: _renderingProgram( 0 )
	, _vertexArrayObject( 0 )
	, _hasBeenInitialized( false )
	, _hasBeenShutdown( false ) {
}

ShaderApp::~ShaderApp() {
	if ( !_hasBeenShutdown ) {
		Shutdown();
	}
}

void ShaderApp::Initialize() {
	if ( !_hasBeenInitialized ) {
		glewInit();
		_renderingProgram = CompileShaders();
		glGenVertexArrays( 1, &_vertexArrayObject );
		glBindVertexArray( _vertexArrayObject );
		_hasBeenInitialized = true;
	}
}

void ShaderApp::Shutdown() {
	if ( !_hasBeenShutdown ) {
		glDeleteVertexArrays( 1, &_vertexArrayObject );
		glDeleteProgram( _renderingProgram );
		_hasBeenShutdown = true;
	}
}

void ShaderApp::Render( double currentTime ) {
	if ( !_hasBeenInitialized ) {
		Initialize();
	}
	const float color[] = {
		float( sin( currentTime ) ) * 0.5f + 0.5f,
		float( cos( currentTime ) ) * 0.5f + 0.5f,
		0.0f,
		1.0f
	};
	glClearBufferfv( GL_COLOR, 0, color );
	glUseProgram( _renderingProgram );

	GLfloat attrib[] = {
		float( sin( currentTime ) ) * 0.5f,
		float( cos( currentTime ) ) * 0.6f,
		0.0f,
		0.0f
	};
	glVertexAttrib4fv( 0, attrib );

	GLfloat colorAttrib[] {
		float( cos(currentTime) ) * 0.5f + 0.5f,
		float( sin(currentTime) ) * 0.5f + 0.5f,
		0.0f,
		1.0f
	};
	glVertexAttrib4fv(1, colorAttrib);

	glDrawArrays( GL_TRIANGLES, 0, 3 );
}

GLuint ShaderApp::CompileShaders() {
	std::vector<GLuint> shaderList;
	shaderList.push_back( CompileShaderFromFile( GL_VERTEX_SHADER, "Shaders\\test.vert" ) );
	shaderList.push_back( CompileShaderFromFile( GL_FRAGMENT_SHADER, "Shaders\\test.frag" ) );

	auto program = glCreateProgram();
	for ( auto& shader : shaderList ) {
		glAttachShader( program, shader );
	}
	glLinkProgram( program );

	GLint status;
	glGetProgramiv( program, GL_LINK_STATUS, &status );
	if ( status == GL_FALSE ) {
		GLint infoLogLength;
		glGetProgramiv( program, GL_INFO_LOG_LENGTH, &infoLogLength );
		GLchar* strInfoLog = new GLchar[infoLogLength + 1];
		glGetProgramInfoLog( program, infoLogLength, NULL, strInfoLog );
		slog::out() << "Linker failure: " << strInfoLog;
		delete[] strInfoLog;
	}

	for ( auto& shader : shaderList ) {
		glDeleteShader( shader );
	}

	return program;
}

GLuint ShaderApp::CompileShaderFromFile( GLenum shaderType, const string& fileName ) {
	ifstream file( fileName );
	if ( !file.is_open() ) {
		return 0;
	}
	file.seekg( 0, ios::end );
	auto bufferSize = static_cast<size_t>( file.tellg() );
	string buffer( bufferSize, ' ' );
	file.seekg( 0 );
	file.read( &buffer[0], bufferSize );

	const GLchar* shaderSources[] = { buffer.c_str() };
	auto shader = glCreateShader( shaderType );
	glShaderSource( shader, 1, shaderSources, nullptr );
	glCompileShader( shader );
	return shader;
}