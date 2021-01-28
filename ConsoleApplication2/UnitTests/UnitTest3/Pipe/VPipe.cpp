#include "VPipe.h"
#include "Util.h"
#include "PipeManager.h"

VPipe::VPipe(std::string& scene, uint x0, uint x1, uint y)
	:x0(x0), x1(x1), y(y), Pipe(scene)
{
	PipeManager::GetSingleton().AddVPipe(this);
}

bool VPipe::RectCollidesWithPipe(const Rect& rect) {
	if (isNearlyEqual(float(rect.y + rect.h), float(y))) {
		if (rect.x > x0 && rect.x < x1)
			return true;
	}
	return false;
}