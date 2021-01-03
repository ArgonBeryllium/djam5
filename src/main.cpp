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
	ip.scenes = new Scene*[4]{new SplashScene(), new Level(6, 3, {{0, 5}, {1, 2}}, 200, 10, 0), new ResultScene(), new DeathScene()};

	Game::init(ip);
	Mix_AllocateChannels(32);

	shitrndr::WindowProps::setPixScale(3);
	Assets::init();
	UI::loadFont("res/ProggyTiny.ttf", .05);
	UI::loadFont("res/Ludum-Dairy-0.2.0.ttf", .07);
	UI::loadFont("res/KingthingsExtortion-YMAa.ttf", .075);
	SDL_SetRenderDrawBlendMode(shitrndr::ren, SDL_BLENDMODE_BLEND);

	Game::start();
}
