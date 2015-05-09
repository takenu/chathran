/*
Copyright 2015, Matthijs van Dorp.

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#pragma once

#include <tiny/os/sdlapplication.h>

#include "appl.hpp"
#include "ui.hpp"
#include "render.hpp"
#include "terrain.hpp"
#include "veg.hpp"
#include "sky.hpp"

namespace ch
{
	namespace core
	{
		class Game
		{
			private:
				tiny::os::Application *application; /**< The engine's application class, for user input, OpenGL, OpenAL, SDL, etcetera. */

				ApplManager applManager; /**< Manage application specifics (key presses, low level rendering, sound, etcetera). */
				RenderManager renderManager; /**< Manage graphics rendering. */
				UIManager uiManager; /**< Manage user input and user interface. */
				TerrainManager terrainManager; /**< Manage terrain and heightmap. */
				VegManager vegManager; /**< Manage terrain and heightmap. */
				SkyManager skyManager; /**< Manage sky and weather. */
			public:
				Game(void) :
					applManager(),
					renderManager(static_cast<intf::ApplInterface*>(&applManager)),
					uiManager(static_cast<intf::ApplInterface*>(&applManager),static_cast<intf::RenderInterface*>(&renderManager)),
					terrainManager(static_cast<intf::RenderInterface*>(&renderManager)),
					vegManager(static_cast<intf::ApplInterface*>(&applManager),static_cast<intf::RenderInterface*>(&renderManager),static_cast<intf::TerrainInterface*>(&terrainManager)),
					skyManager(static_cast<intf::RenderInterface*>(&renderManager))
				{
					mainLoop();
				}

				~Game(void)
				{
				}

				void updateCamera(double dt)
				{
					tiny::vec3 cameraPosition = renderManager.getCameraPosition();
					tiny::vec4 cameraOrientation = renderManager.getCameraOrientation();
					applManager.updateSimpleCamera(dt, cameraPosition, cameraOrientation);						
					cameraPosition.y = std::max(cameraPosition.y, terrainManager.getHeight(cameraPosition));
					renderManager.setCameraPosition(cameraPosition);
					renderManager.setCameraOrientation(cameraOrientation);
				}

				void mainLoop(void)
				{
					while(applManager.isRunning())
					{
						double dt = applManager.pollEvents();
						updateCamera(dt);
						renderManager.update(dt);
						uiManager.update(dt);
						terrainManager.update(dt);
						vegManager.update(dt);
						skyManager.update(dt);
						applManager.paint();
					}
					std::cout << " Game : exiting main loop. "<<std::endl;
				}
		};
	}
}
