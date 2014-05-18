#ifndef SHADER_APP_H_INCLUDED
#define SHADER_APP_H_INCLUDED

#include <GL/glew.h>
#include <string>

namespace Meta {

class ShaderApp {
public:
	ShaderApp();
	~ShaderApp();
	void Initialize();
	void Render( double currentTime );
	void Shutdown();

private:
	static GLuint CompileShaders();
	static GLuint CompileShaderFromFile( GLenum shaderType, const std::string& fileName );

	GLuint _renderingProgram;
	GLuint _vertexArrayObject;
	bool _hasBeenInitialized;
	bool _hasBeenShutdown;
};

} // end of namespace Meta

#endif // SHADER_APP_H_INCLUDED