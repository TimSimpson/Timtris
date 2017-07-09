#include "Timtris.hpp"
#include <lp3/gfx.hpp>
#include "Controller.hpp"

namespace lp3 { namespace timtris {

namespace {
    const glm::ivec2 res2d(640, 480);
}

class Screen {
public:
	virtual ~Screen() {}
	virtual void render(gfx::programs::SimpleTextured & program) = 0;
	virtual void update(ControllerManager & controls) = 0;
};

class TitleScreen : public Screen {
public:
	TitleScreen(lp3::core::MediaManager & media)
	:	bg_elements{ 4 },
		title{ IMG_Load_RW(media.load("timtrisTitle.png"), 0) }
	{
		gfx::upright_quad(
			bg_elements.add_quad(),
			glm::vec2{ 0.0f, 0.0f }, glm::vec2{ 640.0f, 480.0f }, 0.0f,
			glm::vec2{ 0.0f, 0.0f }, glm::vec2{ 1.0f, 1.0f });
	}

	void render(gfx::programs::SimpleTextured & program) override {
		program.set_texture(title.gl_id());
		program.draw(bg_elements);
	}

	void update(ControllerManager & controls) override {
		if (controls.any_accept()) {
			// TODO: Go on
		}
	}

private:
	gfx::ElementWriter<gfx::TexVert> bg_elements;
	gfx::Texture title;
};

class OptionsScreen : public Screen {
public:
	OptionsScreen(lp3::core::MediaManager & media)
	:	optionScreenImage{ IMG_Load_RW(media.load("OptionScreen.png"), 0) },
		scoreText{ IMG_Load_RW(media.load("scoreText.png"), 0) },
		bg_elements{ 4 },
		text_elements{ (640 / 16) * (480 / 16) * 4 },
		options_layer{ { 16, 16 },{ 640 / 16, 480 / 16 } },
		options_quads{ options_layer.create_quads(text_elements) }
	{
		gfx::upright_quad(
			bg_elements.add_quad(),
			glm::vec2{0.0f, 0.0f}, glm::vec2{640.0f, 480.0f}, 0.0f,
			glm::vec2{0.0f, 0.0f}, glm::vec2{1.0f, 1.0f});	
	}

	void render(gfx::programs::SimpleTextured & program) override {	
		program.set_texture(optionScreenImage.gl_id());
		program.draw(bg_elements);
		program.set_texture(scoreText.gl_id());
		program.draw(text_elements);
	}

	void update(ControllerManager & controls) override {
		const glm::vec2 tile_resolution{ 16.0f, 16.0f };
		options_layer.set_quads(
			{ 0.0f, 0.0f }, 0.0f, options_quads, 
			tile_resolution, scoreText.size());
	}
private:
	gfx::Texture optionScreenImage;
	gfx::Texture scoreText;
	gfx::ElementWriter<gfx::TexVert> bg_elements;
	gfx::ElementWriter<gfx::TexVert> text_elements;
	gfx::TileMap options_layer;
	gfx::QuadArray<gfx::TexVert> options_quads;
};

class Game::Impl {
public:
	Impl(lp3::core::MediaManager & media)
	:	program{},
		title(media),
		option(media),
		current_screen(&title)
	{
	}

	void operator()(const glm::mat4 & previous) {
		auto _2d = gfx::create_2d_screen(previous, res2d);

		program.use();
		program.set_mvp(_2d);
		current_screen->render(program);
	}

	void update(ControllerManager & controls) {
		current_screen->update(controls);
	}

private:
	gfx::programs::SimpleTextured program;
	TitleScreen title;
	OptionsScreen option;
	Screen * current_screen;
};
//class Game::Impl {
//public:
//    Impl(lp3::core::MediaManager & media)
//    :   program{},
//        gameOver{ IMG_Load_RW(media.load("timtrisGameOver.png"), 0)},
//        gameBg{ IMG_Load_RW(media.load("timtris.png"), 0)},
//        gridTiles{ IMG_Load_RW(media.load("timtrisTiles.png"), 0)},
//        backgroundImage{ IMG_Load_RW(media.load("bg.png"), 0)},
//        titleScreenImage{ IMG_Load_RW(media.load("timtrisTitle.png"), 0)},
//        optionScreenImage{ IMG_Load_RW(media.load("OptionScreen.png"), 0)},
//        scoreText{ IMG_Load_RW(media.load("scoreText.png"), 0)},
//		bg_elements{ 4 },
//        text_elements{ (640 / 16) * (480 / 16) * 4 } 
//    {
//		gfx::Quad<gfx::TexVert> backgroundSprite = bg_elements.add_quad();
//
//		gfx::upright_quad(
//			backgroundSprite,
//			glm::vec2{ 0.0f, 0.0f }, glm::vec2{ 640.0f, 480.0f }, 0.0f,
//			glm::vec2{ 0, 0 }, glm::vec2{ 1.0f, 1.0f });
//
//
//
//    }
//
//    void operator()(const glm::mat4 & previous) {
//        auto _2d = gfx::create_2d_screen(previous, res2d);
//
//        program.use();
//        program.set_mvp(_2d);
//        program.set_texture(titleScreenImage.gl_id());
//        program.draw(bg_elements);
//    }
//
//private:
//
//    gfx::programs::SimpleTextured program;
//
//  
//    gfx::Texture gameOver;
//    gfx::Texture gameBg;
//    gfx::Texture gridTiles;
//    gfx::Texture backgroundImage;
//    gfx::Texture titleScreenImage;
//    gfx::Texture optionScreenImage;
//    gfx::Texture scoreText;
//
//	gfx::ElementWriter<gfx::TexVert> bg_elements;
//
//    gfx::ElementWriter<gfx::TexVert> text_elements;
//
//
//};


// Public View methods:

Game::Game(lp3::core::MediaManager & media)
:   impl(new Game::Impl(media)) {
}

Game::~Game() {
	delete impl;
}

void Game::operator()(const glm::mat4 & previous) {
    this->impl->operator()(previous);
}

void Game::update(ControllerManager & controls) {
	this->impl->update(controls);
}

}   }
