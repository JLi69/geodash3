#include <AL/alc.h>

namespace Geodash3
{
	class SoundDev
	{
		SoundDev();
		~SoundDev();
		ALCdevice* mDevice;
		ALCcontext* mContext;
	public:
		static SoundDev* get();
	};
}
