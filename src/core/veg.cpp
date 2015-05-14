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

#include <cassert>

#include <config.h>

#include <tiny/math/vec.h>
#include <tiny/mesh/io/staticmesh.h>
#include <tiny/img/io/image.h>

#include <selene.h>

#include "veg.hpp"

using namespace ch::core;

void VegManager::update(double )
{
	if (renderer->lodUpdates())
	{
		for(std::map<std::string, veg::VegetationTiledHorde*>::iterator it = hordemap.begin(); it != hordemap.end(); it++)
		{
			it->second->update(renderer->getCameraPosition(), terrain->getHeightFunc());
		}
	}
}

void VegManager::addLOD(std::string name, float _tileSize, std::string meshLoc, std::string difTexLoc, bool alphaChannel, std::string normTexLoc, int maxObjects, float maxRange)
{
	std::cout << " call: name= "<<name<<" tilesize= "<<_tileSize<<" mesh= "<<meshLoc<<" diftex= "<<difTexLoc<<" normtex= "<<normTexLoc<<std::endl;
	if(difTexLoc == "") { std::cerr << " Warning: VegManager::addLOD() doesn't have a texture for '"<<name<<"'. Skipping object creation. "<<std::endl; return; }
	if(meshLoc == "" && !alphaChannel) { std::cerr << " Warning: VegManager::addLOD() found icon texture without alpha channel, cannot process. "<<std::endl; return; }
	if( hordemap.find(name) == hordemap.end() ) hordemap.insert( std::make_pair(name, new veg::VegetationTiledHorde(name, _tileSize) ) );
	veg::VegetationTiledHorde * vegHorde = hordemap.find(name)->second;
	tiny::draw::RGBTexture2D * difTex = 0;
	tiny::draw::RGBATexture2D * difTexAlpha = 0;
	tiny::draw::RGBTexture2D * normTex = 0;
	if(alphaChannel) difTexAlpha = new tiny::draw::RGBATexture2D(tiny::img::io::readImage(DATA_DIRECTORY + "img/" + difTexLoc));
	else difTex = new tiny::draw::RGBTexture2D(tiny::img::io::readImage(DATA_DIRECTORY + "img/" + difTexLoc));
	assert(difTex || difTexAlpha);
	if(normTexLoc != "") normTex = new tiny::draw::RGBTexture2D(tiny::img::io::readImage(DATA_DIRECTORY + "img/" + normTexLoc));
	if(meshLoc != "")
	{
		tiny::draw::StaticMeshHorde * mesh = new tiny::draw::StaticMeshHorde(tiny::mesh::io::readStaticMesh(DATA_DIRECTORY + "mesh/" + meshLoc), maxObjects);
		assert(mesh);
		if(normTex) mesh->setNormalTexture(*normTex);
		renderer->addWorldRenderable(mesh);
		if(alphaChannel)
		{
			mesh->setDiffuseTexture(*difTexAlpha);
			vegHorde->addLOD(maxObjects, maxRange, mesh, difTexAlpha, normTex);
		}
		else
		{
			mesh->setDiffuseTexture(*difTex);
			vegHorde->addLOD(maxObjects, maxRange, mesh, difTex, normTex);
		}
	}
	else
	{
//		if(normTex) icon->setNormalTexture(*normTex);
		assert(difTexAlpha);
		tiny::draw::WorldIconHorde * icon = new tiny::draw::WorldIconHorde(maxObjects, false);
		assert(icon);
		icon->setIconTexture(*difTexAlpha);
		vegHorde->addLOD(maxObjects, maxRange, icon, difTexAlpha, normTex);
		renderer->addWorldRenderable(icon);
//		else vegHorde->addLOD(maxObjects, maxRange, icon, difTex, normTex);
	}
}

void VegManager::registerLuaFunctions(sel::State & luaState)
{
	luaState["veg"].SetObj(*this,
			"loadTiledVegObject", &VegManager::addLOD);
}
