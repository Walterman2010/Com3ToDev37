# Com3ToDev37
a tool to Connect the Com-Port and the Device ID at USB Ports in Windows systems. It is developed at Windows 10 system.

This tool is developed in Windwos 10 and not testet at Windows 11 jet. Also it is not known, there is still these Issue.

USB Camera Devices and also Webcams are using the Windows Device ID for Video or Audio functions. These are also important if you want to use the Camera in OpenCV. Some of these USB Camera are also controlled by a Serial Com Port.
An Example is the Flir Boson infrared camera.
In normal applications, with only one Camera, the will be now problem to connect the right Com Port to the right Device ID, but if you want to use more then one Device at one computer you can get in truble.

The to Com3ToDev37 (because these Problem was a bloody hell) is a tool what you can Implement in your Program. You need to Feed it with the Com Port and get the Device ID back.

This reposetory inclode to tools:
the Com3ToDev37 And the TestProjekt. The second shows a way to include it.

I Always wanted to continue the develop and write a Libary, but I dosn´t have a camera like these, So the Project is stuct. May be it is still usefull. In Case, please let me know ;)
Don´t use in comercial!

-----------------------------------------------------------------------------
Include code from:
David Gil de Gómez Pérez (Studiosi) (2018)
https://github.com/studiosi/OpenCVDeviceEnumerator
-----------------------------------------------------------------------------

MIT License

Copyright (c) 2021 Andy Langer & Max Planck Institut Greifswald

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
