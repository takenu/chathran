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

#include <config.h>

#include <tiny/draw/computetexture.h>
#include <tiny/img/io/image.h>

#include "terrain.hpp"

//A GLSL program that determines the terrain type (forest/grass/mud/stone) from height information on the terrain.
template<typename TextureType1, typename TextureType2>
void computeTerrainTypeFromHeight(const TextureType1 &heightMap, TextureType2 &colourMap, const float &mapScale)
{
	std::vector<std::string> inputTextures;
	std::vector<std::string> outputTextures;
	const std::string fragmentShader =
"#version 150\n"
"\n"
"precision highp float;\n"
"\n"
"uniform sampler2D source;\n"
"uniform vec2 sourceInverseSize;\n"
"uniform float mapScale;\n"
"\n"
"in vec2 tex;\n"
"out vec4 colour;\n"
"\n"
"void main(void)\n"
"{\n"
"   float height = texture(source, tex).x;\n"
"   float east = texture(source, tex + vec2(sourceInverseSize.x, 0.0f)).x;\n"
"   float west = texture(source, tex - vec2(sourceInverseSize.x, 0.0f)).x;\n"
"   float north = texture(source, tex + vec2(0.0f, sourceInverseSize.y)).x;\n"
"   float south = texture(source, tex - vec2(0.0f, sourceInverseSize.y)).x;\n"
"   \n"
"   vec3 normal = normalize(vec3(west - east, mapScale, south - north));\n"
"   \n"
"	float slope = 1.0f - normal.y;\n"
"	float forestFrac = clamp(max(0.0f, 1.0f - 9.0f*slope)*max(0.0f, 1.0f - 0.1f*(height - 450.0f)), 0.0f, 1.0f);\n"
"	float grassFrac = (1.0f - forestFrac)*clamp(max(0.0f, 1.0f - 7.0f*slope)*max(0.0f, 1.0f - 0.1f*(height - 1200.0f)), 0.0f, 1.0f);\n"
"	float mudFrac = (1.0f - grassFrac - forestFrac)*clamp(max(0.0f, 1.0f - 1.0f*slope), 0.0f, 1.0f);\n"
"	float rockFrac = 1.0f - forestFrac - grassFrac - mudFrac;\n"
"	\n"
"   colour = vec4((0.0f*forestFrac + 1.0f*grassFrac + 2.0f*mudFrac + 3.0f*rockFrac)/255.0f, 0.0f, 0.0f, 0.0f);\n"
"}\n";
	
	inputTextures.push_back("source");
	outputTextures.push_back("colour");

	tiny::draw::ComputeTexture * computeTexture = new tiny::draw::ComputeTexture(inputTextures, outputTextures, fragmentShader);
	
	computeTexture->uniformMap().setFloatUniform(2.0f*mapScale, "mapScale");
	computeTexture->setInput(heightMap, "source");
	computeTexture->setOutput(colourMap, "colour");
	computeTexture->compute();
	colourMap.getFromDevice();
	
	delete computeTexture;
}


using namespace ch::core;

