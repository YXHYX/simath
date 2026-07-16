
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
*		+ better solvers						RK4 and euler for now
*		/ consistency checks (energy)			later
*		/ constraints
*			/ angular							later
*			+ linear							done
*		+ springs								done
*		+ forcefields							done
*		/ controllers							PID for the instant (MPC and LQR come later)
*	- graphics
*		- line rendering						done but gotta fix and optimize
*		+ z fighting and viewport clipping		grah
*		+ more dithering!						not yet
*		+ LIGHTING!!!!							important but not yet
*		+ custom texts and fonts				done ( only need proper cursor resetting when exceeding certain buffers )
*		- see possibility of using escape       NOT POSSIBLE, not at all, i have to modify the speed at which pty parses the stringes
*		- combine ASCII art of texture			on it
*			alongside for better results
*		- anti aliasing
*		- better texture coloring
*	- resource manager
*		+ texture manager						done
*		+ models ( if need for any)				done, need for specific triangular models only
*												which i will have to design ( LOW POLY OR MY PC DIES ) 
*		+ sounds manager						done
*		- fonts									incorporate with resource manager and unify the asset folders

ADD AUTOMATIC CONFIGURATION THROUGH THE WINDOWS PATH:
C:\Users\<currentUser>\AppData\Local\Packages\Microsoft.WindowsTerminal_8wekyb3d8bbwe\LocalState
by configuring the settings json file for easier fucking startup without fucking up my terminal shit

its done but dawg shit ui, maybe add launcher using imGUI instead?


*/

int main() {
	int status = video_player_demo();
	return status;
}
