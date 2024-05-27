system.call "setup_mingw.bat", "", ".\\"
system.call "setup_eclipse.bat", "", ".\\"

if.file "notepad\\notepad++.exe"
	// nothing to do...
if.else
	system.msg "[[[ System will download Notepad++. Please wait... ]]]\n\n"
	system.call "setup_notepad.bat", "", ".\\"
	system.msg "\nNotepad++ installation is done!\n"
if.end


#lua
-- initialization for TestDrive Profiling Master
-- sort by key
local function __collect_keys(t, sort)
	local _k = {}
	for k in pairs(t) do
		_k[#_k+1] = k
	end
	table.sort(_k, sort)
	return _k
end

function key_pairs(t)
	local keys = __collect_keys(t, __key_compare)
	local i = 0
	return function()
		i = i+1
		if keys[i] then
			return keys[i], t[keys[i]]
		end
	end
end

-- lfs extension utils
--[[
lfs.IsExist = function(path)
	return (lfs.attributes(path) ~= nil)
end
--]]

-- Failure should be handled with an error
function CreateMemory(mem_size, mem_name)
	if System.CreateMemory(mem_size, mem_name) == false then
		error("Can't create memory model : " .. mem_name)
	end
end

-- Failure should be handled with an error
function RunProfile(filename)
	if System.RunProfile(filename) == false then
		error("Can't Run profile script : " .. filename)
	end
end
