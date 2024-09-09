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
-- Extra wrapper function
-------------------------------------------------------
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

-------------------------------------------------------
-- initialization for TestDrive Profiling Master
-------------------------------------------------------
System.Execute("setup_mingw.bat", "", ".\\", nil)
System.Execute("setup_eclipse.bat", "", ".\\", nil)

-------------------------------------------------------
--check installed msys2 package
-------------------------------------------------------
local __installed_package = {}
do
	local	sList	= String(exec("pacman -Q"))
	
	while (sList.TokenizePos >= 0) do
		local	sPackage	= sList:Tokenize("\r\n")
		sPackage:Trim(" ")
		sPackage:CutBack(" ", true)
		
		if sPackage:Length() > 0 then
			__installed_package[#__installed_package + 1] = sPackage.s
		end
	end
end

function RequireMingwPackage(package_name)
	local bInstalled = false
	for key,val in pairs(__installed_package) do
		if val == package_name then
			bInstalled	= true
			break
		end
	end
	
	if bInstalled == false then
		LOGI("Installing new requried package : " .. package_name)
		System.Execute("pacman", "-S --noconfirm " .. package_name, ".\\msys64\\usr\\bin", nil)
		__installed_package[#__installed_package + 1] = package_name
	end
end

-- Most recent required package
RequireMingwPackage("mingw-w64-ucrt-x86_64-graphviz")				-- 2024/1
RequireMingwPackage("mingw-w64-ucrt-x86_64-source-highlight")		--
RequireMingwPackage("mingw-w64-ucrt-x86_64-lua-luarocks")			--
RequireMingwPackage("mingw-w64-ucrt-x86_64-pdf2djvu")				-- 2024/3
RequireMingwPackage("mingw-w64-ucrt-x86_64-qrencode")				-- 2024/6
RequireMingwPackage("mingw-w64-ucrt-x86_64-xmake")					-- 2024/7
RequireMingwPackage("tree")											-- 2024/7
RequireMingwPackage("whois")										-- 2024/7
RequireMingwPackage("mingw-w64-ucrt-x86_64-unrar")					-- 2024/7
RequireMingwPackage("mingw-w64-ucrt-x86_64-libevent")				-- 2024/8
RequireMingwPackage("mingw-w64-ucrt-x86_64-tgui")					-- 2024/8
RequireMingwPackage("mingw-w64-ucrt-x86_64-enet")					-- 2024/9

-------------------------------------------------------
-- check notepad++ installation
-------------------------------------------------------
if lfs.IsExist("notepad\\notepad++.exe") == false then
	LOGI("[[[ System will download Notepad++. Please wait... ]]]\n")
	
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
