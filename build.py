#!/usr/bin/env python3

import os
import sys
import subprocess
import platform

def run(command, use_shell=False):
    print(f"> {command}")
    if use_shell:
        subprocess.run(command, shell=True, check=True)
    else:
        subprocess.run(command, check=True)

def get_python_path():
    if os.name == "nt":
        return os.path.join("venv", "Scripts", "python.exe")
    else:
        return os.path.join("venv", "bin", "python")

def get_executable_path():
    if os.name == "nt":
        return os.path.join("build", "my_app.exe")
    else:
        return "./build/my_app"

def is_installed(python_executable, package_name):
    try:
        subprocess.run(
            [python_executable, "-m", "pip", "show", package_name],
            check=True,
            stdout=subprocess.DEVNULL,
            stderr=subprocess.DEVNULL
        )
        return True
    except subprocess.CalledProcessError:
        return False

def main():
    if not os.path.exists("venv"):
        print("Creating virtual environment...")
        subprocess.run([sys.executable, "-m", "venv", "venv"])

    venv_python = get_python_path()

    run([venv_python, "-m", "pip", "install", "--upgrade", "pip"])

    required_packages = ["kconfiglib", "requests"]

    missing = [pkg for pkg in required_packages if not is_installed(venv_python, pkg)]

    if missing:
        print(f"Installing missing dependencies: {', '.join(missing)}")
        run([venv_python, "-m", "pip", "install"] + missing)
    else:
        print("All dependencies already installed.")

    print("Running CMake...")
    run(["cmake", "-S", ".", "-B", "build"])
    run(["cmake", "--build", "build"])

    print("Running executable...")
    run(get_executable_path(), use_shell=True)

if __name__ == "__main__":
    main()