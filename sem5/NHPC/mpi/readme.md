# MPI Installation Guidelines

## 1. Install MPICH and development libraries

```sh
sudo apt install mpich
sudo apt install libmpich-dev
```

## 2. Find the `mpicc` path

```sh
find /usr -name mpi.h 2>/dev/null
```

## 3. Configure VS Code for MPI development

- Create a new folder for your MPI codes.
- Add a `.vscode` folder inside it.
- Create a file named `c_cpp_properties.json` with the following content:

```json
{
    "configurations": [
        {
            "name": "Linux",
            "includePath": [
                "${workspaceFolder}/**",
                "/usr/lib/x86_64-linux-gnu/mpich/include/**"
            ],
            "defines": [],
            "compilerPath": "/usr/bin/mpicc",
            "cStandard": "c11",
            "cppStandard": "c++17",
            "intelliSenseMode": "linux-gcc-x64"
        }
    ],
    "version": 4
}
```

