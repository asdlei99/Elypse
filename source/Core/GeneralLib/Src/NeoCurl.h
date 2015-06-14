/*
This source file is part of ElypsePlayer (https://sourceforge.net/projects/elypse/)

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License as published by the Free Software
Foundation; either version 2 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with
the program; if not, write to the Free Software Foundation, Inc., 59 Temple
Place - Suite 330, Boston, MA 02111-1307, USA, or go to
http://www.gnu.org/copyleft/lesser.txt.
*/
#ifndef ___CURL_PLUS_PLUS_H___
#define ___CURL_PLUS_PLUS_H___

#define CURL_STATICLIB
#include "Config.h"

#include <string>
#include <functional>

#include <curl/curl.h>
/*
#if GENLIB_WINDOWS
#	if defined(_MT) || defined(__MT__)
#		pragma comment( lib, "curllib_mt.lib")
#	else
#		pragma comment( lib, "curllib_md.lib")
#	endif
#	pragma comment( lib, "ws2_32.lib")
#	pragma comment( lib, "winmm.lib")
#endif
*/
namespace General
{
	namespace Files
	{
		class CURLManager
		{
		public:
			CURLManager( bool p_autoInitialise = true );
			~CURLManager();

		public:
			bool Initialise();
			void Close();
			CURLcode OpenUrl( const std::string & p_url, size_t ( * p_function )( void *, size_t, size_t, void * ), void * p_data, const std::string & p_postParams = std::string() );

			CURLcode GetStringFromUrl( const std::string & p_url, std::string & p_contents, const std::string & p_postParams = std::string() );
			CURLcode DownloadFile( const std::string & p_url, const std::string & p_filename, const std::string & p_postParams = std::string() );

			std::string GetLastContentType();
			std::string GetContentType( const std::string & p_url, const std::string & p_postParams = std::string() );

			CURLcode SetCookieFile( const char * p_filename );
			CURLcode SetCookieString( const char * p_cookieString );

		public:
			static std::string CurlError( CURLcode p_errorCode );
			static std::string MimeEncode( const std::string & p_string );

		private:
			static size_t _getString( void * p_buffer, size_t p_size, size_t p_numItems, void * p_data );
			static size_t _download( void * p_buffer, size_t p_size, size_t p_numItems, void * p_data );
			static size_t _null( void * p_buffer, size_t p_size, size_t p_numItems, void * p_data );

		private:
			CURL * m_handle;
			bool m_initialised;
		};
	}
}

#endif
