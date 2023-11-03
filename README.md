# BreakBreacher
An SFML Game with Windows

# How to build
git clone https://github.com/bradleeharr/BreakBreacher.git

## Setup SFML in Visual Studio (VS) Project
### Step 1: Download SFML
You can download SFML by visiting their official website: [SFML Downloads](https://www.sfml-dev.org/download.php)

### Step 2: Extract SFML
Extract SFML to a location on your computer.

### Step 3: Setup SFML for your project in Visual Studio
* Open this project in Visual Studio.
* Go to Project -> Properties -> VC++ Directories
* Edit 'Include Directories' to add path to SFML `include` directory
* Edit 'Library Directories' to add path to SFML `lib` directory
c++ /include /lib

### Step 4: Add SFML linker dependencies
* Go to Project -> Properties -> Linker -> Input
* Add following dependencies in ‘Additional Dependencies’
c++ sfml-graphics.lib sfml-window.lib sfml-system.lib

For debug configuration, use the debug libraries:
c++ sfml-graphics-d.lib sfml-window-d.lib sfml-system-d.lib

### Step 5: Copy DLLs to project source
* Copy all dll files from SFML’s `bin` folder to your project source folder (where your .vcxproj file is)

This should finish the SFML setup for a project in Visual Studio.
