#include "definitions.h"
#include <bj/common.h>
using namespace bj;

Player* Player::instance;
void Player::onStart()
{
	parentObj->addComponent(new BasicRen(parentObj));
	instance = this;
}
void Player::onEvent(const ecs::Event &e)
{
	switch(e.type)
	{
		case ecs::Event::frame:
		{
			constexpr float speed = 3;
			
			v2f iv = common::inVec();
			parentObj->transform.pos += iv*e.delta*speed;
			if(iv.getLengthSquare()!=0) dir=iv;

			vec2<uint32_t> sp = Camera::getActiveCam()->getScreenPoint(parentObj->transform.pos+v2f{parentObj->transform.scl.x/2, -parentObj->transform.scl.y});
			v2f ssp = {sp.x/(float)shitrndr::WindowProps::getWidth(), sp.y/(float)shitrndr::WindowProps::getHeight()};

			Interactable* sel = check();
			if(sel)
			{
				UI::renderStaticText(ssp.x, ssp.y, sel->getAction() + "[SPACE]", {UI::CENTRED});
				UI::renderStaticText(ssp.x, ssp.y-.1, sel->getName(), {UI::CENTRED});
			}

			UI::renderText(0, 0, ("H2O: " + std::to_string(water)).c_str(), {});
			UI::renderText(0, .05, ("POW: " + std::to_string(power)).c_str(), {});
			UI::renderText(0, .1, ("$$$: " + std::to_string(money)).c_str(), {});
		}
		break;
		case ecs::Event::keyH:
		case ecs::Event::keyD:
			if(e.key==SDLK_SPACE)
			{
				Interactable* sel = check();
				if(sel)
				{
					if(dynamic_cast<Plant*>(sel))
					{
						if(!((Plant*)sel)->getAction().compare("harvest"))
							harvest((Plant*)sel);
						else if(((Plant*)sel)->hydration<1 && instance->water > 0)
						{
							float amt = e.type==ecs::Event::keyD?.1:.2;
							water-=amt;
							((Plant*)sel)->water(amt);
						}
					}
					else if(dynamic_cast<Well*>(sel))
					{
						if(water<=.9)
							water+=.1;
					}
					else if(dynamic_cast<Ground*>(sel))
					{
						sel->parentObj->addComponent(new Plant(sel->parentObj));
						sel->parentObj->removeComponent(sel);
					}
				}
			}
		break;

		default: break;
	}
}
Interactable* Player::check()
{
	for(auto p : SceneManager::getActiveScene()->getObjs())
	{
		Interactable* out = p.second->getComponent<Interactable>();
		if(out && (p.second->transform.pos - instance->parentObj->transform.pos - instance->dir).getLengthSquare()<=1)
			return out;
	}
	return 0;
}
void Player::harvest(Plant *plant)
{
	instance->money += plant->getWorth();
	plant->parentObj->addComponent(new Ground(plant->parentObj));
	plant->parentObj->removeComponent(plant);
}
