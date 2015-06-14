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
#ifndef ___ELYPSE_COMPILE_EXCEPTION_H___
#define ___ELYPSE_COMPILE_EXCEPTION_H___

#include <Exception.h>

namespace Elypse
{
	namespace Script
	{
		class CompileError
			: public General::Utils::GenException
		{
		public:
			CompileError( const std::string & p_description, const char * p_file, const char * p_function, unsigned int p_line )
				: General::Utils::GenException( p_description, p_file, p_function, p_line )
			{
			}

			virtual ~CompileError() d_no_throw
			{
			}
		};
	}
}

#define COMPILE_EXCEPTION( p_text) throw Elypse::Script::CompileError( p_text, __FILE__, __FUNCTION__, __LINE__)

#endif
