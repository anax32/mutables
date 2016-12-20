#include <assert.h>

#ifndef GL_TESTS
#define GL_TESTS

#include <gl\context_tests.h>
#include <gl\texture_tests.h>

namespace gl
{
	namespace test
	{
		void run_all()
		{
			context::test::run_all();
			texture::test::run_all();
		}
	}
}
#endif
