-- test UTF-8 BOM
-- this file must be encoded by UTF-8 with BOM

-- test wide-char variable name
do
	local 变量a1 = 123.456
	print(变量a1 .. " = 123.456")
end

-- test break [N] and continue [N] statement
do
	local sum = 0
	for i = 1, 10 do
		if i == 5 then continue end
		sum = sum + i
	end
	print(sum .. " = 50")

	local sum = 0
	local i = 0
	while true do
		i = i + 1
		if i > 10 then break end
		if i == 5 then continue end
		sum = sum + i
	end
	print(sum .. " = 50")

	local sum = 0
	local i = 0
	repeat
		i = i + 1
		if i == 5 then continue end
		sum = sum + i
		if i >= 10 then break end
	until false
	print(sum .. " = 50")

	local sum = 0
	for i = 1, 5 do
		sum = sum + 1
		while true do
			sum = sum + i
			i = i + 1
			if i == 3 then continue 2 end
			if i > 5 then break 2 end
		end
		sum = sum + 1
	end
	print(sum .. " = 20")
end

-- test != operator

do
	local i = 3
	if i != 5 then
		print "!= = ~="
	else
		print "!= != ~="
	end
end

-- test hex-number in string (for Lua 5.2)
print("\x41\x42\x43 = ABC")
