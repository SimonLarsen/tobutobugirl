local d = tonumber(arg[1])
local n = tonumber(arg[2])

function math.round(x)
	return math.floor(x + 0.5)
end

for i=0, n-1 do
	local x = i/n * 2 * math.pi
	local v = math.round(math.sin(x)*d/2 + d/2)
	print(v)
end
