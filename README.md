UltraTTS

UltraTTS is a simple, graphical front-end for the powerful Piper Text-to-Speech engine. It provides a user-friendly interface to generate speech from local, high-quality voice models.
## Features

    Local Operation: All text-to-speech generation happens locally.

    Virtual Microphone: Creates a virtual audio device, allowing you to stream generated speech into other applications.

    User-Friendly Interface: A simple GUI makes it easy to select a voice and generate speech.

    Robust Cleanup: Automatically cleans up its virtual audio devices on startup and graceful shutdown.

🛠️ Installation
Prerequisites

You need the following tools installed on your system:

    CMake: A cross-platform build system.

    Qt6: The cross-platform application framework.

    Piper: The text-to-speech engine. Follow the official Piper installation guide for your system.

    paplay: The PulseAudio command-line tool.

    pactl: The PulseAudio control tool.

    gcc or clang: A C++ compiler.

On Arch Linux, you can install the necessary dependencies with:

sudo pacman -S cmake qt6-base piper pulseaudio-utils

Building the Application

    Clone the repository:

    git clone https://github.com/Ultraguy24/UltraTTS
    cd UltraTTS

    Build the application using CMake:

    mkdir build
    cd build
    cmake ..
    cmake --build .

Voice Models

UltraTTS uses voice models from the Piper project. You must download a voice model and place it in the correct directory.

    Download your preferred voice model from the Piper Voices page.

    Create the UltraTTS voices directory and move the downloaded files (.onnx and .json) into it:

    mkdir -p ~/.local/share/ultratts/voices
    mv ~/Downloads/your_voice_model.onnx ~/.local/share/ultratts/voices/
    mv ~/Downloads/your_voice_model.json ~/.local/share/ultratts/voices/

🚀 Usage

    Run the executable from the build directory:

    cd build
    ./UltraTTS.x86_64

    The application will automatically create a virtual microphone named "UltraTTS". Select this device in other applications to capture the generated speech.

    Enter text into the input box, select a voice, and click "Speak" to generate audio.

📄 Credits & License

    Ultraguy: Project creator.

    Piper: For the text-to-speech engine.

    PulseAudio: For the audio server tools.

    Qt: For the application framework.

This project is licensed under the MIT License - see the LICENSE file for details.