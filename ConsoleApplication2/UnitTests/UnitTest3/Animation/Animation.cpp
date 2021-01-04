#include "Animation.h"

Animation::~Animation(void) {}

Animation::Animation(const std::string& _id) : id(_id) {}

const std::string& Animation::GetId(void) {
	return id;
}

void Animation::SetId(const std::string& _id) {
	id = _id;
}