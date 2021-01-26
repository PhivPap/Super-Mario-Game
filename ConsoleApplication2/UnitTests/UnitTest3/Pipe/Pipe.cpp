#include "Pipe.h"

Pipe::Pipe(std::string& scene)
	: scene(scene)
{}

const std::string& Pipe::GetScene() const{
	return scene;
}