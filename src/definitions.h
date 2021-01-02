#pragma once
#include <bj/berryJam.h>

struct Interactable;
struct Plant;
struct Player : public bj::ecs::Com
{
	static Player* instance;
	bj::v2f dir;
	float water = 1, power = 1;
	int money = 0;

	Player(bj::GameObj* po) : bj::ecs::Com(bj::BehSys::getInstance(), po) { onStart(); }

	Interactable* check();
	static void harvest(Plant* plant);
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

struct Plant : public Interactable
{
	float hydration = 1, ripeness = 0;
	float deh_rate = .1, rip_rate = .05;
	static constexpr float HARVEST_T = .8;
	Plant(bj::GameObj* po) : Interactable(po) {}

	std::string getAction() override;
	std::string getName() override;
	virtual int getWorth();
	void water(const float& amt);
	static void kill(Plant* plant);

	void onStart() override;
	void onEvent(const bj::ecs::Event& e) override;
};

struct Well : public Interactable
{
	Well(bj::GameObj* po) : Interactable(po) {}
	std::string getAction() override;
	std::string getName() override;
};
struct Ground : public Interactable
{
	Ground(bj::GameObj* po) : Interactable(po) {}
	std::string getAction() override;
	std::string getName() override;
};

struct Assets
{
	static Mix_Chunk* sfx_money;
	static void init();
};
