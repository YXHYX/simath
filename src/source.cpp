
#include "../examples/example_demos.h"


// MAIN PROGRAM ENTRY

/*
* TODO:
*	- Z DEPTH buffer			done
*	- clipping space			done
*   - orthographic projetion
*   - shading					SOMEWHAT DONE
*   - proper dithering			ordered bayer 4x4 and floyd steinberg (add more dithering methods and optimize them)
*   - lighting					somewhat made?
*	- physics
*		- better solvers						RK4 and euler for now
*		- consistency checks (energy)			later
*		- constraints
*			- angular							later
*			- linear							done
*		- springs								done
*		- forcefields							done
*		- controllers							PID for the instant (MPC and LQR come later)
*	- graphics
*		- line rendering						done but gotta fix and optimize
*		- z fighting and viewport clipping		grah
*		- more dithering!						not yet
*		- LIGHTING!!!!							important but not yet
*		- custom texts and fonts				on it
*		- see possibility of using escape sequences ???
*	- resource manager
*		- texture manager						done
*		- models ( if need for any)				prob wont be done
*		- sounds manager						sorta done


ADD AUTOMATIC CONFIGURATION THROUGH THE WINDOWS PATH:
C:\Users\pear\AppData\Local\Packages\Microsoft.WindowsTerminal_8wekyb3d8bbwe\LocalState
by configuring the settings json file for easier fucking startup without fucking up my terminal shit


*/

int main() {
	font_display_demo();
}
