# code_samples

This project called Checker_Lite is a command-line multi-player checkers game on the network written in C++ using STL/ boost libraries. 

The game is hosted by a server, that provides the list of available moves for a player, and validates players moves after each turn. The player that has no turns remaining loses. 

Checkers Lite is the same as American Checkers(also known as English draughts) except there is no promotion to king by moving a piece to the last rank (therefore stalemate is possible). Pieces move along diagonals, when available, jumps must be taken in preference to normal moves; and the board is 8 x 8. Multiple jumps in a single turn are not allowed.

The board output format is ASCII - a dot(.) symbolizes an empty square, X for black, O for white.

Working notes
1) The connection between player should be established with a simple IP:port. The server program binds to ip localhost and port 50000. Clients connect to the same.The players are on the same network with no firewall.
2) Capturing Moves are given preference to non- capturing moves. If a capturing move is available, the server will only suggest that to the player. If no capturing moves are available, then only will a non- capturing move be suggested by the server.
3) The current board information is sent to both players after every move, but the available moves information is only sent to the player, whose turn it is. 
4)The moves are presented as a list, with numbers in front of them. The clients can enter any of the numbers, to make a move.

Compilation / Running

Building Server / Client binaries
make

Clean binaries
make clean

Running server binary
./checkers_lite_server

Running client binary
./checkers_lite_client

External Libs. Used
Boost  - Compiled using boost v.1.54.0, which has been precompiled for FC15 using gcc version 4.6.3
