local levels = {
	"1.lua",
}

local basedir = "maps/"

local MAX_ENTITIES = 15
local TYPES = {
	{ name = "E_SPIKES", dir = "NONE" },
	{ name = "E_SEAL", dir = "NONE" },
	{ name = "E_BIRD", dir = "LEFT" },
	{ name = "E_BIRD", dir = "RIGHT" },
	{ name = "E_BAT", dir = "NONE" },
	{ name = "E_GRAPES", dir = "NONE" },
	{ name = "E_PEACH", dir = "NONE" },
	{ name = "E_DOOR", dir = "NONE" },
}

local HEADER = [[#ifndef LEVELS_H
#define LEVELS_H
#include "defines.h"
]]

local FOOTER = "#endif"

function parseLevel(path)
	local f = loadfile(path)
	local t = f()

	local entities = {}

	for i, layer in ipairs(t.layers) do
		if layer.type == "objectgroup" then
			if #layer.objects > MAX_ENTITIES then
				io.stderr:write("error: Too many entities in level " .. path .. "\n")
				os.exit(1)
			end
			for j, o in ipairs(layer.objects) do
				local type = TYPES[o.gid-1]
				local e = { name = type.name, x = o.x+8, y = o.y-24, dir = type.dir }
				table.insert(entities, e)
			end
		end
	end

	return entities
end

local output = {}
for i,v in ipairs(levels) do
	output[i] = parseLevel(basedir .. v)
end

print(HEADER)
print("#define NUM_LEVELS " .. #levels)
print(string.format("const char levels[%d][%d][%d] = {", #levels, MAX_ENTITIES, 4))
for j, e in ipairs(output) do
	print("\t{")
	for i=1, MAX_ENTITIES do
		if e[i] then
			print(string.format("\t\t{ %s, %dU, %dU, %s },", e[i].name, e[i].x, e[i].y, e[i].dir))
		else
			print("\t\t{ 0U, 0U, 0U, 0U },")
		end
	end
	print("\t},")
end
print("};\n")

print(FOOTER)
