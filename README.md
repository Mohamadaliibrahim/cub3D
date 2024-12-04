# Cub3D - 42 School Project  

Cub3D is a 3D graphics project inspired by Wolfenstein 3D. It allows you to explore a simple 3D environment using raycasting techniques, rendered using the **MiniLibX** graphics library.

---

## 📂 Repository  
Clone the repository to get started:  
```bash
git clone https://github.com/Mohamadaliibrahim/cub3D.git
cd cub3D
```

---

## 📦 Requirements  

To compile and run the project, you need the following:  
1. **MiniLibX** - Install it before running the project:  
   - For Linux:  
     ```bash
     sudo apt-get install libxext-dev libxrandr-dev libbsd-dev
     git clone https://github.com/42Paris/minilibx-linux.git
     cd minilibx-linux
     make
     export LIBRARY_PATH=$(pwd)
     cd ..
     ```
   - For macOS, MiniLibX is usually provided in 42’s environment.

2. **Make** and **GCC** or equivalent C compiler.

3. **A valid map file**: Maps are available in the repository under the `maps/` folder.

---

## ⚙️ Usage  

### Compile the project:
Run the `make` command to compile the project:
```bash
make
```

### Run the program:
```bash
./cub3D <map_file>
```

### Example commands:
1. For a valid map:
   ```bash
   ./cub3D maps/good/<choose_a_map>
   ```
2. For a map with errors:
   ```bash
   ./cub3D maps/bad/<choose_a_map>
   ```

---

## 🗺️ Map Files  

Maps are located in the `maps/` directory:  
- `maps/good/`: Valid maps for smooth gameplay.
- `maps/bad/`: Maps intentionally designed to test error handling.

You can create or modify map files using the `.cub` format. Ensure the maps adhere to the required format.

---

## 🖥️ Features  
- **Raycasting engine**: 2D map converted to a 3D visual environment.  
- **Custom map support**: Load different maps easily.  
- **Error handling**: Detect and report errors in invalid map files (e.g., non-`.xpm` textures, incorrect formatting).

---

## 📜 Author  
This project was developed as part of the 42 School curriculum by [Mohamad Ali Ibrahim](https://github.com/Mohamadaliibrahim).

For any questions or feedback, feel free to contact me via GitHub.

--- 

## 🔧 Future Improvements  
- Enhanced texture support.  
- Additional gameplay mechanics.  
- Extended error handling for user-defined maps.
