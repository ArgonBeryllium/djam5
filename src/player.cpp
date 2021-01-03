#include "definitions.h"
#include "level.h"
#include "monsters.h"
#include <bj/common.h>
using namespace bj;

Player* Player::instance;
template <typename T>
static T lerp(const T& a, const T& b, const float& f) { return a + (b-a)*f; }

void Player::onStart()
{
	if(this==instance) return;
	parentObj->addComponent(new SpriteRen(parentObj, Assets::tex_mc_down));
	parentObj->getComponent<SpriteRen>()->sourceRect = new SDL_Rect{0,0,540,540};
	parentObj->addComponent(new RBody(parentObj));
	parentObj->getComponent<RBody>()->onCollision = [](RBody* self, RBody* other, const intersection* in)
	{
		self->vel *= 0;
	};
	heldObj = parentObj->parentScene->instantiate();
	heldObj->addComponent(new SpriteRen(heldObj, Assets::tex_saw));
	heldObj->getComponent<SpriteRen>()->sourceRect = new SDL_Rect{0,0,540,540};
	instance = this;
}
void Player::onEvent(const ecs::Event &e)
{
	switch(e.type)
	{
		case ecs::Event::frame:
		{
			constexpr float speed = 3;
			static float fid = 0;
			static SpriteRen* sr = parentObj->getComponent<SpriteRen>();
			static SpriteRen* hsr = heldObj->getComponent<SpriteRen>();
			
			cd -= e.delta;

			v2f iv = common::inVec();
			parentObj->transform.pos += iv*e.delta*speed;
			if(iv.getLengthSquare()!=0)
			{
				dir=iv;
				fid+= e.delta*15;
				hsr->angle = std::atan2(dir.y, dir.x)-M_PI_2;
			}

			if(!hasSaw)hsr->tex=Assets::tex_water_can[0];
			if((hsr->tex==Assets::tex_saw && hasSaw) || hsr->tex!=Assets::tex_saw)
				heldObj->transform.pos = lerp(heldObj->transform.pos, parentObj->transform.pos+dir*.6, e.delta*8);
			else
				heldObj->transform.pos = charger->transform.pos;
			if(Input::getKey(SDLK_SPACE))
			{
				if(hasSaw && hsr->tex==Assets::tex_saw) heldObj->transform.pos += (v2f{common::frand(), common::frand()}-v2f{.5,.5})*6*e.delta;
			}
			else if(hsr->tex==Assets::tex_water_can[1])hsr->tex=Assets::tex_water_can[0];

			if(iv.x>0) sr->tex = Assets::tex_mc_right;
			else if(iv.x<0) sr->tex = Assets::tex_mc_left;
			if(iv.y>0) sr->tex = Assets::tex_mc_down;
			else if(iv.y<0) sr->tex = Assets::tex_mc_up;

			sr->sourceRect->x = int(fid)%9*540;

			vec2<uint32_t> sp = Camera::getActiveCam()->getScreenPoint(parentObj->transform.pos+v2f{parentObj->transform.scl.x/2, -parentObj->transform.scl.y});
			v2f ssp = {sp.x/(float)shitrndr::WindowProps::getWidth(), sp.y/(float)shitrndr::WindowProps::getHeight()};

			Interactable* sel = check();
			if(sel)
			{
				UI::renderStaticText(ssp.x, ssp.y, sel->getAction() + "[SPACE]", {UI::CENTRED});
				UI::renderStaticText(ssp.x, ssp.y-.1, sel->getName(), {UI::CENTRED});
			}

			int w = shitrndr::WindowProps::getWidth();
			shitrndr::Copy(Assets::tex_power_meter, {0,0,(int)w/8, (int)w/16});
			shitrndr::Copy(Assets::tex_power_meter_fill, {0,0,(int)(power*w)/8, (int)w/16});
			shitrndr::Copy(Assets::tex_water_meter, {0,(int)w/16,(int)(w*.1), (int)(w*.075)});
			shitrndr::Copy(Assets::tex_water_meter_fill, {0,(int)(w/16+w*.07*(1-water)),(int)(w*.1), (int)(w*.075*water)});
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
								cd = .3;
								harvest((Plant*)sel);
								return;
							}
							else
								Audio::playSound(Assets::sfx_no_power);
						}

						float amt = e.type==ecs::Event::keyD?.075:.15;
						if(((Plant*)sel)->hydration<1 && instance->water >= amt-.01)
						{
							heldObj->getComponent<SpriteRen>()->tex=Assets::tex_water_can[1];
							Audio::playSound(Assets::sfx_water[std::rand()%2]);
							water-=amt;
							((Plant*)sel)->water(amt);
						}
					}
					else if(dynamic_cast<Well*>(sel))
					{
						heldObj->getComponent<SpriteRen>()->tex=Assets::tex_water_can[1];
						if(water<=.91)
						{
							Audio::playSound(Assets::sfx_refill_water);
							water+=.1;
						}
					}
					else if(dynamic_cast<Charger*>(sel))
					{
						heldObj->getComponent<SpriteRen>()->tex=Assets::tex_saw;
						Audio::playSound(Assets::sfx_charge);
						hasSaw = !hasSaw;
						charger = sel->parentObj;
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
								case 1: p = new Scorn(sel->parentObj); break;
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
							heldObj->getComponent<SpriteRen>()->tex=Assets::tex_saw;
							cd = amt*3;
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
			else if(e.key==SDLK_TAB)
			{
				Level::instance->selSeed++;
				Level::instance->selSeed%=2;
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
