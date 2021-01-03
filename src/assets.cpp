#include "definitions.h"

using namespace bj;

Mix_Chunk* Assets::sfx_money, *Assets::sfx_splash;
void Assets::init()
{
	sfx_money = fileIO::loadSound("res/sfx/money.wav");
	sfx_splash = fileIO::loadSound("res/sfx/arbe startup.wav");
}
