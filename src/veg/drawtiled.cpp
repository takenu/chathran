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

#include <tiny/math/vec.h>
#include "drawtiled.hpp"

using namespace ch::veg;

//Function to populate a specific tile with trees.
//template<typename TextureType1, typename TextureType2>
//int plantTreesTiled(const TextureType1 &heightTexture, const TextureType2 &attributeTexture, const tiny::vec2 &scale,
int plantTreesTiled(std::function<float(tiny::vec2)> & getHeight,
			   const tiny::vec3 &origin, float tileSize, const int &maxNrTrees,
			   std::vector<tiny::draw::StaticMeshInstance> &highDetailInstances, std::vector<tiny::draw::WorldIconInstance> &lowDetailInstances, std::vector<tiny::vec3> &positions)
{
	int nrTrees = 0;
	
	highDetailInstances.clear();
	lowDetailInstances.clear();
	positions.clear();
	
	if (maxNrTrees <= 0)
	{
		std::cerr << "Warning: Not placing any trees!" << std::endl;
		return 0;
	}

	highDetailInstances.reserve(maxNrTrees);
	lowDetailInstances.reserve(maxNrTrees);
	positions.reserve(maxNrTrees);
	
	for (int i = 0; i < maxNrTrees; ++i)
	{
		//Determine the random spot where we want to place a tree.
		const tiny::vec2 treePlanePosition = tiny::randomVec2(0.5*tileSize) + tiny::vec2(origin.x+0.5*tileSize, origin.z+0.5*tileSize);
		
		//Are we going to place a tree here?
//		const float placeProbability = sampleTextureBilinear(attributeTexture, scale, treePlanePosition).x;
		const float placeProbability = 0.0f;
		
		if (placeProbability <= 0.5f/255.0f)
		{
			//Determine height.
//			const tiny::vec3 treePosition = tiny::vec3(treePlanePosition.x, sampleTextureBilinear(heightTexture, scale, treePlanePosition).x, treePlanePosition.y);
//			const tiny::vec3 treePosition = tiny::vec3(treePlanePosition.x, terrain->getHeight(treePlanePosition), treePlanePosition.y);
			const tiny::vec3 treePosition = tiny::vec3(treePlanePosition.x, getHeight(treePlanePosition), treePlanePosition.y);
			
			highDetailInstances.push_back(tiny::draw::StaticMeshInstance(tiny::vec4(treePosition.x, treePosition.y, treePosition.z, 1.0f),
																   tiny::vec4(0.0f, 0.0f, 0.0f, 1.0f)));
			lowDetailInstances.push_back(tiny::draw::WorldIconInstance(tiny::vec4(treePosition.x, treePosition.y + 4.0f, treePosition.z, 1.0f),
																 tiny::vec2(4.0f, 4.0f),
																 tiny::vec4(0.0f, 0.0f, 1.0f, 1.0f),
																 tiny::vec4(1.0f, 1.0f, 1.0f, 1.0f)));
			positions.push_back(treePosition);
			++nrTrees;
		}
	}

	return nrTrees;
}

void VegetationTiledHorde::update(const tiny::vec3 & cameraPosition, std::function<float(tiny::vec2)> getHeight)
{
	tiledHorde->removeOldTiles(cameraPosition);
	tiledHorde->listNewTiles(cameraPosition);

	tiny::vec3 tilepos;
	while(tiledHorde->getNewStaticMeshTile(tilepos))
	{
		std::vector<tiny::draw::StaticMeshInstance> nearTrees;
		std::vector<tiny::draw::WorldIconInstance> farTrees;
		std::vector<tiny::vec3> tmpTreePositions;
		plantTreesTiled(getHeight,
				   tilepos, tileSize, 150,
				   nearTrees, farTrees, tmpTreePositions);
		tiledHorde->addTile(tilepos,nearTrees);
	}
	while(tiledHorde->getNewIconTile(tilepos))
	{
		std::vector<tiny::draw::StaticMeshInstance> nearTrees;
		std::vector<tiny::draw::WorldIconInstance> farTrees;
		std::vector<tiny::vec3> tmpTreePositions;
		plantTreesTiled(getHeight,
				   tilepos, tileSize, 150,
				   nearTrees, farTrees, tmpTreePositions);
		tiledHorde->addTile(tilepos,farTrees);
	}
	tiledHorde->recalculateLOD(cameraPosition);
}
