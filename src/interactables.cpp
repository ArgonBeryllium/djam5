#include "definitions.h"

using namespace bj;

void Interactable::onStart()
{
	parentObj->addComponent(new BasicRen(parentObj, {0,155,0,255}));
}
void Interactable::onEvent(const bj::ecs::Event& e) {}
std::string Interactable::getAction() { return "action"; }
std::string Interactable::getName() { return "interactable"; }

std::string Plant::getAction() { return ripeness<HARVEST_T?"water":"harvest"; }
std::string Plant::getName() { return "generic plant"; }
int Plant::getWorth() { return 10; }
void Plant::onStart()
{
	Interactable::onStart();
}
void Plant::onEvent(const bj::ecs::Event& e)
{
	if(e.type!=ecs::Event::frame) return;
	if((parentObj->transform.pos - Player::instance->parentObj->transform.pos).getLengthSquare() < 4)
	{
		constexpr float r = 5;
		vec2<uint32_t> sv = Camera::getActiveCam()->getScreenPoint(parentObj->transform.pos);
		SDL_SetRenderDrawColor(shitrndr::ren, 15, 15, 15, 255);
		shitrndr::FillCircle(sv.x+r, sv.y, r);
		shitrndr::FillCircle(sv.x+3.1*r, sv.y, r);
		SDL_SetRenderDrawColor(shitrndr::ren, 215, 15, 15, 255);
		shitrndr::FillCircle(sv.x+r, sv.y, int(hydration*r));
		SDL_SetRenderDrawColor(shitrndr::ren, 15, 215, 15, 255);
		shitrndr::FillCircle(sv.x+3.1*r, sv.y, int(ripeness*r));
	}
	hydration -= deh_rate*e.delta;
	parentObj->transform.scl = v2f{1,1}*(.6+.4*hydration);
	if(hydration>=.7) ripeness += rip_rate*e.delta;
}
void Plant::water(const float& amt)
{
	hydration += amt;
}
void Plant::kill(Plant* plant)
{
	GameObj* o = plant->parentObj;
	o->removeComponent(plant);
	o->addComponent(new Ground(o));
}

std::string Well::getName() { return "well"; }
std::string Well::getAction() { return "get water"; }

std::string Ground::getName() { return "ground"; }
std::string Ground::getAction() { return "plant"; }
