#include "Screens.hpp"

namespace gfx = lp3::gfx;

namespace timtris { namespace screens {

const glm::ivec2 res2d(640, 480);

class TitleScreen : public Screen {
public:
    TitleScreen(lp3::core::MediaManager & media)
    :   media(media),
        program{},
        bg_elements{ 4 },
        title{ IMG_Load_RW(media.load("timtrisTitle.png"), 0) }
    {
        gfx::upright_quad(
            bg_elements.add_quad(),
            glm::vec2{ 0.0f, 0.0f }, glm::vec2{ 640.0f, 480.0f }, 0.0f,
            glm::vec2{ 0.0f, 0.0f }, glm::vec2{ 1.0f, 1.0f });
    }

    void render(const glm::mat4 & previous) override {
        auto _2d = gfx::create_2d_screen(previous, res2d);

        program.use();
        program.set_mvp(_2d);

        program.set_texture(title.gl_id());
        program.draw(bg_elements);
    }

    gsl::owner<Screen *> update(std::int64_t ms,
                                ControllerManager & controls) override
    {
        if (controls.any_accept()) {
            return create_options_screen(media, controls).release();
        }
        return nullptr;
    }

private:
    lp3::core::MediaManager & media;
    gfx::programs::SimpleTextured program;
    gfx::ElementWriter<gfx::TexVert> bg_elements;
    gfx::Texture title;
};

std::unique_ptr<Screen> create_title_screen(lp3::core::MediaManager & media) {
    return std::make_unique<TitleScreen>(media);
}

}   }
