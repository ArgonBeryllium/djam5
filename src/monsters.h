#include "definitions.h"

struct Sparrot : public Plant
{
	Sparrot(bj::GameObj* po) : Plant(po)
	{
		deh_rate = .02;
		rip_rate = .02;
	}
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
	SparrotMon(bj::GameObj* po) : Monster(po) {}
	void onStart() override;
	void onEvent(const bj::ecs::Event& e) override;
	std::string getName() override;
	int getWorth() override;
	void takeDamage(float amt) override;
};
