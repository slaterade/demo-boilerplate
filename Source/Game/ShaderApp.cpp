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
	, _hasBeenShutdown( false )
	, _windowWidth( 640 )
	, _windowHeight( 480 ) {
}

ShaderApp::~ShaderApp() {
	if ( !_hasBeenShutdown ) {
		Shutdown();
	}
}

void ShaderApp::SetWindowSize( int widthInPixels, int heightInPixels ) {
	_windowWidth = widthInPixels;
	_windowHeight = heightInPixels;
	auto aspectRatio = (float)widthInPixels / (float)heightInPixels;
	_projectionMatrix = vmath::perspective( 50.0f, aspectRatio, 0.1f, 1000.0f );
}

void ShaderApp::Initialize() {
	if ( !_hasBeenInitialized ) {
		_hasBeenInitialized = true;
		glewInit();
		_renderingProgram = CompileShaders();
		glGenVertexArrays( 1, &_vertexArrayObject );
		glBindVertexArray( _vertexArrayObject );

		static const GLfloat vertexPositions[] = {
			-0.25f, 0.25f, -0.25f,
			-0.25f, -0.25f, -0.25f,
			0.25f, -0.25f, -0.25f,

			0.25f, -0.25f, -0.25f,
			0.25f, 0.25f, -0.25f,
			-0.25f, 0.25f, -0.25f,

			0.25f, -0.25f, -0.25f,
			0.25f, -0.25f, 0.25f,
			0.25f, 0.25f, -0.25f,

			0.25f, -0.25f, 0.25f,
			0.25f, 0.25f, 0.25f,
			0.25f, 0.25f, -0.25f,

			0.25f, -0.25f, 0.25f,
			-0.25f, -0.25f, 0.25f,
			0.25f, 0.25f, 0.25f,

			-0.25f, -0.25f, 0.25f,
			-0.25f, 0.25f, 0.25f,
			0.25f, 0.25f, 0.25f,

			-0.25f, -0.25f, 0.25f,
			-0.25f, -0.25f, -0.25f,
			-0.25f, 0.25f, 0.25f,

			-0.25f, -0.25f, -0.25f,
			-0.25f, 0.25f, -0.25f,
			-0.25f, 0.25f, 0.25f,

			-0.25f, -0.25f, 0.25f,
			0.25f, -0.25f, 0.25f,
			0.25f, -0.25f, -0.25f,

			0.25f, -0.25f, -0.25f,
			-0.25f, -0.25f, -0.25f,
			-0.25f, -0.25f, 0.25f,

			-0.25f, 0.25f, -0.25f,
			0.25f, 0.25f, -0.25f,
			0.25f, 0.25f, 0.25f,

			0.25f, 0.25f, 0.25f,
			-0.25f, 0.25f, 0.25f,
			-0.25f, 0.25f, -0.25f
		};

		glGenBuffers( 1, &_buffer );
		glBindBuffer( GL_ARRAY_BUFFER, _buffer );
		glBufferData( GL_ARRAY_BUFFER, sizeof( vertexPositions ), vertexPositions, GL_STATIC_DRAW );

		glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, NULL );
		glEnableVertexAttribArray( 0 );

		glEnable( GL_CULL_FACE );
		glFrontFace( GL_CW );

		glEnable( GL_DEPTH_TEST );
		glDepthFunc( GL_LEQUAL );
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
	glViewport( 0, 0, _windowWidth, _windowHeight );

	const GLfloat green[] = { 0.0f, 0.25f, 0.0f, 1.0f };
	glClearBufferfv( GL_COLOR, 0, green );
	
	static const GLfloat one = 1.0f;
	glClearBufferfv( GL_DEPTH, 0, &one );

	glUseProgram( _renderingProgram );

	glUniformMatrix4fv( _projectionLocation, 1, GL_FALSE, _projectionMatrix );

	for ( int i = 0; i < 24; ++i ) {
		float f = (float)i + (float)currentTime * 0.3f;
		vmath::mat4 mvMatrix = vmath::translate( 0.0f, 0.0f, -6.0f )
			* vmath::rotate( (float)currentTime * 45.0f, 0.0f, 1.0f, 0.0f )
			* vmath::rotate( (float)currentTime * 21.0f, 1.0f, 0.0f, 0.0f )
			* vmath::translate(
				sinf( 2.1f * f ) * 2.0f,
				cosf( 1.7f * f ) * 2.0f,
				sinf( 1.3f * f ) * cosf( 1.5f * f ) * 2.0f
			);
		glUniformMatrix4fv( _mvLocation, 1, GL_FALSE, mvMatrix );
		glDrawArrays( GL_TRIANGLES, 0, 36 );
	}
}

GLuint ShaderApp::CompileShaders() {
	std::vector<GLuint> shaderList;
	shaderList.push_back( CompileShaderFromFile( GL_VERTEX_SHADER, "Shaders/test.vert" ) );
	//shaderList.push_back( CompileShaderFromFile( GL_TESS_CONTROL_SHADER, "Shaders/test.tesc" ) );
	//shaderList.push_back( CompileShaderFromFile( GL_TESS_EVALUATION_SHADER, "Shaders/test.tese" ) );
	//shaderList.push_back( CompileShaderFromFile( GL_GEOMETRY_SHADER, "Shaders/test.geom" ) );
	shaderList.push_back( CompileShaderFromFile( GL_FRAGMENT_SHADER, "Shaders/test.frag" ) );

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

	_mvLocation = glGetUniformLocation( program, "mv_matrix" );
	_projectionLocation = glGetUniformLocation( program, "proj_matrix" );

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

	GLint isCompiled = 0;
	glGetShaderiv( shader, GL_COMPILE_STATUS, &isCompiled );
	if ( isCompiled == GL_FALSE ) {
		GLint logSize = 0;
		glGetShaderiv( shader, GL_INFO_LOG_LENGTH, &logSize );
		vector<char> errorLog( logSize );
		glGetShaderInfoLog( shader, logSize, &logSize, &errorLog[0] );
		slog::out() << "Compile failure: " << &errorLog[0];

		glDeleteShader( shader );
		return 0;
	} else {
		return shader;
	}
}