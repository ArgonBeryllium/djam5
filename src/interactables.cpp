#include "definitions.h"

using namespace bj;

void Interactable::onStart()
{
//	parentObj->addComponent(new BasicRen(parentObj, {0,155,0,255}));
}
void Interactable::onEvent(const bj::ecs::Event& e) {}
std::string Interactable::getAction() { return "action"; }
std::string Interactable::getName() { return "interactable"; }


std::string Radio::getAction() { return "change station"; }
std::string Radio::getName() { return "radio"; }

std::vector<Mix_Chunk*> Radio::songs;
uint8_t Radio::cp;
void Radio::changeStation()
{
	Audio::playSound(Assets::sfx_static);
	Mix_FadeOutChannel(Audio::channels[songs[cp]], 1000);
	cp++;
	cp%=songs.size();
	Audio::playSound(songs[cp]);
	Mix_FadeInChannel(Audio::channels[songs[cp]], songs[cp], 0, 1000);
}
void Radio::onEvent(const bj::ecs::Event& e)
{
	if(e.type!=ecs::Event::frame) return;
	if(!Mix_Playing(Audio::channels[songs[cp]]))
		changeStation();
}


std::string Monster::getAction() { return "attack"; }
std::string Monster::getName() { return "generic monster"; }

std::string Plant::getAction() { return ripeness<HARVEST_T?"water":"harvest"; }
std::string Plant::getName() { return "generic plant"; }
int Plant::getWorth() { return 10; }
float Plant::getPower() { return .05; }
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
	if(hydration>=.7) ripeness += rip_rate*e.delta;
}
void Plant::water(const float& amt)
{
	hydration += amt;
}
void Plant::kill(Plant* plant)
{
	GameObj* o = plant->parentObj;
	o->removeComponent(plant->sr);
	o->removeComponent(plant);
	o->addComponent(new Ground(o));
}


std::string Well::getName() { return "well"; }
std::string Well::getAction() { return "get water"; }

std::string Charger::getName() { return "charger"; }
std::string Charger::getAction() { return Player::instance->hasSaw?"leave saw to charge":"take saw"; }
void Charger::onEvent(const bj::ecs::Event &e)
{
	if(!Player::instance->hasSaw && Player::instance->power<1)
		Player::instance->power += e.delta*.1;
}

std::string Ground::getName() { return "ground"; }
std::string Ground::getAction() { return "plant"; }
