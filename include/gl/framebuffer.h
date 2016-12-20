/*
* framebuffer management
*/
namespace gl
{
	namespace framebuffer
	{
		/*
		* dictionary-of-framebuffers type
		*/
		typedef resource_set_t	framebuffer_set_t;
		const unsigned int		invalid_framebuffer_id = -1;

		/*
		* create a framebuffer object
		*/
		unsigned int create()
		{
			unsigned int	fboid;
			glGenFramebuffers(1, &fboid);
			return fboid;
		}
		/*
		* verify the currently bound framebuffer object
		* if glCheckFramebufferStatus is GL_FRAMEBUFFER_COMPLETE return true,
		* otherwise, write the error to stderr and return false.
		*/
		bool verify()
		{
			switch (glCheckFramebufferStatus(GL_FRAMEBUFFER))
			{
			case GL_FRAMEBUFFER_COMPLETE:							/*OutputDebugString ("FrameBuffer complete\n\0");	*/					return true;
			case GL_FRAMEBUFFER_UNDEFINED:							std::cerr << "ERR: frameBuffer undefined\0" << std::endl;				break;
			case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:				std::cerr << "ERR: frameBuffer incomplete\0" << std::endl;				break;
			case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:		std::cerr << "ERR: frameBuffer incomplete (missing)\0" << std::endl;	break;
			case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:				std::cerr << "ERR: frameBuffer incomplete (draw)\0" << std::endl;		break;
			case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:				std::cerr << "ERR: frameBuffer incomplete (read)\0" << std::endl;		break;
			case GL_FRAMEBUFFER_UNSUPPORTED:						std::cerr << "ERR: frameBuffer unsupported\0" << std::endl;				break;
			case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:				std::cerr << "ERR: frameBuffer incomplete (ms)\0" << std::endl;			break;
			case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS:			std::cerr << "ERR: frameBuffer incomplete (layers)\0" << std::endl;		break;
			}

			return false;
		}
		/*
		* call glDeleteFramebuffers with the framebuffer object id
		*/
		void clean(unsigned int fboid)
		{
			glDeleteFramebuffers(1, &fboid);
		}
	}
}
