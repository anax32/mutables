/*
* texture management
*/
namespace gl
{
	namespace texture
	{
		/*
		* dictionary-of-textures type
		*/
		typedef resource_set_t	texture_set_t;
		const unsigned int		invalid_texture_id = -1;

		/*
		* create a 2d texture
		* if internal format is a GL_DEPTH* type a GL_DEPTH_COMPONENT image is create
		* otherwise, a GL_RGBA image is created
		*/
		unsigned int create(const size2d size, const GLenum internal_format = GL_RGBA32F)
		{
			unsigned int texid = 0;

			glGenTextures(1, &texid);
			glBindTexture(GL_TEXTURE_2D, texid);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

			switch (internal_format)
			{
			case GL_DEPTH_COMPONENT:
			case GL_DEPTH_COMPONENT16:
			case GL_DEPTH_COMPONENT24:
			case GL_DEPTH_COMPONENT32:
				glTexImage2D(GL_TEXTURE_2D, 0, internal_format, std::get<0>(size), std::get<1>(size), 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
				break;
			default:
				glTexImage2D(GL_TEXTURE_2D, 0, internal_format, std::get<0>(size), std::get<1>(size), 0, GL_RGBA, GL_FLOAT, NULL);
				break;
			}

			return texid;
		}
		/*
		* create a 3d texture
		*/
		unsigned int create(const size3d size, const GLenum internal_format = GL_RGBA32F)
		{
			unsigned int texid = 0;

			glGenTextures(1, &texid);
			glBindTexture(GL_TEXTURE_3D, texid);
			glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP);
			glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP);
			glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP);
			glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			//	glTexImage2D (GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
			glTexImage3D(GL_TEXTURE_3D, 0, internal_format, std::get<0>(size), std::get<1>(size), std::get<2>(size), 0, GL_RGBA, GL_FLOAT, NULL);

			return texid;
		}

		typedef std::function <bool(const int, const int, const int, const unsigned char*)>		write_fn_t;

		/*
		* write a texture object using the png_io namespace
		* NB: this is only valid for GL_TEXTURE_2D targets at the moment
		*/
		void write(const unsigned int texid, write_fn_t write_fn)
		{
			int		w, h;

			glGetError();

			glBindTexture(GL_TEXTURE_2D, texid);
			glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &w);
			glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &h);

			if (glGetError() != GL_NONE)
			{
				return;
			}
			else if ((w == 0) || (h == 0))
			{
				return;
			}

			float* img_f = new float[w*h * 4];
			unsigned char* img_ub = new unsigned char[w*h * 4];

			glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_FLOAT, img_f);

			// convert the float to bytes
			std::transform(img_f + (0), img_f + (w*h * 4),
				img_ub, [](const float f){ return static_cast<unsigned int>(f*255.0f); });

			if (write_fn(w, h, 4, img_ub) == false)
			{
				std::cerr << "ERR: Could not output texture '" << texid << "' (" << w << ", " << h << ")" << std::endl;
			}

			// cleanup
			delete[] img_ub;
			delete[] img_f;
		}
		/*
		* call glDeleteTextures for a texture object id
		*/
		void clean(unsigned int texid)
		{
			glDeleteTextures(1, &texid);
		}
	}
}
