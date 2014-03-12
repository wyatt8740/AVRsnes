AVRsnes
=======

Arduino leonardo/atmega32u4 SNES controller HID program


This one is special. When first plugged in it uses a mapping of buttons
that I find optimal for emulator controls. It allows me to use the
same keyboard keys for playing as I am when using this controller,
which just sends those keys.

It can be switched, however, to a 'mouse mode', which is good for
playing Minecraft and navigating the screen like you are using
a mouse. In this mode, the mouse is controlled with A B X and Y.

To change modes, press L+R+Start.



Default Mode
SNES	--	Keyboard
A	--	X
B	--	Z
X	--	S
Y	--	A
L	--	D
R	--	C
UP	--	UP
DOWN	--	DOWN
LEFT	--	LEFT
RIGHT	--	RIGHT
START	-- 	RETURN/ENTER
SELECT	--	BACKSPACE


There are some key combinations in minecraft-mode.

SELECT+R - open inventory (press 'e')

SELECT+L - pause minecraft (press 'escape')

Have fun! And sorry for the really inefficient code!

