# DiscordCoreLoader
![Lines of code](https://img.shields.io/tokei/lines/github/realtimechris/discordcoreloader?&style=plastic&label=Lines%20of%20Code)   
### A stress-tester for Discord bot libraries.
# Quick Start
---
## Install the Dependencies, which are:  
----
  [Simdjson](https://github.com/simdjson/simdjson)  
  [OpenSSL](https://github.com/openssl/openssl)  
  [CMake](https://cmake.org/)
----
----
### 1. First things first - be sure to aim your library at the IP and port that is set in the Config.json file of this repo. For instance on my setup I chose the IP and port 192.168.0.28:443.
----
### 1. Navigate a console into the top-level folder of the repo.
----
### 2. Run the command `cmake -S . --preset=Windows_OR_Linux-Release_OR_Debug`.
----
### 3. Then, run `cmake --build --preset=Windows_OR_Linux-Release_OR_Debug`.
----
### 4. Now after setting the settings in the Config.json found in the top-level folder of the repo, run `cmake --install ./Build/Release_OR_Debug`.
----
### 5. Run the program! Found in `repo/Output Files/Release_OR_Debug`.
----
