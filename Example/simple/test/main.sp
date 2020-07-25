
system.document.add "Test"{
	USE_EDIT_MODE
	button("run", 0,0, 300, 30){
		SetText("&Press to run...");
	}

	screen("ABC", 0, 40, 500, 300){
		Create(640,480, RGBA_8888);
	}
	
	SetProgram("test.dll");
}

