The application, namely ‘kode’, receives a a command, a parameter, and a
filename in the command line and processes the file based on the given command.
At the end the program reports of the changes in file, to the console.

Allowable inputs:
                            1. kode RC <word> <file>
(Remove Case Sensitive)
Replaces all instance of <word> in <file> with asterisks. The number of asterisks
correspond to the number of characters in <word>. The search is case-sensitive. 

                            2. kode RI <word> <file>
(Remove Ignore Case)
Replaces all instance of <word> in <file> with asterisks. The search is case-insensitive.

                            3. kode UK <word> <file>
(Un-remove Keep Case)
Searches for possible places that <word> may have been removed (by matching the
number of asterisks with the word length). Replaces the asterisks with <word> in <file>. 

                            4. kode UM <word> <file>
(Un-remove Match Case)
Searches for possible places that <word> may have been removed.
Replaces the asterisks with <word> in <file>. 
The word is transformed to upper-case or lower-case,depending on the letter before. 
If there is no letter before the placeholder, the letter after is checked. 
If there is no letter after, it follows the UK option above.
