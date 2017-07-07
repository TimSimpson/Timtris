#include "View.hpp"
#include <lp3/gfx.hpp>

namespace lp3 { namespace timtris {

namespace {
    const glm::ivec2 res2d(640, 480);
}

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
		elements{ 10 }
    {
    }

    void operator()(const glm::mat4 & previous) {
        auto _2d = gfx::create_2d_screen(previous, res2d);

        program.use();
        program.set_mvp(_2d);
        program.set_texture(title.gl_id());
        program.draw(elements);

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

    gfx::ElementWriter<gfx::TexVert> elements;

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
