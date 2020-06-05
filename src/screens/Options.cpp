#include "Screen.hpp"

namespace gfx = lp3::gfx;

namespace timtris { namespace screens {

namespace {
    const glm::ivec2 res2d(640, 480);

    const lp3::gfx::BoxTiles blue_box_tiles = {
        96 + 9,
        96 + 5,
        96 + 4,
        96,
        96,
        96 + 2,
        96 + 5,
        96 + 7
    };

    const lp3::gfx::BoxTiles blank_tiles = {
        0, 0, 0, 0, 0, 0, 0, 0
    };
}

class OptionsScreen : public Screen {
public:
    OptionsScreen(lp3::core::MediaManager & media,
		          ControllerManager & controls)
    :   media(media),
        program{},
        optionScreenImage{ IMG_Load_RW(media.load("OptionScreen.png"), 0) },
        scoreText{ IMG_Load_RW(media.load("scoreText.png"), 0) },
        bg_elements{ 4 },
        text_elements{ (640 / 16) * (480 / 16) * 4 },
        options_layer(glm::ivec2{ 640 / 16, 480 / 16 }),
        options_quads{ options_layer.create_quads(text_elements) },
        optionsCursor{ 0 },
        optionsGridSize{ 0 },
        cursor{},
        playerCount{}
    {
        gfx::upright_quad(
            bg_elements.add_quad(),
            glm::vec2{0.0f, 0.0f}, glm::vec2{640.0f, 480.0f}, 0.5f,
            glm::vec2{0.0f, 0.0f}, glm::vec2{1.0f, 1.0f});



        // Create border around stuff.
        options_layer.write_box(blue_box_tiles, { 0, 0 }, { 40, 30 });


        options_layer.write({ 16, 3 }, "OPTIONS");
        options_layer.write({ 3, 5 }, "Players");
        options_layer.write({ 3, 7 }, " 1   2   3   4");
        options_layer.write({ 3, 9 }, "Difficulty");

        options_layer.write({ 3, 11 }, " Easy   Normal   Hard");
        options_layer.write({ 3, 13 }, "Play Field");
        options_layer.write({ 3, 15 }, " Standard Fat");

        cursor.init(controls);
        playerCount = 1;
        optionsGridSize = 10;
        optionsCursor = 0;
    }

    void render(const glm::mat4 & previous) override {
        auto _2d = gfx::create_2d_screen(previous, res2d);

        program.use();
        program.set_mvp(_2d);

        program.set_texture(optionScreenImage.gl_id());
        program.draw(bg_elements);
        program.set_texture(scoreText.gl_id());
        program.draw(text_elements);
    }

    gsl::owner<Screen *> update(std::int64_t ms, 
		                        ControllerManager & controls) override {
        cursor.update(controls, ms);

        if (cursor.down()) {
            ++ optionsCursor;
            if (optionsCursor > 2) {
                optionsCursor = 2;
            }
        }
        if (cursor.up()) {
            -- optionsCursor;
            if (optionsCursor < 0) {
                optionsCursor = 0;
            }
        }
        if (cursor.accept()) {
			return nullptr;  // TODO
        }

        switch (optionsCursor)
        {
            case 0: {
                // Players
                options_layer.write_box(blank_tiles, { 3, 6 }, { 3, 3 });
                options_layer.write_box(blank_tiles, { 7, 6 }, { 3, 3 });
                options_layer.write_box(blank_tiles, { 11, 6 }, { 3, 3 });
                options_layer.write_box(blank_tiles, { 15, 6 }, { 3, 3 });
                if (cursor.left() && playerCount > 1) {
                    playerCount--;
                }
                if (cursor.right() && playerCount < 4) {
                    playerCount++;
                }
                if (playerCount == 1) {
                    options_layer.write_box(blue_box_tiles, { 3, 6 }, { 3, 3 });
                }
                if (playerCount == 2) {
                    options_layer.write_box(blue_box_tiles, { 7, 6 }, { 3, 3 });
                }
                if (playerCount == 3) {
                    options_layer.write_box(blue_box_tiles, { 11, 6 }, { 3, 3 });
                }
                if (playerCount == 4) {
                    options_layer.write_box(blue_box_tiles, { 15, 6 }, { 3, 3 });
                }
                break;
            }
            case 1:
                // TODO: difficulty selection
                break;
            case 2:
            {   // GRID SIZE
                if (cursor.left()) {
                    if (optionsGridSize == 20) {
                        optionsGridSize = 10;
                    }
                }
                if (cursor.right()) {
                    if (optionsGridSize == 10) {
                        optionsGridSize = 20;
                    }
                }
                options_layer.write_box(blank_tiles, { 2, 14 }, { 11, 3 });
                options_layer.write_box(blank_tiles, { 12, 14 }, { 5, 3 });
                if (optionsGridSize == 10) {
                    options_layer.write_box(blue_box_tiles, { 2, 14 }, { 11, 3 });
                }
                if (optionsGridSize == 20) {
                    options_layer.write_box(blue_box_tiles, { 12, 14 }, { 5, 3 });
                }
                break;
            }
            case 3:
                LP3_ASSERT(false);  // was in old code, do not understand
                break;
            default:
                LP3_ASSERT(false);
        }   // end giant switch statement

        const glm::vec2 tile_resolution{ 16.0f, 16.0f };
        options_layer.set_quads(
            { 0.0f, 0.0f }, 0.2f, { 16.0f, 16.0f }, options_quads,
            tile_resolution, scoreText.size());
        return nullptr;
    }
private:
    lp3::core::MediaManager & media;
    gfx::programs::SimpleTextured program;
    gfx::Texture optionScreenImage;
    gfx::Texture scoreText;
    gfx::ElementWriter<gfx::TexVert> bg_elements;
    gfx::ElementWriter<gfx::TexVert> text_elements;
    gfx::TileMap options_layer;
    gfx::QuadArray<gfx::TexVert> options_quads;

    int optionsCursor;
    int optionsGridSize;
    CursorPad cursor;
    int playerCount;
};

std::unique_ptr<Screen> create_options_screen(lp3::core::MediaManager & media,
	                                          ControllerManager & controls) {
    return std::make_unique<OptionsScreen>(media, controls);
}


}   }
