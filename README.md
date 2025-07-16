# UDP-guessinggame
How to Run the UDP “Guess the Number” Game in VS Code 
(Windows) 


1. Setup Requirements (One-time only) 

    Install MinGW (with gcc)  
    During setup, install the following packages: 

        mingw32-base 
        mingw32-gcc-g++ 
    Install Visual Studio Code  
    Verify gcc installation: Open Command Prompt and type: gcc --version If 
    the version is displayed, the installation is successful. If not, add the 
    MinGW bin folder to the system PATH. 



2. Set Up the Project Folder 

    Create a folder, udp_guess_game 
    Add two C source files inside this folder: 

        udp_server.c 
        udp_client.c 



3. Open the Project in VS Code 

    Launch VS Code 
    Select the udp_guess_game folder 



4. Compile the Source Files 

    Compile the server source file: gcc udp_server.c -o server.exe -lws2_32 
    Compile the client source file: gcc udp_client.c -o client.exe -lws2_32 



5. Run the Game 

    In Terminal server, start the server: server.exe  
    The server will display the number to be guessed and wait for incoming 
    guesses 

    In Terminal client, start the client: client.exe 
    The client allows the user to input guesses. The server responds with 
    hints: 

        -Too low 
        -low, but close 
        -Too high 
        -high, but close 
        -Correct 
        
    Once the correct number is guessed, the server stops and the game ends. 


Conclusion 
    The UDP "Guess the Number" game is now running successfully using Visual 
    Studio Code on Windows. 
