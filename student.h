/**
 * Some features to handle students representation between SRC_ELS et SRC_PRF
 *
 * @file product.h
 * @author Jimmy Tournemaine <jimmy.tournemaine@yahoo.fr>
 * @version 1.0
 * @date 23 feb. 2016
 *
 * Some utilities to use sockets easily.
 */

/**
 * Represents a student
 * @struct student
 *
 * A student is represented by a name and a account.
 * The account is used by the authentication to the server.
 * the name is used for the professor display.
 */
struct student {
    char login[16];         /*!< The student account login */
    char password[32];      /*!< The password of the student account */
    char firstname[64];     /*!< The firstname of the student */
    char lastname[64];      /*!< The lastname of the student */
};

/**
 * Identify a student
 * @return The representation of the identified student.
 */
struct student student_id();