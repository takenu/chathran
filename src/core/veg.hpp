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

#include <tiny/draw/texture2d.h>
#include <tiny/draw/staticmesh.h>
#include <tiny/draw/staticmeshhorde.h>
#include <tiny/draw/iconhorde.h>
#include <tiny/draw/tiledhorde.h>

#include "interface/appl.hpp"
#include "interface/render.hpp"
#include "interface/terrain.hpp"

#include "../veg/drawtiled.hpp"

namespace sel { class State; }

namespace ch
{
	namespace core
	{
		/** Take care of management of vegetation objects. */
		class VegManager
		{
			private:
				intf::ApplInterface * applInterface;
				intf::RenderInterface * renderer;
				intf::TerrainInterface * terrain;

				/** A map to contain all VegetationTiledHordes, organised by unique name. */
				std::map<std::string, veg::VegetationTiledHorde*> hordemap;
			public:
				VegManager(intf::ApplInterface * _applInterface, intf::RenderInterface * _renderer, intf::TerrainInterface * _terrain) :
					applInterface(_applInterface),
					renderer(_renderer),
					terrain(_terrain)
				{
				}

				~VegManager(void)
				{
				}

				/** Add a LOD for a type of vegetation. Use 'double' for tileSize/maxRange because Lua likes that. */
				void addLOD(std::string name, float tileSize, std::string meshLoc, std::string difTexLoc, bool alphaChannel, std::string normTexLoc, int maxObjects, float maxRange);
				/** Add all LODs to their TiledHordes. */
				void addToTiledHorde(void) { for(std::map<std::string, veg::VegetationTiledHorde*>::iterator it = hordemap.begin(); it != hordemap.end(); it++) it->second->addToTiledHorde(); }
				/** Register Lua functions. */
				void registerLuaFunctions(sel::State & luaState);

				void update(double dt);
		};
	}
}
