function string:trim()
	return self:match'^%s*(.*%S)' or ''
end

function string:split(sep)
	local sep, fields = sep or ":", {}
	local pattern = string.format("([^%s]+)", sep)
	self:gsub(pattern, function(c) fields[#fields+1] = c end)
	return fields
end

function rle_encode(data)
	local out = {}

	local run = 1
	for i=2, #data do
		if data[i] == data[i-1] then
			run = run+1
		else
			table.insert(out, data[i-1])
			if run > 1 then
				table.insert(out, data[i-1])
				table.insert(out, run)
			end
			run = 1
		end
	end

	table.insert(out, data[#data])
	if run > 1 then
		table.insert(out, data[#data])
		table.insert(out, run)
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

	local rle = rle_encode(data)
	for i,v in ipairs(rle) do
		io.write(v)
		if i < #rle then
			io.write(", ")
		end
	end
	io.write("\n")
end

main()
