#include "monsters.h"

template <typename T>
static T lerp(const T& a, const T& b, const float& f) { return a + (b-a)*f; }

using namespace bj;

void Monster::takeDamage(float amt) { life -= amt; }

std::string Sparrot::getName() { return "sparrot"; }
int Sparrot::getWorth() { return 10; }
float Sparrot::getPower() { return .05; }
void Sparrot::onStart()
{
	sr = new SpriteRen(parentObj, Assets::tex_sparrot);
	parentObj->addComponent(sr);
}
void Sparrot::onEvent(const ecs::Event& e)
{
	Plant::onEvent(e);
	if(e.type!=ecs::Event::frame) return;
	parentObj->getComponent<SpriteRen>()->offset.scl = v2f{1,1}*(.6+.4*hydration);
}
Monster* Sparrot::getMut(GameObj* obj) { return new SparrotMon(obj); }

std::string SparrotMon::getName() { return "mutant sparrot"; }
void SparrotMon::onStart()
{
	life = .2;
	parentObj->transform.scl *= .8;
}
void SparrotMon::takeDamage(float amt)
{
	Monster::takeDamage(amt);
	if(jt != 0) jt = 0;
	dir *= -1;
}
void SparrotMon::onEvent(const bj::ecs::Event &e)
{
	if(e.type!=ecs::Event::frame) return;

	cd -= e.delta;
	if(jt>0 && cd <= 0)
	{
		parentObj->transform.pos += dir*e.delta*8;
		jt -= e.delta*8;
		if(!hit && (Player::instance->parentObj->transform.pos - parentObj->transform.pos).getLengthSquare()<.3)
		{
			hit = 1;
			Player::hurt(.2);
			cd = 1;
		}
		SDL_Rect r = parentObj->transform.getScreenRect();
		shitrndr::CopyEx(Assets::tex_sparrot_walk, {540, 0, 540, 540}, r, std::atan2(dir.y, dir.x)*180/M_PI, {r.w/2, r.h/2}, SDL_FLIP_NONE);
		return;
	}
	else dir = lerp(dir, (Player::instance->parentObj->transform.pos - parentObj->transform.pos).normalised(), e.delta);
	if(jt <= 0 && (Player::instance->parentObj->transform.pos - parentObj->transform.pos).getLengthSquare() <= 4.5)
	{
		jt = 1;
		hit = 0;
	}
	SDL_Rect r = parentObj->transform.getScreenRect();
	shitrndr::CopyEx(Assets::tex_sparrot_walk, {int(e.elapsed*10)%9*540, 0, 540, 540}, r, std::atan2(dir.y, dir.x)*180/M_PI, {r.w/2, r.h/2}, SDL_FLIP_NONE);
	parentObj->transform.pos += dir*e.delta;
}
int SparrotMon::getWorth() { return 20; }



std::string Scorn::getName() { return "scorn"; }
int Scorn::getWorth() { return 15; }
float Scorn::getPower() { return .08; }
void Scorn::onStart()
{
	sr = new SpriteRen(parentObj, Assets::tex_scorn);
	sr->sourceRect = new SDL_Rect{0,0,540,540};
	parentObj->addComponent(sr);
}
void Scorn::onEvent(const ecs::Event& e)
{
	Plant::onEvent(e);
	if(e.type!=ecs::Event::frame) return;
	sr->offset.scl = v2f{1,1}*(.6+.4*hydration);
}
Monster* Scorn::getMut(bj::GameObj *obj) { return new ScornMon(obj); }

std::string ScornMon::getName() { return "mutant scorn"; }
void ScornMon::onStart()
{
	life = .4;
	parentObj->transform.scl *= 1.5;
}
void ScornMon::onEvent(const bj::ecs::Event &e)
{
	if(e.type!=ecs::Event::frame) return;

	cd -= e.delta;
	shitrndr::Copy(Assets::tex_scorn, {int(cd*6)%9*540,0,540,540}, parentObj->transform.getScreenRect());
	if(cd<=0)
	{
		cd = 1.5;
		for(GameObj* pop : pops) parentObj->parentScene->destroy(pop->getID());
		pops.clear();
		int c = std::rand()%3 + 4;
		for(int i = 0; i != c; i++)
		{
			GameObj* pop = parentObj->parentScene->instantiate();
			pop->addComponent(new SpriteRen(pop, Assets::tex_pop_scorn[std::rand()%4]));
			float a = M_PI*2*i/c;
			pop->transform.pos = parentObj->transform.pos + v2f{std::cos(a), std::sin(a)};
			pops.push_back(pop);
		}
	}
	for(auto i = pops.begin(); i != pops.end(); i++)
	{
		GameObj* pop = *i.base();
		pop->transform.pos += (pop->transform.pos - parentObj->transform.pos).normalised()*e.delta*3;
		if((pop->transform.pos-Player::instance->parentObj->transform.pos).getLengthSquare()<1.1)
		{
			parentObj->parentScene->destroy(pop->getID());
			Player::hurt(.2);
			i = pops.erase(i);
		}
	}
}
int ScornMon::getWorth() { return 30; }
void ScornMon::takeDamage(float amt)
{
	life -= amt;
	if(life<=0)
		for(GameObj* pop : pops) parentObj->parentScene->destroy(pop->getID());
}
