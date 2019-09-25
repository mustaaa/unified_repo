# How to use this template 

## Copy and rename the folder

Rename the folder and main cmake file to the name of the project

## Do git init

Initialize git repository by `git init`
add and commit all the files to the repository

## Do git add submodule 

To add SFML libs to the project run `git submodule add https://github.com/SFML/SFML.git ./3rdParty/SFML`
after cloning is complete commit the current changes in the repository

## Build and run the project normally

create a `build` folder and run `cmake ..` and `make`
