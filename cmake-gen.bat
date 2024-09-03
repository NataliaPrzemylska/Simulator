@echo off
if not exist build (
	mkdir build
)
cmake .
pause