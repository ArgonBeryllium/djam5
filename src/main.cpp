#include <bj/berryJam.h>
#include <bj/shitrndr.h>
using namespace bj;

struct GameScene : public Scene
{
	void onStart() override
	{
		UI::loadFont("res/ProggyTiny.ttf", .05);
		UI::loadFont("res/Ludum-Dairy-0.2.0.ttf", .07);
		UI::loadFont("res/KingthingsExtortion-YMAa.ttf", .075);
		shitrndr::bg_col = {2,2,22,255};
		std::cout << shitrndr::WindowProps::getWidth() << '\n';
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
	void onRenderBG(float d, float t) override
	{
		SDL_SetRenderDrawColor(shitrndr::ren, shitrndr::bg_col.r, shitrndr::bg_col.g, shitrndr::bg_col.b, shitrndr::bg_col.a);
		shitrndr::FillRect(shitrndr::WindowProps::getSizeRect());
	}
};

int BJ_MULP_MAIN()
{
	initParams ip;
	ip.nscenes = 1;
	ip.scenes = new Scene*[1]{new GameScene()};

	Game::init(ip);
	shitrndr::WindowProps::setPixScale(3);
	Game::start();
}