void TerrainManager::setup(void)
{
	//Create large example terrain.
	terrain = new tiny::draw::Terrain(6, 8);
	
	//Read heightmap for the far-away terrain.
	heightTexture = new tiny::draw::FloatTexture2D(tiny::img::io::readImage(DATA_DIRECTORY + "terrain/512hills.png"), tiny::draw::tf::filter);
	farHeightTexture = new tiny::draw::FloatTexture2D(heightTexture->getWidth(), heightTexture->getHeight(), tiny::draw::tf::filter);
	
	//Scale vertical range of the far-away heightmap.
	tiny::draw::computeScaledTexture(*heightTexture, *farHeightTexture, tiny::vec4(heightScale/255.0f), tiny::vec4(0.0f));
	
	//Zoom into a small area of the far-away heightmap.
	tiny::draw::computeResizedTexture(*farHeightTexture, *heightTexture,
								tiny::vec2(1.0f/static_cast<float>(farScale.x), 1.0f/static_cast<float>(farScale.y)),
								farOffset);
	
	//Apply the diamond-square fractal algorithm to make the zoomed-in heightmap a little less boring.
	tiny::draw::computeDiamondSquareRefinement(*heightTexture, *heightTexture, farScale.x);
	
	//Create normal maps for the far-away and zoomed-in heightmaps.
	farNormalTexture = new tiny::draw::RGBTexture2D(heightTexture->getWidth(), heightTexture->getHeight());
	normalTexture = new tiny::draw::RGBTexture2D(heightTexture->getWidth(), heightTexture->getHeight());
	farTangentTexture = new tiny::draw::RGBTexture2D(heightTexture->getWidth(), heightTexture->getHeight());
	tangentTexture = new tiny::draw::RGBTexture2D(heightTexture->getWidth(), heightTexture->getHeight());
	
	tiny::draw::computeNormalMap(*farHeightTexture, *farNormalTexture, scale.x*farScale.x);
	tiny::draw::computeNormalMap(*heightTexture, *normalTexture, scale.x);
	tiny::draw::computeTangentMap(*farHeightTexture, *farTangentTexture, scale.x*farScale.x);
	tiny::draw::computeTangentMap(*heightTexture, *tangentTexture, scale.x);
	
	//Read diffuse textures and make them tileable.
	if (true)
	{
		std::vector<tiny::img::Image> diffuseTextures;
		std::vector<tiny::img::Image> normalTextures;
		
		diffuseTextures.push_back(tiny::img::io::readImage(DATA_DIRECTORY + "img/terrain/forest.jpg"));
		diffuseTextures.push_back(tiny::img::io::readImage(DATA_DIRECTORY + "img/terrain/grass.jpg"));
		diffuseTextures.push_back(tiny::img::io::readImage(DATA_DIRECTORY + "img/terrain/dirt.jpg"));
		diffuseTextures.push_back(tiny::img::io::readImage(DATA_DIRECTORY + "img/terrain/rocks.jpg"));
		localDiffuseTextures = new tiny::draw::RGBTexture2DArray(diffuseTextures.begin(), diffuseTextures.end());
		
		normalTextures.push_back(tiny::img::io::readImage(DATA_DIRECTORY + "img/terrain/forest_normal.jpg"));
		normalTextures.push_back(tiny::img::io::readImage(DATA_DIRECTORY + "img/terrain/grass_normal.jpg"));
		normalTextures.push_back(tiny::img::io::readImage(DATA_DIRECTORY + "img/terrain/dirt_normal.jpg"));
		normalTextures.push_back(tiny::img::io::readImage(DATA_DIRECTORY + "img/terrain/rocks_normal.jpg"));
		localNormalTextures = new tiny::draw::RGBTexture2DArray(normalTextures.begin(), normalTextures.end());
	}
	
	//Create an attribute texture that determines the terrain type (forest/grass/mud/stone) based on the altitude and slope.
	//We do this for both the zoomed-in and far-away terrain.
	attributeTexture = new tiny::draw::RGBATexture2D(tiny::img::Image::createSolidImage(heightTexture->getWidth(), 255, 0, 0, 0));
	farAttributeTexture = new tiny::draw::RGBATexture2D(tiny::img::Image::createSolidImage(heightTexture->getWidth()));
	
	computeTerrainTypeFromHeight(*heightTexture, *attributeTexture, scale.x);
	computeTerrainTypeFromHeight(*farHeightTexture, *farAttributeTexture, scale.x*farScale.x);
	attributeTexture->getFromDevice();
	
	//Paint the terrain with the zoomed-in and far-away textures.
	terrain->setFarHeightTextures(*heightTexture, *farHeightTexture,
								  *tangentTexture, *farTangentTexture,
								  *normalTexture, *farNormalTexture,
								  scale, farScale, farOffset);
	terrain->setFarDiffuseTextures(*attributeTexture, *farAttributeTexture,
								   *localDiffuseTextures, *localNormalTextures,
								   diffuseScale);

	renderer->addWorldRenderable(terrain);
}

void TerrainManager::cleanup(void)
{
	delete terrain;
	
	delete farHeightTexture;
	delete heightTexture;
	delete farTangentTexture;
	delete tangentTexture;
	delete farNormalTexture;
	delete normalTexture;
	
	delete farAttributeTexture;
	delete attributeTexture;
	delete localDiffuseTextures;
	delete localNormalTextures;
}
