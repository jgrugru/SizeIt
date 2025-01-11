# SizePath
A cli tool written in c++ that determines the size of folders and files.

# Demo
![Demo](docs/demo.gif)

## Execute binary
`bin/sizeit <file_path>`

## Arguments to binary
`-v` -> verbose

```bin/sizeit <file_path> -v```

`--units` -> output storage units ("b", "mb", "gb")

```bin/sizeit <file_path> --units gb```

`--file-type` -> only check storage for specific file endings

```bin/sizeit <file_path> --file-type .py```

## Examples
Check only .py files in my current directory with the units of megabytes
```./bin/sizeit . --file-type .py -v --units gb```

## Installation
Git clone and build the c++ binary
```git clone git@github.com:jgrugru/SizeIt.git```

then 
```g++ -std=c++17 src/sizeit.cpp  -o bin/sizeit```

# Add it to your global zsh environment
Add this line in your *.zshrc* file
```bash
alias sizeit="<filepath_to_bin>/sizeit"
```

Then run `source .zshrc`

and now you can execute *sizeit* anywhere on your computer.
Ex:
`sizeit /tmp`