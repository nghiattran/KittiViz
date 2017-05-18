#ifndef PROGRAMDEFINES_H_INCLUDED
#define PROGRAMDEFINES_H_INCLUDED

/**
\file ProgramDefines.h

\brief Header file that loads in the defines needed for the entire program.

The file is simply for convenience so that multiple headers do not need to redefine these values.

\author    Don Spickler
\version   1.1
\date      Written: 1/9/2016  <BR> Revised: 1/9/2016

*/

// SetVS sets the Vertical Sync for SFML.  When true, the frame rate will be very close to the
// refresh rate of the machine.  When false, the frame rate will run at maximum speed.
#define SetVS true

#define BUFFER_OFFSET(x) ((const void*) (x))
#define PI 3.14159265358979323846264338328
#define PI_DIV_180 0.0174532925199432957692369076849
#define PI_DIV_180f 0.0174532925199432957692369076849f
#define deg PI_DIV_180
#define degf PI_DIV_180f

#endif // PROGRAMDEFINES_H_INCLUDED
