#include "gl/texture.h"
#include <fstream>

#include "../tests.h"

void create_test ()
{
	auto A = texture::create(gl::size2d(64, 64));
	auto B = texture::create(gl::size2d(64, 64), GL_LUMINANCE);
	auto C = texture::create(gl::size2d(64, 64), GL_RG);
	auto D = texture::create(gl::size3d(64, 64, 64));
	auto E = texture::create(gl::size3d(64, 64, 64), GL_LUMINANCE);
	auto F = texture::create(gl::size2d(64, 64), GL_DEPTH_COMPONENT);
	auto G = texture::create(gl::size2d(64, 64), GL_DEPTH_COMPONENT16);

	assert(A != texture::invalid_texture_id);
	assert(B != texture::invalid_texture_id);
	assert(C != texture::invalid_texture_id);
	assert(D != texture::invalid_texture_id);
	assert(E != texture::invalid_texture_id);
	assert(F != texture::invalid_texture_id);
	assert(G != texture::invalid_texture_id);

	texture::clean(A);
	texture::clean(B);
	texture::clean(C);
	texture::clean(D);
	texture::clean(E);
	texture::clean(F);
	texture::clean(G);
}

void write_test ()
{
	const unsigned char *img_data = (unsigned char*)"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyzABABABABABAB";
	auto A = texture::create(size2d(32, 32));

	// actual image data
	glBindTexture(GL_TEXTURE_2D, A);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, 32, 32, 0, GL_RGBA, GL_UNSIGNED_BYTE, img_data);

	// read back image data
	unsigned char	*exp_buf = new unsigned char[32 * 32 * 4];
	auto write_func = [exp_buf](int w, int h, int d, const unsigned char* buf)->bool {memcpy(exp_buf, buf, w*h*d); return true; };

	// write function
	texture::write(A, write_func);
	texture::clean(A);

	// test
	assert(memcmp(img_data, exp_buf, 32 * 32 * 4) == 0);

	// cleanup
	delete[] exp_buf;
}

void write_file_test ()
{
	const unsigned char *img_data = (unsigned char*)"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyzABABABABABAB";
	auto A = texture::create(size2d(32, 32));

	// actual image data
	glBindTexture(GL_TEXTURE_2D, A);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, 32, 32, 0, GL_RGBA, GL_UNSIGNED_BYTE, img_data);

	// read back image data
	auto tmp_filename = std::string(std::tmpnam(NULL));
	auto write_func_with_filename = [](const std::string& filename, int w, int h, int d, const unsigned char* buf)->bool
	{
		std::ofstream ofs(filename.c_str());
		ofs.write(reinterpret_cast<const char *>(buf), w*h*d);
		ofs.close();
		return true;
	};

	auto write_func = std::bind(write_func_with_filename,
		tmp_filename,
		std::placeholders::_1,
		std::placeholders::_2,
		std::placeholders::_3,
		std::placeholders::_4);

	// write function
	texture::write(A, write_func);
	texture::clean(A);

	std::ifstream ifs(tmp_filename);
	assert(ifs.good() == true);

	unsigned char	*exp_buf = new unsigned char[32 * 32 * 4];
	ifs.read((char*)exp_buf, 32 * 32 * 4);
	ifs.close();

	// test
	assert(memcmp(img_data, exp_buf, 32 * 32 * 4) == 0);

	// cleanup
	delete[] exp_buf;
	std::remove(tmp_filename.c_str());
}

void gl_texture_tests ()
{
	gl::context::create ();
	TEST(create_test);
	TEST(write_test);
	TEST(write_file_test);
	gl::context::clean ();
}

int main (int argc, char** argv)
{	
	TEST_GROUP(gl_window_tests);	
}
