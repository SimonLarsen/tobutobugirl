if #arg ~= 32 then
	io.stderr:write("Input must be 32 integers.\n")
	os.exit(1)
end

for i=1,32 do
	if tonumber(arg[i]) < 0 or tonumber(arg[i]) > 15 then
		io.stderr:write("Values must be in [0,15]\n")
		os.exit(1)
	end
end

for i=1, 31, 2 do
	print(tonumber(arg[i]) * 16 + tonumber(arg[i+1]))
end
