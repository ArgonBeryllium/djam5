#pragma once
#include <bj/berryJam.h>

struct Level : public bj::Scene
{
	static Level* instance;

	std::map<uint8_t, uint8_t> seeds;
	uint8_t selType, w, h;

	Level(uint8_t w_, uint8_t h_, std::vector<std::pair<uint8_t, uint8_t>> seeds_);
	
	void onStart() override;
	void onRenderFG(float d, float t) override;
};
