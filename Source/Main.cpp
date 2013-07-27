#include <GLFW/glfw3.h>
#include <iostream>
#include <thread>

static void error_callback( int error, const char* description ) {
	std::cerr << description << ", code: " << error;
}

static void key_callback( GLFWwindow* window, int key, int scancode, int action, int mods ) {
	if ( key == GLFW_KEY_ESCAPE && action == GLFW_PRESS ) {
		glfwSetWindowShouldClose( window, GL_TRUE );
	}
}

int main( int argc, char* argv[] ) {
	GLFWwindow* window = nullptr;
	glfwSetErrorCallback( error_callback );
	if ( !glfwInit() ) {
		exit( EXIT_FAILURE );
	}
	window = glfwCreateWindow( 640, 480, "Simple example", NULL, NULL );
	if ( window == nullptr ) {
	    glfwTerminate();
		exit( EXIT_FAILURE );
	}
	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, key_callback);

	while ( !glfwWindowShouldClose( window ) ) {
		float ratio;
		int width, height;
		glfwGetFramebufferSize( window, &width, &height );
		ratio = width / static_cast<float>( height );
		glViewport( 0, 0, width, height );
		glClear( GL_COLOR_BUFFER_BIT );
		glMatrixMode( GL_PROJECTION );
		glLoadIdentity();
		glOrtho( -ratio, ratio, -1.f, 1.f, 1.f, -1.f );
		glMatrixMode( GL_MODELVIEW );
		glLoadIdentity();
		glRotatef( (float) glfwGetTime() * 50.f, 0.f, 0.f, 1.f );
		glBegin( GL_TRIANGLES );
		glColor3f( 1.f, 0.f, 0.f );
		glVertex3f( -0.6f, -0.4f, 0.f );
		glColor3f( 0.f, 1.f, 0.f );
		glVertex3f( 0.6f, -0.4f, 0.f );
		glColor3f( 0.f, 0.f, 1.f );
		glVertex3f( 0.f, 0.6f, 0.f );
		glEnd();
		glfwSwapBuffers( window );
		glfwPollEvents();

		std::this_thread::sleep_for( std::chrono::milliseconds( 1 ) );
	}
	glfwDestroyWindow( window );
	glfwTerminate();
	exit( EXIT_SUCCESS );
}