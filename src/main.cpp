#include <lp3/core.hpp>
#include <lp3/gfx.hpp>
#include <lp3/input.hpp>
#include <lp3/sims.hpp>
#include <lp3/main.hpp>

#include "input/Controller.hpp"
#include "screens/Screens.hpp"

namespace core = lp3::core;
namespace gfx = lp3::gfx;
namespace input = lp3::input;
namespace sims = lp3::sims;
namespace sdl = lp3::sdl;


int _main(core::PlatformLoop & loop) {
	using namespace timtris;

    sdl::SDL2 sdl2(SDL_INIT_VIDEO);
    core::LogSystem log;

    core::MediaManager base_media;
    core::MediaManager media = base_media.sub_directory("Timtris");

	ControllerManager controls;

    gfx::Window window("Timtris", glm::vec2{ 640, 480 });
    glEnable(GL_DEPTH_TEST);

    std::unique_ptr<Screen> current_screen
        = screens::create_title_screen(media);

    const std::int64_t ms_per_update = 1000 / 60;  //16 ms for 60 fps
    sims::GameClock clock(ms_per_update);

    return loop.run([&]() {
        bool quit = false;
        SDL_Event e;
        if (SDL_PollEvent(&e)) {
            switch (e.type) {
            case SDL_QUIT:
                quit = true;
                break;
            case SDL_WINDOWEVENT:
                // resize our graphics when the window resizes
                window.handle_events(e.window);
                break;
            }
        }

        controls.update();

        clock.run_updates([&current_screen, &controls](std::int64_t ms) {
            Screen * s = current_screen->update(ms, controls);
            if (s) {
                current_screen.reset(s);
            }
        });

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Draw some graphics

        window.render([&current_screen](const glm::mat4 & previous) {
            current_screen->render(previous);
        });

        return !quit;
    });
}

LP3_MAIN(_main)
