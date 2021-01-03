#include "definitions.h"

struct Sparrot : public Plant
{
	Sparrot(bj::GameObj* po) : Plant(po)
	{
		deh_rate = .02;
		rip_rate = .02;
		onStart();
	}
	~Sparrot() { parentObj->removeComponent(sr); }
	float getPower() override;
	Monster* getMut(bj::GameObj* obj) override;
	std::string getName() override;
	int getWorth() override;

	void onStart() override;
	void onEvent(const bj::ecs::Event& e) override;
};
struct SparrotMon : public Monster
{
	bj::v2f dir;
	float jt = 0, cd = 1;
	bool hit = 0;
	SparrotMon(bj::GameObj* po) : Monster(po) { onStart(); }
	void onStart() override;
	void onEvent(const bj::ecs::Event& e) override;
	std::string getName() override;
	int getWorth() override;
	void takeDamage(float amt) override;
};

struct Scorn : public Plant
{
	Scorn(bj::GameObj* po) : Plant(po)
	{
		deh_rate = .03;
		rip_rate = .015;
		onStart();
	}
	~Scorn() { parentObj->removeComponent(sr); }
	float getPower() override;
	Monster* getMut(bj::GameObj* obj) override;
	std::string getName() override;
	int getWorth() override;

	void onStart() override;
	void onEvent(const bj::ecs::Event& e) override;
};
struct ScornMon : public Monster
{
	float cd = 1.5;
	std::vector<bj::GameObj*> pops;
	ScornMon(bj::GameObj* po) : Monster(po) { onStart(); }
	void onStart() override;
	void onEvent(const bj::ecs::Event& e) override;
	std::string getName() override;
	int getWorth() override;
	void takeDamage(float amt) override;
};


