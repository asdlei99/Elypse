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
#ifndef ___TROLL_ELYPSE_PREREQUISITES_H___
#define ___TROLL_ELYPSE_PREREQUISITES_H___

#include <queue>

#include <Elypse.h>
#include <Module_Script.h>

#include "TrollEditorPrerequisites.h"

#define TROLL_ELYPSE_NAMESPACE TROLL_NAMESPACE::ElypseRW
#define BEGIN_TROLL_ELYPSE_NAMESPACE BEGIN_TROLL_NAMESPACE { namespace ElypseRW
#define END_TROLL_ELYPSE_NAMESPACE } END_TROLL_NAMESPACE

BEGIN_TROLL_NAMESPACE
{
	namespace ElypseRW
	{
		class TrollScriptBlock;
		class TrollScriptEngine;
		class TrollScriptCompiler;
		class TrollScriptFunction;
		class TrollMuseFile;
		class TrollSceneFileParser;
		class TrollInstance;
		class TrollFrameListener;

		String ValueTypeToString( unsigned int p_type );
	}
}
END_TROLL_NAMESPACE

#endif