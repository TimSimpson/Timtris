#include "Timtris.hpp"
#include <lp3/gfx.hpp>
#include "Controller.hpp"
#include "TimtrisGrid.hpp"

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

};

class GameScreen : public Screen {
public:
	GameScreen(lp3::core::MediaManager & media)
	:	media(media),
		bg_image{ IMG_Load_RW(media.load("bg.png"), 0) },
		scoreText{ IMG_Load_RW(media.load("scoreText.png"), 0) },
		timtris_tiles{ IMG_Load_RW(media.load("TimtrisTiles.png"), 0) },
		bg_elements{ 4 },
		text_elements{ (640 / 16) * (480 / 16) * 4 },
		options_layer{ glm::ivec2{ 640 / 16, 480 / 16 } },
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
		if (grid) {
			grid->render(program);
		}
	}

	void start_up(ControllerManager & controls) override {
		const glm::vec2 tile_resolution{ 16.0f, 16.0f };
		options_layer.set_quads(
			{ 0.0f, 0.0f }, 0.2f, { 16.0f, 16.0f }, options_quads,
			tile_resolution, scoreText.size());

		// Initialize grid.
		// TODO: Pass grid size in here from options instead of "10"
		grid = std::make_unique<TimtrisGrid>(media, 10, 17, 32, 32, 7);
		grid->Clear();

		// Initialize player score.
		// TODO: Pass in playerCount from Options
		//const int playerCount = 1;
		//for (int i = 0; i < playerCount; i++)
		//{
		//	playerScore[i] = new TimtrisScore(screen, scoreText);
		//	gameOverBox[i] = new GameOverBox(screen, scoreText);
		//	// Initialize player.
		//	players[i] = new TimtrisPlayer(i, playerScore[0], gameOverBox[0]);
		//	players[i]->SetGrid(grid);
		//	players[i]->SetControl(LpInputGetControl(i));
		//}
	}

	ScreenId update(std::int64_t ms, ControllerManager & controls) override {
		grid->Update(ms);
		return ScreenId::Game;
	}

private:
	lp3::core::MediaManager & media;
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
	:	screen()
	{
		screens.reset(new TitleScreen(media));
		// screens.push_back(std::make_unique<TitleScreen>(media));
		// screens.push_back(std::make_unique<OptionsScreen>(media));
		// screens.push_back(std::make_unique<GameScreen>(media));

		// current_screen = screens[0].get();
	}

	void operator()(const glm::mat4 & previous) {
		screen->render(previous);
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
	std::unique_ptr<Screen> screen;
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
