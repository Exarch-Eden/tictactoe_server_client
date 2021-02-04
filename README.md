# tictactoe_server_client

Note: in order to run the program, you must have DC library already installed on your Linux system

run this command in the terminal under root directory:
gcc main.c server.c helper.c -pthread

BNF:
<turn> ::= [1-9] <player>
<move> ::= <player> <row> <col>
<player> ::= "1" | "2"
<grid> ::= [1-9]
<row> ::= [1-3]
<col> ::= [1-3]
<game_status> ::= [0-2]
<error_code> ::= [0-2]
