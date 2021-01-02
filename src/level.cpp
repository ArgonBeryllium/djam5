#include "level.h"
#include "definitions.h"

template <typename T>
static T lerp(const T& a, const T& b, const float& f) { return a + (b-a)*f; }

using namespace bj;

Mix_Chunk* Assets::sfx_money;
void Assets::init()
{
	sfx_money = fileIO::loadSound("res/sfx/money.wav");
}

Level* Level::instance;
Level::Level (uint8_t w_, uint8_t h_, std::vector<std::pair<uint8_t, uint8_t>> seeds_, float time_, int minSum_, bool lc) : w(w_), h(h_), time(time_), minSum(minSum_), lastChance(lc)
{
	instance = this;
	for(auto p : seeds_)
		seeds[p.first] = p.second;
}

void Level::onStart()
{
	GameObj* o = instantiate();
	o->addComponent(new Player(o));
	for(float y = 0; y != h; y++)
		for(float x = 0; x != w; x++)
		{
			GameObj* obj = instantiate();
			obj->transform.pos = {2+1.5f*x,1.5f*y};
			obj->addComponent(new Ground(obj));
		}
	o = instantiate();
	o->transform.pos = {-3, -3};
	o->addComponent(new Well(o));

	cam.scale = .3;
}

void Level::onRenderFG(float d, float t)
{
	time -= d;
	UI::renderStaticText(1, 0, ("level " + std::to_string(ResultScene::level)), {UI::RIGHT});
	UI::renderText(1, .1, ("time left: " + std::to_string(time)).c_str(), {UI::RIGHT});
	if(time < 10)
	{
		SDL_SetRenderDrawColor(shitrndr::ren, 0,0,0,255-(255*time/10));
		shitrndr::FillRect(shitrndr::WindowProps::getSizeRect());
	}
	if(time < 0)
	{
		SceneManager::setActiveScene(2);
	}
	cam.pos = lerp(cam.pos, Player::instance->parentObj->transform.pos, d*3);
	for(auto p : getObjs())
	{
		if(p.second->getComponent<Plant>() && p.second->getComponent<Plant>()->hydration <= 0)
			Plant::kill(p.second->getComponent<Plant>());
	}
}

uint8_t ResultScene::level = 0;
void ResultScene::onStart()
{
}
void ResultScene::onLoad() { loadS = 0; }
void ResultScene::onRenderFG(float d, float t)
{
	if(!loadS) loadS = t;
	float ts = t - loadS;

	if(ts==0)
		Audio::playSound(Assets::sfx_money, 1);
	bool passed = Player::instance->money>=Level::instance->minSum;

	if(ts > 2.3)
	{
		UI::renderText(.5, .4, +std::to_string(Player::instance->money).c_str(), {UI::CENTRED, 1});
		if(!passed)
		{
			if(!Level::instance->lastChance) UI::renderStaticText(.5, .6, "Lookin' thin.", {UI::CENTRED});
			else UI::renderStaticText(.5, .6, "You failed to survive.", {UI::CENTRED});
		}
		else UI::renderStaticText(.5, .6, "You survived.", {UI::CENTRED});

		if(ts>3)
		{
			UI::renderStaticText(.5, .7, "[SPACE]", {UI::CENTRED});
			if(Input::getKey(SDLK_SPACE))
			{
				Level::instance->clearObjs();
				delete Level::instance;
				//TODO actual levels
				Level::instance = new Level(10, 10, {}, 15, 10);
				Level::instance->onStart();
				SceneManager::scenes[1] = Level::instance;
				SceneManager::setActiveScene(1);
			}
		}
	}
}
