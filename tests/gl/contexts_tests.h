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
#elif __linux__
				int window_handle = 0;
#endif
				assert(gl::context::create(window_handle) == true);
				gl::context::clean(window_handle);
			}
			void run_all()
			{
				create();
			}
		}
	}
}
