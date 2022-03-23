/**
 * @file error.h
 * @author Julianne Adams <julianne@julianneadams.info>
 * @brief Error codes for chiton
 * @date 2022-03-22
 *
 * @copyright MIT License
 *
 */

/* Condition for breaking main program loop */
#define CHITON_ERROR_EXIT -1

/* Successful return after running a command */
#define CHITON_ERROR_SUCCESS 0

/* Catchall for general errors */
#define CHITON_ERROR_GENERAL 1

/* Misuse of shell builtins (ex. missing keywords or permission problems) */
#define CHITON_ERROR_BUILTIN_MISUSE 2

/* Command invoked cannot execute */
#define CHITON_ERROR_CANNOT_EXECUTE 126

/* Command not found */
#define CHITON_ERROR_COMMAND_NOT_FOUND 127

/* Invalid argument to exit */
#define CHITON_ERROR_INVALID_EXIT 128

/* Fatal error signal 1 (SIGHUP) */
#define CHITON_ERROR_SIGNAL_SIGHUP 129

/* Fatal error signal 2 (SIGINT) */
#define CHITON_ERROR_SIGNAL_SIGINT 130

/* Fatal error signal 3 (SIGQUIT) */
#define CHITON_ERROR_SIGNAL_SIGQUIT 131

/* Fatal error signal 9 (SIGKILL) */
#define CHITON_ERROR_SIGNAL_SIGKILL 137

/* Exit status out of range */
#define CHITON_ERROR_STATUS_RANGE 255
