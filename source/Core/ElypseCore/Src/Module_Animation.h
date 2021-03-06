/*
See LICENSE file in root folder
*/
#ifndef ___MODULE_ANIMATION_H___
#define ___MODULE_ANIMATION_H___

#include "Elypse.h"

namespace Elypse
{
	namespace Animation
	{
		enum AnimationStatus
		{
			AS_STOPPED,
			AS_PLAYING,
			AS_PAUSED,
			AS_TIMEOUT
		};

		class EMAnimation;
		class AnimatedObject;
		class AnimatedObjectGroup;
		class AnimationManager;

		typedef std::map<String, EMAnimation *> AnimationMap;
		typedef std::map<String, AnimatedObject *> AnimatedObjectMap;
		typedef std::map<String, AnimatedObjectGroup *> AnimatedObjectGroupMap;
	}
}

using namespace Elypse::Animation;

#endif
