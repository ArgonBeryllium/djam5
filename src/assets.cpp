#include "definitions.h"

using namespace bj;

Mix_Chunk* Assets::sfx_money, *Assets::sfx_splash, *Assets::sfx_plant, *Assets::sfx_harvest, **Assets::sfx_water, *Assets::sfx_refill_water, *Assets::sfx_charge, *Assets::sfx_attack, *Assets::sfx_no_power, *Assets::sfx_hurt, *Assets::sfx_pop, *Assets::sfx_scorn_boom, *Assets::sfx_sparrot_bite, **Assets::sfx_steps, *Assets::sfx_static;
SDL_Texture* Assets::tex_tentato, *Assets::tex_sparrot, *Assets::tex_sparrot_walk, *Assets::tex_scorn, **Assets::tex_pop_scorn, *Assets::tex_mc_down, *Assets::tex_mc_right, *Assets::tex_mc_up, *Assets::tex_mc_left, *Assets::tex_saw, *Assets::tex_power_meter, **Assets::tex_water_can, *Assets::tex_water_meter, *Assets::tex_power_meter_fill, *Assets::tex_water_meter_fill, *Assets::tex_ground, *Assets::tex_well, *Assets::tex_charger, **Assets::tex_rubbish, *Assets::tex_radio;
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
	sfx_no_power = fileIO::loadSound("res/sfx/chainsaw not workey.wav");
	sfx_hurt = fileIO::loadSound("res/sfx/hurt.wav");
	sfx_pop = fileIO::loadSound("res/sfx/pulling out.wav");
	sfx_scorn_boom = fileIO::loadSound("res/sfx/scorn boom.wav");
	sfx_sparrot_bite = fileIO::loadSound("res/sfx/sparot slash.wav");
	sfx_steps = new Mix_Chunk*[4]{fileIO::loadSound("res/sfx/step1.wav"), fileIO::loadSound("res/sfx/step2.wav"), fileIO::loadSound("res/sfx/step3.wav"), fileIO::loadSound("res/sfx/step4.wav")};
	sfx_static = fileIO::loadSound("res/sfx/static.wav");

	tex_tentato = fileIO::loadImage("res/sprites/tentato.png");
	tex_sparrot = fileIO::loadImage("res/sprites/sparrot.png");
	tex_sparrot_walk = fileIO::loadImage("res/sprites/sparrot_walk.png");
	tex_scorn = fileIO::loadImage("res/sprites/scorn.png");
	tex_pop_scorn = new SDL_Texture*[4]{fileIO::loadImage("res/sprites/pop_scorn_1.png"), fileIO::loadImage("res/sprites/pop_scorn_2.png"), fileIO::loadImage("res/sprites/pop_scorn_3.png"), fileIO::loadImage("res/sprites/pop_scorn_4.png")};
	tex_mc_down = fileIO::loadImage("res/sprites/mc_walk_down.png");
	tex_mc_right = fileIO::loadImage("res/sprites/mc_walk_right.png");
	tex_mc_up = fileIO::loadImage("res/sprites/mc_walk_up.png");
	tex_mc_left = fileIO::loadImage("res/sprites/mc_walk_left.png");
	tex_saw = fileIO::loadImage("res/sprites/chainsaw.png");
	tex_water_can = new SDL_Texture*[2]{fileIO::loadImage("res/sprites/watering_can.png"), fileIO::loadImage("res/sprites/watering_can_1.png")};
	tex_power_meter = fileIO::loadImage("res/sprites/power_meter.png");
	tex_water_meter = fileIO::loadImage("res/sprites/water_meter.png");
	tex_power_meter_fill = fileIO::loadImage("res/sprites/power_meter_fill.png");
	tex_water_meter_fill = fileIO::loadImage("res/sprites/water_meter_fill.png");
	tex_ground = fileIO::loadImage("res/sprites/ground.png");
	tex_well = fileIO::loadImage("res/sprites/well.png");
	tex_charger = fileIO::loadImage("res/sprites/charger.png");
	tex_rubbish = new SDL_Texture*[2]{fileIO::loadImage("res/sprites/rubbish_1.png"), fileIO::loadImage("res/sprites/rubbish_2.png")};
	tex_radio = fileIO::loadImage("res/sprites/radio.png");
}
