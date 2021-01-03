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
	ip.scenes = new Scene*[4]{new SplashScene(), new Level(6, 3, {{0, 5}}, 60, 10, 0), new ResultScene(), new DeathScene()};

	Game::init(ip);

	shitrndr::WindowProps::setPixScale(3);
	Assets::init();
	UI::loadFont("res/ProggyTiny.ttf", .05);
	UI::loadFont("res/Ludum-Dairy-0.2.0.ttf", .07);
	UI::loadFont("res/KingthingsExtortion-YMAa.ttf", .075);
	SDL_SetRenderDrawBlendMode(shitrndr::ren, SDL_BLENDMODE_BLEND);
	SceneManager::getScene(1)->instantiate()->addComponent(new SparrotMon(SceneManager::getScene(1)->getObj(0)));
	SceneManager::getScene(1)->getObj(0)->transform.pos = {-3, 0};

	Game::start();
}
