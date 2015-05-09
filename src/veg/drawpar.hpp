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

namespace ch
{
	namespace veg
	{
		/** A class for describing a single mesh component of a LOD. The diffuse can be either rgb or rgba, but not both. */
		template <typename MeshType, typename DiffuseTexType>
		class VegetationLodComponent
		{
			private:
			public:
				MeshType * mesh;
				DiffuseTexType * diffuseTexture;
				tiny::draw::RGBTexture2D * normalTexture;

				VegetationLodComponent(MeshType * _mesh, DiffuseTexType * _dt, tiny::draw::RGBTexture2D * _nt) : mesh(_mesh), diffuseTexture(_dt), normalTexture(_nt) {}

				void freeAll(void)
				{
					delete mesh;
					delete diffuseTexture;
					if(normalTexture) delete normalTexture;
				}
		};

		/** A class for describing a single LOD of a Vegetation type. */
		template <typename MeshType>
		class VegetationLOD
		{
			private:
			public:
				int maxObjectsRendered;
				float maxRadius;
				bool addedToTiledHorde;
				std::vector< VegetationLodComponent<MeshType, tiny::draw::RGBTexture2D> > lodComponents;
				std::vector< VegetationLodComponent<MeshType, tiny::draw::RGBATexture2D> > lodComponentsAlpha;

				VegetationLOD(const int & _mor, const float & _mr) : maxObjectsRendered(_mor), maxRadius(_mr), addedToTiledHorde(false) {}

				void addLodComponent(MeshType * _mesh, tiny::draw::RGBTexture2D * _dt, tiny::draw::RGBTexture2D * _nt)
				{ lodComponents.push_back( VegetationLodComponent<MeshType, tiny::draw::RGBTexture2D>(_mesh,_dt,_nt) ); }
				void addLodComponent(MeshType * _mesh, tiny::draw::RGBATexture2D * _dt, tiny::draw::RGBTexture2D * _nt)
				{ lodComponentsAlpha.push_back( VegetationLodComponent<MeshType, tiny::draw::RGBATexture2D>(_mesh,_dt,_nt) ); }

				/** Once all mesh/texture combinations for this particular LOD are known, add them all at once to the tiled horde. */
				void addToTiledHorde(tiny::draw::TiledHorde * tiledHorde)
				{
					if(addedToTiledHorde) return; // should not add more than once, TiledHorde doesn't ignore duplicates
					std::vector<MeshType*> meshes;
					for(unsigned int i = 0; i < lodComponents.size(); i++) meshes.push_back(lodComponents[i].mesh);
					for(unsigned int i = 0; i < lodComponentsAlpha.size(); i++) meshes.push_back(lodComponentsAlpha[i].mesh);
					tiledHorde->addLOD(meshes, maxRadius);
				}

				void freeComponents(void)
				{
					for(unsigned int i = 0; i < lodComponents.size(); i++) lodComponents[i].freeAll();
					for(unsigned int i = 0; i < lodComponentsAlpha.size(); i++) lodComponentsAlpha[i].freeAll();
				}
		};

		/** A base class for any horde-based Vegetation type. */
		class VegetationHorde
		{
			private:
				// Do not copy construct or assign: we destroy the tiledHorde and all LODs in our destructor
				VegetationHorde(const VegetationHorde &);
				VegetationHorde & operator= (const VegetationHorde &);
			public:
				tiny::draw::TiledHorde * tiledHorde;

				std::map<float, VegetationLOD<tiny::draw::StaticMeshHorde> > meshLod;
				std::map<float, VegetationLOD<tiny::draw::WorldIconHorde> > iconLod;

				VegetationHorde(std::string _name, const float & _tileSize) : tiledHorde(new tiny::draw::TiledHorde(_tileSize, _name)) {}
				~VegetationHorde(void)
				{
					delete tiledHorde;
					for(std::map<float,VegetationLOD<tiny::draw::StaticMeshHorde> >::iterator it = meshLod.begin(); it != meshLod.end(); it++) it->second.freeComponents();
					for(std::map<float,VegetationLOD<tiny::draw::WorldIconHorde> >::iterator it = iconLod.begin(); it != iconLod.end(); it++) it->second.freeComponents();
					meshLod.clear();
					iconLod.clear();
				}

				template <typename DiffuseTexType>
				void addLOD(const int & _mor, const float & _mr, tiny::draw::StaticMeshHorde * _mesh, DiffuseTexType * _dt, tiny::draw::RGBTexture2D * _nt = 0)
				{
					if(meshLod.find(_mr) == meshLod.end()) meshLod.insert( std::make_pair(_mr, VegetationLOD<tiny::draw::StaticMeshHorde>(_mor, _mr) ) );
					meshLod.find(_mr)->second.addLodComponent(_mesh, _dt, _nt);
				}
				template <typename DiffuseTexType>
				void addLOD(const int & _mor, const float & _mr, tiny::draw::WorldIconHorde * _mesh, DiffuseTexType * _dt, tiny::draw::RGBTexture2D * _nt = 0)
				{
					if(iconLod.find(_mr) == iconLod.end()) iconLod.insert( std::make_pair(_mr, VegetationLOD<tiny::draw::WorldIconHorde>(_mor, _mr) ) );
					iconLod.find(_mr)->second.addLodComponent(_mesh, _dt, _nt);
				}

				void addToTiledHorde(void)
				{
					for(std::map<float,VegetationLOD<tiny::draw::StaticMeshHorde> >::iterator it = meshLod.begin(); it != meshLod.end(); it++) it->second.addToTiledHorde(tiledHorde);
					for(std::map<float,VegetationLOD<tiny::draw::WorldIconHorde> >::iterator it = iconLod.begin(); it != iconLod.end(); it++) it->second.addToTiledHorde(tiledHorde);
				}
		};
	}
}
