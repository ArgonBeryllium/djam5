#pragma once
#include <bj/berryJam.h>

struct Assets
{
	static Mix_Chunk* sfx_money, *sfx_splash,  *sfx_plant, *sfx_harvest, **sfx_water, *sfx_refill_water, *sfx_charge, *sfx_attack, *sfx_no_power;
	static SDL_Texture* tex_tentato, *tex_sparrot, *tex_sparrot_walk, *tex_scorn, **tex_pop_scorn, *tex_mc_down, *tex_mc_right, *tex_mc_up, *tex_mc_left, *tex_saw, *tex_power_meter, *tex_power_meter_fill, **tex_water_can, *tex_water_meter, *tex_water_meter_fill, *tex_ground, *tex_well, *tex_charger, **tex_rubbish;
	static void init();
};

struct Interactable;
struct Plant;
struct Player : public bj::ecs::Com
{
	static Player* instance;
	bj::GameObj* heldObj, *charger;
	bj::v2f dir;
	float water = 1, power = 1, life = 1, cd = 0;
	int money = 0;
	bool hasSaw = 1;

	Player(bj::GameObj* po) : bj::ecs::Com(bj::BehSys::getInstance(), po) { onStart(); }

	Interactable* check();
	static void harvest(Plant* plant);
	static void hurt(float dmg);
	static void die(const std::string& caption);
	void onStart() override;
	void onEvent(const bj::ecs::Event& e) override;
};

struct Interactable : public bj::ecs::Com
{
	virtual std::string getAction();
	virtual std::string getName();

	Interactable(bj::GameObj* po) : bj::ecs::Com(bj::BehSys::getInstance(), po) {onStart();}

	void onStart() override;
	void onEvent(const bj::ecs::Event& e) override;
};

struct Monster : public Interactable
{
	float life = 1;
	Monster(bj::GameObj* po) : Interactable(po) { onStart(); }
	virtual int getWorth() = 0;
	virtual void takeDamage(float amt);
	std::string getAction() override;
	std::string getName() override;
};
struct Plant : public Interactable
{
	bj::SpriteRen* sr;
	float hydration = 1, ripeness = 0;
	float deh_rate = .1, rip_rate = .05;
	static constexpr float HARVEST_T = .8;
	Plant(bj::GameObj* po) : Interactable(po) {}

	std::string getAction() override;
	std::string getName() override;
	virtual int getWorth();
	virtual float getPower();
	virtual Monster* getMut(bj::GameObj* obj) = 0;
	void water(const float& amt);
	static void kill(Plant* plant);

	void onStart() override;
	void onEvent(const bj::ecs::Event& e) override;
};


struct Well : public Interactable
{
	Well(bj::GameObj* po) : Interactable(po)
	{
		parentObj->addComponent(new bj::SpriteRen(parentObj, Assets::tex_well));
		parentObj->getComponent<bj::SpriteRen>()->offset.scl = bj::v2f{1,1}*1.5;
	}
	std::string getAction() override;
	std::string getName() override;
};
struct Charger : public Interactable
{
	Charger(bj::GameObj* po) : Interactable(po)
	{
		parentObj->addComponent(new bj::SpriteRen(parentObj, Assets::tex_charger));
		parentObj->getComponent<bj::SpriteRen>()->offset.scl = bj::v2f{1,1}*1.5;
	}
	std::string getAction() override;
	std::string getName() override;
	void onEvent(const bj::ecs::Event& e) override;
};
struct Ground : public Interactable
{
	Ground(bj::GameObj* po) : Interactable(po) { parentObj->addComponent(new bj::SpriteRen(parentObj, Assets::tex_ground)); parentObj->transform.scl = bj::v2f{1,1}*1.3; }
	std::string getAction() override;
	std::string getName() override;
};
