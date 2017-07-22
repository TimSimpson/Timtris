#include "Timtris.hpp"
#include <lp3/gfx.hpp>
#include "Controller.hpp"
#include "TimtrisGrid.hpp"
#include "TimtrisScore.hpp"

namespace lp3 { namespace timtris {

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

enum class ScreenId {
	Title,
	Options,
	Game,
	GameOver
};

class Screen {
public:
	virtual ~Screen() {}
	virtual ScreenId id() const = 0;
	virtual void render(gfx::programs::SimpleTextured & program) = 0;
	virtual void start_up(ControllerManager & controls) = 0;
	virtual ScreenId update(std::int64_t ms, ControllerManager & controls) = 0;
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

	ScreenId id() const override {
		return ScreenId::Title;
	}

	void render(gfx::programs::SimpleTextured & program) override {
		program.set_texture(title.gl_id());
		program.draw(bg_elements);
	}

	void start_up(ControllerManager & controls) override {
	}

	ScreenId update(std::int64_t ms, ControllerManager & controls) override {
		if (controls.any_accept()) {
			return ScreenId::Options;
		}
		return ScreenId::Title;
	}

private:
	gfx::ElementWriter<gfx::TexVert> bg_elements;
	gfx::Texture title;
};

class OptionsScreen : public Screen {
public:
	OptionsScreen(lp3::core::MediaManager & media)
	: optionScreenImage{ IMG_Load_RW(media.load("OptionScreen.png"), 0) },
		scoreText{ IMG_Load_RW(media.load("scoreText.png"), 0) },
		bg_elements{ 4 },
		text_elements{ (640 / 16) * (480 / 16) * 4 },
		options_layer{ { 16, 16 }, { 640 / 16, 480 / 16 } },
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
	}

	ScreenId id() const override {
		return ScreenId::Options;
	}

	void render(gfx::programs::SimpleTextured & program) override {	
		program.set_texture(optionScreenImage.gl_id());
		program.draw(bg_elements);
		program.set_texture(scoreText.gl_id());
		program.draw(text_elements);
	}

	void start_up(ControllerManager & controls) override {
		cursor.init(controls);
		playerCount = 1;
		optionsGridSize = 10;
		optionsCursor = 0;
	}

	ScreenId update(std::int64_t ms, ControllerManager & controls) override {
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
			return ScreenId::Game;
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
		}	// end giant switch statement

		const glm::vec2 tile_resolution{ 16.0f, 16.0f };
		options_layer.set_quads(
			{ 0.0f, 0.0f }, 0.2f, options_quads, 
			tile_resolution, scoreText.size());		
		return ScreenId::Options;
	}
private:
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

class GameScreen : public Screen {
public:
	GameScreen(lp3::core::MediaManager & media)
	:	bg_image{ IMG_Load_RW(media.load("bg.png"), 0) },
		scoreText{ IMG_Load_RW(media.load("scoreText.png"), 0) },
		timtris_tiles{ IMG_Load_RW(media.load("TimtrisTiles.png"), 0) },
		bg_elements{ 4 },
		text_elements{ (640 / 16) * (480 / 16) * 4 },
		options_layer{ { 16, 16 },{ 640 / 16, 480 / 16 } },
		options_quads{ options_layer.create_quads(text_elements) },
		grid(nullptr)
	{
		gfx::upright_quad(
			bg_elements.add_quad(),
			glm::vec2{ 0.0f, 0.0f }, glm::vec2{ 640.0f, 480.0f }, 0.5f,
			glm::vec2{ 0.0f, 0.0f }, glm::vec2{ 1.0f, 1.0f });
	}

	ScreenId id() const override {
		return ScreenId::Game;
	}

	void render(gfx::programs::SimpleTextured & program) override {
		program.set_texture(bg_image.gl_id());
		program.draw(bg_elements);
		///*program.set_texture(scoreText.gl_id());
		//progra*/m.draw(text_elements);
	}

	void start_up(ControllerManager & controls) override {
		const glm::vec2 tile_resolution{ 16.0f, 16.0f };
		options_layer.set_quads(
			{ 0.0f, 0.0f }, 0.2f, options_quads,
			tile_resolution, scoreText.size());

		// Initialize grid.
		// TODO: Pass grid size in here from options instead of "10"
		grid = std::make_unique<TimtrisGrid>(10, 17, 32, 32, 7);
		grid->Clear();

		// Initialize player score.
		// TODO: Pass in playerCount from Options
		const int playerCount = 1;
		for (int i = 0; i < playerCount; i++)
		{
			playerScore[i] = new TimtrisScore(screen, scoreText);
			gameOverBox[i] = new GameOverBox(screen, scoreText);
			// Initialize player.
			players[i] = new TimtrisPlayer(i, playerScore[0], gameOverBox[0]);
			players[i]->SetGrid(grid);
			players[i]->SetControl(LpInputGetControl(i));
		}
	}

	ScreenId update(std::int64_t ms, ControllerManager & controls) override {
		return ScreenId::Game;
	}

private:
	gfx::Texture bg_image;
	gfx::Texture scoreText;
	gfx::Texture timtris_tiles;
	gfx::ElementWriter<gfx::TexVert> bg_elements;
	gfx::ElementWriter<gfx::TexVert> text_elements;
	gfx::TileMap options_layer;
	gfx::QuadArray<gfx::TexVert> options_quads;

	std::unique_ptr<TimtrisGrid> grid;
};

class Game::Impl {
public:
	Impl(lp3::core::MediaManager & media)
	:	program{},		
		screens{},
		current_screen(nullptr)
	{
		screens.push_back(std::make_unique<TitleScreen>(media));
		screens.push_back(std::make_unique<OptionsScreen>(media));
		screens.push_back(std::make_unique<GameScreen>(media));

		current_screen = screens[0].get();
	}

	void operator()(const glm::mat4 & previous) {
		auto _2d = gfx::create_2d_screen(previous, res2d);

		program.use();
		program.set_mvp(_2d);
		current_screen->render(program);
	}

	void update(std::int64_t ms, ControllerManager & controls) {
		const ScreenId id = current_screen->update(ms, controls);
		if (id != current_screen->id()) {
			for (std::unique_ptr<Screen> & s : screens) {
				if (id == s->id()) {
					current_screen = s.get();
					current_screen->start_up(controls);
					break;
				}
			}
		}
	}

private:
	gfx::programs::SimpleTextured program;
	std::vector<std::unique_ptr<Screen>> screens;
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

void Game::update(std::int64_t ms, ControllerManager & controls) {
	this->impl->update(ms, controls);
}

}   }
