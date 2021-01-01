#include <bj/berryJam.h>
#include <bj/shitrndr.h>
using namespace bj;

struct SplashScene : public Scene
{
	// ooga booga me big man
	void onStart() override
	{
		UI::loadFont("res/ProggyTiny.ttf", .05);
		UI::loadFont("res/Ludum-Dairy-0.2.0.ttf", .07);
		UI::loadFont("res/KingthingsExtortion-YMAa.ttf", .075);
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
		if(t<M_PI) SDL_SetRenderDrawColor(shitrndr::ren, 3, 0, 4, (Uint8)((1-std::sin(t))*255));
		UI::renderStaticText(.5, .8, "a game by ArBe", {UI::CENTRED});
		shitrndr::FillRect(shitrndr::WindowProps::getSizeRect());

		if(t>4.5) SceneManager::setActiveScene(1);
	}
};
struct GameScene : public Scene
{
	void onStart() override
	{
		cam.pos = {.5,.5};
		instantiate()->addComponent(new BasicRen(getObj(0)));
		getObj(0)->addComponent(new Behaviour(getObj(0), [](Behaviour* self, const ecs::Event& e)
					{
						if(e.type!=ecs::Event::frame) return;
						self->parentObj->transform.pos.y = std::sin(e.elapsed);
						UI::renderStaticText(.5, .5, "pain time", {UI::CENTRED});
						UI::renderStaticText(.498 - std::abs(std::sin(e.elapsed*2))*.005, .097, "DJAM5", {UI::CENTRED, 1, {10, 30, 255, 255}});
						UI::renderStaticText(.499 - std::abs(std::sin(e.elapsed*2+.1))*.005, .099, "DJAM5", {UI::CENTRED, 1, {255, 80, 0, 255}});
						UI::renderStaticText(.5, .1, "DJAM5", {UI::CENTRED, 1});
						UI::renderStaticText(.5, .7, "POG", {UI::CENTRED, 2});
					}));
	}
	void onLoad() override { shitrndr::bg_col = {2,2,22,255}; }
};

int BJ_MULP_MAIN()
{
	initParams ip;
	ip.nscenes = 2;
	ip.scenes = new Scene*[2]{new SplashScene(), new GameScene()};

	Game::init(ip);
	shitrndr::WindowProps::setPixScale(3);
	SDL_SetRenderDrawBlendMode(shitrndr::ren, SDL_BLENDMODE_BLEND);
	Game::start();
}
