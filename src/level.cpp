#include "level.h"
#include "definitions.h"

template <typename T>
static T lerp(const T& a, const T& b, const float& f) { return a + (b-a)*f; }

Level* Level::instance;
Level::Level (uint8_t w, uint8_t h, std::vector<std::pair<uint8_t, uint8_t>> seeds_)
{
	using namespace bj;
	instance = this;
	for(auto p : seeds_)
		seeds[p.first] = p.second;
	instantiate()->addComponent(new Player(getObj(0)));
	for(float y = 0; y != h; y++)
		for(float x = 0; x != w; x++)
		{
			GameObj* obj = instantiate();
			obj->transform.pos = {2+1.5f*x,1.5f*y};
			obj->addComponent(new Ground(obj));
		}
	GameObj* o = instantiate();
	o->transform.pos = {-3, -3};
	o->addComponent(new Well(o));
}

void Level::onRenderFG(float d, float t)
{
	cam.pos = lerp(cam.pos, Player::instance->parentObj->transform.pos, d*3);
	for(auto p : getObjs())
	{
		if(p.second->getComponent<Plant>() && p.second->getComponent<Plant>()->hydration <= 0)
			Plant::kill(p.second->getComponent<Plant>());
	}
}

