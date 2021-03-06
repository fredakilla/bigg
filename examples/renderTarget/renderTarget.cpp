#include <bigg.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>

struct PCT_Vertex
{
	float x;
	float y;
	float z;
	uint32_t abgr;
    float u;
    float v;
	static void init()
	{
		ms_decl
			.begin()
			.add( bgfx::Attrib::Position, 3, bgfx::AttribType::Float )
			.add( bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true )
            .add( bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float, true)
			.end();
	}
	static bgfx::VertexDecl ms_decl;
};
bgfx::VertexDecl PCT_Vertex::ms_decl;

static PCT_Vertex s_quadVertices[] =
{
    // triangle strip
    {-1.0f, -1.0f,  0.0f, 0xffffffff, 0,1 },    // bottom left
    {-1.0f,  1.0f,  0.0f, 0xffffffff, 0,0 },    // top left
    { 1.0f, -1.0f,  0.0f, 0xffffffff, 1,1 },    // bottom right
    { 1.0f,  1.0f,  0.0f, 0xffffffff, 1,0 },    // top right
};

class ExampleTexturedQuad : public bigg::Application
{
	void initialize( int _argc, char** _argv )
    {
        // Create renderTarget and framebuffer
        uint16_t width = 256;
        uint16_t height = 256;
        bgfx::TextureHandle fbTextures[2];
        fbTextures[0] = bgfx::createTexture2D(width, height, false, 1, bgfx::TextureFormat::BGRA8, BGFX_TEXTURE_RT);
        fbTextures[1] = bgfx::createTexture2D(width, height, false, 1, bgfx::TextureFormat::D24,  BGFX_TEXTURE_RT );
        m_gbuffer = bgfx::createFrameBuffer(BX_COUNTOF(fbTextures), fbTextures, true);


        _texHandle = bigg::loadTexture("smiley.jpg");
        _texUniform = bgfx::createUniform("s_texColor", bgfx::UniformType::Int1);
        _program = bigg::loadProgram( "shaders/glsl/vs_textured.bin", "shaders/glsl/fs_textured.bin" );
        assert(bgfx::isValid(_program));

        _texUniform2 = bgfx::createUniform("u_texture", bgfx::UniformType::Int1);
        _programFx = bigg::loadProgram( "shaders/glsl/vs_textured.bin", "shaders/glsl/fx_pixelate.bin" );
        assert(bgfx::isValid(_programFx));

        PCT_Vertex::init();
        _vbh = bgfx::createVertexBuffer(bgfx::makeRef(s_quadVertices, sizeof(s_quadVertices)), PCT_Vertex::ms_decl );


        _uniformPixelate = bgfx::createUniform("u_pixelate", bgfx::UniformType::Vec4);

        _time = 0.0f;
	}

	void onReset()
	{
		bgfx::setViewClear( 0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x303030ff, 1.0f, 0 );
		bgfx::setViewRect( 0, 0, 0, uint16_t( getWidth() ), uint16_t( getHeight() ) );

        bgfx::setViewClear(1, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x101030ff, 1.0f, 0 );
        bgfx::setViewRect(1, 0, 0, uint16_t( getWidth() ), uint16_t( getHeight() ) );
	}

	void update( float dt )
	{
        _time += dt;


        static float guiPixelateValue = 64.0f;

        ImGui::Begin("Controls");
        ImGui::SliderFloat("Pixelate", &guiPixelateValue, 12.0f, 128.0f);
        ImGui::End();



        bgfx::ViewId viewRT = 1;
        bgfx::ViewId viewFinal = 0;

        // render in target
        {
            uint16_t viewWidth = 256;
            uint16_t viewHeight = 256;

            glm::mat4 view = glm::lookAt(glm::vec3( 0.0f, 0.0f, -5.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f, 1.0f, 0.0f));
            glm::mat4 proj = bigg::perspective(glm::radians(45.0f), (float)viewWidth / (float)viewHeight, 0.1f, 100.0f);

            bgfx::setViewTransform(viewRT, &view[0][0], &proj[0][0]);
            bgfx::setViewRect(viewRT, 0, 0, viewWidth, viewHeight);
            bgfx::setViewFrameBuffer(viewRT, m_gbuffer);

            bgfx::touch(viewRT);

            glm::mat4 mtx;
            mtx = glm::rotate(mtx, _time, glm::vec3(0.0f,0.0f,1.0f));
            bgfx::setTransform(&mtx);

            bgfx::setVertexBuffer(0, _vbh);
            bgfx::setTexture(0, _texUniform, _texHandle);

            uint64_t state = 0
                    | BGFX_STATE_WRITE_RGB
                    | BGFX_STATE_WRITE_A
                    | BGFX_STATE_WRITE_Z
                    | BGFX_STATE_DEPTH_TEST_LESS
                    //| BGFX_STATE_CULL_CW
                    | BGFX_STATE_MSAA
                    | BGFX_STATE_PT_TRISTRIP;
            bgfx::setState(state);
            bgfx::submit(viewRT, _program);
        }

        // render in main view
        {
            uint16_t viewWidth = getWidth();
            uint16_t viewHeight = getHeight();

            glm::mat4 view = glm::lookAt(glm::vec3( 0.0f, 0.0f, -3.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f, 1.0f, 0.0f));
            glm::mat4 proj = bigg::perspective(glm::radians(45.0f), (float)viewWidth / (float)viewHeight, 0.1f, 100.0f);

            bgfx::setViewTransform(viewFinal, &view[0][0], &proj[0][0]);
            bgfx::setViewRect(viewFinal, 0, 0, uint16_t(getWidth()), uint16_t(getHeight()));
            bgfx::touch(viewFinal);

            glm::mat4 mtx;
            mtx = glm::yawPitchRoll(0.21f, 0.37f, 0.0f);
            bgfx::setTransform(&mtx);

            bgfx::setVertexBuffer(0, _vbh);
            bgfx::setTexture(0, _texUniform2, bgfx::getTexture(m_gbuffer) );

            glm::vec4 pixelate = glm::vec4(guiPixelateValue, 0.0f, 0.0f, 0.0f);
            bgfx::setUniform(_uniformPixelate, &pixelate.data);

            uint64_t state = 0
                    | BGFX_STATE_WRITE_RGB
                    | BGFX_STATE_WRITE_A
                    | BGFX_STATE_WRITE_Z
                    | BGFX_STATE_DEPTH_TEST_LESS
                    //| BGFX_STATE_CULL_CW
                    | BGFX_STATE_MSAA
                    | BGFX_STATE_PT_TRISTRIP;
            bgfx::setState(state);
            bgfx::submit(viewFinal, _programFx);
        }
	}

private:

    float _time;
    bgfx::VertexBufferHandle _vbh;

    bgfx::ProgramHandle _program;
    bgfx::TextureHandle _texHandle;
    bgfx::UniformHandle _texUniform;

    bgfx::FrameBufferHandle m_gbuffer;
    bgfx::ProgramHandle _programFx;
    bgfx::TextureHandle _texHandle2;
    bgfx::UniformHandle _texUniform2;

    bgfx::UniformHandle _uniformPixelate;
};

int main( int argc, char** argv )
{
    ExampleTexturedQuad app;
	return app.run( argc, argv );
}
