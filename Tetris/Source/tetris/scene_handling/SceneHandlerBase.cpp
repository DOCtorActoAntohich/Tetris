#include "tetris/scene_handling/SceneHandlerBase.h"

using namespace tetris::scene_handling;


SceneHandlerBase::SceneHandlerBase() {
	this->nextScene = Scene::NONE;
}


Scene SceneHandlerBase::getNextScene() {
	return this->nextScene;
}