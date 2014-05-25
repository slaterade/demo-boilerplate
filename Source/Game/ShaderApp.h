#ifndef SHADER_APP_H_INCLUDED
#define SHADER_APP_H_INCLUDED

#include <GL/glew.h>
#include <string>
#include "vmath.h"

namespace Meta {

class ShaderApp {
public:
	ShaderApp();
	~ShaderApp();
	void Initialize();
	void SetWindowSize( int widthInPixels, int heightInPixels );
	void Render( double currentTime );
	void Shutdown();

private:
	GLuint CompileShaders();
	static GLuint CompileShaderFromFile( GLenum shaderType, const std::string& fileName );

	GLuint _renderingProgram;
	GLuint _vertexArrayObject;
	GLuint _buffer;
	bool _hasBeenInitialized;
	bool _hasBeenShutdown;
	int _windowWidth;
	int _windowHeight;

	GLint _mvLocation;
	GLint _projectionLocation;

	vmath::mat4 _projectionMatrix;
};

} // end of namespace Meta

#endif // SHADER_APP_H_INCLUDED