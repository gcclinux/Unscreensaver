# Unscreensaver

Unsplash API Screensaver for Linux   
<br>  
- Dependancies Unsplash API FREE account - [Unsplash](https://unsplash.com/)  
<br>   
  
### System Dependancies
``` 
# Debian
$ sudo apt-get install libsfml-dev yaml-cpp-devel libexiv2-dev libcurl4-openssl-dev nlohmann-json3-dev

# Fedora    
$ sudo dnf install SFML yaml-cpp-devel exiv2 exiv2-devel libcurl-devel nlohmann-json-devel

# ArchLinux    
$ sudo pacman -S sfml yaml-cpp exiv2 curl nlohmann-json
```

### Compile or build unscreensaver 
```
# Compile manually
$ g++ -o unscreensaver *.cpp -I./include -I/usr/include/nlohmann -lsfml-graphics -lsfml-window -lsfml-system -lyaml-cpp -lexiv2 -lcurl

# Alternative build:
$ make

# Clean build:
$ make clean
```
### Targeted executing options
```
./unscreensaver                 # Black background (default)
./unscreensaver --help          # CLI Display help info on command
./unscreensaver --setup         # Create / Update config.yml
./unscreensaver --colour Black  # Black background
./unscreensaver --color Black   # The same as --colour

Press ESC or "Q" to exit the Unscreensaver
```   


#### ScreenSaver running example:
<img src="example.png" alt="Unscreensaver" width="600" height="400"> 
