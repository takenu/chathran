function veg_tree_autumn(path)
	print(" Lua : load autumn tree...")
	local v = TiledVegObject:new{
		name = "AutumnTree",	-- name:	 should be same for all LOD models of the same object, otherwise multiple hordes will be formed
		tilesize = 50,			-- tilesize: will be ignored if name already exists, so should not be defined in same-name objects
		diftex = "tree0_trunk.png",
		normtex = "tree0_trunk_normal.png",
		mesh = "tree0_trunk.obj",
		maxobj = 11000,
		range = 150,
	}
	local v2 = v:new{
		diftex = "tree0_leaves.png",
		normtex = "",
		mesh = "tree0_leaves.obj",
		alpha = true,
	}
	local v3 = v2:new{
		diftex = "tree0_sprite.png",
		mesh = "",
		maxobj = 150000,
		range = 750,
	}
	veg.loadTiledVegObject(v:collectArgs(path))
	veg.loadTiledVegObject(v2:collectArgs(path))
	veg.loadTiledVegObject(v3:collectArgs(path))
	print(" Lua : finished loading autumn tree.")
end

