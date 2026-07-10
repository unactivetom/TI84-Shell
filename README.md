# TI SHELL

This is a shell type application for the TI84 plus CE.

At the moment you can only write/display text, using that I will implement commands in a later stage.
This project is not under a license, so you are free to use it for whatever you like.

***
## How to use

Start the software... well, like you would with any program.
Type using the keys, you don't have to press alpha to use them. (I would recommend using an emulator and a keyboard).

Enter - Enter  
Backspace - Mode  
Quit program - Clear  

There is nothing restricting you from writing outside the screen, but I would not recommend it since you are literally writing stuff to a random place in ram, which *will* crash something if you're not careful.  
If your calculator is having problems for this reason please reset it with the button on the back.

***
## Targets for this project

My goal is to implement msbasic programming, I do not know what family of it, nor do I know how to use it.
But something in the trent of a script based programming language would be amazing. Also would I love to implement VIM as a text editor.
I think the hardest part will be making a compiler from the script language to a .8xp file, but we'll see how that'll work later.


### *TODO*  

* Implement a better key input system
* Restrict the user's destruction of strings
* Rewrite the code to more files if it gets messy
* Add a some basic commands
* Add file saving
* Add file opening
* Split the code in multiple .8xp files for better performance
* Add a unix like file system
* Implement a compiler from msbasic to .8xp
* Be able to run .8xp files