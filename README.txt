GPPrimitiveEngine v0.1
Copyright (C) 2014  by Giuseppe Perniola

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.ion,  Inc., 59
    Temple Place, Suite 330, Boston, MA 02111-1307 USA


GPPrimitiveEngine is a small 3D engine made by myself and it is based on a my very old
project, called GPJShowRoom. It is written using C++ and SFML/SFGUI library, and there are
executables for Windows 32, Linux 32 and 64 bit, as well as sources.
If you are running on Linux platform then first you should install SFML and SFGUI, with all their
dependencies, go to its site for more info (http://www.sfml-dev.org). Instead, you need to do 
nothing on Windows, just move the folder with all dll files where you want.

Here there is a list of supported features:
_ 32-bit color depth, with alpha blending;
_ 32-bit Z-buffer depth;
_ unlimited number of cameras;
_ positioning and rotation of cameras on all axes;
_ unlimited number of 3D triangles, lines and points;
_ clipping of triangles, lines and points along Z-far and Z-near planes and on screen coords;
_ back-face culling for removing of hidden triangles;
_ texture mapping with perspective correction;
_ various kind of lights and possibility to add up to two types (one ambient and one directional/point/spot);
_ flat and gouraud shading;
_ loading 3ds object files.

You can use arrows keys and mouse to move the camera and press F1/F2 to show/hide a panel
where you can select flat/gouraud shading and choose lighting.

