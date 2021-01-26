#include "HPipe.h"
#include "Util.h"
#include "PipeManager.h"

HPipe::HPipe(std::string& scene, uint y0, uint y1, uint x)
	:y0(y0), y1(y1), x(x), Pipe(scene)
{
	PipeManager::GetSingleton().AddHPipe(this);
}


bool HPipe::RectCollidesWithPipe(const Rect& rect) {
	if (isNearlyEqual(float(rect.x + rect.w), float(x))) {
		if (rect.y > y0 && rect.y < y1)
			return true;
	}
	return false;
}