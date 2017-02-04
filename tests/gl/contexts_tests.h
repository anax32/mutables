#include <cassert>

#include "gl/defs.h"
#include "gl/context.h"

namespace gl
{
	namespace context
	{
		namespace test
		{
			void create()
			{
#ifdef _WIN32
				HWND window_handle = window::create();
				assert(gl::context::create(window_handle) == true);
				gl::context::clean(window_handle);
#elif __linux__
				assert(gl::context::create() == true);
				gl::context::clean();
#endif
			}
			void run_all()
			{
				create();
			}
		}
	}
}
