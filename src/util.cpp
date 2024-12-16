/*
 * @Author: Xudong0722
 * @Date: 2024-09-01 13:34:31
 * @Last Modified by: Xudong0722
 * @Last Modified time: 2024-09-01 13:48:52
 */

#include "util.h"
#include <stdio.h>
#include <stdlib.h>

void errif(bool condition, const char *message) {
  if (condition) {
    perror(message);
    exit(EXIT_FAILURE);
  }
}