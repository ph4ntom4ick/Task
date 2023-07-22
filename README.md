## Build Instructions
To build the project, follow these steps:

1. Clone the repository:
   ```
   git clone <repository-url>
   cd <repository-directory>
   ```

2. Build the static library and the application:
   ```
   make
   ```

   If you don't have `make` installed, you can manually compile the library and application by executing the following commands:

   ```
   g++ -c filesearchlib.cpp -o filesearchlib.o
   ar rcs libfilesearchlib.a filesearchlib.o
   g++ main.cpp -L. -lfilesearchlib -o filesearch_app
   ```

## Usage
1. Run the application:
   ```
   ./filesearch_app
   ```

2. Enter the name of the file you want to search for when prompted.

The application will start searching for the specified file in every directory starting from the root using multiple threads. Once the file is found, the application will display the path to the file. If the file is not found, it will print "File not found."

## Cleaning Up
To clean up the build artifacts, run the following command:
```
make clean
```

This will remove the object files, the static library, and the application executable.
