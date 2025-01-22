# Unscreensaver

Unsplash API Screensaver

- Dependancies per system
``` 
# Debian
$ sudo apt-get install libsfml-dev yaml-cpp-devel libexiv2-dev
```
```
# Fedora    
$ sudo dnf install SFML yaml-cpp-devel exiv2 exiv2-devel
```
```
# ArchLinux    
$ sudo pacman -S sfml yaml-cpp exiv2
```

- Compile main.cpp, help.cpp, readconfig.cpp program
```
$ g++ *.cpp -o screensaver -I./include \
-lsfml-graphics \
-lsfml-window \
-lsfml-system \
-lyaml-cpp \
-lexiv2
```
- Executing command
```
./screensaver                 # Black background (default)
./screensaver --info          # Display information on screen
./screensaver --help          # Help menu
./screensaver --colour Red    # Red background
./screensaver --color Blue    # Blue background

Press ESC to exit the screensaver
```