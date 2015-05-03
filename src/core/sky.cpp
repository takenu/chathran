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

#include <tiny/mesh/io/staticmesh.h>
#include <tiny/img/io/image.h>

#include <config.h>

#include "sky.hpp"

using namespace ch::core;

void SkyManager::setup(void)
{
	//Create sky (a simple cube containing the world).
	skyBox = new tiny::draw::StaticMesh(tiny::mesh::StaticMesh::createCubeMesh(-1.0e6));
	skyBoxTexture = new tiny::draw::RGBTexture2D(tiny::img::Image::createSolidImage(16), tiny::draw::tf::filter);
	skyBox->setDiffuseTexture(*skyBoxTexture);
	
	//Render using a more advanced shading model.
	sunSky = new tiny::draw::effects::SunSky();
	skyGradientTexture = new tiny::draw::RGBATexture2D(tiny::img::io::readImage(DATA_DIRECTORY + "img/sky/sky.png"));
	sunSky->setSkyTexture(*skyGradientTexture);

	renderer->addWorldRenderable(skyBox);
	renderer->addScreenRenderable(sunSky,false,false);
}

void SkyManager::cleanup(void)
{
	delete sunSky;
	
	delete skyBox;
	delete skyBoxTexture;
	delete skyGradientTexture;	
}
