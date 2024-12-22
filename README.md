# SizePath
A cli tool written in c++ that determines the size of folders and files.


## Build
`g++ -std=c++17 src/sizeit.cpp  -o bin/sizeit`

## Execute bin
`bin/sizeit <folder_path>`

![Screenshot 1](docs/example_output.png)

Use the quiet flag to not output any text but the totals
`bin/sizeit <folder_path> -q`

![Screenshot 2](docs/example_output_quiet.png)

# Add it to your global zsh environment

Add this line in your *.zshrc* file
```bash
alias sizeit="<filepath_to_bin>/sizeit"
```

Then run `source .zshrc`

and now you can execute *sizeit* anywhere on your computer.
Ex:
`sizeit /tmp`