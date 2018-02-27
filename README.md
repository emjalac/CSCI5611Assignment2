# CSCI5611 : HW2 - Physically-Based Simulation
### Emma Lacroix

Compile cloth simulation in terminal with:
g++ -std=c++11 Camera.cpp Cloth.cpp glad.cpp main.cpp Material.cpp Node.cpp Sphere.cpp Spring.cpp Util.cpp Vec3D.cpp World.cpp WorldObject.cpp -I/Library/Frameworks/SDL2.framework/Headers -F/Library/Frameworks -framework SDL2 -framework OpenGL -o ../build/bin/cloth

Compile sound simulation in terminal with:
g++ -std=c++11 Camera.cpp glad.cpp music.cpp Material.cpp MusicWorld.cpp Node.cpp Spring.cpp String.cpp Util.cpp Vec3D.cpp -I/Library/Frameworks/SDL2.framework/Headers -F/Library/Frameworks -framework SDL2 -framework OpenGL -o ../build/bin/music