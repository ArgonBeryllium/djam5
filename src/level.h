#pragma once
#include <bj/berryJam.h>

struct Level : public bj::Scene
{
	static Level* instance;

	std::map<uint8_t, uint8_t> seeds;
	uint8_t selType, w, h, selSeed=0;
	float time;
	int minSum;
	bool lastChance;

	Level(uint8_t w_, uint8_t h_, std::vector<std::pair<uint8_t, uint8_t>> seeds_, float time_, int minSum, bool lc = 0);

	void onStart() override;
	void onLoad() override;
	void onRenderFG(float d, float t) override;
};

struct ResultScene : public bj::Scene
{
	float loadS = 0;
	static uint8_t level;
	void onStart() override;
	void onLoad() override;
	void onRenderFG(float d, float t) override;
};

struct DeathScene : public bj::Scene
{
	static std::string caption;
	void onRenderFG(float d, float t) override;
};
