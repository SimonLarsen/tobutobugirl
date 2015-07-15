function string:trim()
	return self:match'^%s*(.*%S)' or ''
end

function string:split(sep)
	local sep, fields = sep or ":", {}
	local pattern = string.format("([^%s]+)", sep)
	self:gsub(pattern, function(c) fields[#fields+1] = c end)
	return fields
end

function rle_decode(data)
	local out = {}

	local i = 1
	while i <= #data do
		if i <= #data-2 and data[i] == data[i+1] then
			local count = data[i+2]
			for j=1,count do
				table.insert(out, data[i])
			end
			i = i+3
		else
			table.insert(out, data[i])
			i = i+1
		end
	end

	return out
end

function main()
	local data = {}
	for line in io.lines(arg[1]) do
		local parts = line:split(",")
		for i,v in ipairs(parts) do
			table.insert(data, tonumber(v:trim()))
		end
	end

	local dec = rle_decode(data)
	for i,v in ipairs(dec) do
		io.write(v)
		if i < #dec then
			io.write(", ")
		end
	end
	io.write("\n")
end

main()
