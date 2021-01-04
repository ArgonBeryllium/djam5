#pragma once
#include <bj/berryJam.h>
#include "definitions.h"
using namespace bj;

struct SplashScene : public Scene
{
	bool played = 0;
	void onStart() override
	{
		instantiate()->addComponent(new SpriteRen(getObj(0), "res/logo.png"));
		getObj(0)->transform.scl.y = 0;
		cam.pos = {.5,.5};
		cam.scale = 2;
	}
	void onLoad() override { shitrndr::bg_col = {3,0,4,255}; }
	void onRenderFG(float d, float t) override
	{
		constexpr float start = 1, speed = 4.5;
		if(t>start && t-start<M_PI/(speed*1.8))
		{
			getObj(0)->transform.scl.y = std::sin((t-start)*speed);
			getObj(0)->transform.pos.y = .5-getObj(0)->transform.scl.y/2;
		}
		if(t>start && !played)
		{
			Audio::playSound(Assets::sfx_splash);
			played = 1;
		}
		if(t<M_PI) SDL_SetRenderDrawColor(shitrndr::ren, 3, 0, 4, (Uint8)((1-std::sin(t))*255));
		UI::renderStaticText(.5, .8, "a \"game\" by ArBe", {UI::CENTRED});
		shitrndr::FillRect(shitrndr::WindowProps::getSizeRect());

		if(t>4.5) SceneManager::setActiveScene(1);
		//SceneManager::setActiveScene(1);
	}
};
