@echo off
pushd %~dp0
..\egfx\tools\eGFX_Tools.exe GenerateSprites Sprites eGFX_IMAGE_PLANE_16BPP_RGB565 Sprites
popd