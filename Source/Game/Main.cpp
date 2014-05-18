#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <thread>
#include <chrono>
#include <iostream>

#include "Clock.h"
#include "ShaderApp.h"
#include "slog.h"

using namespace Meta;
using namespace std;

static void ErrorCallback( int error, const char* description ) {
	cerr << description;
}

static void KeyCallback( GLFWwindow* window, int key, int scancode, int action, int mods ) {
	if ( key == GLFW_KEY_ESCAPE && action == GLFW_PRESS ) {
		glfwSetWindowShouldClose( window, GL_TRUE );
	}
}

int main( int argc, char* argv[] ) {
	slog::init();
	slog::set_timestamp_format( slog::format_time_only );

	glfwSetErrorCallback( ErrorCallback );
	if ( !glfwInit() ) {
		return 1;
	}

	auto window = glfwCreateWindow( 640, 480, "GameBro", nullptr, nullptr );
	if ( window == nullptr ) {
		glfwTerminate();
		return 1;
	}
	glfwMakeContextCurrent( window );
	glfwSetKeyCallback( window, KeyCallback );

	Clock clock;
	ShaderApp app;
	while ( !glfwWindowShouldClose( window ) ) {

		app.Render( clock.GetTimeNow() );

		glfwSwapBuffers( window );
		glfwPollEvents();
		
		this_thread::sleep_for( chrono::milliseconds( 1 ) );
	}

	glfwDestroyWindow( window );
	glfwTerminate();
	return 0;
}