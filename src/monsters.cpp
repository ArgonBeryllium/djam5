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
	parentObj->removeComponent(parentObj->getComponent<BasicRen>());
}
void Sparrot::onEvent(const ecs::Event& e)
{
	Plant::onEvent(e);
	if(e.type!=ecs::Event::frame) return;
	parentObj->transform.scl = v2f{1,1}*(.6+.4*hydration);
	shitrndr::Copy(Assets::tex_sparrot, parentObj->transform.getScreenRect());
}
Monster* Sparrot::getMut(GameObj* obj) { return new SparrotMon(obj); }

std::string SparrotMon::getName() { return "mutant sparrot"; }
void SparrotMon::onStart()
{
	life = .3;
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
