# Tash

## Description

Tash - TexAs SHell

Minimal command line shell for Linux and rest of UNIX family
It supports basic parse and run functionalities of shell along with some additional features
described later.

## Compile & Run

gcc tash.c -o tash -Wall -Werror -O

Interactive Mode : ./tash

Batch       Mode : ./tash <path to text with commands>


## Modes

There are two types of modes to execute the shell

  1. Interactive mode: It allows the user to accept commands directly from stdin

  2. Batch mode: It reads input from a file and executes the commands.

## Features 

### Redirection
  User can send the output(stdout and stderr) of the program to a file.
  Example:  ls -la /tmp > redirect

  The output file is created if it does not exists and overites it if it exists.
  Multiple redirection operators or multiple files to the right of redirection operator are considered as errors.

### Parallel Processes
  User can fork multiple process in parallel and wait for completion of parallel processes
  Example : ls & pwd & uname &

  '&' is a binary operator whose left operand is not null by design ( else it throws error )

### PATH
  Array of paths where the shell searches for executable
  For any given executable , shell searches for it from all the paths stored and executes the first
  match.
  Return with -1 code if shell cannot the executable

### Builtin functions
  
  Provided three in built functions as follows :

#### exit
  Exits from the shell using syscall exit
  Does not accept any arguments i.e throws error if any arguments are passed along with exit call

  Syntax : exit

#### cd 
  Changes directory from present working directory to path provided as argument ( throws error if
  such path is not accessible)
  if no argument provided, it changes directory to HOME of the user

  Syntax : cd <p1>
            cd 

  Does not accept more than one argument

### path
  Overrides the PATH i.e array of search with list of space separated paths provided as arguments
  If no arguments provided , PATH is cleared out and shell will not find executable anywhere

  Syntax : path <p1> <p2> 
            path

## Contributors :

  Harish Gontu , MSCS UTD
  Sharath Nakka, MSCS UTD
