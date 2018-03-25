/*
 * include custom header
 */
#include "20131575.h" 


/* 1. Print "sicsim>" through infinite loop.
 * 2. Get user command.
 * 3. Tokenize user command and get the number of tokenized words.
 * 4. Save user command to history linked list no matter what. (even if the command is invalid)
 * 5. Check the first tokenized word from user's command matches the following functions.
 * 6. Handle exceptions and print error if exception happens.
 * 7. If no errors were found, execute following functions.
 *       h[elp] : Show all possible command.
 *       hi[story] : Show the history of commands that user entered.
 *       q[uit] : Quit the program.
 *       d[ir] : Show all directories and files in current directory.
 *       du[mp] : Show memory of what user wants to know.
 *       e[dit] : Edit memory value of which user wants to change.
 *       f[ill] : Fill memory with a value of which user wants in the boundary.
 *       reset : Reset all memory value with 0.
 *       opcodelist : Show opcode hash table.
 *       opcode mnemonic : Print opcode number of corresponding mnemonic.
 * 8. Set memory to NULL  when one command is performed
 *
 *
 *
 * CRITICAL!! When using custom header file, (In this case "20131575.h") DO NOT USE the followings...
 * 
 * Variables : mem, tmp_s, tmp_e, address, tmp_value, hash_head, hash_tail, working
 *             tmp_s2, tmp_e2, tmp_value2, start, end, token, totalToken, flag
 *
 * Functions : printHelp, printDirectory, dump, printList, edit, findLastIndex, 
 *             findCharNum, index_return_func, append, fill, reset, handle_error, 
 *             syntax_error, opcode_make_hash, init_hash, insert_hash, make_hash_table, 
 *             find_opcode, print_oplist, commaNum, executeShellCommand 
 *
 * Type definitions : node, nptr, _list, Node, oplist, opptr 
 *
 */
int main(){
  executeShellCommand();
  return 0;
}
