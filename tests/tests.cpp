#include "gl/contexts_tests.h"
#include "gl/texture_tests.h"

#include "img_io/png_io_tests.h"

int main (int argc, char **argv)
{
    gl::context::test::run_all();
    gl::texture::test::run_all();
    
    png_io::test::run_all();

    return 0;
}