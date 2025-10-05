// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include <Application.h>
#include <Nt/Core/Console.h>

int main() {
	_controlfp_s(NULL, 0, _EM_INVALID);
	srand(uInt(time(nullptr)));

	try {
		Application app;
		app.Initialize();
		app.Run();
	}
	catch (const Nt::Error& error) {
		error.Show();
		Nt::Console::Pause();
	}

	return 0;
}