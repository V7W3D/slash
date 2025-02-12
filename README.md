# **Slash - A Custom Unix Shell Utility**

**Slash** is a **C-based** project that implements advanced features of a shell command in Unix/Linux environments. It provides tools for directory management, input/output stream manipulation, and wildcard expansion (`*`, `**`).

## **Key Features**
**Directory Management** – Custom implementation of the `cd` command and path handling.  
**Stream Redirections** – Supports input/output redirections (`>`, `<`, `|`).  
**Wildcard Expansion** – Implements `*` and `**` for advanced file and directory matching.  
**String Manipulation** – Utility functions for processing and transforming strings in a shell context.  
**Built-in Test Script** – `test.sh` to validate project functionality.  

## **Project Structure**
📂 **Key Files:**  
- `main.c` → Program entry point.  
- `cd.c` / `cd.h` → Directory navigation management.  
- `star.c` / `star.h` → Wildcard `*` expansion implementation.
- `double_star.c` / `double_star.h` → Wildcard `**` expansion implementation.    
- `redirections.c` / `redirections.h` → Input/output redirections handling.  
- `split_string.c` → String parsing and manipulation utilities.  

🔧 **Compile with Makefile:**  
```bash
make
```

🚀 **Run the program:**  
```bash
./main
```

## Contributing
Contributions are welcome! Feel free to fork this repository, submit issues, or create pull requests.

## License
This project is licensed under the MIT License.

