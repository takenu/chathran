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

#include "terrain.hpp"

using namespace ch::core::intf;
using namespace std::placeholders;

float TerrainInterface::getHeightIndirect(tiny::vec2 pos)
{
	return getHeightFromTexture(pos);
}

std::function<float(tiny::vec2)> TerrainInterface::getHeightFunc(void)
{
	std::function<float(tiny::vec2)> func = std::bind(&TerrainInterface::getHeightIndirect, this, _1);
	return func;
}
