-- Load all vegetation.
function load_veg()
	loadscr("veg/tree/load.lua"); load_veg_tree()
end

--	A base table to be used when creating TiledHordes for vegetation.
--	This implements inheritance as illustrated in Programming in Lua, second edition, chapter 13.4 (Table-Access Metamethods) and 16.1 (Classes).
TiledVegObject = {
	name = "unnamedVegObject",
	tileSize = 50.0,
	mesh = "",
	diftex = "",
	alpha = false,
	normtex = "",
	maxobj = 1000,
	range = 50,
}

function TiledVegObject:new(o)
	o = o or {}
	setmetatable(o,self)
	self.__index = self
	return o
end
function TiledVegObject:collectArgs(path)
	local prefix = findprefix(path)
	return path .. "::" .. self.name, self.tileSize, self.mesh == "" and "" or prefix .. self.mesh, prefix .. self.diftex, self.alpha,
			self.normtex == "" and "" or prefix .. self.normtex, self.maxobj, self.range
end

