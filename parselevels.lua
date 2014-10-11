local levels = {
	"1.lua",
}

local basedir = "maps/"

local MAX_ENTITIES = 20
local LAST_ENTITY = 11
local TYPES = {
	{ },
	{ name = "E_SPIKES", dir = "NONE" },
	{ name = "E_SEAL", dir = "NONE" },
	{ name = "E_BIRD", dir = "LEFT" },
	{ name = "E_BIRD", dir = "RIGHT" },
	{ name = "E_BAT", dir = "NONE" },
	{ name = "E_GHOST", dir = "LEFT" },
	{ name = "E_GHOST", dir = "RIGHT" },
	{ name = "E_GRAPES", dir = "NONE" },
	{ name = "E_PEACH", dir = "NONE" },
	{ name = "E_DOOR", dir = "NONE" },

	{ name = "T_WALL" },
	{ name = "T_UNDEF" },
}

local MAPW = 10
local MAPH = 16

local HEADER = [[#ifndef LEVELS_H
#define LEVELS_H
#include "defines.h"
]]

local FOOTER = "#endif"

function parseLevel(path)
	local f = loadfile(path)
	local t = f()

	local entities = {}
	local tiles = {}
	for ix = 0,MAPW-1 do
		tiles[ix] = {}
		for iy = 0,MAPH-1 do
			tiles[ix][iy] = "0U"
		end
	end

	for i, layer in ipairs(t.layers) do
		if layer.type == "objectgroup" then
			for j, o in ipairs(layer.objects) do
				local type = TYPES[o.gid]
				if o.gid <= LAST_ENTITY then
					local e = { name = type.name, x = o.x+8, y = o.y-24, dir = type.dir }
					table.insert(entities, e)
				else
					local cx = o.x / 16
					local cy = o.y / 16
					tiles[cx][cy] = type.name
				end
			end
		end
	end

	return {
		entities = entities,
		tiles = tiles
	}
end

local output = {}
for i,v in ipairs(levels) do
	output[i] = parseLevel(basedir .. v)
end

print(HEADER)
print("#define NUM_LEVELS " .. #levels)
print(string.format("const char level_entities[%d][%d][%d] = {", #levels, MAX_ENTITIES, 4))
for j, level in ipairs(output) do
	print("\t{")
	for i=1, MAX_ENTITIES do
		if level.entities[i] then
			local e = level.entities[i]
			print(string.format("\t\t{ %s, %dU, %dU, %s },", e.name, e.x, e.y, e.dir))
		else
			print("\t\t{ 0U, 0U, 0U, 0U },")
		end
	end
	print("\t},")
end
print("};\n")

print(string.format("const char level_tiles[%d][%d] = {", #levels, MAPW*MAPH))
for j, level in ipairs(output) do
	local tiles = level.tiles
	print("\t{")
	for iy=0,MAPH-1 do
		io.write("\t\t")
		for ix = 0,MAPW-1 do
			io.write(string.format("%s, ", tiles[ix][iy]))
		end
		io.write("\n")
	end
	print("\t},")
end
print("};\n")

print(FOOTER)
