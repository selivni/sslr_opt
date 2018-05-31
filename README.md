This project draws Sponza, has a SSLR implementation for glossy reflections, also has a shadow map that looks ugly.

To make:
Linux: cd build && cmake .. && make all

Windows: Not really ported, but if you mess around with libs and compile/link them all, everything whould be fine.

Controls:
	
	Q - quit;
	[Ctrl +] W - [small] step forward;
	[Ctrl +] S - [small] step backwards;
	M - toggle mouse;
	Shift + O - toggle SSLR reflections;
	F - turn on FPS count (spams the console ATM);
	P - print camera location;
	T - dont press it;
	L - toggle shadowmap;
	C - Put camera in a certain (programmer-defined) position.

Libs needed:
	GLEW
	GLUT
	OpenGL
	JPEG
