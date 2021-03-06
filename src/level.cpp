#include "level.h"
#include "definitions.h"
#include <bj/common.h>
#include "monsters.h"

template <typename T>
static T lerp(const T& a, const T& b, const float& f) { return a + (b-a)*f; }

using namespace bj;

Level* Level::instance;
Level::Level (uint8_t w_, uint8_t h_, std::vector<std::pair<uint8_t, uint8_t>> seeds_, float time_, int minSum_, bool lc) : w(w_), h(h_), time(time_), minSum(minSum_), lastChance(lc)
{
	instance = this;
	for(auto p : seeds_)
		seeds[p.first] = p.second;
}
void Level::onLoad()
{
	shitrndr::bg_col = {0xdf, 0xbb, 0x59};

	clearObjs();
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
	o->transform.pos = {-3, 3};
	o->addComponent(new Well(o));
	o = instantiate();
	o->transform.pos = {-3, -3};
	o->addComponent(new Charger(o));

	for(int i = 0; i < 6; i++)
	{
		o = instantiate();
		o->addComponent(new SpriteRen(o, Assets::tex_rubbish[std::rand()%2]));
		o->transform.scl = v2f{1,1}*(5+common::frand()*5);
		o->transform.pos = {(float)i*5-13, -17};
		o->addComponent(new RBody(o, 0));
		o = instantiate();
		o->addComponent(new SpriteRen(o, Assets::tex_rubbish[std::rand()%2]));
		o->transform.scl = v2f{1,1}*(5+common::frand()*5);
		o->transform.pos = {(float)i*5-13, 7};
		o->addComponent(new RBody(o, 0));
	}
	for(int i = 0; i < 5; i++)
	{
		o = instantiate();
		o->addComponent(new SpriteRen(o, Assets::tex_rubbish[std::rand()%2]));
		o->transform.scl = v2f{1,1}*(5+common::frand()*5);
		o->transform.pos = {-16, (float)i*5-17};
		o->addComponent(new RBody(o, 0));
		o = instantiate();
		o->addComponent(new SpriteRen(o, Assets::tex_rubbish[std::rand()%2]));
		o->transform.scl = v2f{1,1}*(5+common::frand()*5);
		o->transform.pos = {16, (float)i*5-17};
		o->addComponent(new RBody(o, 0));
	}

	o = instantiate();
	o->addComponent(new Radio(o));
	cam.scale = .3;
}

void Level::onStart()
{
}

void Level::onRenderFG(float d, float t)
{
	time -= d;
	UI::renderStaticText(1, 0, ("level " + std::to_string(ResultScene::level)), {UI::RIGHT});
	UI::renderText(.5, .1, (std::to_string(int(time/60)) + "m " + std::to_string(int(time)%60)+"s").c_str(), {UI::CENTRED, 1});
	UI::renderStaticText(1, .2, "SEEDS ([TAB])", {UI::RIGHT});
	UI::renderText(1, .25, (std::string(selSeed==0?">>":"- ") + "sparrot seeds (" + std::to_string(seeds[0]) + ")").c_str(), {UI::RIGHT});
	UI::renderText(1, .3, (std::string(selSeed==1?">>":"- ") + "scorn seeds (" + std::to_string(seeds[1]) + ")").c_str(), {UI::RIGHT});

	v2f pp = Player::instance->parentObj->transform.pos;
	if(pp.x>-3 && pp.x<8) cam.pos.x = lerp(cam.pos.x, pp.x, d*3);
	if(pp.y>-5 && pp.y<15) cam.pos.y = lerp(cam.pos.y, pp.y, d*3);
	for(auto p : getObjs())
	{
		if(p.second->getComponent<Plant>())
		{
			if(p.second->getComponent<ScornMon>())
				for(GameObj* pop : p.second->getComponent<ScornMon>()->pops)
					pop->getComponent<SpriteRen>()->onEvent({ecs::Event::frame, d, t});
			Plant* plant = p.second->getComponent<Plant>();
			shitrndr::Copy(plant->sr->tex, *plant->sr->sourceRect, (plant->sr->offset+p.second->transform).getScreenRect());
			if(plant->hydration <= 0)
				Plant::kill(plant);
			else if(plant->ripeness>Plant::HARVEST_T/2)
				if(common::frand()<plant->ripeness*plant->ripeness/1800)
				{
					GameObj* m = p.second->parentScene->instantiate();
					m->transform.pos = p.second->transform.pos;
					m->addComponent(plant->getMut(m));
					Plant::kill(plant);
				}
		}
	}
	if(time < 10)
	{
		SDL_SetRenderDrawColor(shitrndr::ren, 0,0,0,255-(255*time/10));
		shitrndr::FillRect(shitrndr::WindowProps::getSizeRect());
	}
	if(time < 0)
	{
		Mix_FadeOutChannel(Audio::channels[Radio::songs[Radio::cp]], 1500);
		SceneManager::setActiveScene(2);
	}
	Player::instance->parentObj->getComponent<SpriteRen>()->onEvent(ecs::Event{ecs::Event::frame, d, t});
	Player::instance->heldObj->getComponent<SpriteRen>()->onEvent(ecs::Event{ecs::Event::frame, d, t});
}

