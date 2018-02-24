/*
 * This is free and unencumbered software released into the public domain. 
 */

#pragma once

#include <bgfx/bgfx.h>
#include <imgui.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <bx/allocator.h>

namespace bigg
{
	// bgfx utils
    void* load(const char* filename, size_t* size);
	const bgfx::Memory* loadMemory( const char* filename );
	bgfx::ShaderHandle loadShader( const char* shader );
	bgfx::ProgramHandle loadProgram( const char* vsName, const char* fsName );
    bgfx::TextureHandle loadTexture(const char* filePath, uint32_t flags = 0);

	// glm utils
	glm::tmat4x4<float, glm::defaultp> perspective( float fovy, float aspect, float zNear, float zFar );
	glm::tmat4x4<float, glm::defaultp> ortho( float left, float right, float bottom, float top, float zNear, float zFar, float offset );

	// allocator
	class Allocator : public bx::AllocatorI
	{
	public:
		void* realloc( void* _ptr, size_t _size, size_t _align, const char* _file, uint32_t _line )
		{
			if ( _size == 0 )
			{
				free( _ptr );
				return nullptr;
			}
			else
			{
				return malloc( _size );
			}
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

		void imguiEvents( float dt );
        void toggleStats();
        void toggleVsync();

    public:
		Application();

		int run(
			int argc,
			char** argv,
			bgfx::RendererType::Enum type = bgfx::RendererType::Count,
			uint16_t vendorId = BGFX_PCI_ID_NONE,
			uint16_t deviceId = 0,
			bgfx::CallbackI* callback = NULL,
			bx::AllocatorI* allocator = NULL
		);

		void reset( uint32_t flags = 0 );
		uint32_t getWidth();
		uint32_t getHeight();

        virtual void initialize( int _argc, char** _argv ) {}
        virtual void update( float dt ) {}
        virtual int shutdown() { return 0; }

        virtual void onReset() {}
		virtual void onKey( int key, int scancode, int action, int mods ) {}
		virtual void onChar( unsigned int codepoint ) {}
		virtual void onCharMods( int codepoint, unsigned int mods ) {}
		virtual void onMouseButton( int button, int action, int mods ) {}
		virtual void onCursorPos( double xpos, double ypos ) {}
		virtual void onCursorEnter( int entered ) {}
		virtual void onScroll( double xoffset, double yoffset ) {}
		virtual void onDrop( int count, const char** paths ) {}
	protected:
		GLFWwindow* mWindow;
		bigg::Allocator mAllocator;
	private:
		uint32_t mReset;
		uint32_t mWidth;
		uint32_t mHeight;
		bool  mMousePressed[ 3 ];
		float mMouseWheel;
        uint32_t mDebugFlag;
    };
}
