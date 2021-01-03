#include "definitions.h"

using namespace bj;

Mix_Chunk* Assets::sfx_money, *Assets::sfx_splash, *Assets::sfx_plant, *Assets::sfx_harvest, **Assets::sfx_water, *Assets::sfx_refill_water, *Assets::sfx_charge, *Assets::sfx_attack;
SDL_Texture* Assets::tex_tentato, *Assets::tex_sparrot, *Assets::tex_sparrot_walk;
void Assets::init()
{
	sfx_money = fileIO::loadSound("res/sfx/money.wav");
	sfx_splash = fileIO::loadSound("res/sfx/arbe startup.wav");
	sfx_plant = fileIO::loadSound("res/sfx/planting.wav");
	sfx_harvest = fileIO::loadSound("res/sfx/chainsaw short.wav");
	sfx_water = new Mix_Chunk*[2]{fileIO::loadSound("res/sfx/watering1.wav"), fileIO::loadSound("res/sfx/watering2.wav")};
	sfx_refill_water = fileIO::loadSound("res/sfx/restoring water.wav");
	sfx_charge = fileIO::loadSound("res/sfx/chainsaw charge.wav");
	sfx_attack = fileIO::loadSound("res/sfx/chainsaw long.wav");

	tex_tentato = fileIO::loadImage("res/sprites/tentato.png");
	tex_sparrot = fileIO::loadImage("res/sprites/sparrot.png");
	tex_sparrot_walk = fileIO::loadImage("res/sprites/sparrot_walk.png");
}
