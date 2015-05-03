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

#include <tiny/math/vec.h>

#include <tiny/draw/texture2d.h>
#include <tiny/draw/texture2darray.h>
#include <tiny/draw/terrain.h>
#include <tiny/draw/heightmap/scale.h>
#include <tiny/draw/heightmap/resize.h>
#include <tiny/draw/heightmap/normalmap.h>
#include <tiny/draw/heightmap/tangentmap.h>
#include <tiny/draw/heightmap/diamondsquare.h>

#include "interface/render.hpp"
#include "interface/terrain.hpp"

namespace ch
{
	namespace core
	{
		/** Manage all terrain. */
		class TerrainManager : public intf::TerrainInterface
		{
			private:
				intf::RenderInterface * renderer;

				tiny::draw::Terrain * terrain;

				const float heightScale;
				tiny::draw::RGBTexture2D * normalTexture;
				tiny::draw::RGBTexture2D * tangentTexture;

				const tiny::vec2 diffuseScale;
				tiny::draw::RGBATexture2D * attributeTexture;
				tiny::draw::RGBTexture2DArray * localDiffuseTextures;
				tiny::draw::RGBTexture2DArray * localNormalTextures;

				const tiny::ivec2 farScale;
				const tiny::vec2 farOffset;
				tiny::draw::FloatTexture2D * farHeightTexture;
				tiny::draw::RGBTexture2D * farNormalTexture;
				tiny::draw::RGBTexture2D * farTangentTexture;

				tiny::draw::RGBATexture2D * farAttributeTexture;

				void setup(void);
				void cleanup(void);
			public:
				TerrainManager(intf::RenderInterface * _renderer) :
					intf::TerrainInterface(),
					renderer(_renderer),
					terrain(0),
					heightScale(1617.0f),normalTexture(0),tangentTexture(0),
					diffuseScale(1024.0f,1024.0f), attributeTexture(0), localDiffuseTextures(0), localNormalTextures(0),
					farScale(tiny::ivec2(2,2)), farOffset(tiny::vec2(0.4f,0.4f)), farHeightTexture(0), farNormalTexture(0), farTangentTexture(0),
					farAttributeTexture(0)
				{
					setup();
				}

				void update(double)
				{
					if(renderer->lodUpdates()) terrain->setCameraPosition( renderer->getCameraPosition() );
				}
		};
	}
}
