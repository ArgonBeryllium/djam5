#include "definitions.h"
#include "level.h"
#include "monsters.h"
#include <bj/common.h>
using namespace bj;

Player* Player::instance;
void Player::onStart()
{
	if(this==instance) return;
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
			
			cd -= e.delta;

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
						if(!((Plant*)sel)->getAction().compare("harvest") && hasSaw && cd <= 0)
						{
							if(power >= ((Plant*)sel)->getPower())
							{
								cd = .5;
								harvest((Plant*)sel);
								return;
							}
							else
								Audio::playSound(Assets::sfx_no_power);
						}

						float amt = e.type==ecs::Event::keyD?.1:.2;
						if(((Plant*)sel)->hydration<1 && instance->water >= amt-.01)
						{
							Audio::playSound(Assets::sfx_water[std::rand()%2]);
							water-=amt;
							((Plant*)sel)->water(amt);
						}
					}
					else if(dynamic_cast<Well*>(sel))
					{
						if(water<=.91)
						{
							Audio::playSound(Assets::sfx_refill_water);
							water+=.1;
						}
					}
					else if(dynamic_cast<Charger*>(sel))
					{
						Audio::playSound(Assets::sfx_charge);
						hasSaw = !hasSaw;
					}
					else if(dynamic_cast<Ground*>(sel))
					{
						if( Level::instance->seeds[Level::instance->selSeed]>0)
						{
							Audio::playSound(Assets::sfx_plant);
							Level::instance->seeds[Level::instance->selSeed]--;
							Plant* p;
							switch(Level::instance->selSeed)
							{
								case 0: p = new Sparrot(sel->parentObj); break;
							}
							sel->parentObj->addComponent(p);
							sel->parentObj->removeComponent(sel);
						}
					}
					else if(dynamic_cast<Monster*>(sel))
					{
						float amt = e.type==ecs::Event::keyD?.1:.2;
						if(hasSaw && power >= amt && cd <= 0)
						{
							cd = amt*5;
							power -= amt;
							Audio::playSound(e.type==ecs::Event::keyD?Assets::sfx_harvest:Assets::sfx_attack);
							((Monster*)sel)->takeDamage(amt);
							if(((Monster*)sel)->life<=0)
							{
								money += ((Monster*)sel)->getWorth();
								parentObj->parentScene->destroy(sel->parentObj->getID());
							}
						}
						else if(hasSaw && cd <=0)
							Audio::playSound(Assets::sfx_no_power);
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
	instance->power -= plant->getPower();
	plant->parentObj->addComponent(new Ground(plant->parentObj));
	plant->parentObj->removeComponent(plant);
	Audio::playSound(Assets::sfx_harvest);
}
void Player::hurt(float dmg)
{
	instance->life -= dmg;
	if(instance->life<=0) die("getting wounded is usually not very healthy.");
	common::shakeCam(5,.25, Camera::getActiveCam(), 0);
}
void Player::die(const std::string &caption)
{
	DeathScene::caption = caption;
	SceneManager::setActiveScene(3);
}
