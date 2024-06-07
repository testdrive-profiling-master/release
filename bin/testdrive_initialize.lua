-- lfs extension utils
lfs.IsExist = function(path)
	return (lfs.attributes(path) ~= nil)
end

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


-------------------------------------------------------
-- initialization for TestDrive Profiling Master
-------------------------------------------------------
System.Execute("setup_mingw.bat", "", ".\\", nil)
System.Execute("setup_eclipse.bat", "", ".\\", nil)

if lfs.IsExist("notepad\\notepad++.exe") == false then
	LOGI("[[[ System will download Notepad++. Please wait... ]]]\n")
	--System.Execute("setup_notepad.bat", "", ".\\", nil)
	
	if lfs.IsExist(".\\notepad\\") then
		System.Execute("rmdir", "/S /Q notepad", ".\\", nil)
		exec("rmdir /S /Q notepad")
	end
	
	LOGI("NotePad++ downloading...")
	System.Execute("ToolAutoDownload.exe", "notepad++", ".\\", nil)
	
	if lfs.IsExist("notepad.zip") then
		System.Execute("unzip.exe", "notepad.zip -d notepad", ".\\", nil)
		exec("del notepad.zip")
		System.Execute("install_notepad_plugin", "HexEditor", ".\\", nil)
		System.Execute("install_notepad_plugin", "XMLTools", ".\\", nil)
		System.Execute("install_notepad_plugin", "Explorer", ".\\", nil)
		System.Execute("install_notepad_plugin", "ComparePlugin", ".\\", nil)
		System.Execute("install_notepad_plugin", "DSpellCheck", ".\\", nil)
		System.Execute("install_notepad_plugin", "NppEditorConfig", ".\\", nil)
		exec("codegen notepad++_postfix.lua")
		LOGI("Notepad++ installation is done!\n")
	else
		LOGW("Notepad++ installation is failed!\n")
	end
end

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
