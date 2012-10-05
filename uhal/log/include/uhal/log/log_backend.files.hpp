
#ifndef _log_backend_files_hxx_
#define _log_backend_files_hxx_

#include <stdio.h>
#include "boost/thread.hpp" 

namespace uhal
{
	class log_file
	{
		private:
			log_file();
			~log_file();

			class log_file_helper
			{
				public:
					log_file_helper();
		
					~log_file_helper();
		
					FILE* mFile;
			};

			static boost::thread_specific_ptr< log_file_helper > mLogFileHelper;

		public:
			static FILE* get();
	};
}

#endif