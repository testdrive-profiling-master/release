local f = TextFile()

-- update model.xml
if f:Open("./notepad/stylers.model.xml") == false then
	error("Can't open style file.")
	os.exit(1)
end

local s = String(f:GetAll(false))
f:Close()

local iPos = s:find("<GlobalStyles>", 0)
if iPos > 0 then
	local iStart = s:find("\"Global override\"", iPos)
	if iStart > 0 then
		iStart	= iStart + 17
		local iEnd = s:find("/>", iStart) + 2
		s:erase(iStart, (iEnd - iStart))
		s:insert(iStart, " styleID=\"0\" fgColor=\"FFFF80\" bgColor=\"FF8000\" fontName=\"Cascadia Mono\" fontStyle=\"0\" fontSize=\"10\" />")
	end
end

f:Create("./notepad/stylers.model.xml")
f:Put(s.s)
f:Close()


-- update config.xml
if f:Open("./notepad/config.xml") == false then
	error("Can't open config file.")
	os.exit(1)
end

s = String(f:GetAll(false))
f:Close()

-- fix for global font apply
iPos = s:find("\"globalOverride\"", 0)
if iPos > 0 then
	iPos = iPos + 16
	local iEnd = s:find("/>", iPos) + 2
	s:erase(iPos, (iEnd - iPos))
	s:insert(iPos, " fg=\"no\" bg=\"no\" font=\"yes\" fontSize=\"yes\" bold=\"yes\" italic=\"no\" underline=\"no\" />")
end

-- enable explorer
iPos = s:find("<ActiveTabs cont=\"0\" activeTab=\"-1\" />", 0)
if iPos > 0 then
	s:erase(iPos, 38)
	s:insert(iPos,	"<PluginDlg pluginName=\"Explorer.dll\" id=\"0\" curr=\"0\" prev=\"-1\" isVisible=\"yes\" />\n" ..
					"            <ActiveTabs cont=\"0\" activeTab=\"0\" />")
end

f:Create("./notepad/config.xml")
f:Put(s.s)
f:Close()
