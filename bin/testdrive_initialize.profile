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
