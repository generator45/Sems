# MPI Installation Guidelines (OpenMPI)

## 1. Install OpenMPI and development libraries

```sh
sudo apt install openmpi-bin libopenmpi-dev
```

## 2. Find the `mpicc` and `mpi.h` paths

```sh
which mpicc
which mpirun
find /usr -name mpi.h 2>/dev/null
```

Typical output:
- mpicc: `/usr/bin/mpicc`
- mpirun: `/usr/bin/mpirun`
- mpi.h: `/usr/lib/x86_64-linux-gnu/openmpi/include/mpi.h`

## 3. Configure VS Code for MPI development (OpenMPI)

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
                "/usr/lib/x86_64-linux-gnu/openmpi/include/**"
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

