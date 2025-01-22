# Unscreensaver

Unsplash API Screensaver for Linux

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
- Targeted executing options (Currently not finished)
```
./screensaver                 # Black background (default)
./screensaver --info          # GUI Display help info on screen
./screensaver --help          # CLI Display help info on command
./screensaver --setup         # Create / Update config.yml
./screensaver --colour Black  # Black background
./screensaver --color Black   # The same as --colour

Press ESC or "Q" to exit the Unscreensaver
```