#include "gl/defs.h"
#include "gl/context.h"
#include "gl/texture.h"
#include "gl/framebuffer.h"
#include <fstream>

#include "../tests.h"

void create_test ()
{
	auto A = gl::framebuffer::create();

	assert_are_not_equal (A, gl::framebuffer::invalid_framebuffer_id);

	gl::framebuffer::clean(A);
}
void bind_test ()
{
	auto A = gl::framebuffer::create ();
	assert_are_not_equal (A, gl::framebuffer::invalid_framebuffer_id);

	gl::framebuffer::bind(A);
	gl::framebuffer::unbind();
	gl::framebuffer::clean(A);
}
void attach_single_test ()
{
	auto A = gl::framebuffer::create ();
	assert_are_not_equal (A, gl::framebuffer::invalid_framebuffer_id);
	auto T = gl::texture::create (gl::size2d (64,64));
	assert_are_not_equal (T, gl::texture::invalid_texture_id);

	gl::framebuffer::bind(A);
	gl::framebuffer::attachTexture(T);
	
	assert_is_true (gl::framebuffer::verify());

	gl::framebuffer::unbind();
	gl::texture::clean(T);
	gl::framebuffer::clean(A);
}
void attach_bunch_test ()
{
	auto A = gl::framebuffer::create ();
	assert_are_not_equal (A, gl::framebuffer::invalid_framebuffer_id);
	auto T = new unsigned int[4];
	T[0] = gl::texture::create (gl::size2d (64, 64));
	T[1] = gl::texture::create (gl::size2d (64, 64));
	T[2] = gl::texture::create (gl::size2d (64, 64));
	T[3] = gl::texture::create (gl::size2d (64, 64));

	gl::framebuffer::bind (A);
	gl::framebuffer::attachTexture (T, 4);

	assert_is_true (gl::framebuffer::verify ());

	gl::framebuffer::unbind ();
	gl::texture::clean (T[0]);
	gl::texture::clean (T[1]);
	gl::texture::clean (T[2]);
	gl::texture::clean (T[3]);
	gl::framebuffer::clean (A);
	delete[] T;
}

void gl_framebuffer_tests ()
{
	gl::context::create ();
	TEST(create_test);
	TEST(bind_test);
	TEST(attach_single_test);
	TEST(attach_bunch_test);
	gl::context::clean ();
}

int main (int argc, char** argv)
{	
	TEST_GROUP(gl_framebuffer_tests);	
}
