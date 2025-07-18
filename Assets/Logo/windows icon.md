https://forums.unrealengine.com/t/how-to-change-the-icon-of-your-game/352581/7
https://www.icoconverter.com/

Windows has a requirement that any icon file must contain the icon at multiple resolutions, specifically 16x16, 32x32, 64x64, 128x128 and 256x256.

So make sure you create a subfolder to package into like so: ../Build/Windows/shipping/.
You should see the icon copied and renamed in: ../Build/Windows/Application.ico.
