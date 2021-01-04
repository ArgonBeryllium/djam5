#include <bj/berryJam.h>

#include "splash.h"
#include "definitions.h"
#include "level.h"
#include "monsters.h"

using namespace bj;

int BJ_MULP_MAIN()
{
	initParams ip;
	ip.nscenes = 4;
	ip.scenes = new Scene*[4]{new SplashScene(), new Level(3, 2, {{0, 3}, {1, 3}}, 0, 0, 0), new ResultScene(), new DeathScene()};

	Game::init(ip);
	Mix_AllocateChannels(24);

	shitrndr::WindowProps::setPixScale(3);
	Assets::init();
	UI::loadFont("res/ProggyTiny.ttf", .05);
	UI::loadFont("res/Ludum-Dairy-0.2.0.ttf", .07);
	UI::loadFont("res/KingthingsExtortion-YMAa.ttf", .075);
	SDL_SetRenderDrawBlendMode(shitrndr::ren, SDL_BLENDMODE_BLEND);

	Game::start();
}
