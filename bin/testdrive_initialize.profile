system.call "setup_mingw.bat", "", ".\\"
system.call "setup_eclipse.bat", "", ".\\"

if.file "notepad\\notepad++.exe"
	// nothing to do...
if.else
	system.msg "[[[ System will download Notepad++. Please wait... ]]]\n\n"
	system.call "setup_notepad.bat", "", ".\\"
	system.msg "\nNotepad++ installation is done!\n"
if.end
