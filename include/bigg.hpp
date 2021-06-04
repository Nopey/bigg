/*
 * This is free and unencumbered software released into the public domain. 
 */

#pragma once

#include <bgfx/bgfx.h>
#include <imgui.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <cstdlib>
#include <exception>

#include <bx/allocator.h>

namespace bigg
{
	// bgfx utils
	const bgfx::Memory* loadMemory( const char* filename );
	bgfx::ShaderHandle loadShader( const char* shader );
	bgfx::ProgramHandle loadProgram( const char* vsName, const char* fsName );

	// allocator
	class Allocator : public bx::AllocatorI
	{
	public:
		void* realloc( void* ptr, size_t size, size_t _align, const char* _file, uint32_t _line )
		{
			return std::realloc( ptr, size );
		}
	};

	class glfw_init_failed : public std::exception
	{
		virtual const char* what() const throw()
		{
			return "Failed to initialize GLFW";
		}
	};

	class glfw_window_creation_failed : public std::exception
	{
		virtual const char* what() const throw()
		{
			return "Failed to create a GLFW window";
		}
	};

	// application
	class Application
	{
		static void keyCallback( GLFWwindow* window, int key, int scancode, int action, int mods );
		static void charCallback( GLFWwindow* window, unsigned int codepoint );
		static void charModsCallback( GLFWwindow* window, unsigned int codepoint, int mods );
		static void mouseButtonCallback( GLFWwindow* window, int button, int action, int mods );
		static void cursorPosCallback( GLFWwindow* window, double xpos, double ypos );
		static void cursorEnterCallback( GLFWwindow* window, int entered );
		static void scrollCallback( GLFWwindow* window, double xoffset, double yoffset );
		static void dropCallback( GLFWwindow* window, int count, const char** paths );
		static void windowSizeCallback( GLFWwindow* window, int width, int height );
	public:
		Application(
			const char* title = "",
			uint32_t width = 1280,
			uint32_t height = 768,
			bgfx::RendererType::Enum type = bgfx::RendererType::Count,
			uint16_t vendorId = BGFX_PCI_ID_NONE,
			uint16_t deviceId = 0,
			bgfx::CallbackI* callback = NULL,
			bx::AllocatorI* allocator = NULL
		);

		~Application();

		int run( int argc, char** argv );

		void reset( uint32_t flags = 0 );
		uint32_t getWidth() const;
		uint32_t getHeight() const;
		void setSize( int width, int height );
		const char* getTitle() const;
		void setTitle( const char* title );
		bool isKeyDown( int key ) const;
		bool isMouseButtonDown( int button ) const;
		float getMouseWheelH() const;
		float getMouseWheel() const;

		// initialize, update, and shutdown are called by run.
		// run is re-entrant, and so should be your implementation of these.
		virtual void initialize( int _argc, char** _argv ) {};
		virtual void update( float dt ) {};
		// shutdown's return value becomes run's return value, and should usually end up being main's return value.
		virtual int shutdown() { return 0; };

		virtual void onReset() {};
		virtual void onKey( int key, int scancode, int action, int mods ) {}
		virtual void onChar( unsigned int codepoint ) {}
		virtual void onCharMods( int codepoint, unsigned int mods ) {}
		virtual void onMouseButton( int button, int action, int mods ) {}
		virtual void onCursorPos( double xpos, double ypos ) {}
		virtual void onCursorEnter( int entered ) {}
		virtual void onScroll( double xoffset, double yoffset ) {}
		virtual void onDrop( int count, const char** paths ) {}
		virtual void onWindowSize( int width, int height ) {}
	protected:
		GLFWwindow* mWindow;
		bigg::Allocator mAllocator;
		float mFpsLock;
	private:
		uint32_t mReset;
		uint32_t mWidth;
		uint32_t mHeight;
		const char* mTitle;
		float mMouseWheelH = 0.0f;
		float mMouseWheel = 0.0f;
	};
}
