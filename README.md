# BreakBreacher

![ezgif-3-afe59fccfa](https://github.com/bradleeharr/BreakBreacher/assets/56418392/e70531a3-b4b7-44c4-9bc7-ce001ebf6335)

Welcome to BreakBreacher, a small WIP game project using meta-level interaction with windows, powered by SFML (Simple and Fast Multimedia Library). A unique experience that transcends the boundaries of your screen!

## Getting Started

These instructions will get you a copy of the project up and running on your local machine for development and testing purposes.

### Prerequisites

Before you begin, ensure you have the following installed:
- [Git](https://git-scm.com/)
- [Visual Studio](https://visualstudio.microsoft.com/) with C++ development tools
- [SFML](https://www.sfml-dev.org/download.php)

### How to Build

To clone and build BreakBreacher, follow these steps:

```bash
git clone https://github.com/bradleeharr/BreakBreacher.git
```

Navigate into the project directory before proceeding with the setup.

## Setup SFML in Visual Studio (VS) Project

### Step 1: Download SFML

Download SFML from the official website:

[SFML Downloads](https://www.sfml-dev.org/download.php)

Choose the version that matches your Visual Studio setup.

### Step 2: Extract SFML

Extract the downloaded SFML to a known location on your computer.

### Step 3: Configure Visual Studio

Configure your Visual Studio project to use SFML:

- Open the BreakBreacher project in Visual Studio.
- Navigate to `Project -> Properties -> VC++ Directories`.
- Edit 'Include Directories' to add the path to SFML's `include` directory.
- Edit 'Library Directories' to add the path to SFML's `lib` directory.

```plaintext
For example: 
Include Directories: C:\SFML-2.5.1\include
Library Directories: C:\SFML-2.5.1\lib
```

### Step 4: Link SFML Libraries

Specify the SFML libraries to link against:

- Go to `Project -> Properties -> Linker -> Input`.
- Add the following dependencies to ‘Additional Dependencies’:

```plaintext
sfml-graphics.lib
sfml-window.lib
sfml-system.lib
```

For the debug configuration, append `-d` to use the debug libraries:

```plaintext
sfml-graphics-d.lib
sfml-window-d.lib
sfml-system-d.lib
```

### Step 5: Manage SFML DLLs

Ensure the SFML DLLs are available to your project:

- Copy all `.dll` files from SFML's `bin` folder to your project's source folder (where your .vcxproj file is located).

## Running the Game

After completing the above setup, you can build and run the game from Visual Studio. 

## Contributing

Please feel free to send a pull request.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Acknowledgments

- Kudos to the [SFML](https://www.sfml-dev.org/) team for providing a powerful yet straightforward library for game development.

<!-- etc. -->

