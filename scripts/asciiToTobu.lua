local input = arg[1]:upper()
local chars = { string.byte(input, 1, input:len()) }

local output = {}
for i,v in ipairs(chars) do
	if v >= string.byte('0') and v <= string.byte('9') then
		output[i] = v - string.byte('0')
	elseif v == string.byte(' ') then
		output[i] = 10
	elseif v >= string.byte('A') and v <= string.byte('Z') then
		output[i] = v - string.byte('A') + 11
	else
		io.stderr:write("Invalid ascii code: " .. v .. "\n")
		os.exit(1)
	end
end

for i,v in ipairs(output) do
	print(v .. "U,")
end
