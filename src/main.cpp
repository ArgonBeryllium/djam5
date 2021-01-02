#include <bj/berryJam.h>

#include "splash.h"
#include "definitions.h"
#include "level.h"

using namespace bj;

int BJ_MULP_MAIN()
{
	initParams ip;
	ip.nscenes = 2;
	ip.scenes = new Scene*[2]{new SplashScene(), new Level(6, 3, {})};

	Game::init(ip);
	UI::loadFont("res/ProggyTiny.ttf", .05);
	UI::loadFont("res/Ludum-Dairy-0.2.0.ttf", .07);
	UI::loadFont("res/KingthingsExtortion-YMAa.ttf", .075);
	shitrndr::WindowProps::setPixScale(3);
	SDL_SetRenderDrawBlendMode(shitrndr::ren, SDL_BLENDMODE_BLEND);
	Game::start();
}
