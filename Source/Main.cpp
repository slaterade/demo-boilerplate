// the source code below is kind of a hybrid between two tutorials
// primarily this boilerplate will test glew and glfw and access to the shader pipeline
// http://www.glfw.org
// http://www.arcsynthesis.org/gltut

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <thread>
#include <vector>
#include <algorithm>
#include "lua.hpp"

GLuint CreateShader( GLenum eShaderType, const std::string& strShaderFile ) {
	GLuint shader = glCreateShader( eShaderType );
	const char* strFileData = strShaderFile.c_str();
	glShaderSource( shader, 1, &strFileData, NULL );

	glCompileShader( shader );

	GLint status;
	glGetShaderiv( shader, GL_COMPILE_STATUS, &status );
	if ( status == GL_FALSE ) {
		GLint infoLogLength;
		glGetShaderiv( shader, GL_INFO_LOG_LENGTH, &infoLogLength );

		GLchar* strInfoLog = new GLchar[infoLogLength + 1];
		glGetShaderInfoLog( shader, infoLogLength, NULL, strInfoLog );

		const char* strShaderType = NULL;
		switch ( eShaderType ) {
			case GL_VERTEX_SHADER:
				strShaderType = "vertex";
				break;
			case GL_GEOMETRY_SHADER:
				strShaderType = "geometry";
				break;
			case GL_FRAGMENT_SHADER:
				strShaderType = "fragment";
				break;
			default:
				break;
		}

		fprintf( stderr, "Compile failure in %s shader:\n%s\n", strShaderType, strInfoLog );
		delete[] strInfoLog;
	}
	return shader;
}

GLuint CreateProgram( const std::vector<GLuint>& shaderList ) {
	GLuint program = glCreateProgram();

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
		fprintf( stderr, "Linker failure: %s\n", strInfoLog );
		delete[] strInfoLog;
	}

	for ( auto& shader : shaderList ) {
		glDetachShader( program, shader );
	}

	return program;
}

GLuint theProgram;

const std::string strVertexShader(
	"#version 150\n"
	"#extension GL_ARB_explicit_attrib_location : enable\n" // required for osx
	"layout (location = 0) in vec4 position;\n"
	"void main()\n"
	"{\n"
	"	gl_Position = position;\n"
	"}\n"
);

const std::string strFragmentShader(
	"#version 150\n"
	"out vec4 outputColor;\n"
	"void main()\n"
	"{\n"
	"	outputColor = vec4( 1.0f, 1.0f, 1.0f, 1.0f );\n"
	"}\n"
);

void InitializeProgram() {
	std::vector<GLuint> shaderList;
	shaderList.push_back( CreateShader( GL_VERTEX_SHADER, strVertexShader ) );
	shaderList.push_back( CreateShader( GL_FRAGMENT_SHADER, strFragmentShader ) );

	theProgram = CreateProgram( shaderList );

	std::for_each( shaderList.begin(), shaderList.end(), glDeleteShader );
}

const float vertexPositions[] = {
	0.75f, 0.75f, 0.0f, 1.0f,
	0.75f, -0.75f, 0.0f, 1.0f,
	-0.75f, -0.75f, 0.0f, 1.0f,
};

GLuint positionBufferObject;
GLuint vao;

void InitializeVertexBuffer() {
	glGenBuffers( 1, &positionBufferObject );
	glBindBuffer( GL_ARRAY_BUFFER, positionBufferObject );
	glBufferData( GL_ARRAY_BUFFER, sizeof( vertexPositions), vertexPositions, GL_STATIC_DRAW );
	glBindBuffer( GL_ARRAY_BUFFER, 0 );
}

void init() {
	InitializeProgram();
	InitializeVertexBuffer();

	glGenVertexArrays( 1, &vao );
	glBindVertexArray( vao );
}

void display() {
	glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
	glClear( GL_COLOR_BUFFER_BIT );

	glUseProgram( theProgram );

	glBindBuffer( GL_ARRAY_BUFFER, positionBufferObject );
	glEnableVertexAttribArray( 0 );
	glVertexAttribPointer( 0, 4, GL_FLOAT, GL_FALSE, 0, 0 );

	glDrawArrays( GL_TRIANGLES, 0, 3 );

	glDisableVertexAttribArray( 0 );
	glUseProgram( 0 );
}

static void error_callback( int error, const char* description ) {
	std::cerr << description << ", code: " << error;
}

static void key_callback( GLFWwindow* window, int key, int scancode, int action, int mods ) {
	if ( key == GLFW_KEY_ESCAPE && action == GLFW_PRESS ) {
		glfwSetWindowShouldClose( window, GL_TRUE );
	}
}

int main( int argc, char* argv[] ) {

	// just testing Lua compile
	lua_State* luaState = luaL_newstate();

	GLFWwindow* window = nullptr;
	glfwSetErrorCallback( error_callback );
	if ( !glfwInit() ) {
		exit( EXIT_FAILURE );
	}

	// hints required for osx
	glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE );
	glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );

	window = glfwCreateWindow( 640, 480, "game-boilerplate", NULL, NULL );
	if ( window == nullptr ) {
	    glfwTerminate();
		exit( EXIT_FAILURE );
	}
	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, key_callback);

	glewExperimental = GL_TRUE; // without this we get a segfault in osx
	glewInit();
	init();

	while ( !glfwWindowShouldClose( window ) ) {
		float ratio;
		int width, height;
		glfwGetFramebufferSize( window, &width, &height );
		ratio = width / static_cast<float>( height );
		glViewport( 0, 0, width, height );

		display();

		glfwSwapBuffers( window );
		glfwPollEvents();

		// C++11 standard library test
		std::this_thread::sleep_for( std::chrono::milliseconds( 1 ) );
	}
	glfwDestroyWindow( window );
	glfwTerminate();
	exit( EXIT_SUCCESS );
}