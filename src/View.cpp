#include "View.hpp"
#include <lp3/gfx.hpp>

namespace lp3 { namespace timtris {

namespace {
    const glm::ivec2 res2d(640, 480);
}

class OptionsView {
public:
	OptionsView(lp3::core::MediaManager & media, gfx::Texture & scoreText)
	:	optionScreenImage{ IMG_Load_RW(media.load("OptionScreen.png"), 0) },
		scoreText(scoreText),
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

	void render(gfx::programs::SimpleTextured & program) {
		program.set_texture(optionScreenImage.gl_id());
		program.draw(bg_elements);
		program.set_texture(scoreText.gl_id());
		program.draw(text_elements);
	}

	void update() {
		const glm::vec2 tile_resolution{ 16.0f, 16.0f };
		options_layer.set_quads(
			{ 0.0f, 0.0f }, 0.0f, options_quads,
			tile_resolution, scoreText.size());
	}
private:
	gfx::Texture optionScreenImage;
	gfx::Texture & scoreText;
	gfx::ElementWriter<gfx::TexVert> bg_elements;
	gfx::ElementWriter<gfx::TexVert> text_elements;
	gfx::TileMap options_layer;
	gfx::QuadArray<gfx::TexVert> options_quads;
};
class View::Impl {
public:
    Impl(lp3::core::MediaManager & media)
    :   program{},
        title{ IMG_Load_RW(media.load("timtrisTitle.png"), 0)},
        gameOver{ IMG_Load_RW(media.load("timtrisGameOver.png"), 0)},
        gameBg{ IMG_Load_RW(media.load("timtris.png"), 0)},
        gridTiles{ IMG_Load_RW(media.load("timtrisTiles.png"), 0)},
        backgroundImage{ IMG_Load_RW(media.load("bg.png"), 0)},
        titleScreenImage{ IMG_Load_RW(media.load("timtrisTitle.png"), 0)},
        optionScreenImage{ IMG_Load_RW(media.load("OptionScreen.png"), 0)},
        scoreText{ IMG_Load_RW(media.load("scoreText.png"), 0)},
		bg_elements{ 4 },
        text_elements{ (640 / 16) * (480 / 16) * 4 }
    {
		gfx::Quad<gfx::TexVert> backgroundSprite = bg_elements.add_quad();

		gfx::upright_quad(
			backgroundSprite,
			glm::vec2{ 0.0f, 0.0f }, glm::vec2{ 640.0f, 480.0f }, 0.0f,
			glm::vec2{ 0, 0 }, glm::vec2{ 1.0f, 1.0f });



    }

    void operator()(const glm::mat4 & previous) {
        auto _2d = gfx::create_2d_screen(previous, res2d);

        program.use();
        program.set_mvp(_2d);
        program.set_texture(titleScreenImage.gl_id());
        program.draw(bg_elements);
    }

private:

    gfx::programs::SimpleTextured program;

    gfx::Texture title;
    gfx::Texture gameOver;
    gfx::Texture gameBg;
    gfx::Texture gridTiles;
    gfx::Texture backgroundImage;
    gfx::Texture titleScreenImage;
    gfx::Texture optionScreenImage;
    gfx::Texture scoreText;

	gfx::ElementWriter<gfx::TexVert> bg_elements;

    gfx::ElementWriter<gfx::TexVert> text_elements;


};


// Public View methods:

View::View(lp3::core::MediaManager & media)
:   impl(new View::Impl(media)) {
}

View::~View() {
	delete impl;
}

void View::operator()(const glm::mat4 & previous) {
    this->impl->operator()(previous);
}

}   }
