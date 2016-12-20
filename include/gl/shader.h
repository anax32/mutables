namespace gl
{
	/*
	 * shader management
	 */
	namespace shader
	{
		/*
		* shader_t is a collection of GL_PROGRAM, GL_VERTEX_SHADER, GL_FRAGMENT_SHADER etc objects
		*/
		typedef resource_set_t						shader_t;
		/*
		* definition_t is a list of shader types and filenames containing the source for a shader of that type
		*/
		typedef std::map<std::string, std::string>	definition_t;
		/*
		* pipeline_t is a definition and the compiled objects associated with that definition
		*/
		typedef std::pair<definition_t, shader_t>	pipeline_t;
		/*
		* dictionary-of-pipelines type
		*/
		typedef std::map<std::string, pipeline_t>	pipeline_set_t;

		/*
		* convert string names to gl enum types
		*/
		static const GLenum shaderTypeEnum(const std::string& type)
		{
			if (type.compare("vertex\0") == 0)			{ return GL_VERTEX_SHADER; }
			else if (type.compare("geometry\0") == 0)	{ return GL_GEOMETRY_SHADER; }
			else if (type.compare("fragment\0") == 0)	{ return GL_FRAGMENT_SHADER; }
			else if (type.compare("program\0") == 0)	{ return GL_PROGRAM; }
			else										{ return 0; }
		}
		/*
		* convert string names to gl enum types as strings
		*/
		static const std::string shaderTypeName(const std::string& type)
		{
			if (type.compare("vertex\0") == 0)			{ return "GL_VERTEX_SHADER\0"; }
			else if (type.compare("geometry\0") == 0)	{ return "GL_GEOMETRY_SHADER\0"; }
			else if (type.compare("fragment\0") == 0)	{ return "GL_FRAGMENT_SHADER\0"; }
			else if (type.compare("program\0") == 0)	{ return "GL_PROGRAM\0"; }
			else										{ return "GL_NONE\0"; }
		}

		/**
		* prints the log associated with a shader to fout.
		* @param shader ShaderProfile object describing the shader to inspect
		* @param shaderType shader element to return the log for (vertex, geom, etc)
		* @param fout filestream to print output on
		*/
		void get_program_log(shader_t& shader, const std::string& type, const std::string& prefix)
		{
			GLsizei		slen = 0;
			int			log_len = 0;
			size_t		tag_len = 0;
			std::string	log;

			// get the length of the log file
			if (type.compare("program\0") == 0)			{ glGetProgramiv(shader[type], GL_INFO_LOG_LENGTH, &log_len); }
			else										{ glGetShaderiv(shader[type], GL_INFO_LOG_LENGTH, &log_len); }

			// if there is no file stream or no log, exit
			if (/*(fout != NULL) ||*/ (log_len < 2))	{ return; }

			tag_len = prefix.length() + 2;

			// copy the long to a string
			{
				char *buf = new char[log_len];
				memset(buf, 0, log_len*sizeof(char));

				// copy the log into the allocated storage space
				if (type.compare("program\0") == 0)			{ glGetProgramInfoLog(shader[type], log_len, &slen, buf); }
				else										{ glGetShaderInfoLog(shader[type], log_len, &slen, buf); }

				log.append(buf);
				delete[] buf;
			}

			// write the log to the stream
#ifdef _WIN32
			if (type.compare("program\0") != 0)		// FIXME: program info log is a slightly different format than the shader info logs
			{
				unsigned int		errid, perid;		// error id in list, previous error
				std::string::iterator	st, p, nd;		// pointer to start

				// print
				for (st = std::begin(log), perid = 0;
					(std::distance(log.begin(), st) < log_len);
					)
				{
					// find the marker for the line number and end of error
					p = std::find(st, std::end(log), '(');
					nd = std::find(st, std::end(log), '\n');

					// get the error id
					errid = atoi(std::string(st, p).c_str());

					// check if the error number is incremented
					if (perid != errid)							{ perid = errid; }

					// output the error in debug string format
					//sprintf_s(out, 1024, "%s%.*s\0", prefix, (int)(nd - p), p);

					std::cerr << prefix << std::setw(std::distance(p, nd)) << (*p) << std::endl;

					// find the beginning of the next error string
					//if ((st = strchr(st, '\n')) == NULL)		{ break; }
					//else										{ st++; }

					if ((st = std::find(st, std::end(log), '\n')) == std::end(log))	{ break; }
					else													{ std::next(st); }
				}
			}
			else
#endif
			{
				std::cerr << log << std::endl;
			}
		}

#if 0
		bool compile(shader& shader, const std::string& type, const std::string& sourceFilePath)
		{
			size_t	blen = 0;
			char	*src = NULL;

			read(sourceFilePath.c_str(), src, blen);

			if (blen == 0)											{ fprintf(stdout, "ERR: Could not query '%s'\0", sourceFilePath); return false; }

			// create storage for reading the files
			src = new char[blen + 1];
			memset(src, 0, (1 + blen)*sizeof(char));

			if (read(sourceFilePath.c_str(), src, blen) == 0)		{ fprintf(stdout, "ERR: Could not read '%s'\n\0", sourceFilePath);	return false; }

			auto retval = compile(shader, type, src);

			// cleanup
			delete[] src;
			src = NULL;

			return retval;
		}
#endif
		/*
		* verify a shader by calling glLinkProgram and glValidateProgram.
		* returns true if GL_LINE_STATUS and GL_VALIDATE_STATUS == GL_TRUE, false otherwise
		*/
		bool verify(shader_t& shader)
		{
			GLint	valid, link;

			glLinkProgram(shader["program\0"]);
			glGetProgramiv(shader["program\0"], GL_LINK_STATUS, &link);

			if (link != GL_TRUE)
			{
				get_program_log(shader, "program\0", "PROGRAM\0");
				return false;
			}

			glValidateProgram(shader["program\0"]);
			glGetProgramiv(shader["program\0"], GL_VALIDATE_STATUS, &valid);

			if (valid != GL_TRUE)
			{
				get_program_log(shader, "program\0", "PROGRAM\0");
				return false;
			}

			return true;
		}

		/*
		* fill a shader_t by loading the source files in the definition_t
		*/
		void create(const definition_t& definition, shader_t& shader)
		{
			shader["program\0"] = glCreateProgram();

			for (auto d : definition)
			{
				if (d.second.length() == 0)						{ return; }

				auto	type = d.first;
				auto	fname = d.second;
				std::string	fsource = "";
				int		status = 0;
				FILE	*f;

				auto gl_type = shaderTypeEnum(type);
				if (gl_type == GL_NONE)							{ continue; }
				shader[type] = glCreateShader(gl_type);

				if ((f = fopen(fname.c_str(), "rb")) == NULL)	{ return; }
				size_t len = 0;
				fseek(f, 0, SEEK_END);
				len = ftell(f);
				fseek(f, 0, SEEK_SET);
				len -= ftell(f);

				if (len == 0)									{ fclose(f); continue; }

				auto buf = new char[len + 1];
				std::fill(buf, buf + len + 1, 0);

				fread(buf, 1, len, f);
				fclose(f);

				glShaderSource(shader[type], 1, (const GLchar**)&buf, NULL);
				glCompileShader(shader[type]);
				//delete[] buf;

				// check it compiled
				glGetShaderiv(shader[type], GL_COMPILE_STATUS, &status);

				get_program_log(shader, type, fname.c_str());

				if (status == GL_FALSE)							{ glDeleteShader(shader[type]); shader[type] = 0; }

				if (glIsShader(shader[type]) == GL_TRUE)		{ glAttachShader(shader["program\0"], shader[type]); }
			}

			// validate the program
			verify(shader);
		}
		/*
		* load the definition in pipeline_t into the shader in pipeline_t
		*/
		void create(pipeline_t& p)
		{
			create(p.first, p.second);
		}
		/*
		* load all the pipeline_ts in pipeline_set_t
		*/
		void create(pipeline_set_t& p)
		{
			for (auto &d : p)
			{
				create(d.second);
			}
		}
		/*
		* delete the shaders and programs associated with the shader_t
		*/
		void clean(shader_t& shader)
		{
			for (auto s : shader)
			{
				if (glIsShader(s.second) == GL_TRUE)			{ glDeleteShader(s.second); }
				else if (glIsProgram(s.second) == GL_TRUE)		{ glDeleteProgram(s.second); }
			}
		}
		void clean(pipeline_t& pipeline)
		{
			clean(pipeline.second);
		}
		void clean(pipeline_set_t& pipeline_set)
		{
			for (auto d : pipeline_set)
			{
				clean(d.second);
			}
		}
	}
}
