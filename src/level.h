#pragma once
#include <bj/berryJam.h>

struct Level : public bj::Scene
{
	static Level* instance;

	std::map<uint8_t, uint8_t> seeds;
	uint8_t selType;

	Level(uint8_t w, uint8_t h, std::vector<std::pair<uint8_t, uint8_t>> seeds_);

	void onRenderFG(float d, float t) override;
};