uint8_t ResultScene::level = 0;
void ResultScene::onStart()
{
}
void ResultScene::onLoad()
{
	loadS = 0;
	shitrndr::bg_col = {5, 0, 7};
}
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
			else UI::renderStaticText(.5, .6, "That's... not enough.", {UI::CENTRED});
		}
		else UI::renderStaticText(.5, .6, "You survived.", {UI::CENTRED});

		if(ts>3)
		{
			UI::renderStaticText(.5, .7, "[SPACE]", {UI::CENTRED});
			if(Input::getKey(SDLK_SPACE))
			{
				if(!passed && Level::instance->lastChance)
				{
					Player::die("tip: sustinance is necessary for survival.");
					return;
				}
				Level::instance->clearObjs();
				delete Level::instance;
				if(passed) level++;
				switch(level)
				{
					case 0:	Level::instance = new Level(3, 2, {{0,3},{1,3}}, 15, 10, !passed); break;
					case 1:	Level::instance = new Level(4, 4, {{0,6},{1,4}}, 15, 10, !passed); break;
				}
				Level::instance->onStart();
				SceneManager::scenes[1] = Level::instance;
				SceneManager::setActiveScene(1);
			}
		}
	}
}

std::string DeathScene::caption;
void DeathScene::onLoad()
{
	shitrndr::bg_col = {5, 0, 7};
}
void DeathScene::onRenderFG(float d, float t)
{
	shitrndr::bg_col = {Uint8(std::abs(std::sin(t))*25), 0, 7};
	UI::renderStaticText(.5, .4, "you failed to survive", {UI::CENTRED, 1, {255,0,0,255}});
	UI::renderStaticText(.5, .7, caption, {UI::CENTRED});
	static float st = t;
	if(t - st > 2)
	{
		UI::renderStaticText(.5, .6, "press [SPACE] to exit", {UI::CENTRED});
		if(Input::getKey(SDLK_SPACE))
			exit(0);
	}
}

void CreditScene::onRenderFG(float d, float t)
{
	static float st = t;
	float o = (t-st) * .05-.5;
	UI::renderStaticText(.5,  -o, "SOW SAW", {UI::CENTRED, 1});
	UI::renderStaticText(.5, .2 - o, "A disappointment of a game", {UI::CENTRED});
	UI::renderStaticText(.5, .25 - o, "Made for DJam 5", {UI::CENTRED});
	UI::renderStaticText(.5, .3 - o, "Graphics, programming and segfaults by ArBe", {UI::CENTRED});
	UI::renderStaticText(.5, .35 - o, "Music & SFX by Jakub Pietrzak", {UI::CENTRED});
	UI::renderStaticText(.5, .8 - o, "big pog chug cum", {UI::CENTRED});
	if(o>1) exit(0);
}
